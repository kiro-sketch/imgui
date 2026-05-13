// imgui_c_api.cpp - C API wrapper implementation for ImGui static library (Windows only)
// This file implements all C-compatible functions that wrap the C++ ImGui API

#include "imgui.h"
#include "imgui_internal.h"

// Windows-specific backends
#ifdef _WIN32
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx9.h"
#include "backends/imgui_impl_dx10.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_opengl3.h"
// NOTE: Vulkan, GLFW, SDL2 excluded to avoid external dependencies in CI
#endif

#include "imgui_c_api.h"

// ============================================================================
// Core Functions
// ============================================================================

ImGuiContext* igCreateContext(void) {
    return ImGui::CreateContext();
}

void igDestroyContext(ImGuiContext* ctx) {
    ImGui::DestroyContext(ctx);
}

ImGuiContext* igGetCurrentContext(void) {
    return ImGui::GetCurrentContext();
}

void igSetCurrentContext(ImGuiContext* ctx) {
    ImGui::SetCurrentContext(ctx);
}

// ============================================================================
// IO Functions
// ============================================================================

ImGuiIO* igGetIO(void) {
    return &ImGui::GetIO();
}

ImGuiBool igIO_WantCaptureMouse(ImGuiIO* io) {
    return io->WantCaptureMouse;
}

ImGuiBool igIO_WantCaptureKeyboard(ImGuiIO* io) {
    return io->WantCaptureKeyboard;
}

ImGuiBool igIO_WantTextInput(ImGuiIO* io) {
    return io->WantTextInput;
}

float igIO_GetDeltaTime(ImGuiIO* io) {
    return io->DeltaTime;
}

void igIO_SetDisplaySize(ImGuiIO* io, float width, float height) {
    io->DisplaySize = ImVec2(width, height);
}

void igIO_SetMousePos(ImGuiIO* io, float x, float y) {
    io->MousePos = ImVec2(x, y);
}

void igIO_SetMouseDown(ImGuiIO* io, int button, ImGuiBool down) {
    io->MouseDown[button] = down != 0;
}

void igIO_SetMouseWheel(ImGuiIO* io, float wheel) {
    io->MouseWheel = wheel;
}

void igIO_AddKeyEvent(ImGuiIO* io, ImGuiKey key, ImGuiBool down) {
    io->AddKeyEvent((ImGuiKey)key, down != 0);
}

void igIO_AddCharacterEvent(ImGuiIO* io, unsigned int c) {
    io->AddInputCharacter(c);
}

// ============================================================================
// Style Functions
// ============================================================================

ImGuiStyle* igGetStyle(void) {
    return &ImGui::GetStyle();
}

void igStyleColorsDark(ImGuiStyle* dst) {
    ImGui::StyleColorsDark(dst);
}

void igStyleColorsLight(ImGuiStyle* dst) {
    ImGui::StyleColorsLight(dst);
}

void igStyleColorsClassic(ImGuiStyle* dst) {
    ImGui::StyleColorsClassic(dst);
}

// ============================================================================
// Main Demo Function
// ============================================================================

void igShowDemoWindow(ImGuiBool* p_open) {
    bool open = p_open ? (*p_open != 0) : true;
    ImGui::ShowDemoWindow(&open);
    if (p_open) *p_open = open;
}

// ============================================================================
// Window Functions
// ============================================================================

ImGuiBool igBegin(const char* name, ImGuiBool* p_open, ImGuiWindowFlags flags) {
    bool open = p_open ? (*p_open != 0) : true;
    ImGuiBool result = ImGui::Begin(name, p_open ? &open : nullptr, flags);
    if (p_open) *p_open = open;
    return result;
}

void igEnd(void) {
    ImGui::End();
}

ImGuiBool igBeginChild(const char* str_id, const ImVec2 size, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags) {
    return ImGui::BeginChild(str_id, size, child_flags, window_flags);
}

void igEndChild(void) {
    ImGui::EndChild();
}

// ============================================================================
// Content Functions
// ============================================================================

void igSeparator(void) {
    ImGui::Separator();
}

void igSameLine(float offset_from_start_x, float spacing) {
    ImGui::SameLine(offset_from_start_x, spacing);
}

void igNewLine(void) {
    ImGui::NewLine();
}

void igSpacing(void) {
    ImGui::Spacing();
}

void igDummy(const ImVec2 size) {
    ImGui::Dummy(size);
}

void igIndent(float indent_w) {
    ImGui::Indent(indent_w);
}

