using System;
using System.Runtime.InteropServices;
using System.Text;

namespace ImGuiNET
{
    /// <summary>
    /// Полная C# обертка для ImGui C API через P/Invoke.
    /// Позволяет рисовать окна и элементы интерфейса в приложении,
    /// не зная конкретный рендер (DirectX 9/10/11/12 или OpenGL).
    /// </summary>
    public static class ImGuiOverlay
    {
        private const string DllName = "MyInjectLibrary.dll";

        #region Core Lifecycle

        /// <summary>
        /// Инициализация оверлея. Вызывать после загрузки DLL.
        /// </summary>
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
        /// Освобождение ресурсов и отключение оверлея.
        /// </summary>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_Shutdown")]
        public static extern void Shutdown();

        #endregion

        #region Window Management

        /// <summary>
        /// Рисование окна.
        /// </summary>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_DrawWindow")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool DrawWindow(
            [MarshalAs(UnmanagedType.LPStr)] string title,
            float x, float y, float w, float h,
            [MarshalAs(UnmanagedType.I1)] bool visible);

        #endregion

        #region Input Handling

        /// <summary>
        /// Проверка клика мыши.
        /// </summary>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Ns_IsMouseClicked")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool IsMouseClicked(int button);

        #endregion

        #region ImGui Core Context

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igCreateContext")]
        public static extern IntPtr CreateContext(IntPtr fontData);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igDestroyContext")]
        public static extern void DestroyContext(IntPtr ctx);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igSetCurrentContext")]
        public static extern void SetCurrentContext(IntPtr ctx);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igGetCurrentContext")]
        public static extern IntPtr GetCurrentContext();

        #endregion

        #region ImGui IO

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igGetIO")]
        public static extern IntPtr GetIO();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igSetMousePos")]
        public static extern void SetMousePos(float x, float y);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igSetMouseDown")]
        public static extern void SetMouseDown(int button, [MarshalAs(UnmanagedType.I1)] bool down);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igSetKey")]
        public static extern void SetKey(int key, [MarshalAs(UnmanagedType.I1)] bool down);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igAddInputCharacter")]
        public static extern void AddInputCharacter(uint c);

        #endregion

        #region ImGui Main Loop

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igNewFrame")]
        public static extern void NewFrame();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igRender")]
        public static extern void Render();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igGetDrawData")]
        public static extern IntPtr GetDrawData();

        #endregion

