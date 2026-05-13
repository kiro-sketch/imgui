// dear imgui - C API wrapper implementation
// This file provides C-compatible wrappers for the Dear ImGui C++ API

#include "imgui.h"
#include "imgui_c_api.h"
#include <stdarg.h>

//-----------------------------------------------------------------------------
// Context creation and access
//-----------------------------------------------------------------------------
extern "C" ImGuiContext* igCreateContext(ImFontAtlas* shared_font_atlas) {
    return ImGui::CreateContext(shared_font_atlas);
}

extern "C" void igDestroyContext(ImGuiContext* ctx) {
    ImGui::DestroyContext(ctx);
}

extern "C" ImGuiContext* igGetCurrentContext(void) {
    return ImGui::GetCurrentContext();
}

extern "C" void igSetCurrentContext(ImGuiContext* ctx) {
    ImGui::SetCurrentContext(ctx);
}

//-----------------------------------------------------------------------------
// Main functions
//-----------------------------------------------------------------------------
extern "C" ImGuiIO* igGetIO(void) {
    return &ImGui::GetIO();
}

extern "C" ImGuiPlatformIO* igGetPlatformIO(void) {
    return &ImGui::GetPlatformIO();
}

extern "C" ImGuiStyle* igGetStyle(void) {
    return &ImGui::GetStyle();
}

extern "C" void igNewFrame(void) {
    ImGui::NewFrame();
}

extern "C" void igEndFrame(void) {
    ImGui::EndFrame();
}

extern "C" void igRender(void) {
    ImGui::Render();
}

extern "C" ImDrawData* igGetDrawData(void) {
    return ImGui::GetDrawData();
}

//-----------------------------------------------------------------------------
// Demo, Debug, Information
//-----------------------------------------------------------------------------
extern "C" void igShowDemoWindow(int* p_open) {
    ImGui::ShowDemoWindow(p_open ? reinterpret_cast<bool*>(p_open) : nullptr);
}

extern "C" void igShowMetricsWindow(int* p_open) {
    ImGui::ShowMetricsWindow(p_open ? reinterpret_cast<bool*>(p_open) : nullptr);
}

extern "C" void igShowDebugLogWindow(int* p_open) {
    ImGui::ShowDebugLogWindow(p_open ? reinterpret_cast<bool*>(p_open) : nullptr);
}

extern "C" void igShowIDStackToolWindow(int* p_open) {
    ImGui::ShowIDStackToolWindow(p_open ? reinterpret_cast<bool*>(p_open) : nullptr);
}

extern "C" void igShowAboutWindow(int* p_open) {
    ImGui::ShowAboutWindow(p_open ? reinterpret_cast<bool*>(p_open) : nullptr);
}

extern "C" void igShowStyleEditor(ImGuiStyle* ref) {
    ImGui::ShowStyleEditor(ref);
}

extern "C" int igShowStyleSelector(const char* label) {
    return ImGui::ShowStyleSelector(label);
}

extern "C" void igShowFontSelector(const char* label) {
    ImGui::ShowFontSelector(label);
}

extern "C" void igShowUserGuide(void) {
    ImGui::ShowUserGuide();
}

extern "C" const char* igGetVersion(void) {
    return ImGui::GetVersion();
}

//-----------------------------------------------------------------------------
// Styles
//-----------------------------------------------------------------------------
extern "C" void igStyleColorsDark(ImGuiStyle* dst) {
    ImGui::StyleColorsDark(dst);
}

extern "C" void igStyleColorsLight(ImGuiStyle* dst) {
    ImGui::StyleColorsLight(dst);
}

extern "C" void igStyleColorsClassic(ImGuiStyle* dst) {
    ImGui::StyleColorsClassic(dst);
}

//-----------------------------------------------------------------------------
// Windows
//-----------------------------------------------------------------------------
extern "C" int igBegin(const char* name, int* p_open, int flags) {
    return ImGui::Begin(name, p_open ? reinterpret_cast<bool*>(p_open) : nullptr, static_cast<ImGuiWindowFlags>(flags));
}

extern "C" void igEnd(void) {
    ImGui::End();
}