void igUnindent(float indent_w) {
    ImGui::Unindent(indent_w);
}

// ============================================================================
// Text Functions
// ============================================================================

void igText(const char* fmt) {
    ImGui::Text("%s", fmt);
}

void igTextColored(const ImVec4 col, const char* fmt) {
    ImGui::TextColored(col, "%s", fmt);
}

void igTextDisabled(const char* fmt) {
    ImGui::TextDisabled("%s", fmt);
}

void igTextWrapped(const char* fmt) {
    ImGui::TextWrapped("%s", fmt);
}

void igLabelText(const char* label, const char* fmt) {
    ImGui::LabelText(label, "%s", fmt);
}

void igBulletText(const char* fmt) {
    ImGui::BulletText("%s", fmt);
}

// ============================================================================
// Button Functions
// ============================================================================

ImGuiBool igButton(const char* label, const ImVec2 size) {
    return ImGui::Button(label, size);
}

ImGuiBool igSmallButton(const char* label) {
    return ImGui::SmallButton(label);
}

ImGuiBool igInvisibleButton(const char* str_id, const ImVec2 size, ImGuiButtonFlags flags) {
    return ImGui::InvisibleButton(str_id, size, flags);
}

ImGuiBool igArrowButton(const char* str_id, ImGuiDir dir) {
    return ImGui::ArrowButton(str_id, dir);
}

void igImage(void* user_texture_id, const ImVec2 size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 tint_col, const ImVec4 border_col) {
    ImGui::Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
}

ImGuiBool igImageButton(const char* str_id, void* user_texture_id, const ImVec2 image_size, const ImVec2 uv0, const ImVec2 uv1, const ImVec4 bg_col, const ImVec4 tint_col) {
    return ImGui::ImageButton(str_id, user_texture_id, image_size, uv0, uv1, bg_col, tint_col);
}

// ============================================================================
// Checkbox/Radio/Slider Functions
// ============================================================================

ImGuiBool igCheckbox(const char* label, ImGuiBool* v) {
    bool val = (*v != 0);
    ImGuiBool result = ImGui::Checkbox(label, &val);
    *v = val;
    return result;
}

ImGuiBool igRadioButton(const char* label, ImGuiBool active) {
    return ImGui::RadioButton(label, active != 0);
}

ImGuiBool igSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::SliderFloat(label, v, v_min, v_max, format, flags);
}

ImGuiBool igSliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::SliderInt(label, v, v_min, v_max, format, flags);
}

ImGuiBool igDragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);
}

ImGuiBool igDragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::DragInt(label, v, v_speed, v_min, v_max, format, flags);
}

// ============================================================================
// Combo/ListBox Functions
// ============================================================================

ImGuiBool igBeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags) {
    return ImGui::BeginCombo(label, preview_value, flags);
}

void igEndCombo(void) {
    ImGui::EndCombo();
}

ImGuiBool igCombo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items) {
    return ImGui::Combo(label, current_item, items, items_count, popup_max_height_in_items);
}

ImGuiBool igBeginListBox(const char* label, const ImVec2 size) {
    return ImGui::BeginListBox(label, size);
}

void igEndListBox(void) {
    ImGui::EndListBox();
}

// ============================================================================
// Input Functions
// ============================================================================

ImGuiBool igInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags) {
    return ImGui::InputText(label, buf, buf_size, flags);
}

ImGuiBool igInputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2 size, ImGuiInputTextFlags flags) {
    return ImGui::InputTextMultiline(label, buf, buf_size, size, flags);
}

ImGuiBool igInputFloat(const char* label, float* v, float step, float step_fast, const char* format, ImGuiInputTextFlags flags) {
    return ImGui::InputFloat(label, v, step, step_fast, format, flags);
}

ImGuiBool igInputInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags) {
    return ImGui::InputInt(label, v, step, step_fast, flags);
}

// ============================================================================
// Color Editor Functions
// ============================================================================

ImGuiBool igColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags) {
    return ImGui::ColorEdit3(label, col, flags);
}

ImGuiBool igColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags) {
    return ImGui::ColorEdit4(label, col, flags);
}

ImGuiBool igColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags) {
    return ImGui::ColorPicker3(label, col, flags);
}

ImGuiBool igColorPicker4(const char* label, float col[4], const float* ref_col, ImGuiColorEditFlags flags) {
    return ImGui::ColorPicker4(label, col, ref_col, flags);
}

// ============================================================================
// Tree/TreeNode Functions
// ============================================================================