        #region ImGui Windows

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igBegin")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool Begin(string name, ref bool pOpen, int flags);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igBegin")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool Begin(string name, IntPtr pOpen, int flags);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igEnd")]
        public static extern void End();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igBeginChild")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool BeginChild(string strId, float w, float h, [MarshalAs(UnmanagedType.I1)] bool border, int flags);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igEndChild")]
        public static extern void EndChild();

        #endregion

        #region ImGui Basic Widgets

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igText")]
        public static extern void Text(string fmt);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igTextColored")]
        public static extern void TextColored(float r, float g, float b, float a, string fmt);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igButton")]
        public static extern void Button(string label, float w, float h);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igCheckbox")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool Checkbox(string label, ref bool v);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igSliderFloat")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool SliderFloat(string label, ref float v, float vMin, float vMax, string fmt);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igSliderInt")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool SliderInt(string label, ref int v, int vMin, int vMax, string fmt);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igInputText")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool InputText(string label, StringBuilder buf, int bufSize, int flags);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igInputFloat")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool InputFloat(string label, ref float v, float step, float stepFast, string fmt);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igInputInt")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool InputInt(string label, ref int v, int step, int stepFast, int flags);

        #endregion

        #region ImGui Colors

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igColorPicker3")]
        public static extern void ColorPicker3(string label, float[] col);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igColorPicker4")]
        public static extern void ColorPicker4(string label, float[] col, int flags);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igColorEdit3")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool ColorEdit3(string label, float[] col, int flags);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igColorEdit4")]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool ColorEdit4(string label, float[] col, int flags);

        #endregion

        #region ImGui Layout & Style

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igSetNextWindowSize")]
        public static extern void SetNextWindowSize(float w, float h, int cond);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igSetNextWindowPos")]
        public static extern void SetNextWindowPos(float x, float y, int cond);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igPushStyleColor")]
        public static extern void PushStyleColor(int idx, float r, float g, float b, float a);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igPopStyleColor")]
        public static extern void PopStyleColor(int count);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igPushStyleVar")]
        public static extern void PushStyleVar(int idx, float val);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igPushStyleVarVec2")]
        public static extern void PushStyleVarVec2(int idx, float x, float y);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igPopStyleVar")]
        public static extern void PopStyleVar(int count);

        #endregion

        #region ImGui Drawing (ImDrawList)

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igGetBackgroundDrawList")]
        public static extern IntPtr GetBackgroundDrawList();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igGetForegroundDrawList")]
        public static extern IntPtr GetForegroundDrawList();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ImDrawList_AddLine")]
        public static extern void ImDrawList_AddLine(IntPtr list, float x1, float y1, float x2, float y2, uint col, float thickness);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ImDrawList_AddRect")]
        public static extern void ImDrawList_AddRect(IntPtr list, float x1, float y1, float x2, float y2, uint col, float rounding, int flags, float thickness);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ImDrawList_AddRectFilled")]
        public static extern void ImDrawList_AddRectFilled(IntPtr list, float x1, float y1, float x2, float y2, uint col, float rounding, int flags);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ImDrawList_AddCircle")]
        public static extern void ImDrawList_AddCircle(IntPtr list, float cx, float cy, float radius, uint col, int numSegments, float thickness);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ImDrawList_AddCircleFilled")]
        public static extern void ImDrawList_AddCircleFilled(IntPtr list, float cx, float cy, float radius, uint col, int numSegments);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ImDrawList_AddText")]
        public static extern void ImDrawList_AddText(IntPtr list, float x, float y, uint col, string text);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "ImDrawList_AddBezierCubic")]
        public static extern void ImDrawList_AddBezierCubic(IntPtr list, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y, uint col, float thickness, int numSegments);

        #endregion

        #region ImGui Helpers

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igGetItemRectMin")]
        public static extern void GetItemRectMin(out float x, out float y);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igGetItemRectMax")]
        public static extern void GetItemRectMax(out float x, out float y);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "igGetItemRectSize")]
        public static extern void GetItemRectSize(out float x, out float y);

        #endregion

        #region High-Level Helpers

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
            // Используем низкоуровневый метод рисования текста через DrawList для простоты
            // или можно вызвать Text, если нужно внутри окна
            var drawList = GetForegroundDrawList();
            ImDrawList_AddText(drawList, x, y, 0xFFFFFFFF, text);
            EndFrame();
        }

        #endregion
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

    /// <summary>
    /// Флаги окон ImGui.
    /// </summary>
    [Flags]
    public enum ImGuiWindowFlags
    {
        None = 0,
        NoTitleBar = 1 << 0,
        NoResize = 1 << 1,
        NoMove = 1 << 2,
        NoScrollbar = 1 << 3,
        NoScrollWithMouse = 1 << 4,
        NoCollapse = 1 << 5,
        AlwaysAutoResize = 1 << 6,
        NoBackground = 1 << 7,
        NoSavedSettings = 1 << 8,
        NoMouseInputs = 1 << 9,
        MenuBar = 1 << 10,
        HorizontalScrollbar = 1 << 11,
        NoFocusOnAppearing = 1 << 12,
        NoBringToFrontOnFocus = 1 << 13,
        AlwaysVerticalScrollbar = 1 << 14,
        AlwaysHorizontalScrollbar = 1 << 15,
        NoNavInputs = 1 << 16,
        NoNavFocus = 1 << 17,
        UnsavedDocument = 1 << 18,
        NoNav = NoNavInputs | NoNavFocus,
        NoDecoration = NoTitleBar | NoResize | NoMove | NoCollapse | NoScrollbar | NoBackground,
        NoInputs = NoMouseInputs | NoNavInputs | NoNavFocus,
    }

    /// <summary>
    /// Условия позиционирования/размера окна.
    /// </summary>
    public enum ImGuiCond
    {
        None = 0,
        Always = 1 << 0,
        Once = 1 << 1,
        FirstUseEver = 1 << 2,
        Appearing = 1 << 3,
    }
}