//-----------------------------------------------------------------------------
// Child Windows
//-----------------------------------------------------------------------------
extern "C" int igBeginChild_Str(const char* str_id, ImVec2 size, int child_flags, int window_flags) {
    return ImGui::BeginChild(str_id, *reinterpret_cast<ImVec2*>(&size), static_cast<ImGuiChildFlags>(child_flags), static_cast<ImGuiWindowFlags>(window_flags));
}

extern "C" int igBeginChild_ID(ImGuiID id, ImVec2 size, int child_flags, int window_flags) {
    return ImGui::BeginChild(id, *reinterpret_cast<ImVec2*>(&size), static_cast<ImGuiChildFlags>(child_flags), static_cast<ImGuiWindowFlags>(window_flags));
}

extern "C" void igEndChild(void) {
    ImGui::EndChild();
}

//-----------------------------------------------------------------------------
// Window state
//-----------------------------------------------------------------------------
extern "C" int igIsWindowAppearing(void) {
    return ImGui::IsWindowAppearing();
}

extern "C" int igIsWindowCollapsed(void) {
    return ImGui::IsWindowCollapsed();
}

extern "C" int igIsWindowFocused(int flags) {
    return ImGui::IsWindowFocused(static_cast<ImGuiFocusedFlags>(flags));
}

extern "C" int igIsWindowHovered(int flags) {
    return ImGui::IsWindowHovered(static_cast<ImGuiHoveredFlags>(flags));
}

extern "C" ImDrawList* igGetWindowDrawList(void) {
    return ImGui::GetWindowDrawList();
}

extern "C" ImVec2 igGetWindowPos(void) {
    return ImGui::GetWindowPos();
}

extern "C" ImVec2 igGetWindowSize(void) {
    return ImGui::GetWindowSize();
}

extern "C" float igGetWindowWidth(void) {
    return ImGui::GetWindowWidth();
}

extern "C" float igGetWindowHeight(void) {
    return ImGui::GetWindowHeight();
}

//-----------------------------------------------------------------------------
// Window position
//-----------------------------------------------------------------------------
extern "C" void igSetNextWindowPos(ImVec2 pos, int cond, ImVec2 pivot) {
    ImGui::SetNextWindowPos(*reinterpret_cast<ImVec2*>(&pos), static_cast<ImGuiCond>(cond), *reinterpret_cast<ImVec2*>(&pivot));
}

extern "C" void igSetNextWindowSize(ImVec2 size, int cond) {
    ImGui::SetNextWindowSize(*reinterpret_cast<ImVec2*>(&size), static_cast<ImGuiCond>(cond));
}

extern "C" void igSetNextWindowContentSize(ImVec2 size) {
    ImGui::SetNextWindowContentSize(*reinterpret_cast<ImVec2*>(&size));
}

extern "C" void igSetNextWindowCollapsed(int collapsed, int cond) {
    ImGui::SetNextWindowCollapsed(collapsed != 0, static_cast<ImGuiCond>(cond));
}

extern "C" void igSetNextWindowFocus(void) {
    ImGui::SetNextWindowFocus();
}

extern "C" void igSetNextWindowBgAlpha(float alpha) {
    ImGui::SetNextWindowBgAlpha(alpha);
}

//-----------------------------------------------------------------------------
// Content region
//-----------------------------------------------------------------------------
extern "C" ImVec2 igGetContentRegionAvail(void) {
    return ImGui::GetContentRegionAvail();
}

extern "C" ImVec2 igGetCursorScreenPos(void) {
    return ImGui::GetCursorScreenPos();
}

extern "C" void igSetCursorScreenPos(ImVec2 pos) {
    ImGui::SetCursorScreenPos(*reinterpret_cast<ImVec2*>(&pos));
}

//-----------------------------------------------------------------------------
// Parameters stacks (shared)
//-----------------------------------------------------------------------------
extern "C" void igPushFont(ImFont* font, float font_size_base_unscaled) {
    ImGui::PushFont(font, font_size_base_unscaled);
}

extern "C" void igPopFont(void) {
    ImGui::PopFont();
}

extern "C" void igPushStyleColor_U32(int idx, ImU32 col) {
    ImGui::PushStyleColor(static_cast<ImGuiCol>(idx), col);
}

extern "C" void igPushStyleColor_Vec4(int idx, ImVec4 col) {
    ImGui::PushStyleColor(static_cast<ImGuiCol>(idx), *reinterpret_cast<ImVec4*>(&col));
}

