#pragma once

#include <windows.h>
#include <string>

// Перечисление поддерживаемых рендереров
enum class RendererType
{
    Unknown = 0,
    DirectX9,
    DirectX10,
    DirectX11,
    DirectX12,
    OpenGL,
    Vulkan,
    Software // Для отладки или fallback
};

class RendererDetector
{
public:
    static RendererType Detect();
    static const char* GetRendererName(RendererType type);

private:
    static bool IsModuleLoaded(const char* moduleName);
    static RendererType DetectViaModules();
    // Можно добавить детекцию через перехват вызовов (Hooking), если модулей недостаточно
};
