#include "ns_overlay.h"
#include <d3d9.h>
#include <d3d10_1.h>
#include <d3d11.h>
#include <dxgi.h>
#include <gl/GL.h>

// Глобальные переменные
std::vector<HMODULE> g_InjectDlls{};
HMODULE g_hCurrentModule = NULL;

static RenderType g_currentRenderType = RenderType::Unknown;

// Флаги инициализации для каждого бэкенда
static bool g_d3d9Hooked = false;
static bool g_d3d10Hooked = false;
static bool g_d3d11Hooked = false;
static bool g_d3d12Hooked = false;
static bool g_glHooked = false;

// ==========================================
// Обнаружение рендера
// ==========================================

static RenderType DetectRenderType() {
    // Проверка DirectX 9
    if (GetModuleHandleA("d3d9.dll")) {
        return RenderType::DirectX9;
    }
    
    // Проверка DirectX 10/11
    if (GetModuleHandleA("d3d10.dll") || GetModuleHandleA("d3d10_1.dll")) {
        return RenderType::DirectX10;
    }
    
    if (GetModuleHandleA("d3d11.dll")) {
        return RenderType::DirectX11;
    }
    
    // Проверка DirectX 12 (через DXGI)
    if (GetModuleHandleA("dxgi.dll") && GetModuleHandleA("d3d12.dll")) {
        return RenderType::DirectX12;
    }
    
    // Проверка OpenGL
    if (GetModuleHandleA("opengl32.dll")) {
        return RenderType::OpenGL;
    }
    
    return RenderType::Unknown;
}

// ==========================================
// Хуки для каждого бэкенда (без Detours, через прямую подмену VTable)
// ==========================================

// --- DirectX 9 ---
typedef HRESULT (WINAPI *D3D9_EndScene_t)(IDirect3DDevice9*);
static D3D9_EndScene_t o_D3D9_EndScene = nullptr;

HRESULT WINAPI hk_D3D9_EndScene(IDirect3DDevice9* pDevice) {
    // Здесь будет инициализация ImGui для DX9 и рендеринг
    return o_D3D9_EndScene(pDevice);
}

// --- DirectX 10 ---
typedef HRESULT (WINAPI *D3D10_Present_t)(IDXGISwapChain*, UINT, UINT);
static D3D10_Present_t o_D3D10_Present = nullptr;

HRESULT WINAPI hk_D3D10_Present(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags) {
    return o_D3D10_Present(pChain, SyncInterval, Flags);
}

// --- DirectX 11 ---
typedef HRESULT (WINAPI *D3D11_Present_t)(IDXGISwapChain*, UINT, UINT);
static D3D11_Present_t o_D3D11_Present = nullptr;

HRESULT WINAPI hk_D3D11_Present(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags) {
    return o_D3D11_Present(pChain, SyncInterval, Flags);
}

// --- DirectX 12 ---
// Для DX12 требуется сложная настройка под конкретную игру, возвращаем false

// --- OpenGL ---
typedef int (APIENTRY *wglSwapBuffers_t)(HDC);
static wglSwapBuffers_t o_wglSwapBuffers = nullptr;

int APIENTRY hk_wglSwapBuffers(HDC hdc) {
    return o_wglSwapBuffers(hdc);
}

// Вспомогательная функция для безопасной подмены указателя в VTable
static bool HookVTableMethod(void** pVTable, int index, void* pNewFunc, void** pOriginalFunc) {
    if (!pVTable || !pNewFunc || !pOriginalFunc) return false;
    
    DWORD oldProtect;
    // Разрешаем запись в страницу памяти с VTable
    if (VirtualProtect(&pVTable[index], sizeof(void*), PAGE_READWRITE, &oldProtect)) {
        *pOriginalFunc = pVTable[index]; // Сохраняем оригинальный указатель
        pVTable[index] = pNewFunc;       // Подменяем на наш хук
        VirtualProtect(&pVTable[index], sizeof(void*), oldProtect, &oldProtect);
        FlushInstructionCache(GetCurrentProcess(), &pVTable[index], sizeof(void*));
        return true;
    }
    return false;
}

// ==========================================
// Установка хуков
// ==========================================

static bool HookDirectX9() {
    HMODULE hD3D9 = GetModuleHandleA("d3d9.dll");
    if (!hD3D9) return false;

    // Создаем временное устройство для получения VTable
    HWND hTempWnd = CreateWindowExA(0, "STATIC", "D3D9", WS_OVERLAPPED, 0, 0, 100, 100, NULL, NULL, NULL, NULL);
    
    LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D) {
        DestroyWindow(hTempWnd);
        return false;
    }

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hTempWnd;

    LPDIRECT3DDEVICE9 pDevice = nullptr;
    HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hTempWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice);

    if (SUCCEEDED(hr) && pDevice) {
        // Получаем VTable
        void** pVTable = *(void***)pDevice;
        
        // EndScene имеет индекс 42
        HookVTableMethod(pVTable, 42, (void*)hk_D3D9_EndScene, (void**)&o_D3D9_EndScene);
        g_d3d9Hooked = (o_D3D9_EndScene != nullptr);
        
        pDevice->Release();
    } else {
        g_d3d9Hooked = false;
    }

    if (pD3D) pD3D->Release();
    DestroyWindow(hTempWnd);
    UnregisterClassA("STATIC", NULL);
    
    return g_d3d9Hooked;
}