extern "C" void igPopStyleColor(int count) {
    ImGui::PopStyleColor(count);
}

extern "C" void igPushStyleVar_Float(int idx, float val) {
    ImGui::PushStyleVar(static_cast<ImGuiStyleVar>(idx), val);
}

extern "C" void igPushStyleVar_Vec2(int idx, ImVec2 val) {
    ImGui::PushStyleVar(static_cast<ImGuiStyleVar>(idx), *reinterpret_cast<ImVec2*>(&val));
}

extern "C" void igPopStyleVar(int count) {
    ImGui::PopStyleVar(count);
}

//-----------------------------------------------------------------------------
// Style read access
//-----------------------------------------------------------------------------
extern "C" ImU32 igGetColorU32_Col(int idx, float alpha_mul) {
    return ImGui::GetColorU32(static_cast<ImGuiCol>(idx), alpha_mul);
}

extern "C" ImU32 igGetColorU32_Vec4(ImVec4 col) {
    return ImGui::GetColorU32(*reinterpret_cast<ImVec4*>(&col));
}

extern "C" ImVec4 igGetStyleColorVec4(int idx) {
    return ImGui::GetStyleColorVec4(static_cast<ImGuiCol>(idx));
}

//-----------------------------------------------------------------------------
// Fonts
//-----------------------------------------------------------------------------
extern "C" ImFont* igGetFont(void) {
    return ImGui::GetFont();
}

extern "C" float igGetFontSize(void) {
    return ImGui::GetFontSize();
}

extern "C" ImVec2 igGetFontTexUvWhitePixel(void) {
    return ImGui::GetFontTexUvWhitePixel();
}

//-----------------------------------------------------------------------------
// Text
//-----------------------------------------------------------------------------
extern "C" void igTextUnformatted(const char* text, const char* text_end) {
    ImGui::TextUnformatted(text, text_end);
}

extern "C" void igText(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextV(fmt, args);
    va_end(args);
}

extern "C" void igTextColored(ImVec4 col, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextColoredV(*reinterpret_cast<ImVec4*>(&col), fmt, args);
    va_end(args);
}

extern "C" void igTextDisabled(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextDisabledV(fmt, args);
    va_end(args);
}

extern "C" void igTextWrapped(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextWrappedV(fmt, args);
    va_end(args);
}

extern "C" void igLabelText(const char* label, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::LabelTextV(label, fmt, args);
    va_end(args);
}

extern "C" void igBulletText(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::BulletTextV(fmt, args);
    va_end(args);
}

//-----------------------------------------------------------------------------
// Buttons
//-----------------------------------------------------------------------------
extern "C" int igButton(const char* label, ImVec2 size) {
    return ImGui::Button(label, *reinterpret_cast<ImVec2*>(&size));
}

extern "C" int igSmallButton(const char* label) {
    return ImGui::SmallButton(label);
}

extern "C" int igInvisibleButton(const char* str_id, ImVec2 size, int flags) {
    return ImGui::InvisibleButton(str_id, *reinterpret_cast<ImVec2*>(&size), static_cast<ImGuiItemFlags>(flags));
}

extern "C" int igArrowButton(const char* str_id, int dir) {
    return ImGui::ArrowButton(str_id, static_cast<ImGuiDir>(dir));
}

//-----------------------------------------------------------------------------
// Inputs
//-----------------------------------------------------------------------------
extern "C" int igIsItemHovered(int flags) {
    return ImGui::IsItemHovered(static_cast<ImGuiHoveredFlags>(flags));
}

extern "C" int igIsItemActive(void) {
    return ImGui::IsItemActive();
}

extern "C" int igIsItemFocused(void) {
    return ImGui::IsItemFocused();
}

extern "C" int igIsItemClicked(int mouse_button) {
    return ImGui::IsItemClicked(mouse_button);
}

extern "C" int igIsItemVisible(void) {
    return ImGui::IsItemVisible();
}

extern "C" int igIsItemEdited(void) {
    return ImGui::IsItemEdited();
}

extern "C" int igIsItemActivated(void) {
    return ImGui::IsItemActivated();
}

extern "C" int igIsItemDeactivated(void) {
    return ImGui::IsItemDeactivated();
}

extern "C" int igIsItemDeactivatedAfterEdit(void) {
    return ImGui::IsItemDeactivatedAfterEdit();
}

