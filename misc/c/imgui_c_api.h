// dear imgui - C API wrapper
// This file provides C-compatible wrappers for the Dear ImGui C++ API
// Useful for linking with Dear ImGui as a static library from C or other languages

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations (actual structures are defined in imgui.h)
typedef struct ImGuiContext ImGuiContext;
typedef struct ImGuiIO ImGuiIO;
typedef struct ImGuiStyle ImGuiStyle;
typedef struct ImGuiPlatformIO ImGuiPlatformIO;
typedef struct ImDrawData ImDrawData;
typedef struct ImDrawList ImDrawList;
typedef struct ImFont ImFont;
typedef struct ImFontAtlas ImFontAtlas;
typedef struct ImVec2 ImVec2;
typedef struct ImVec4 ImVec4;
typedef struct ImColor ImColor;

// Basic types
typedef uint32_t ImGuiID;
typedef uint32_t ImU32;

// Note: ImVec2 and ImVec4 are defined in imgui.h.
// When including this header after imgui.h, those definitions will be used.
// For standalone C usage without imgui.h, uncomment the definitions below:
/*
struct ImVec2 { float x, y; };
struct ImVec4 { float x, y, z, w; };
*/

//-----------------------------------------------------------------------------
// Context creation and access
//-----------------------------------------------------------------------------
IMGUI_API ImGuiContext* igCreateContext(ImFontAtlas* shared_font_atlas);
IMGUI_API void          igDestroyContext(ImGuiContext* ctx);
IMGUI_API ImGuiContext* igGetCurrentContext(void);
IMGUI_API void          igSetCurrentContext(ImGuiContext* ctx);

//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
IMGUI_API ImGuiIO*      igGetIO(void);
IMGUI_API ImGuiPlatformIO* igGetPlatformIO(void);
IMGUI_API ImGuiStyle*   igGetStyle(void);
IMGUI_API void          igNewFrame(void);
IMGUI_API void          igEndFrame(void);
IMGUI_API void          igRender(void);
IMGUI_API ImDrawData*   igGetDrawData(void);

//-----------------------------------------------------------------------------
// Demo, Debug, Information
//-----------------------------------------------------------------------------
IMGUI_API void          igShowDemoWindow(int* p_open);
IMGUI_API void          igShowMetricsWindow(int* p_open);
IMGUI_API void          igShowDebugLogWindow(int* p_open);
IMGUI_API void          igShowIDStackToolWindow(int* p_open);
IMGUI_API void          igShowAboutWindow(int* p_open);
IMGUI_API void          igShowStyleEditor(ImGuiStyle* ref);
IMGUI_API int           igShowStyleSelector(const char* label);
IMGUI_API void          igShowFontSelector(const char* label);
IMGUI_API void          igShowUserGuide(void);
IMGUI_API const char*   igGetVersion(void);

//-----------------------------------------------------------------------------
// Styles
//-----------------------------------------------------------------------------
IMGUI_API void          igStyleColorsDark(ImGuiStyle* dst);
IMGUI_API void          igStyleColorsLight(ImGuiStyle* dst);
IMGUI_API void          igStyleColorsClassic(ImGuiStyle* dst);

//-----------------------------------------------------------------------------
// Windows
//-----------------------------------------------------------------------------
IMGUI_API int           igBegin(const char* name, int* p_open, int flags);
IMGUI_API void          igEnd(void);

//-----------------------------------------------------------------------------
// Child Windows
//-----------------------------------------------------------------------------
IMGUI_API int           igBeginChild_Str(const char* str_id, ImVec2 size, int child_flags, int window_flags);
IMGUI_API int           igBeginChild_ID(ImGuiID id, ImVec2 size, int child_flags, int window_flags);
IMGUI_API void          igEndChild(void);