ImGuiBool igTreeNode(const char* label) {
    return ImGui::TreeNode(label);
}

ImGuiBool igTreeNodeEx(const char* label, ImGuiTreeNodeFlags flags) {
    return ImGui::TreeNodeEx(label, flags);
}

void igTreePop(void) {
    ImGui::TreePop();
}

float igGetTreeNodeToLabelSpacing(void) {
    return ImGui::GetTreeNodeToLabelSpacing();
}

// ============================================================================
// Collapsing Header Functions
// ============================================================================

ImGuiBool igCollapsingHeader(const char* label, ImGuiTreeNodeFlags flags) {
    return ImGui::CollapsingHeader(label, flags);
}

ImGuiBool igCollapsingHeaderWithOpen(const char* label, ImGuiBool* p_visible, ImGuiTreeNodeFlags flags) {
    bool visible = p_visible ? (*p_visible != 0) : true;
    ImGuiBool result = ImGui::CollapsingHeader(label, p_visible ? &visible : nullptr, flags);
    if (p_visible) *p_visible = visible;
    return result;
}

// ============================================================================
// Selectable Functions
// ============================================================================

ImGuiBool igSelectable(const char* label, ImGuiBool selected, ImGuiSelectableFlags flags, const ImVec2 size) {
    return ImGui::Selectable(label, selected != 0, flags, size);
}

// ============================================================================
// Menu Functions
// ============================================================================

ImGuiBool igBeginMainMenuBar(void) {
    return ImGui::BeginMainMenuBar();
}

void igEndMainMenuBar(void) {
    ImGui::EndMainMenuBar();
}

ImGuiBool igBeginMenuBar(void) {
    return ImGui::BeginMenuBar();
}

void igEndMenuBar(void) {
    ImGui::EndMenuBar();
}

ImGuiBool igBeginMenu(const char* label, ImGuiBool enabled) {
    return ImGui::BeginMenu(label, enabled != 0);
}

void igEndMenu(void) {
    ImGui::EndMenu();
}

ImGuiBool igMenuItem(const char* label, const char* shortcut, ImGuiBool selected, ImGuiBool enabled) {
    return ImGui::MenuItem(label, shortcut, selected != 0, enabled != 0);
}

// ============================================================================
// Tooltip Functions
// ============================================================================

void igBeginTooltip(void) {
    ImGui::BeginTooltip();
}

void igEndTooltip(void) {
    ImGui::EndTooltip();
}

void igSetTooltip(const char* fmt) {
    ImGui::SetTooltip("%s", fmt);
}

// ============================================================================
// Popup Functions
// ============================================================================

void igOpenPopup(const char* str_id, ImGuiPopupFlags popup_flags) {
    ImGui::OpenPopup(str_id, popup_flags);
}

ImGuiBool igBeginPopup(const char* str_id, ImGuiWindowFlags flags) {
    return ImGui::BeginPopup(str_id, flags);
}

ImGuiBool igBeginPopupModal(const char* name, ImGuiBool* p_open, ImGuiWindowFlags flags) {
    bool open = p_open ? (*p_open != 0) : true;
    ImGuiBool result = ImGui::BeginPopupModal(name, p_open ? &open : nullptr, flags);
    if (p_open) *p_open = open;
    return result;
}

void igEndPopup(void) {
    ImGui::EndPopup();
}

void igCloseCurrentPopup(void) {
    ImGui::CloseCurrentPopup();
}

// ============================================================================
// Clipping Functions
// ============================================================================

void igPushClipRect(const ImVec2 clip_rect_min, const ImVec2 clip_rect_max, ImGuiBool intersect_with_current_clip_rect) {
    ImGui::PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect != 0);
}

void igPopClipRect(void) {
    ImGui::PopClipRect();
}

// ============================================================================
// Focus Functions
// ============================================================================

void igSetItemDefaultFocus(void) {
    ImGui::SetItemDefaultFocus();
}

void igSetKeyboardFocusHere(int offset) {
    ImGui::SetKeyboardFocusHere(offset);
}

// ============================================================================
// Item/Widget Utilities
// ============================================================================

ImGuiBool igIsItemHovered(ImGuiHoveredFlags flags) {
    return ImGui::IsItemHovered(flags);
}

ImGuiBool igIsItemActive(void) {
    return ImGui::IsItemActive();
}

ImGuiBool igIsItemFocused(void) {
    return ImGui::IsItemFocused();
}

ImGuiBool igIsItemClicked(ImGuiMouseButton mouse_button) {
    return ImGui::IsItemClicked(mouse_button);
}

