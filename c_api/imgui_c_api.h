// imgui_c_api.h - C API wrapper for ImGui static library (Windows only)
// This header exposes all ImGui functionality through C-compatible functions
// for use with C# P/Invoke and other foreign function interfaces.

#ifndef IMGUI_C_API_H
#define IMGUI_C_API_H

#ifdef _WIN32
    #define IMGUI_C_API_EXPORT __declspec(dllexport)
#else
    #define IMGUI_C_API_EXPORT
#endif

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Forward declarations for opaque types
// ============================================================================
typedef struct ImGuiContext ImGuiContext;
typedef struct ImGuiIO ImGuiIO;
typedef struct ImGuiStyle ImGuiStyle;
typedef struct ImGuiInputTextCallbackData ImGuiInputTextCallbackData;
typedef struct ImGuiSizeCallbackData ImGuiSizeCallbackData;
typedef struct ImGuiPayload ImGuiPayload;
typedef struct ImGuiStorage ImGuiStorage;
typedef struct ImGuiTextFilter ImGuiTextFilter;
typedef struct ImGuiTextBuffer ImGuiTextBuffer;

// ============================================================================
// Basic Types
// ============================================================================
typedef int ImGuiBool;
typedef unsigned int ImGuiID;
typedef int ImGuiCol;
typedef int ImGuiCond;
typedef int ImGuiDataType;
typedef int ImGuiDir;
typedef int ImGuiKey;
typedef int ImGuiMouseCursor;
typedef int ImGuiMouseButton;
typedef int ImGuiSortDirection;
typedef int ImGuiStyleVar;
typedef int ImGuiTableBgTarget;
typedef int ImGuiTabBarFlags;
typedef int ImGuiTabItemFlags;
typedef int ImGuiTreeNodeFlags;
typedef int ImGuiWindowFlags;
typedef int ImGuiHoveredFlags;
typedef int ImGuiFocusedFlags;
typedef int ImGuiPopupFlags;
typedef int ImGuiSelectableFlags;
typedef int ImGuiComboFlags;
typedef int ImGuiInputTextFlags;
typedef int ImGuiColorEditFlags;
typedef int ImGuiSliderFlags;
typedef int ImGuiDragDropFlags;
typedef int ImGuiDockNodeFlags;
typedef int ImGuiConfigFlags;
typedef int ImGuiBackendFlags;
typedef int ImGuiButtonFlags;
typedef int ImGuiChildFlags;
typedef int ImGuiItemFlags;
typedef int ImGuiAxis;
typedef int ImGuiPlotType;

typedef struct ImVec2 {
    float x, y;
} ImVec2;

typedef struct ImVec4 {
    float x, y, z, w;
} ImVec4;

// ============================================================================
// Core Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiContext* igCreateContext(void);
IMGUI_C_API_EXPORT void igDestroyContext(ImGuiContext* ctx);
IMGUI_C_API_EXPORT ImGuiContext* igGetCurrentContext(void);
IMGUI_C_API_EXPORT void igSetCurrentContext(ImGuiContext* ctx);

// ============================================================================
// IO Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiIO* igGetIO(void);
IMGUI_C_API_EXPORT ImGuiBool igIO_WantCaptureMouse(ImGuiIO* io);
IMGUI_C_API_EXPORT ImGuiBool igIO_WantCaptureKeyboard(ImGuiIO* io);
IMGUI_C_API_EXPORT ImGuiBool igIO_WantTextInput(ImGuiIO* io);
IMGUI_C_API_EXPORT float igIO_GetDeltaTime(ImGuiIO* io);
IMGUI_C_API_EXPORT void igIO_SetDisplaySize(ImGuiIO* io, float width, float height);
IMGUI_C_API_EXPORT void igIO_SetMousePos(ImGuiIO* io, float x, float y);
IMGUI_C_API_EXPORT void igIO_SetMouseDown(ImGuiIO* io, int button, ImGuiBool down);
IMGUI_C_API_EXPORT void igIO_SetMouseWheel(ImGuiIO* io, float wheel);
IMGUI_C_API_EXPORT void igIO_AddKeyEvent(ImGuiIO* io, ImGuiKey key, ImGuiBool down);
IMGUI_C_API_EXPORT void igIO_AddCharacterEvent(ImGuiIO* io, unsigned int c);

// ============================================================================
// Style Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiStyle* igGetStyle(void);
IMGUI_C_API_EXPORT void igStyleColorsDark(ImGuiStyle* dst);
IMGUI_C_API_EXPORT void igStyleColorsLight(ImGuiStyle* dst);
IMGUI_C_API_EXPORT void igStyleColorsClassic(ImGuiStyle* dst);

// ============================================================================
// Main Demo Function
// ============================================================================
IMGUI_C_API_EXPORT void igShowDemoWindow(ImGuiBool* p_open);