//-----------------------------------------------------------------------------
// Window state
//-----------------------------------------------------------------------------
IMGUI_API int           igIsWindowAppearing(void);
IMGUI_API int           igIsWindowCollapsed(void);
IMGUI_API int           igIsWindowFocused(int flags);
IMGUI_API int           igIsWindowHovered(int flags);
IMGUI_API ImDrawList*   igGetWindowDrawList(void);
IMGUI_API ImVec2        igGetWindowPos(void);
IMGUI_API ImVec2        igGetWindowSize(void);
IMGUI_API float         igGetWindowWidth(void);
IMGUI_API float         igGetWindowHeight(void);

//-----------------------------------------------------------------------------
// Window position
//-----------------------------------------------------------------------------
IMGUI_API void          igSetNextWindowPos(ImVec2 pos, int cond, ImVec2 pivot);
IMGUI_API void          igSetNextWindowSize(ImVec2 size, int cond);
IMGUI_API void          igSetNextWindowContentSize(ImVec2 size);
IMGUI_API void          igSetNextWindowCollapsed(int collapsed, int cond);
IMGUI_API void          igSetNextWindowFocus(void);
IMGUI_API void          igSetNextWindowBgAlpha(float alpha);

//-----------------------------------------------------------------------------
// Content region
//-----------------------------------------------------------------------------
IMGUI_API ImVec2        igGetContentRegionAvail(void);
IMGUI_API ImVec2        igGetCursorScreenPos(void);
IMGUI_API void          igSetCursorScreenPos(ImVec2 pos);

//-----------------------------------------------------------------------------
// Parameters stacks (shared)
//-----------------------------------------------------------------------------
IMGUI_API void          igPushFont(ImFont* font, float font_size_base_unscaled);
IMGUI_API void          igPopFont(void);
IMGUI_API void          igPushStyleColor_U32(int idx, ImU32 col);
IMGUI_API void          igPushStyleColor_Vec4(int idx, ImVec4 col);
IMGUI_API void          igPopStyleColor(int count);
IMGUI_API void          igPushStyleVar_Float(int idx, float val);
IMGUI_API void          igPushStyleVar_Vec2(int idx, ImVec2 val);
IMGUI_API void          igPopStyleVar(int count);

//-----------------------------------------------------------------------------
// Style read access
//-----------------------------------------------------------------------------
IMGUI_API ImU32         igGetColorU32_Col(int idx, float alpha_mul);
IMGUI_API ImU32         igGetColorU32_Vec4(ImVec4 col);
IMGUI_API ImVec4        igGetStyleColorVec4(int idx);

//-----------------------------------------------------------------------------
// Fonts
//-----------------------------------------------------------------------------
IMGUI_API ImFont*       igGetFont(void);
IMGUI_API float         igGetFontSize(void);
IMGUI_API ImVec2        igGetFontTexUvWhitePixel(void);

//-----------------------------------------------------------------------------
// Text
//-----------------------------------------------------------------------------
IMGUI_API void          igTextUnformatted(const char* text, const char* text_end);
IMGUI_API void          igText(const char* fmt, ...);
IMGUI_API void          igTextColored(ImVec4 col, const char* fmt, ...);
IMGUI_API void          igTextDisabled(const char* fmt, ...);
IMGUI_API void          igTextWrapped(const char* fmt, ...);
IMGUI_API void          igLabelText(const char* label, const char* fmt, ...);
IMGUI_API void          igBulletText(const char* fmt, ...);

//-----------------------------------------------------------------------------
// Buttons
//-----------------------------------------------------------------------------
IMGUI_API int           igButton(const char* label, ImVec2 size);
IMGUI_API int           igSmallButton(const char* label);
IMGUI_API int           igInvisibleButton(const char* str_id, ImVec2 size, int flags);
IMGUI_API int           igArrowButton(const char* str_id, int dir);