extern "C" int igIsItemToggledOpen(void) {
    return ImGui::IsItemToggledOpen();
}

extern "C" int igIsAnyItemHovered(void) {
    return ImGui::IsAnyItemHovered();
}

extern "C" int igIsAnyItemActive(void) {
    return ImGui::IsAnyItemActive();
}

extern "C" int igIsAnyItemFocused(void) {
    return ImGui::IsAnyItemFocused();
}

//-----------------------------------------------------------------------------
// Widgets: Basic
//-----------------------------------------------------------------------------
extern "C" int igCheckbox(const char* label, int* v) {
    return ImGui::Checkbox(label, reinterpret_cast<bool*>(v));
}

extern "C" int igCheckboxFlags_IntPtr(const char* label, int* flags, int flags_value) {
    return ImGui::CheckboxFlags(label, flags, flags_value);
}

extern "C" int igRadioButton_Bool(const char* label, int active) {
    return ImGui::RadioButton(label, active != 0);
}

extern "C" int igRadioButton_IntPtr(const char* label, int* v, int button_value) {
    return ImGui::RadioButton(label, v, button_value);
}

extern "C" void igProgressBar(float fraction, ImVec2 size_arg, const char* overlay) {
    ImGui::ProgressBar(fraction, *reinterpret_cast<ImVec2*>(&size_arg), overlay);
}

extern "C" void igBullet(void) {
    ImGui::Bullet();
}

//-----------------------------------------------------------------------------
// Widgets: Combo Box
//-----------------------------------------------------------------------------
extern "C" int igBeginCombo(const char* label, const char* preview_value, int flags) {
    return ImGui::BeginCombo(label, preview_value, static_cast<ImGuiComboFlags>(flags));
}

extern "C" void igEndCombo(void) {
    ImGui::EndCombo();
}

extern "C" int igCombo_Str_arr(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items) {
    return ImGui::Combo(label, current_item, items, items_count, popup_max_height_in_items);
}