ImGuiBool igIsItemVisible(void) {
    return ImGui::IsItemVisible();
}

ImGuiBool igIsItemEdited(void) {
    return ImGui::IsItemEdited();
}

ImGuiBool igIsItemActivated(void) {
    return ImGui::IsItemActivated();
}

ImGuiBool igIsItemDeactivated(void) {
    return ImGui::IsItemDeactivated();
}

ImGuiBool igIsItemDeactivatedAfterEdit(void) {
    return ImGui::IsItemDeactivatedAfterEdit();
}

ImGuiBool igIsAnyItemHovered(void) {
    return ImGui::IsAnyItemHovered();
}

ImGuiBool igIsAnyItemActive(void) {
    return ImGui::IsAnyItemActive();
}

ImGuiBool igIsAnyItemFocused(void) {
    return ImGui::IsAnyItemFocused();
}

ImVec2 igGetItemRectMin(void) {
    return ImGui::GetItemRectMin();
}

ImVec2 igGetItemRectMax(void) {
    return ImGui::GetItemRectMax();
}

ImVec2 igGetItemRectSize(void) {
    return ImGui::GetItemRectSize();
}

// ============================================================================
// Viewport Functions
// ============================================================================

void igSetNextWindowPos(const ImVec2 pos, ImGuiCond cond, const ImVec2 pivot) {
    ImGui::SetNextWindowPos(pos, cond, pivot);
}

void igSetNextWindowSize(const ImVec2 size, ImGuiCond cond) {
    ImGui::SetNextWindowSize(size, cond);
}

void igSetNextWindowSizeConstraints(const ImVec2 size_min, const ImVec2 size_max) {
    ImGui::SetNextWindowSizeConstraints(size_min, size_max);
}

void igSetNextWindowContentSize(const ImVec2 size) {
    ImGui::SetNextWindowContentSize(size);
}

void igSetNextWindowCollapsed(ImGuiBool collapsed, ImGuiCond cond) {
    ImGui::SetNextWindowCollapsed(collapsed != 0, cond);
}

void igSetNextWindowFocus(void) {
    ImGui::SetNextWindowFocus();
}

void igSetNextWindowBgAlpha(float alpha) {
    ImGui::SetNextWindowBgAlpha(alpha);
}

// ============================================================================
// Drawing Functions
// ============================================================================

void* igGetBackgroundDrawList(void) {
    return ImGui::GetBackgroundDrawList();
}

void* igGetForegroundDrawList(void) {
    return ImGui::GetForegroundDrawList();
}

void igAddLine(void* draw_list, const ImVec2 p1, const ImVec2 p2, unsigned int col, float thickness) {
    reinterpret_cast<ImDrawList*>(draw_list)->AddLine(p1, p2, col, thickness);
}

void igAddRect(void* draw_list, const ImVec2 p_min, const ImVec2 p_max, unsigned int col, float rounding, int rounding_corners, float thickness) {
    reinterpret_cast<ImDrawList*>(draw_list)->AddRect(p_min, p_max, col, rounding, rounding_corners, thickness);
}

void igAddRectFilled(void* draw_list, const ImVec2 p_min, const ImVec2 p_max, unsigned int col, float rounding, int rounding_corners) {
    reinterpret_cast<ImDrawList*>(draw_list)->AddRectFilled(p_min, p_max, col, rounding, rounding_corners);
}

void igAddCircle(void* draw_list, const ImVec2 center, float radius, unsigned int col, int num_segments, float thickness) {
    reinterpret_cast<ImDrawList*>(draw_list)->AddCircle(center, radius, col, num_segments, thickness);
}

void igAddCircleFilled(void* draw_list, const ImVec2 center, float radius, unsigned int col, int num_segments) {
    reinterpret_cast<ImDrawList*>(draw_list)->AddCircleFilled(center, radius, col, num_segments);
}

void igAddText(void* draw_list, const ImVec2 pos, unsigned int col, const char* text_begin) {
    reinterpret_cast<ImDrawList*>(draw_list)->AddText(pos, col, text_begin);
}

void igAddBezierCubic(void* draw_list, const ImVec2 p1, const ImVec2 p2, const ImVec2 p3, const ImVec2 p4, unsigned int col, float thickness, int num_segments) {
    reinterpret_cast<ImDrawList*>(draw_list)->AddBezierCubic(p1, p2, p3, p4, col, thickness, num_segments);
}

// ============================================================================
// Memory Allocation
// ============================================================================

