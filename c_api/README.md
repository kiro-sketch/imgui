# ImGui C API для Windows

Эта директория содержит C-обёртки для библиотеки ImGui, которые позволяют использовать ImGui из языков, не поддерживающих C++ (например, C# через P/Invoke).

## Файлы

- `imgui_c_api.h` - Заголовочный файл с объявлениями всех C-функций
- `imgui_c_api.cpp` - Реализация C-обёрток

## Сборка

### Требования
- Windows 10 или выше
- Visual Studio 2022 или MSVC
- CMake 3.10+

### Команды сборки

```bash
# Создание статической библиотеки
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

## Использование в C#

Пример подключения к C# проекту:

```csharp
using System;
using System.Runtime.InteropServices;

public static class ImGuiNative
{
    private const string LibraryName = "imgui.lib";
    
    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr igCreateContext();
    
    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void igDestroyContext(IntPtr ctx);
    
    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool igBegin(string name, ref bool p_open, uint flags);
    
    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void igEnd();
    
    [DllImport(LibraryName, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool igButton(string label, ImVec2 size);
    
    // ... остальные функции
}

[StructLayout(LayoutKind.Sequential)]
public struct ImVec2
{
    public float x;
    public float y;
    
    public ImVec2(float x, float y)
    {
        this.x = x;
        this.y = y;
    }
}
```

## Поддерживаемые бекенды (Windows)

- **Win32** - Базовый бекенд для Windows API
- **DirectX 9** - Рендеринг через DX9
- **DirectX 10** - Рендеринг через DX10
- **DirectX 11** - Рендеринг через DX11
- **DirectX 12** - Рендеринг через DX12
- **Vulkan** - Рендеринг через Vulkan
- **OpenGL 3** - Рендеринг через OpenGL 3
- **GLFW** - Управление окнами через GLFW
- **SDL2** - Управление окнами через SDL2

## Архитектура

Ваша C++ библиотека должна:

1. Скомпилировать ImGui как статическую библиотеку с этим C API
2. При загрузке определить, какой бекенд использовать (DX11, DX12, Vulkan и т.д.)
3. Предоставить C# функции для инициализации и рендеринга через P/Invoke

Пример архитектуры:

```
YourCppLibrary.dll
├── ImGui Core (статическая линковка)
├── ImGui C API (статическая линковка)
├── Выбранный бекенд (DX11/DX12/Vulkan/etc)
└── Ваши C++ обёртки для C#
    └── Экспорт функций для P/Invoke
```

## Примечания

- Все функции используют соглашение о вызовах `cdecl`
- Строки передаются как UTF-8 `const char*`
- Булевы значения представлены как `int` (0 или 1)
- Указатели на ресурсы (texture IDs, device pointers) передаются как `void*`