//-----------------------------------------------------------------------------
// Inputs
//-----------------------------------------------------------------------------
IMGUI_API int           igIsItemHovered(int flags);
IMGUI_API int           igIsItemActive(void);
IMGUI_API int           igIsItemFocused(void);
IMGUI_API int           igIsItemClicked(int mouse_button);
IMGUI_API int           igIsItemVisible(void);
IMGUI_API int           igIsItemEdited(void);
IMGUI_API int           igIsItemActivated(void);
IMGUI_API int           igIsItemDeactivated(void);
IMGUI_API int           igIsItemDeactivatedAfterEdit(void);
IMGUI_API int           igIsItemToggledOpen(void);
IMGUI_API int           igIsAnyItemHovered(void);
IMGUI_API int           igIsAnyItemActive(void);
IMGUI_API int           igIsAnyItemFocused(void);

//-----------------------------------------------------------------------------
// Widgets: Basic
//-----------------------------------------------------------------------------
IMGUI_API int           igCheckbox(const char* label, int* v);
IMGUI_API int           igCheckboxFlags_IntPtr(const char* label, int* flags, int flags_value);
IMGUI_API int           igRadioButton_Bool(const char* label, int active);
IMGUI_API int           igRadioButton_IntPtr(const char* label, int* v, int button_value);
IMGUI_API void          igProgressBar(float fraction, ImVec2 size_arg, const char* overlay);
IMGUI_API void          igBullet(void);

//-----------------------------------------------------------------------------
// Widgets: Combo Box
//-----------------------------------------------------------------------------
IMGUI_API int           igBeginCombo(const char* label, const char* preview_value, int flags);
IMGUI_API void          igEndCombo(void);
IMGUI_API int           igCombo_Str_arr(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items);
IMGUI_API int           igCombo_Str(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items);

//-----------------------------------------------------------------------------
// Widgets: Drag Sliders
//-----------------------------------------------------------------------------
IMGUI_API int           igDragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, int flags);
IMGUI_API int           igDragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, int flags);
IMGUI_API int           igDragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, int flags);
IMGUI_API int           igDragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, int flags);
IMGUI_API int           igDragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format, int flags);
IMGUI_API int           igDragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* format, int flags);
IMGUI_API int           igDragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* format, int flags);
IMGUI_API int           igDragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* format, int flags);

//-----------------------------------------------------------------------------
// Widgets: Regular Sliders
//-----------------------------------------------------------------------------
IMGUI_API int           igSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, int flags);
IMGUI_API int           igSliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format, int flags);
IMGUI_API int           igSliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, int flags);
IMGUI_API int           igSliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format, int flags);
IMGUI_API int           igSliderAngle(const char* label, float* v_rad, float v_degrees_min, float v_degrees_max, int flags);
IMGUI_API int           igSliderInt(const char* label, int* v, int v_min, int v_max, const char* format, int flags);
IMGUI_API int           igSliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format, int flags);
IMGUI_API int           igSliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format, int flags);
IMGUI_API int           igSliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format, int flags);

//-----------------------------------------------------------------------------
// Widgets: Color Editor/Picker
//-----------------------------------------------------------------------------
IMGUI_API int           igColorEdit3(const char* label, float col[3], int flags);
IMGUI_API int           igColorEdit4(const char* label, float col[4], int flags);
IMGUI_API int           igColorPicker3(const char* label, float col[3], int flags);
IMGUI_API int           igColorPicker4(const char* label, float col[4], int flags, const float* ref_col);
IMGUI_API int           igColorButton(const char* desc_id, ImVec4 col, int flags, ImVec2 size);
IMGUI_API void          igSetColorEditOptions(int flags);

//-----------------------------------------------------------------------------
// Widgets: Trees
//-----------------------------------------------------------------------------
IMGUI_API int           igTreeNodeEx_Str(const char* label, int flags);
IMGUI_API void          igTreePush_Str(const char* str_id);
IMGUI_API void          igTreePush_Ptr(const void* ptr_id);
IMGUI_API void          igTreePop(void);
IMGUI_API int           igTreeNodeToLabelSpacing(void);
IMGUI_API int           igCollapsingHeader_TreeNodeFlags(const char* label, int flags);
IMGUI_API int           igCollapsingHeader_BoolPtr(const char* label, int* p_visible, int flags);
IMGUI_API void          igSetNextItemOpen(int is_open, int cond);

