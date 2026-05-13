#include "ns_render_api.h"
#include "renderer_detector.h"
#include "../c_api/imgui_c_api.h"
#include <map>
#include <string>

// Глобальное состояние
static RendererType g_currentRenderer = RendererType::Unknown;
static bool g_initialized = false;
static int g_nextWindowId = 1;
static std::map<int, std::string> g_windowTitles;
static std::map<int, bool> g_windowClosed;

// Внутренняя функция инициализации бекенда
static bool InitializeBackend(RendererType type)
{
    // Здесь будет логика инициализации нужного бекенда ImGui
    // В зависимости от типа рендерера вызываем соответствующие функции инициализации
    
    switch (type)
    {
    case RendererType::DirectX9:
        // ImGui_ImplDX9_Init(...)
        break;
    case RendererType::DirectX10:
        // ImGui_ImplDX10_Init(...)
        break;
    case RendererType::DirectX11:
        // ImGui_ImplDX11_Init(...)
        break;
    case RendererType::DirectX12:
        // ImGui_ImplDX12_Init(...)
        break;
    case RendererType::OpenGL:
        // ImGui_ImplOpenGL3_Init(...)
        break;
    case RendererType::Vulkan:
        // ImGui_ImplVulkan_Init(...)
        break;
    default:
        return false;
    }
    
    return true;
}

extern "C" __declspec(dllexport) int NsInitialize()
{
    if (g_initialized)
        return 1; // Уже инициализировано

    // Детектируем рендерер хост-приложения
    g_currentRenderer = RendererDetector::Detect();
    
    if (g_currentRenderer == RendererType::Unknown)
    {
        return 0; // Не удалось определить рендерер
    }

    // Инициализируем ImGui контекст
    ImGui_CreateContext(NULL);
    
    // Инициализируем нужный бекенд
    if (!InitializeBackend(g_currentRenderer))
    {
        ImGui_DestroyContext(NULL);
        return 0;
    }

    g_initialized = true;
    return 1;
}

extern "C" __declspec(dllexport) NsRendererType NsGetRendererType()
{
    switch (g_currentRenderer)
    {
    case RendererType::DirectX9:  return NS_RENDERER_DIRECTX9;
    case RendererType::DirectX10: return NS_RENDERER_DIRECTX10;
    case RendererType::DirectX11: return NS_RENDERER_DIRECTX11;
    case RendererType::DirectX12: return NS_RENDERER_DIRECTX12;
    case RendererType::OpenGL:    return NS_RENDERER_OPENGL;
    case RendererType::Vulkan:    return NS_RENDERER_VULKAN;
    case RendererType::Software:  return NS_RENDERER_SOFTWARE;
    default:                      return NS_RENDERER_UNKNOWN;
    }
}

extern "C" __declspec(dllexport) const char* NsGetRendererName()
{
    return RendererDetector::GetRendererName(g_currentRenderer);
}

extern "C" __declspec(dllexport) void NsBeginFrame()
{
    if (!g_initialized)
        return;
    
    ImGui_NewFrame();
}

extern "C" __declspec(dllexport) void NsEndFrame()
{
    if (!g_initialized)
        return;
    
    ImGui_Render();
    // Здесь должен быть вызов функции рендеринга бекенда
    // Например: ImGui_ImplDX11_RenderDrawData(ImGui_GetDrawData());
}

extern "C" __declspec(dllexport) int NsCreateWindow(const char* title, float x, float y, float width, float height)
{
    if (!g_initialized)
        return -1;

    int windowId = g_nextWindowId++;
    
    ImGui_Begin(title, NULL, 0); // Простое окно без дополнительных флагов
    
    // Сохраняем состояние окна
    g_windowTitles[windowId] = title;
    g_windowClosed[windowId] = false;
    
    ImGui_End();
    
    return windowId;
}

extern "C" __declspec(dllexport) void NsDestroyWindow(int windowId)
{
    g_windowTitles.erase(windowId);
    g_windowClosed.erase(windowId);
}

extern "C" __declspec(dllexport) void NsSetWindowPosition(int windowId, float x, float y)
{
    // ImGui не позволяет напрямую установить позицию произвольного окна по ID
    // Это нужно делать в контексте Begin/End конкретного окна
    // Данная функция может быть реализована через сохранение состояния и применение в следующем фрейме
}

extern "C" __declspec(dllexport) void NsSetWindowSize(int windowId, float width, float height)
{
    // Аналогично позиции, требует реализации через сохранение состояния
}

extern "C" __declspec(dllexport) void NsSetWindowTitle(int windowId, const char* title)
{
    auto it = g_windowTitles.find(windowId);
    if (it != g_windowTitles.end())
    {
        it->second = title;
    }
}

extern "C" __declspec(dllexport) int NsIsWindowClosed(int windowId)
{
    auto it = g_windowClosed.find(windowId);
    if (it != g_windowClosed.end())
    {
        return it->second ? 1 : 0;
    }
    return 0;
}

extern "C" __declspec(dllexport) int NsIsWindowFocused(int windowId)
{
    // Требуется дополнительная реализация через ImGui_IsWindowFocused()
    return 0;
}

extern "C" __declspec(dllexport) void NsGetWindowPosition(int windowId, float* outX, float* outY)
{
    // Требуется реализация через ImGui_GetWindowPos()
    if (outX) *outX = 0.0f;
    if (outY) *outY = 0.0f;
}

extern "C" __declspec(dllexport) void NsGetWindowSize(int windowId, float* outWidth, float* outHeight)
{
    // Требуется реализация через ImGui_GetWindowSize()
    if (outWidth) *outWidth = 0.0f;
    if (outHeight) *outHeight = 0.0f;
}

extern "C" __declspec(dllexport) void NsText(int windowId, const char* text)
{
    ImGui_Text("%s", text);
}

extern "C" __declspec(dllexport) int NsButton(int windowId, const char* label)
{
    return ImGui_Button(label, 0, 0) ? 1 : 0;
}

extern "C" __declspec(dllexport) int NsCheckbox(int windowId, const char* label, int* value)
{
    bool boolValue = (*value != 0);
    int changed = ImGui_Checkbox(label, &boolValue) ? 1 : 0;
    *value = boolValue ? 1 : 0;
    return changed;
}

extern "C" __declspec(dllexport) int NsSliderFloat(int windowId, const char* label, float* value, float min, float max)
{
    return ImGui_SliderFloat(label, value, min, max, "%.3f", 1.0f) ? 1 : 0;
}

extern "C" __declspec(dllexport) int NsSliderInt(int windowId, const char* label, int* value, int min, int max)
{
    return ImGui_SliderInt(label, value, min, max, "%d") ? 1 : 0;
}

extern "C" __declspec(dllexport) int NsInputText(int windowId, const char* label, char* buffer, int bufferSize)
{
    return ImGui_InputText(label, buffer, bufferSize, 0, NULL, NULL) ? 1 : 0;
}

extern "C" __declspec(dllexport) void NsSeparator(int windowId)
{
    ImGui_Separator();
}

extern "C" __declspec(dllexport) void NsSpacing(int windowId)
{
    ImGui_Spacing();
}

extern "C" __declspec(dllexport) void NsClearWindows()
{
    g_windowTitles.clear();
    g_windowClosed.clear();
    g_nextWindowId = 1;
}

extern "C" __declspec(dllexport) void NsShutdown()
{
    if (!g_initialized)
        return;

    // Shutdown бекенда
    // ImGui_ImplXXX_Shutdown();
    
    ImGui_DestroyContext(NULL);
    
    g_initialized = false;
    g_currentRenderer = RendererType::Unknown;
    ClearWindows();
}