void* igMemAlloc(size_t size) {
    return ImGui::MemAlloc(size);
}

void igMemFree(void* ptr) {
    ImGui::MemFree(ptr);
}

// ============================================================================
// Platform Dependent Backends (Windows-specific)
// ============================================================================

#ifdef _WIN32

// Win32 Backend
ImGuiBool ImGui_ImplWin32_Init(void* hwnd) {
    return ImGui_ImplWin32_Init(hwnd);
}

void ImGui_ImplWin32_Shutdown(void) {
    ImGui_ImplWin32_Shutdown();
}

void ImGui_ImplWin32_NewFrame(void) {
    ImGui_ImplWin32_NewFrame();
}

ImGuiBool ImGui_ImplWin32_WndProcHandler(void* hwnd, unsigned int msg, uint64_t wParam, int64_t lParam) {
    return ImGui_ImplWin32_WndProcHandler((HWND)hwnd, msg, (WPARAM)wParam, (LPARAM)lParam);
}

// DirectX 9 Backend
ImGuiBool ImGui_ImplDX9_Init(void* device) {
    return ImGui_ImplDX9_Init((LPDIRECT3DDEVICE9)device);
}

void ImGui_ImplDX9_Shutdown(void) {
    ImGui_ImplDX9_Shutdown();
}

void ImGui_ImplDX9_NewFrame(void) {
    ImGui_ImplDX9_NewFrame();
}

void ImGui_ImplDX9_RenderDrawData(void* draw_data) {
    ImGui_ImplDX9_RenderDrawData((ImDrawData*)draw_data);
}

ImGuiBool ImGui_ImplDX9_CreateDeviceObjects(void) {
    return ImGui_ImplDX9_CreateDeviceObjects();
}

void ImGui_ImplDX9_InvalidateDeviceObjects(void) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

// DirectX 10 Backend
ImGuiBool ImGui_ImplDX10_Init(void* device, int render_target_format) {
    return ImGui_ImplDX10_Init((ID3D10Device*)device, (DXGI_FORMAT)render_target_format);
}

void ImGui_ImplDX10_Shutdown(void) {
    ImGui_ImplDX10_Shutdown();
}

void ImGui_ImplDX10_NewFrame(void) {
    ImGui_ImplDX10_NewFrame();
}

void ImGui_ImplDX10_RenderDrawData(void* draw_data) {
    ImGui_ImplDX10_RenderDrawData((ImDrawData*)draw_data);
}

void ImGui_ImplDX10_InvalidateDeviceObjects(void) {
    ImGui_ImplDX10_InvalidateDeviceObjects();
}

ImGuiBool ImGui_ImplDX10_CreateDeviceObjects(void) {
    return ImGui_ImplDX10_CreateDeviceObjects();
}

// DirectX 11 Backend
ImGuiBool ImGui_ImplDX11_Init(void* device, void* context) {
    return ImGui_ImplDX11_Init((ID3D11Device*)device, (ID3D11DeviceContext*)context);
}

void ImGui_ImplDX11_Shutdown(void) {
    ImGui_ImplDX11_Shutdown();
}

void ImGui_ImplDX11_NewFrame(void) {
    ImGui_ImplDX11_NewFrame();
}

void ImGui_ImplDX11_RenderDrawData(void* draw_data) {
    ImGui_ImplDX11_RenderDrawData((ImDrawData*)draw_data);
}

void ImGui_ImplDX11_InvalidateDeviceObjects(void) {
    ImGui_ImplDX11_InvalidateDeviceObjects();
}

ImGuiBool ImGui_ImplDX11_CreateDeviceObjects(void) {
    return ImGui_ImplDX11_CreateDeviceObjects();
}

// DirectX 12 Backend
ImGuiBool ImGui_ImplDX12_Init(void* device, int num_frames_in_flight, void* descriptor_heap, void* cpu_descriptor_handle, void* gpu_descriptor_handle) {
    return ImGui_ImplDX12_Init((ID3D12Device*)device, num_frames_in_flight, 
                               (ID3D12DescriptorHeap*)descriptor_heap, 
                               *(D3D12_CPU_DESCRIPTOR_HANDLE*)cpu_descriptor_handle, 
                               *(D3D12_GPU_DESCRIPTOR_HANDLE*)gpu_descriptor_handle);
}

void ImGui_ImplDX12_Shutdown(void) {
    ImGui_ImplDX12_Shutdown();
}

void ImGui_ImplDX12_NewFrame(void) {
    ImGui_ImplDX12_NewFrame();
}

