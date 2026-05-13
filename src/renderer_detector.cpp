#include "renderer_detector.h"
#include <vector>
#include <cstring>

bool RendererDetector::IsModuleLoaded(const char* moduleName)
{
    return GetModuleHandleA(moduleName) != NULL;
}

RendererType RendererDetector::DetectViaModules()
{
    // Проверка DirectX 12
    if (IsModuleLoaded("d3d12.dll"))
        return RendererType::DirectX12;

    // Проверка DirectX 11
    if (IsModuleLoaded("d3d11.dll"))
        return RendererType::DirectX11;

    // Проверка DirectX 10
    if (IsModuleLoaded("d3d10.dll"))
        return RendererType::DirectX10;

    // Проверка DirectX 9
    if (IsModuleLoaded("d3d9.dll"))
        return RendererType::DirectX9;

    // Проверка OpenGL
    if (IsModuleLoaded("opengl32.dll"))
        return RendererType::OpenGL;

    // Проверка Vulkan
    if (IsModuleLoaded("vulkan-1.dll"))
        return RendererType::Vulkan;

    return RendererType::Unknown;
}

const char* RendererDetector::GetRendererName(RendererType type)
{
    switch (type)
    {
    case RendererType::DirectX9:  return "DirectX 9";
    case RendererType::DirectX10: return "DirectX 10";
    case RendererType::DirectX11: return "DirectX 11";
    case RendererType::DirectX12: return "DirectX 12";
    case RendererType::OpenGL:    return "OpenGL";
    case RendererType::Vulkan:    return "Vulkan";
    case RendererType::Software:  return "Software";
    default:                      return "Unknown";
    }
}

RendererType RendererDetector::Detect()
{
    RendererType type = DetectViaModules();
    
    if (type == RendererType::Unknown)
    {
        // Fallback: можно попробовать другие методы детекции
        // Например, анализ оконных классов или перехват вызовов
        type = RendererType::Software;
    }

    return type;
}
