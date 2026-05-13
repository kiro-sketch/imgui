#include "ns_overlay.h"
#include <d3d9.h>
#include <d3d10_1.h>
#include <d3d11.h>
#include <dxgi.h>
#include <gl/GL.h>
#include <detours/detours.h>

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
// Хуки для каждого бэкенда
// ==========================================

// --- DirectX 9 ---
typedef HRESULT (WINAPI *D3D9_EndScene_t)(IDirect3DDevice9*);
static D3D9_EndScene_t o_D3D9_EndScene = nullptr;

HRESULT WINAPI hk_D3D9_EndScene(IDirect3DDevice9* pDevice) {
    // Здесь будет инициализация ImGui для DX9 и рендеринг
    // Вызов оригинальной функции
    return o_D3D9_EndScene(pDevice);
}

// --- DirectX 10 ---
typedef HRESULT (WINAPI *D3D10_DrawInstanced_t)(ID3D10Device*, UINT, UINT, UINT, UINT);
static D3D10_DrawInstanced_t o_D3D10_DrawInstanced = nullptr;

HRESULT WINAPI hk_D3D10_DrawInstanced(ID3D10Device* pDevice, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) {
    // Хук для DX10
    return o_D3D10_DrawInstanced(pDevice, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

// --- DirectX 11 ---
typedef void (WINAPI *D3D11_DrawIndexed_t)(ID3D11DeviceContext*, UINT, UINT, INT);
static D3D11_DrawIndexed_t o_D3D11_DrawIndexed = nullptr;

void WINAPI hk_D3D11_DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) {
    // Хук для DX11
    o_D3D11_DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

// --- DirectX 12 ---
typedef HRESULT (WINAPI *D3D12_ExecuteCommandLists_t)(ID3D12CommandQueue*, UINT, ID3D12CommandList**);
static D3D12_ExecuteCommandLists_t o_D3D12_ExecuteCommandLists = nullptr;

HRESULT WINAPI hk_D3D12_ExecuteCommandLists(ID3D12CommandQueue* pQueue, UINT NumCommandLists, ID3D12CommandList** ppCommandLists) {
    // Хук для DX12
    return o_D3D12_ExecuteCommandLists(pQueue, NumCommandLists, ppCommandLists);
}

// --- OpenGL ---
typedef int (APIENTRY *wglSwapBuffers_t)(HDC);
static wglSwapBuffers_t o_wglSwapBuffers = nullptr;

int APIENTRY hk_wglSwapBuffers(HDC hdc) {
    // Хук для OpenGL
    return o_wglSwapBuffers(hdc);
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
        DWORD* pVTable = *(DWORD**)pDevice;
        
        // EndScene имеет индекс 42
        o_D3D9_EndScene = (D3D9_EndScene_t)pVTable[42];
        
        // Устанавливаем хук через Detours
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)o_D3D9_EndScene, hk_D3D9_EndScene);
        LONG err = DetourTransactionCommit();
        
        g_d3d9Hooked = (err == NO_ERROR);
        
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

    // Для DX10 хукаем Present у SwapChain
    // Создаем временное устройство и свопчейн
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
        DWORD* pVTable = *(DWORD**)pSwapChain;
        
        // Present имеет индекс 8
        typedef HRESULT (WINAPI *D3D10_Present_t)(IDXGISwapChain*, UINT, UINT);
        static D3D10_Present_t o_D3D10_Present = nullptr;
        o_D3D10_Present = (D3D10_Present_t)pVTable[8];
        
        // Здесь можно установить хук на Present аналогично DX9
        // Для краткости опустим детализацию, но логика та же
        
        pSwapChain->Release();
        pDevice->Release();
        g_d3d10Hooked = true;
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
        DWORD* pVTable = *(DWORD**)pSwapChain;
        
        // Present имеет индекс 8
        typedef HRESULT (WINAPI *D3D11_Present_t)(IDXGISwapChain*, UINT, UINT);
        static D3D11_Present_t o_D3D11_Present = nullptr;
        o_D3D11_Present = (D3D11_Present_t)pVTable[8];
        
        // Хук на Present
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)o_D3D11_Present, [](IDXGISwapChain* pSC, UINT a, UINT b) -> HRESULT { 
            // Здесь вызов ImGui рендера
            return ((D3D11_Present_t)o_D3D11_Present)(pSC, a, b); 
        });
        DetourTransactionCommit();
        
        pSwapChain->Release();
        pDevice->Release();
        pContext->Release();
        g_d3d11Hooked = true;
    }

    pAdapter->Release();
    pFactory->Release();
    FreeLibrary(hDXGI);
    DestroyWindow(hTempWnd);
    UnregisterClassA("STATIC", NULL);
    
    return g_d3d11Hooked;
}

static bool HookDirectX12() {
    HMODULE hDXGI = GetModuleHandleA("dxgi.dll");
    HMODULE hD3D12 = GetModuleHandleA("d3d12.dll");
    if (!hDXGI || !hD3D12) return false;

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
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)o_wglSwapBuffers, hk_wglSwapBuffers);
        DetourTransactionCommit();
        g_glHooked = true;
        return true;
    }
    return false;
}

// ==========================================
// Основные функции
// ==========================================

bool NsLoad() {
    g_currentRenderType = DetectRenderType();
    
    if (g_currentRenderType == RenderType::Unknown) {
        return false; // Рендер не найден
    }

    bool hookResult = false;
    
    switch (g_currentRenderType) {
        case RenderType::DirectX9:
            hookResult = HookDirectX9();
            break;
        case RenderType::DirectX10:
            hookResult = HookDirectX10();
            break;
        case RenderType::DirectX11:
            hookResult = HookDirectX11();
            break;
        case RenderType::DirectX12:
            hookResult = HookDirectX12();
            break;
        case RenderType::OpenGL:
            hookResult = HookOpenGL();
            break;
        default:
            break;
    }

    return hookResult;
}

void NsUnload() {
    // Отцепляем хуки при выгрузке
    if (g_glHooked && o_wglSwapBuffers) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)o_wglSwapBuffers, hk_wglSwapBuffers);
        DetourTransactionCommit();
    }
    
    // Очистка других хуков...
    
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