// ============================================================================
// Window Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igBegin(const char* name, ImGuiBool* p_open, ImGuiWindowFlags flags);
IMGUI_C_API_EXPORT void igEnd(void);
IMGUI_C_API_EXPORT ImGuiBool igBeginChild(const char* str_id, const ImVec2 size, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags);
IMGUI_C_API_EXPORT void igEndChild(void);

// ============================================================================
// Content Functions
// ============================================================================
IMGUI_C_API_EXPORT void igSeparator(void);
IMGUI_C_API_EXPORT void igSameLine(float offset_from_start_x, float spacing);
IMGUI_C_API_EXPORT void igNewLine(void);
IMGUI_C_API_EXPORT void igSpacing(void);
IMGUI_C_API_EXPORT void igDummy(const ImVec2 size);
IMGUI_C_API_EXPORT void igIndent(float indent_w);
IMGUI_C_API_EXPORT void igUnindent(float indent_w);

// ============================================================================
// Text Functions
// ============================================================================
IMGUI_C_API_EXPORT void igText(const char* fmt);
IMGUI_C_API_EXPORT void igTextColored(const ImVec4 col, const char* fmt);
IMGUI_C_API_EXPORT void igTextDisabled(const char* fmt);
IMGUI_C_API_EXPORT void igTextWrapped(const char* fmt);
IMGUI_C_API_EXPORT void igLabelText(const char* label, const char* fmt);
IMGUI_C_API_EXPORT void igBulletText(const char* fmt);

// ============================================================================
// Button Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igButton(const char* label, const ImVec2 size);
IMGUI_C_API_EXPORT ImGuiBool igSmallButton(const char* label);
IMGUI_C_API_EXPORT ImGuiBool igInvisibleButton(const char* str_id, const ImVec2 size, ImGuiButtonFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igArrowButton(const char* str_id, ImGuiDir dir);
IMGUI_C_API_EXPORT void igImage(void* user_texture_id, const ImVec2 size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 tint_col, const ImVec4 border_col);
IMGUI_C_API_EXPORT ImGuiBool igImageButton(const char* str_id, void* user_texture_id, const ImVec2 image_size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 bg_col, const ImVec4 tint_col);

// ============================================================================
// Checkbox/Radio/Slider Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igCheckbox(const char* label, ImGuiBool* v);
IMGUI_C_API_EXPORT ImGuiBool igRadioButton(const char* label, ImGuiBool active);
IMGUI_C_API_EXPORT ImGuiBool igSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igSliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igDragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igDragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags);

// ============================================================================
// Combo/ListBox Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igBeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags);
IMGUI_C_API_EXPORT void igEndCombo(void);
IMGUI_C_API_EXPORT ImGuiBool igCombo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items);
IMGUI_C_API_EXPORT ImGuiBool igBeginListBox(const char* label, const ImVec2 size);
IMGUI_C_API_EXPORT void igEndListBox(void);

// ============================================================================
// Input Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igInputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2 size, ImGuiInputTextFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igInputFloat(const char* label, float* v, float step, float step_fast, const char* format, ImGuiInputTextFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igInputInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags);

// ============================================================================
// Color Editor Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igColorPicker4(const char* label, float col[4], const float* ref_col, ImGuiColorEditFlags flags);

// ============================================================================
// Tree/TreeNode Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igTreeNode(const char* label);
IMGUI_C_API_EXPORT ImGuiBool igTreeNodeEx(const char* label, ImGuiTreeNodeFlags flags);
IMGUI_C_API_EXPORT void igTreePop(void);
IMGUI_C_API_EXPORT float igGetTreeNodeToLabelSpacing(void);

// ============================================================================
// Collapsing Header Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igCollapsingHeader(const char* label, ImGuiTreeNodeFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igCollapsingHeaderWithOpen(const char* label, ImGuiBool* p_visible, ImGuiTreeNodeFlags flags);

// ============================================================================
// Selectable Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igSelectable(const char* label, ImGuiBool selected, ImGuiSelectableFlags flags, const ImVec2 size);

// ============================================================================
// Menu Functions
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igBeginMainMenuBar(void);
IMGUI_C_API_EXPORT void igEndMainMenuBar(void);
IMGUI_C_API_EXPORT ImGuiBool igBeginMenuBar(void);
IMGUI_C_API_EXPORT void igEndMenuBar(void);
IMGUI_C_API_EXPORT ImGuiBool igBeginMenu(const char* label, ImGuiBool enabled);
IMGUI_C_API_EXPORT void igEndMenu(void);
IMGUI_C_API_EXPORT ImGuiBool igMenuItem(const char* label, const char* shortcut, ImGuiBool selected, ImGuiBool enabled);

// ============================================================================
// Tooltip Functions
// ============================================================================
IMGUI_C_API_EXPORT void igBeginTooltip(void);
IMGUI_C_API_EXPORT void igEndTooltip(void);
IMGUI_C_API_EXPORT void igSetTooltip(const char* fmt);

