# NS Render API - C# интеграция

Этот документ описывает, как использовать NS Render API из C# кода через P/Invoke.

## Архитектура

```
┌─────────────────┐
│   C# Библиотека  │
│  (рисует окна)  │
└────────┬────────┘
         │ P/Invoke
         ▼
┌─────────────────┐
│ ns_render_api.dll│ ← Автодетект рендерера (DX9/10/11/12, OpenGL, Vulkan)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ ImGui + Бекенды  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Хост-приложение  │ (DirectX/OpenGL/Vulkan)
└─────────────────┘
```

## Пример использования в C#

```csharp
using System;
using System.Runtime.InteropServices;
using System.Text;

public enum RendererType
{
    Unknown = 0,
    DirectX9,
    DirectX10,
    DirectX11,
    DirectX12,
    OpenGL,
    Vulkan,
    Software
}

public static class NsRenderApi
{
    private const string DllName = "ns_render_api.dll";

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int NsInitialize();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern RendererType NsGetRendererType();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr NsGetRendererName();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void NsBeginFrame();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void NsEndFrame();

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int NsCreateWindow(
        [MarshalAs(UnmanagedType.LPStr)] string title,
        float x, float y, float width, float height);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void NsDestroyWindow(int windowId);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int NsButton(int windowId, [MarshalAs(UnmanagedType.LPStr)] string label);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int NsCheckbox(
        int windowId,
        [MarshalAs(UnmanagedType.LPStr)] string label,
        ref int value);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int NsSliderFloat(
        int windowId,
        [MarshalAs(UnmanagedType.LPStr)] string label,
        ref float value,
        float min,
        float max);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void NsShutdown();
}

// Пример использования
public class Example
{
    public static void Main()
    {
        // Инициализация
        if (NsRenderApi.NsInitialize() == 0)
        {
            Console.WriteLine("Не удалось инициализировать NS Render API");
            return;
        }

        // Получение информации о рендерере
        var rendererType = NsRenderApi.NsGetRendererType();
        var rendererNamePtr = NsRenderApi.NsGetRendererName();
        var rendererName = Marshal.PtrToStringAnsi(rendererNamePtr);
        
        Console.WriteLine($"Рендерер: {rendererType} ({rendererName})");

        // Создание окна
        int windowId = NsRenderApi.NsCreateWindow("Мое окно", 100, 100, 400, 300);
        
        bool checkboxValue = false;
        float sliderValue = 0.5f;

        // Главный цикл
        while (true)
        {
            // Начало фрейма
            NsRenderApi.NsBeginFrame();

            // Кнопка
            if (NsRenderApi.NsButton(windowId, "Нажми меня") == 1)
            {
                Console.WriteLine("Кнопка нажата!");
            }

            // Чекбокс
            int cv = checkboxValue ? 1 : 0;
            if (NsRenderApi.NsCheckbox(windowId, "Включить опцию", ref cv) == 1)
            {
                checkboxValue = cv != 0;
            }

            // Слайдер
            if (NsRenderApi.NsSliderFloat(windowId, "Значение", ref sliderValue, 0.0f, 1.0f) == 1)
            {
                Console.WriteLine($"Новое значение: {sliderValue}");
            }

            // Конец фрейма
            NsRenderApi.NsEndFrame();

            // Проверка закрытия окна
            if (NsRenderApi.NsIsWindowClosed(windowId) == 1)
                break;

            // Небольшая задержка для снижения нагрузки на CPU
            System.Threading.Thread.Sleep(16);
        }

        // Завершение работы
        NsRenderApi.NsShutdown();
    }
}
```

## Интеграция с вашей DLL

Ваша текущая DLL должна:

1. Вызвать `NsInitialize()` в `DllMain` при `DLL_PROCESS_ATTACH`
2. Предоставить функции-обёртки для C# (или экспортировать напрямую из ns_render_api.dll)
3. Вызывать `NsBeginFrame()` и `NsEndFrame()` в нужные моменты рендеринга

### Пример модификации вашей DLL:

```cpp
#include "src/ns_render_api.h"

extern "C" __declspec(dllexport) int MyLib_Initialize()
{
    return NsInitialize();
}

extern "C" __declspec(dllexport) void MyLib_BeginFrame()
{
    NsBeginFrame();
}

extern "C" __declspec(dllexport) void MyLib_EndFrame()
{
    NsEndFrame();
}

// ... и так далее для всех необходимых функций
```

## Сборка

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

После сборки вы получите:
- `lib/imgui.lib` - статическая библиотека ImGui
- `lib/imgui_c_api.lib` - статическая библиотека C API
- `bin/ns_render_api.dll` - динамическая библиотека для C#
- `include/ns_render/` - заголовочные файлы

## Важные заметки

1. **Безопасность потоков**: ImGui не является потокобезопасным. Все вызовы API должны происходить из того же потока, где был создан контекст рендеринга.

2. **Жизненный цикл**: Убедитесь, что `NsInitialize()` вызывается после инициализации графического устройства хост-приложения, но до первого кадра рендеринга.

3. **Производительность**: Для лучшей производительности вызывайте `NsBeginFrame()` и `NsEndFrame()` в цикле рендеринга хост-приложения.