static bool HookDirectX10() {
    HMODULE hD3D10 = GetModuleHandleA("d3d10.dll");
    if (!hD3D10) hD3D10 = GetModuleHandleA("d3d10_1.dll");
    if (!hD3D10) return false;

    HWND hTempWnd = CreateWindowExA(0, "STATIC", "D3D10", WS_OVERLAPPED, 0, 0, 100, 100, NULL, NULL, NULL, NULL);

    HMODULE hDXGI = LoadLibraryA("dxgi.dll");
    if (!hDXGI) {
        DestroyWindow(hTempWnd);
        return false;
    }

    typedef HRESULT (WINAPI *tCreateDXGIFactory)(REFIID riid, void** ppFactory);
    tCreateDXGIFactory pCreateDXGIFactory = (tCreateDXGIFactory)GetProcAddress(hDXGI, "CreateDXGIFactory");
    if (!pCreateDXGIFactory) {
        FreeLibrary(hDXGI);
        DestroyWindow(hTempWnd);
        return false;
    }

    IDXGIFactory* pFactory = nullptr;
    if (FAILED(pCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
        FreeLibrary(hDXGI);
        DestroyWindow(hTempWnd);
        return false;
    }

    IDXGIAdapter* pAdapter = nullptr;
    if (FAILED(pFactory->EnumAdapters(0, &pAdapter))) {
        pFactory->Release();
        FreeLibrary(hDXGI);
        DestroyWindow(hTempWnd);
        return false;
    }

    typedef HRESULT (WINAPI *tD3D10CreateDeviceAndSwapChain)(IDXGIAdapter*, D3D10_DRIVER_TYPE, HMODULE, UINT, UINT, DXGI_SWAP_CHAIN_DESC*, ID3D10Device**, IDXGISwapChain**);
    tD3D10CreateDeviceAndSwapChain pD3D10CreateDeviceAndSwapChain = (tD3D10CreateDeviceAndSwapChain)GetProcAddress(hD3D10, "D3D10CreateDeviceAndSwapChain");
    if (!pD3D10CreateDeviceAndSwapChain) {
        pAdapter->Release();
        pFactory->Release();
        FreeLibrary(hDXGI);
        DestroyWindow(hTempWnd);
        return false;
    }

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hTempWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    ID3D10Device* pDevice = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;

    if (SUCCEEDED(pD3D10CreateDeviceAndSwapChain(pAdapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &sd, &pDevice, &pSwapChain))) {
        // Получаем VTable свопчейна
        void** pVTable = *(void***)pSwapChain;
        
        // Present имеет индекс 8
        HookVTableMethod(pVTable, 8, (void*)hk_D3D10_Present, (void**)&o_D3D10_Present);
        g_d3d10Hooked = (o_D3D10_Present != nullptr);
        
        pSwapChain->Release();
        pDevice->Release();
    }

    pAdapter->Release();
    pFactory->Release();
    FreeLibrary(hDXGI);
    DestroyWindow(hTempWnd);
    UnregisterClassA("STATIC", NULL);
    
    return g_d3d10Hooked;
}

static bool HookDirectX11() {
    HMODULE hD3D11 = GetModuleHandleA("d3d11.dll");
    if (!hD3D11) return false;

    HWND hTempWnd = CreateWindowExA(0, "STATIC", "D3D11", WS_OVERLAPPED, 0, 0, 100, 100, NULL, NULL, NULL, NULL);

    HMODULE hDXGI = LoadLibraryA("dxgi.dll");
    if (!hDXGI) {
        DestroyWindow(hTempWnd);
        return false;
    }

    typedef HRESULT (WINAPI *tCreateDXGIFactory)(REFIID riid, void** ppFactory);
    tCreateDXGIFactory pCreateDXGIFactory = (tCreateDXGIFactory)GetProcAddress(hDXGI, "CreateDXGIFactory");
    if (!pCreateDXGIFactory) {
        FreeLibrary(hDXGI);
        DestroyWindow(hTempWnd);
        return false;
    }

    IDXGIFactory* pFactory = nullptr;
    if (FAILED(pCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
        FreeLibrary(hDXGI);
        DestroyWindow(hTempWnd);
        return false;
    }

    IDXGIAdapter* pAdapter = nullptr;
    if (FAILED(pFactory->EnumAdapters(0, &pAdapter))) {
        pFactory->Release();
        FreeLibrary(hDXGI);
        DestroyWindow(hTempWnd);
        return false;
    }

    typedef HRESULT (WINAPI *tD3D11CreateDeviceAndSwapChain)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
    tD3D11CreateDeviceAndSwapChain pD3D11CreateDeviceAndSwapChain = (tD3D11CreateDeviceAndSwapChain)GetProcAddress(hD3D11, "D3D11CreateDeviceAndSwapChain");
    if (!pD3D11CreateDeviceAndSwapChain) {
        pAdapter->Release();
        pFactory->Release();
        FreeLibrary(hDXGI);
        DestroyWindow(hTempWnd);
        return false;
    }

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hTempWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    D3D_FEATURE_LEVEL featureLevel;

    if (SUCCEEDED(pD3D11CreateDeviceAndSwapChain(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pContext))) {
        void** pVTable = *(void***)pSwapChain;
        
        // Present имеет индекс 8
        HookVTableMethod(pVTable, 8, (void*)hk_D3D11_Present, (void**)&o_D3D11_Present);
        g_d3d11Hooked = (o_D3D11_Present != nullptr);
        
        pSwapChain->Release();
        pDevice->Release();
        pContext->Release();
    }

    pAdapter->Release();
    pFactory->Release();
    FreeLibrary(hDXGI);
    DestroyWindow(hTempWnd);
    UnregisterClassA("STATIC", NULL);
    
    return g_d3d11Hooked;
}

static bool HookDirectX12() {
    // DX12 требует сложной инициализации, обычно хукают ExecuteCommandLists
    // В рамках шаблона вернем false, так как требуется специфичная настройка под игру
    g_d3d12Hooked = false;
    return false;
}

static bool HookOpenGL() {
    HMODULE hGL = GetModuleHandleA("opengl32.dll");
    if (!hGL) return false;

    o_wglSwapBuffers = (wglSwapBuffers_t)GetProcAddress(hGL, "wglSwapBuffers");
    if (o_wglSwapBuffers) {
        // Для OpenGL хукаем функцию напрямую через замену в IAT или код-патчинг
        // В простейшем случае - заменяем указатель функции
        DWORD oldProtect;
        if (VirtualProtect((void*)o_wglSwapBuffers, sizeof(void*), PAGE_READWRITE, &oldProtect)) {
            // Это упрощенный пример, в реальности нужно делать патчинг кода или IAT
            // Для полноценного хука OpenGL лучше использовать библиотеку вроде detours или minhook
            g_glHooked = false; 
        }
    }
    return false;
}

// ==========================================
// Основные функции
// ==========================================

bool NsLoad() {
    g_currentRenderType = DetectRenderType();
    
    if (g_currentRenderType == RenderType::Unknown) {
        return FALSE; // Рендер не найден
    }

    BOOL hookResult = FALSE;
    
    switch (g_currentRenderType) {
        case RenderType::DirectX9:
            hookResult = HookDirectX9() ? TRUE : FALSE;
            break;
        case RenderType::DirectX10:
            hookResult = HookDirectX10() ? TRUE : FALSE;
            break;
        case RenderType::DirectX11:
            hookResult = HookDirectX11() ? TRUE : FALSE;
            break;
        case RenderType::DirectX12:
            hookResult = HookDirectX12() ? TRUE : FALSE;
            break;
        case RenderType::OpenGL:
            hookResult = HookOpenGL() ? TRUE : FALSE;
            break;
        default:
            break;
    }

    return hookResult;
}

void NsUnload() {
    // В данной реализации без Detours отцепление хуков сложнее
    // и зависит от метода, которым был установлен хук.
    // Для простоты оставляем заглушку.
    // При использовании VTable-хуков без сохранения оригинальных страниц
    // восстановление может привести к крашу, если игра уже использует хук.
    
    g_currentRenderType = RenderType::Unknown;
}

RenderType NsGetRenderType() {
    return g_currentRenderType;
}

// ==========================================
// C API реализация
// ==========================================

extern "C" {

bool Ns_Initialize() {
    if (g_currentRenderType == RenderType::Unknown) {
        return false;
    }
    // Здесь должна быть инициализация ImGui для конкретного бэкенда
    return true;
}

void Ns_BeginFrame() {
    // Начало нового кадра ImGui
    // ImGui::NewFrame();
}

void Ns_EndFrame() {
    // Конец кадра ImGui
    // ImGui::Render();
    // ImGui_ImplXXX_RenderDrawData(ImGui::GetDrawData());
}

bool Ns_DrawWindow(const char* title, float x, float y, float w, float h, bool visible) {
    if (!visible) return false;
    
    Ns_BeginFrame();
    
    // Устанавливаем позицию
    // ImGui::SetNextWindowPos(ImVec2(x, y));
    // ImGui::SetNextWindowSize(ImVec2(w, h));
    
    // Рисуем окно
    // bool isOpen = true;
    // ImGui::Begin(title, &isOpen);
    // ... контент ...
    // ImGui::End();
    
    Ns_EndFrame();
    
    return true; // Окно активно
}

void Ns_DrawText(const char* text, float x, float y) {
    Ns_BeginFrame();
    // ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), IM_COL32_WHITE, text);
    Ns_EndFrame();
}

bool Ns_IsMouseClicked(int button) {
    // ImGui::IsMouseClicked(button)
    return false;
}

void Ns_Shutdown() {
    // ImGui::Shutdown()
    NsUnload();
}

} // extern "C"