extern "C" int igCombo_Str(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items) {
    return ImGui::Combo(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
}

//-----------------------------------------------------------------------------
// Widgets: Drag Sliders
//-----------------------------------------------------------------------------
extern "C" int igDragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, int flags) {
    return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igDragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, int flags) {
    return ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igDragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, int flags) {
    return ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igDragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, int flags) {
    return ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igDragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format, int flags) {
    return ImGui::DragInt(label, v, v_speed, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igDragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* format, int flags) {
    return ImGui::DragInt2(label, v, v_speed, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igDragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* format, int flags) {
    return ImGui::DragInt3(label, v, v_speed, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igDragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* format, int flags) {
    return ImGui::DragInt4(label, v, v_speed, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

//-----------------------------------------------------------------------------
// Widgets: Regular Sliders
//-----------------------------------------------------------------------------
extern "C" int igSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, int flags) {
    return ImGui::SliderFloat(label, v, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igSliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format, int flags) {
    return ImGui::SliderFloat2(label, v, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igSliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, int flags) {
    return ImGui::SliderFloat3(label, v, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igSliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format, int flags) {
    return ImGui::SliderFloat4(label, v, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igSliderAngle(const char* label, float* v_rad, float v_degrees_min, float v_degrees_max, int flags) {
    return ImGui::SliderAngle(label, v_rad, v_degrees_min, v_degrees_max, "%.0f deg", static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igSliderInt(const char* label, int* v, int v_min, int v_max, const char* format, int flags) {
    return ImGui::SliderInt(label, v, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igSliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format, int flags) {
    return ImGui::SliderInt2(label, v, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igSliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format, int flags) {
    return ImGui::SliderInt3(label, v, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

extern "C" int igSliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format, int flags) {
    return ImGui::SliderInt4(label, v, v_min, v_max, format, static_cast<ImGuiSliderFlags>(flags));
}

//-----------------------------------------------------------------------------
// Widgets: Color Editor/Picker
//-----------------------------------------------------------------------------
extern "C" int igColorEdit3(const char* label, float col[3], int flags) {
    return ImGui::ColorEdit3(label, col, static_cast<ImGuiColorEditFlags>(flags));
}

extern "C" int igColorEdit4(const char* label, float col[4], int flags) {
    return ImGui::ColorEdit4(label, col, static_cast<ImGuiColorEditFlags>(flags));
}

extern "C" int igColorPicker3(const char* label, float col[3], int flags) {
    return ImGui::ColorPicker3(label, col, static_cast<ImGuiColorEditFlags>(flags));
}

extern "C" int igColorPicker4(const char* label, float col[4], int flags, const float* ref_col) {
    return ImGui::ColorPicker4(label, col, static_cast<ImGuiColorEditFlags>(flags), ref_col);
}

extern "C" int igColorButton(const char* desc_id, ImVec4 col, int flags, ImVec2 size) {
    return ImGui::ColorButton(desc_id, *reinterpret_cast<ImVec4*>(&col), static_cast<ImGuiColorEditFlags>(flags), *reinterpret_cast<ImVec2*>(&size));
}

extern "C" void igSetColorEditOptions(int flags) {
    ImGui::SetColorEditOptions(static_cast<ImGuiColorEditFlags>(flags));
}

//-----------------------------------------------------------------------------
// Widgets: Trees
//-----------------------------------------------------------------------------
extern "C" int igTreeNodeEx_Str(const char* label, int flags) {
    return ImGui::TreeNodeEx(label, static_cast<ImGuiTreeNodeFlags>(flags));
}

extern "C" void igTreePush_Str(const char* str_id) {
    ImGui::TreePush(str_id);
}

extern "C" void igTreePush_Ptr(const void* ptr_id) {
    ImGui::TreePush(ptr_id);
}

extern "C" void igTreePop(void) {
    ImGui::TreePop();
}

extern "C" int igTreeNodeToLabelSpacing(void) {
    return ImGui::GetTreeNodeToLabelSpacing();
}

extern "C" int igCollapsingHeader_TreeNodeFlags(const char* label, int flags) {
    return ImGui::CollapsingHeader(label, static_cast<ImGuiTreeNodeFlags>(flags));
}

extern "C" int igCollapsingHeader_BoolPtr(const char* label, int* p_visible, int flags) {
    return ImGui::CollapsingHeader(label, p_visible ? reinterpret_cast<bool*>(p_visible) : nullptr, static_cast<ImGuiTreeNodeFlags>(flags));
}

extern "C" void igSetNextItemOpen(int is_open, int cond) {
    ImGui::SetNextItemOpen(is_open != 0, static_cast<ImGuiCond>(cond));
}

//-----------------------------------------------------------------------------
// Widgets: Selectables
//-----------------------------------------------------------------------------
extern "C" int igSelectable(const char* label, int selected, int flags, ImVec2 size) {
    return ImGui::Selectable(label, selected != 0, static_cast<ImGuiSelectableFlags>(flags), *reinterpret_cast<ImVec2*>(&size));
}

//-----------------------------------------------------------------------------
// Widgets: List Boxes
//-----------------------------------------------------------------------------
extern "C" int igBeginListBox(const char* label, ImVec2 size) {
    return ImGui::BeginListBox(label, *reinterpret_cast<ImVec2*>(&size));
}

extern "C" void igEndListBox(void) {
    ImGui::EndListBox();
}

//-----------------------------------------------------------------------------
// Tooltips
//-----------------------------------------------------------------------------
extern "C" void igBeginTooltip(void) {
    ImGui::BeginTooltip();
}

extern "C" void igEndTooltip(void) {
    ImGui::EndTooltip();
}

extern "C" void igSetTooltip(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::SetTooltipV(fmt, args);
    va_end(args);
}

//-----------------------------------------------------------------------------
// Menus
//-----------------------------------------------------------------------------
extern "C" int igBeginMainMenuBar(void) {
    return ImGui::BeginMainMenuBar();
}

extern "C" void igEndMainMenuBar(void) {
    ImGui::EndMainMenuBar();
}

extern "C" int igBeginMenuBar(void) {
    return ImGui::BeginMenuBar();
}

extern "C" void igEndMenuBar(void) {
    ImGui::EndMenuBar();
}

extern "C" int igBeginMenu(const char* label, int enabled) {
    return ImGui::BeginMenu(label, enabled != 0);
}

extern "C" void igEndMenu(void) {
    ImGui::EndMenu();
}

extern "C" int igMenuItem_Bool(const char* label, const char* shortcut, int selected, int enabled) {
    return ImGui::MenuItem(label, shortcut, selected != 0, enabled != 0);
}

extern "C" int igMenuItem_BoolPtr(const char* label, const char* shortcut, int* p_selected, int enabled) {
    return ImGui::MenuItem(label, shortcut, p_selected ? reinterpret_cast<bool*>(p_selected) : nullptr, enabled != 0);
}

//-----------------------------------------------------------------------------
// Popups
//-----------------------------------------------------------------------------
extern "C" void igOpenPopup_Str(const char* str_id, int popup_flags) {
    ImGui::OpenPopup(str_id, static_cast<ImGuiPopupFlags>(popup_flags));
}

extern "C" void igOpenPopup_ID(ImGuiID id, int popup_flags) {
    ImGui::OpenPopup(id, static_cast<ImGuiPopupFlags>(popup_flags));
}

extern "C" int igBeginPopup(const char* str_id, int flags) {
    return ImGui::BeginPopup(str_id, static_cast<ImGuiWindowFlags>(flags));
}

extern "C" int igBeginPopupModal(const char* name, int* p_open, int flags) {
    return ImGui::BeginPopupModal(name, p_open ? reinterpret_cast<bool*>(p_open) : nullptr, static_cast<ImGuiWindowFlags>(flags));
}

extern "C" void igEndPopup(void) {
    ImGui::EndPopup();
}

extern "C" int igOpenPopupOnItemClick(const char* str_id, int popup_flags) {
    ImGui::OpenPopupOnItemClick(str_id, static_cast<ImGuiPopupFlags>(popup_flags));
    return 1; // OpenPopupOnItemClick returns void, so we always return true for C API compatibility
}

extern "C" void igCloseCurrentPopup(void) {
    ImGui::CloseCurrentPopup();
}

//-----------------------------------------------------------------------------
// Tables
//-----------------------------------------------------------------------------
extern "C" int igBeginTable(const char* str_id, int column, int flags, ImVec2 outer_size, float inner_width) {
    return ImGui::BeginTable(str_id, column, static_cast<ImGuiTableFlags>(flags), *reinterpret_cast<ImVec2*>(&outer_size), inner_width);
}

extern "C" void igEndTable(void) {
    ImGui::EndTable();
}

extern "C" void igTableNextRow(int row_flags, float min_row_height) {
    ImGui::TableNextRow(static_cast<ImGuiTableRowFlags>(row_flags), min_row_height);
}

extern "C" void igTableNextColumn(void) {
    ImGui::TableNextColumn();
}

extern "C" int igTableSetColumnIndex(int column_n) {
    return ImGui::TableSetColumnIndex(column_n);
}

//-----------------------------------------------------------------------------
// Clipping
//-----------------------------------------------------------------------------
extern "C" void igPushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, int intersect_with_current_clip_rect) {
    ImGui::PushClipRect(*reinterpret_cast<ImVec2*>(&clip_rect_min), *reinterpret_cast<ImVec2*>(&clip_rect_max), intersect_with_current_clip_rect != 0);
}

extern "C" void igPopClipRect(void) {
    ImGui::PopClipRect();
}

//-----------------------------------------------------------------------------
// Focus, activation
//-----------------------------------------------------------------------------
extern "C" void igSetItemDefaultFocus(void) {
    ImGui::SetItemDefaultFocus();
}

extern "C" void igSetKeyboardFocusHere(int offset) {
    ImGui::SetKeyboardFocusHere(offset);
}

//-----------------------------------------------------------------------------
// Platform IO
//-----------------------------------------------------------------------------
extern "C" void igLoadIniSettingsFromDisk(const char* ini_filename) {
    ImGui::LoadIniSettingsFromDisk(ini_filename);
}

extern "C" void igSaveIniSettingsToDisk(const char* ini_filename) {
    ImGui::SaveIniSettingsToDisk(ini_filename);
}

extern "C" const char* igSaveIniSettingsToMemory(size_t* out_ini_size) {
    return ImGui::SaveIniSettingsToMemory(out_ini_size);
}

//-----------------------------------------------------------------------------
// Time
//-----------------------------------------------------------------------------
extern "C" double igGetTime(void) {
    return ImGui::GetTime();
}

extern "C" int igGetFrameCount(void) {
    return ImGui::GetFrameCount();
}