void ImGui_ImplDX12_RenderDrawData(void* draw_data, void* command_list) {
    ImGui_ImplDX12_RenderDrawData((ImDrawData*)draw_data, (ID3D12GraphicsCommandList*)command_list);
}

void ImGui_ImplDX12_InvalidateDeviceObjects(void) {
    ImGui_ImplDX12_InvalidateDeviceObjects();
}

ImGuiBool ImGui_ImplDX12_CreateDeviceObjects(void) {
    return ImGui_ImplDX12_CreateDeviceObjects();
}

// Vulkan Backend (Windows)
ImGuiBool ImGui_ImplVulkan_Init(void* init_info) {
    return ImGui_ImplVulkan_Init((ImGui_ImplVulkan_InitInfo*)init_info);
}

void ImGui_ImplVulkan_Shutdown(void) {
    ImGui_ImplVulkan_Shutdown();
}

void ImGui_ImplVulkan_NewFrame(void) {
    ImGui_ImplVulkan_NewFrame();
}

void ImGui_ImplVulkan_RenderDrawData(void* draw_data, void* command_buffer) {
    ImGui_ImplVulkan_RenderDrawData((ImDrawData*)draw_data, (VkCommandBuffer)command_buffer);
}

ImGuiBool ImGui_ImplVulkan_CreateFontsTexture(void) {
    return ImGui_ImplVulkan_CreateFontsTexture();
}

void ImGui_ImplVulkan_DestroyFontUploadObjects(void) {
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void ImGui_ImplVulkan_SetMinImageCount(uint32_t min_image_count) {
    ImGui_ImplVulkan_SetMinImageCount(min_image_count);
}

// OpenGL 3 Backend (Windows)
ImGuiBool ImGui_ImplOpenGL3_Init(const char* glsl_version) {
    return ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGui_ImplOpenGL3_Shutdown(void) {
    ImGui_ImplOpenGL3_Shutdown();
}

void ImGui_ImplOpenGL3_NewFrame(void) {
    ImGui_ImplOpenGL3_NewFrame();
}

void ImGui_ImplOpenGL3_RenderDrawData(void* draw_data) {
    ImGui_ImplOpenGL3_RenderDrawData((ImDrawData*)draw_data);
}

ImGuiBool ImGui_ImplOpenGL3_CreateFontsTexture(void) {
    return ImGui_ImplOpenGL3_CreateFontsTexture();
}

void ImGui_ImplOpenGL3_DestroyFontsTexture(void) {
    ImGui_ImplOpenGL3_DestroyFontsTexture();
}

ImGuiBool ImGui_ImplOpenGL3_CreateDeviceObjects(void) {
    return ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void ImGui_ImplOpenGL3_DestroyDeviceObjects(void) {
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
}

// GLFW Backend (Windows)
ImGuiBool ImGui_ImplGlfw_InitForOpenGL(void* window, ImGuiBool install_callbacks) {
    return ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, install_callbacks != 0);
}

ImGuiBool ImGui_ImplGlfw_InitForVulkan(void* window, ImGuiBool install_callbacks) {
    return ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window, install_callbacks != 0);
}

ImGuiBool ImGui_ImplGlfw_InitForD3D(void* window, ImGuiBool install_callbacks) {
    return ImGui_ImplGlfw_InitForD3D((GLFWwindow*)window, install_callbacks != 0);
}

void ImGui_ImplGlfw_Shutdown(void) {
    ImGui_ImplGlfw_Shutdown();
}

void ImGui_ImplGlfw_NewFrame(void) {
    ImGui_ImplGlfw_NewFrame();
}

// SDL2 Backend (Windows)
ImGuiBool ImGui_ImplSDL2_InitForD3D(void* window) {
    return ImGui_ImplSDL2_InitForD3D((SDL_Window*)window);
}

ImGuiBool ImGui_ImplSDL2_InitForOpenGL(void* window, void* gl_context) {
    return ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window, gl_context);
}

ImGuiBool ImGui_ImplSDL2_InitForVulkan(void* window) {
    return ImGui_ImplSDL2_InitForVulkan((SDL_Window*)window);
}

void ImGui_ImplSDL2_Shutdown(void) {
    ImGui_ImplSDL2_Shutdown();
}

void ImGui_ImplSDL2_NewFrame(void) {
    ImGui_ImplSDL2_NewFrame();
}

#endif // _WIN32

// ============================================================================
// Version Information
// ============================================================================

const char* igGetVersion(void) {
    return ImGui::GetVersion();
}