// ============================================================================
// Popup Functions
// ============================================================================
IMGUI_C_API_EXPORT void igOpenPopup(const char* str_id, ImGuiPopupFlags popup_flags);
IMGUI_C_API_EXPORT ImGuiBool igBeginPopup(const char* str_id, ImGuiWindowFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igBeginPopupModal(const char* name, ImGuiBool* p_open, ImGuiWindowFlags flags);
IMGUI_C_API_EXPORT void igEndPopup(void);
IMGUI_C_API_EXPORT void igCloseCurrentPopup(void);

// ============================================================================
// Clipping Functions
// ============================================================================
IMGUI_C_API_EXPORT void igPushClipRect(const ImVec2 clip_rect_min, const ImVec2 clip_rect_max, ImGuiBool intersect_with_current_clip_rect);
IMGUI_C_API_EXPORT void igPopClipRect(void);

// ============================================================================
// Focus Functions
// ============================================================================
IMGUI_C_API_EXPORT void igSetItemDefaultFocus(void);
IMGUI_C_API_EXPORT void igSetKeyboardFocusHere(int offset);

// ============================================================================
// Item/Widget Utilities
// ============================================================================
IMGUI_C_API_EXPORT ImGuiBool igIsItemHovered(ImGuiHoveredFlags flags);
IMGUI_C_API_EXPORT ImGuiBool igIsItemActive(void);
IMGUI_C_API_EXPORT ImGuiBool igIsItemFocused(void);
IMGUI_C_API_EXPORT ImGuiBool igIsItemClicked(ImGuiMouseButton mouse_button);
IMGUI_C_API_EXPORT ImGuiBool igIsItemVisible(void);
IMGUI_C_API_EXPORT ImGuiBool igIsItemEdited(void);
IMGUI_C_API_EXPORT ImGuiBool igIsItemActivated(void);
IMGUI_C_API_EXPORT ImGuiBool igIsItemDeactivated(void);
IMGUI_C_API_EXPORT ImGuiBool igIsItemDeactivatedAfterEdit(void);
IMGUI_C_API_EXPORT ImGuiBool igIsAnyItemHovered(void);
IMGUI_C_API_EXPORT ImGuiBool igIsAnyItemActive(void);
IMGUI_C_API_EXPORT ImGuiBool igIsAnyItemFocused(void);
IMGUI_C_API_EXPORT ImVec2 igGetItemRectMin(void);
IMGUI_C_API_EXPORT ImVec2 igGetItemRectMax(void);
IMGUI_C_API_EXPORT ImVec2 igGetItemRectSize(void);

// ============================================================================
// Viewport Functions
// ============================================================================
IMGUI_C_API_EXPORT void igSetNextWindowPos(const ImVec2 pos, ImGuiCond cond, const ImVec2 pivot);
IMGUI_C_API_EXPORT void igSetNextWindowSize(const ImVec2 size, ImGuiCond cond);
IMGUI_C_API_EXPORT void igSetNextWindowSizeConstraints(const ImVec2 size_min, const ImVec2 size_max);
IMGUI_C_API_EXPORT void igSetNextWindowContentSize(const ImVec2 size);
IMGUI_C_API_EXPORT void igSetNextWindowCollapsed(ImGuiBool collapsed, ImGuiCond cond);
IMGUI_C_API_EXPORT void igSetNextWindowFocus(void);
IMGUI_C_API_EXPORT void igSetNextWindowBgAlpha(float alpha);

// ============================================================================
// Drawing Functions
// ============================================================================
IMGUI_C_API_EXPORT void* igGetBackgroundDrawList(void);
IMGUI_C_API_EXPORT void* igGetForegroundDrawList(void);
IMGUI_C_API_EXPORT void igAddLine(void* draw_list, const ImVec2 p1, const ImVec2 p2, unsigned int col, float thickness);
IMGUI_C_API_EXPORT void igAddRect(void* draw_list, const ImVec2 p_min, const ImVec2 p_max, unsigned int col, float rounding, int rounding_corners, float thickness);
IMGUI_C_API_EXPORT void igAddRectFilled(void* draw_list, const ImVec2 p_min, const ImVec2 p_max, unsigned int col, float rounding, int rounding_corners);
IMGUI_C_API_EXPORT void igAddCircle(void* draw_list, const ImVec2 center, float radius, unsigned int col, int num_segments, float thickness);
IMGUI_C_API_EXPORT void igAddCircleFilled(void* draw_list, const ImVec2 center, float radius, unsigned int col, int num_segments);
IMGUI_C_API_EXPORT void igAddText(void* draw_list, const ImVec2 pos, unsigned int col, const char* text_begin);
IMGUI_C_API_EXPORT void igAddBezierCubic(void* draw_list, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, unsigned int col, float thickness, int num_segments);

// ============================================================================
// Memory Allocation
// ============================================================================
IMGUI_C_API_EXPORT void* igMemAlloc(size_t size);
IMGUI_C_API_EXPORT void igMemFree(void* ptr);

// ============================================================================
// Platform Dependent Backends (Windows-specific)
// ============================================================================

// Win32 Backend
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplWin32_Init(void* hwnd);
IMGUI_C_API_EXPORT void ImGui_ImplWin32_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplWin32_NewFrame(void);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplWin32_WndProcHandler(void* hwnd, unsigned int msg, uint64_t wParam, int64_t lParam);
#endif

// DirectX 9 Backend
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplDX9_Init(void* device);
IMGUI_C_API_EXPORT void ImGui_ImplDX9_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX9_NewFrame(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX9_RenderDrawData(void* draw_data);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplDX9_CreateDeviceObjects(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX9_InvalidateDeviceObjects(void);
#endif

// DirectX 10 Backend
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplDX10_Init(void* device, int render_target_format);
IMGUI_C_API_EXPORT void ImGui_ImplDX10_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX10_NewFrame(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX10_RenderDrawData(void* draw_data);
IMGUI_C_API_EXPORT void ImGui_ImplDX10_InvalidateDeviceObjects(void);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplDX10_CreateDeviceObjects(void);
#endif

// DirectX 11 Backend
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplDX11_Init(void* device, void* context);
IMGUI_C_API_EXPORT void ImGui_ImplDX11_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX11_NewFrame(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX11_RenderDrawData(void* draw_data);
IMGUI_C_API_EXPORT void ImGui_ImplDX11_InvalidateDeviceObjects(void);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplDX11_CreateDeviceObjects(void);
#endif

// DirectX 12 Backend
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplDX12_Init(void* device, int num_frames_in_flight, void* descriptor_heap, void* cpu_descriptor_handle, void* gpu_descriptor_handle);
IMGUI_C_API_EXPORT void ImGui_ImplDX12_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX12_NewFrame(void);
IMGUI_C_API_EXPORT void ImGui_ImplDX12_RenderDrawData(void* draw_data, void* command_list);
IMGUI_C_API_EXPORT void ImGui_ImplDX12_InvalidateDeviceObjects(void);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplDX12_CreateDeviceObjects(void);
#endif

// Vulkan Backend (Windows)
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplVulkan_Init(void* init_info);
IMGUI_C_API_EXPORT void ImGui_ImplVulkan_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplVulkan_NewFrame(void);
IMGUI_C_API_EXPORT void ImGui_ImplVulkan_RenderDrawData(void* draw_data, void* command_buffer);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplVulkan_CreateFontsTexture(void);
IMGUI_C_API_EXPORT void ImGui_ImplVulkan_DestroyFontUploadObjects(void);
IMGUI_C_API_EXPORT void ImGui_ImplVulkan_SetMinImageCount(uint32_t min_image_count);
#endif

// OpenGL 3 Backend (Windows)
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplOpenGL3_Init(const char* glsl_version);
IMGUI_C_API_EXPORT void ImGui_ImplOpenGL3_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplOpenGL3_NewFrame(void);
IMGUI_C_API_EXPORT void ImGui_ImplOpenGL3_RenderDrawData(void* draw_data);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplOpenGL3_CreateFontsTexture(void);
IMGUI_C_API_EXPORT void ImGui_ImplOpenGL3_DestroyFontsTexture(void);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplOpenGL3_CreateDeviceObjects(void);
IMGUI_C_API_EXPORT void ImGui_ImplOpenGL3_DestroyDeviceObjects(void);
#endif

// GLFW Backend (Windows)
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplGlfw_InitForOpenGL(void* window, ImGuiBool install_callbacks);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplGlfw_InitForVulkan(void* window, ImGuiBool install_callbacks);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplGlfw_InitForD3D(void* window, ImGuiBool install_callbacks);
IMGUI_C_API_EXPORT void ImGui_ImplGlfw_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplGlfw_NewFrame(void);
#endif

// SDL2 Backend (Windows)
#ifdef _WIN32
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplSDL2_InitForD3D(void* window);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplSDL2_InitForOpenGL(void* window, void* gl_context);
IMGUI_C_API_EXPORT ImGuiBool ImGui_ImplSDL2_InitForVulkan(void* window);
IMGUI_C_API_EXPORT void ImGui_ImplSDL2_Shutdown(void);
IMGUI_C_API_EXPORT void ImGui_ImplSDL2_NewFrame(void);
#endif

// ============================================================================
// Version Information
// ============================================================================
IMGUI_C_API_EXPORT const char* igGetVersion(void);

#ifdef __cplusplus
}
#endif

#endif // IMGUI_C_API_H
