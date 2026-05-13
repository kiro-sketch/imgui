using System;
using System.Runtime.InteropServices;

namespace ImGuiNET
{
    /// <summary>
    /// C# обертка для ImGui C API через P/Invoke.
    /// Позволяет рисовать окна и элементы интерфейса в приложении,
    /// не зная конкретный рендер (DirectX 9/10/11/12 или OpenGL).
    /// </summary>
    public static class ImGuiOverlay
    {
        private const string DllName = "MyInjectLibrary.dll";

        /// <summary>
        /// Инициализация оверлея. Вызывать после загрузки DLL.
        /// </summary>
        /// <returns>True если успешно</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_Initialize")]
        public static extern bool Initialize();

        /// <summary>
        /// Начало кадра. Вызывать перед рисованием элементов.
        /// </summary>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_BeginFrame")]
        public static extern void BeginFrame();

        /// <summary>
        /// Конец кадра. Вызывать после рисования элементов.
        /// </summary>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_EndFrame")]
        public static extern void EndFrame();

        /// <summary>
        /// Рисование окна.
        /// </summary>
        /// <param name="title">Заголовок окна (UTF-8)</param>
        /// <param name="x">Позиция X</param>
        /// <param name="y">Позиция Y</param>
        /// <param name="w">Ширина</param>
        /// <param name="h">Высота</param>
        /// <param name="visible">Видимо ли окно</param>
        /// <returns>True если окно активно (не закрыто пользователем)</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_DrawWindow")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool DrawWindow(
            [MarshalAs(UnmanagedType.LPStr)] string title,
            float x, float y, float w, float h,
            [MarshalAs(UnmanagedType.I1)] bool visible);

        /// <summary>
        /// Рисование текста в указанной позиции.
        /// </summary>
        /// <param name="text">Текст (UTF-8)</param>
        /// <param name="x">Позиция X</param>
        /// <param name="y">Позиция Y</param>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_DrawText")]
        public static extern void DrawText(
            [MarshalAs(UnmanagedType.LPStr)] string text,
            float x, float y);

        /// <summary>
        /// Проверка клика мыши.
        /// </summary>
        /// <param name="button">Кнопка мыши (0=левая, 1=правая, 2=средняя)</param>
        /// <returns>True если кнопка была нажата</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_IsMouseClicked")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool IsMouseClicked(int button);

        /// <summary>
        /// Освобождение ресурсов и отключение оверлея.
        /// </summary>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_Shutdown")]
        public static extern void Shutdown();

        // ==========================================
        // Вспомогательные методы для удобного использования
        // ==========================================

        /// <summary>
        /// Полный цикл отрисовки окна (BeginFrame -> Draw -> EndFrame).
        /// </summary>
        public static bool RenderWindow(string title, float x, float y, float w, float h, bool visible)
        {
            if (!visible) return false;

            BeginFrame();
            bool isActive = DrawWindow(title, x, y, w, h, visible);
            EndFrame();

            return isActive;
        }

        /// <summary>
        /// Отрисовка текста с автоматическим управлением кадром.
        /// </summary>
        public static void RenderText(string text, float x, float y)
        {
            BeginFrame();
            DrawText(text, x, y);
            EndFrame();
        }
    }

    /// <summary>
    /// Типы рендеров, которые могут быть определены библиотекой.
    /// </summary>
    public enum RenderType
    {
        Unknown = 0,
        DirectX9 = 1,
        DirectX10 = 2,
        DirectX11 = 3,
        DirectX12 = 4,
        OpenGL = 5
    }
}