//-----------------------------------------------------------------------------
// Widgets: Selectables
//-----------------------------------------------------------------------------
IMGUI_API int           igSelectable(const char* label, int selected, int flags, ImVec2 size);

//-----------------------------------------------------------------------------
// Widgets: List Boxes
//-----------------------------------------------------------------------------
IMGUI_API int           igBeginListBox(const char* label, ImVec2 size);
IMGUI_API void          igEndListBox(void);

//-----------------------------------------------------------------------------
// Tooltips
//-----------------------------------------------------------------------------
IMGUI_API void          igBeginTooltip(void);
IMGUI_API void          igEndTooltip(void);
IMGUI_API void          igSetTooltip(const char* fmt, ...);

//-----------------------------------------------------------------------------
// Menus
//-----------------------------------------------------------------------------
IMGUI_API int           igBeginMainMenuBar(void);
IMGUI_API void          igEndMainMenuBar(void);
IMGUI_API int           igBeginMenuBar(void);
IMGUI_API void          igEndMenuBar(void);
IMGUI_API int           igBeginMenu(const char* label, int enabled);
IMGUI_API void          igEndMenu(void);
IMGUI_API int           igMenuItem_Bool(const char* label, const char* shortcut, int selected, int enabled);
IMGUI_API int           igMenuItem_BoolPtr(const char* label, const char* shortcut, int* p_selected, int enabled);

//-----------------------------------------------------------------------------
// Popups
//-----------------------------------------------------------------------------
IMGUI_API void          igOpenPopup_Str(const char* str_id, int popup_flags);
IMGUI_API void          igOpenPopup_ID(ImGuiID id, int popup_flags);
IMGUI_API int           igBeginPopup(const char* str_id, int flags);
IMGUI_API int           igBeginPopupModal(const char* name, int* p_open, int flags);
IMGUI_API void          igEndPopup(void);
IMGUI_API int           igOpenPopupOnItemClick(const char* str_id, int popup_flags);
IMGUI_API void          igCloseCurrentPopup(void);

//-----------------------------------------------------------------------------
// Tables
//-----------------------------------------------------------------------------
IMGUI_API int           igBeginTable(const char* str_id, int column, int flags, ImVec2 outer_size, float inner_width);
IMGUI_API void          igEndTable(void);
IMGUI_API void          igTableNextRow(int row_flags, float min_row_height);
IMGUI_API void          igTableNextColumn(void);
IMGUI_API int           igTableSetColumnIndex(int column_n);

//-----------------------------------------------------------------------------
// Clipping
//-----------------------------------------------------------------------------
IMGUI_API void          igPushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, int intersect_with_current_clip_rect);
IMGUI_API void          igPopClipRect(void);

//-----------------------------------------------------------------------------
// Focus, activation
//-----------------------------------------------------------------------------
IMGUI_API void          igSetItemDefaultFocus(void);
IMGUI_API void          igSetKeyboardFocusHere(int offset);

//-----------------------------------------------------------------------------
// Platform IO
//-----------------------------------------------------------------------------
IMGUI_API void          igLoadIniSettingsFromDisk(const char* ini_filename);
IMGUI_API void          igSaveIniSettingsToDisk(const char* ini_filename);
IMGUI_API const char*   igSaveIniSettingsToMemory(size_t* out_ini_size);

//-----------------------------------------------------------------------------
// Time
//-----------------------------------------------------------------------------
IMGUI_API double        igGetTime(void);
IMGUI_API int           igGetFrameCount(void);

#ifdef __cplusplus
}
#endif
