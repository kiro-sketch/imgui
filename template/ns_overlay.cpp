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
    // Получаем адрес EndScene через виртуальную таблицу
    // Это упрощённый пример, в реальности нужно создать устройство или найти существующее
    HMODULE hD3D9 = GetModuleHandleA("d3d9.dll");
    if (!hD3D9) return false;

    // В реальном проекте здесь будет более сложная логика поиска vtable
    // Для примера просто ставим заглушку
    g_d3d9Hooked = true;
    return true;
}

static bool HookDirectX10() {
    HMODULE hD3D10 = GetModuleHandleA("d3d10.dll");
    if (!hD3D10) hD3D10 = GetModuleHandleA("d3d10_1.dll");
    if (!hD3D10) return false;

    g_d3d10Hooked = true;
    return true;
}

static bool HookDirectX11() {
    HMODULE hD3D11 = GetModuleHandleA("d3d11.dll");
    if (!hD3D11) return false;

    g_d3d11Hooked = true;
    return true;
}

static bool HookDirectX12() {
    HMODULE hDXGI = GetModuleHandleA("dxgi.dll");
    HMODULE hD3D12 = GetModuleHandleA("d3d12.dll");
    if (!hDXGI || !hD3D12) return false;

    g_d3d12Hooked = true;
    return true;
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
