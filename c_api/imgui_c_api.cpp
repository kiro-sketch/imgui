#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx9.h"
#include "backends/imgui_impl_dx10.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_c_api.h"

// Context
extern "C" void* igCreateContext(void* font_data) {
    return ImGui::CreateContext(reinterpret_cast<ImFontAtlas*>(font_data));
}

extern "C" void igDestroyContext(void* ctx) {
    ImGui::DestroyContext(reinterpret_cast<ImGuiContext*>(ctx));
}

extern "C" void igSetCurrentContext(void* ctx) {
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

extern "C" void* igGetCurrentContext() {
    return ImGui::GetCurrentContext();
}

// IO
extern "C" void* igGetIO() {
    return &ImGui::GetIO();
}

extern "C" void igSetMousePos(float x, float y) {
    ImGui::GetIO().MousePos = ImVec2(x, y);
}

extern "C" void igSetMouseDown(int button, bool down) {
    ImGui::GetIO().MouseDown[button] = down;
}

extern "C" void igSetKey(int key, bool down) {
    ImGui::GetIO().KeysDown[key] = down;
}

extern "C" void igAddInputCharacter(unsigned int c) {
    ImGui::GetIO().AddInputCharacter(c);
}

// Main Loop
extern "C" void igNewFrame() {
    ImGui::NewFrame();
}

extern "C" void igRender() {
    ImGui::Render();
}

extern "C" void* igGetDrawData() {
    return ImGui::GetDrawData();
}

// Windows
extern "C" bool igBegin(const char* name, bool* p_open, int flags) {
    return ImGui::Begin(name, p_open, flags);
}

extern "C" void igEnd() {
    ImGui::End();
}

extern "C" bool igBeginChild(const char* str_id, float w, float h, bool border, int flags) {
    return ImGui::BeginChild(str_id, ImVec2(w, h), border, flags);
}

extern "C" void igEndChild() {
    ImGui::EndChild();
}

// Basic Widgets
extern "C" void igText(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextV(fmt, args);
    va_end(args);
}

extern "C" void igTextColored(float r, float g, float b, float a, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextColoredV(ImVec4(r, g, b, a), fmt, args);
    va_end(args);
}

extern "C" void igButton(const char* label, float w, float h) {
    ImGui::Button(label, ImVec2(w, h));
}

extern "C" bool igCheckbox(const char* label, bool* v) {
    return ImGui::Checkbox(label, v);
}

extern "C" bool igSliderFloat(const char* label, float* v, float v_min, float v_max, const char* fmt) {
    return ImGui::SliderFloat(label, v, v_min, v_max, fmt);
}

extern "C" bool igSliderInt(const char* label, int* v, int v_min, int v_max, const char* fmt) {
    return ImGui::SliderInt(label, v, v_min, v_max, fmt);
}

extern "C" bool igInputText(const char* label, char* buf, int buf_size, int flags) {
    return ImGui::InputText(label, buf, buf_size, flags);
}

extern "C" bool igInputFloat(const char* label, float* v, float step, float step_fast, const char* fmt) {
    return ImGui::InputFloat(label, v, step, step_fast, fmt);
}

extern "C" bool igInputInt(const char* label, int* v, int step, int step_fast, int flags) {
    return ImGui::InputInt(label, v, step, step_fast, flags);
}

// Colors
extern "C" void igColorPicker3(const char* label, float* col) {
    ImGui::ColorPicker3(label, col);
}

extern "C" void igColorPicker4(const char* label, float* col, int flags) {
    ImGui::ColorPicker4(label, col, flags);
}

extern "C" bool igColorEdit3(const char* label, float* col, int flags) {
    return ImGui::ColorEdit3(label, col, flags);
}

extern "C" bool igColorEdit4(const char* label, float* col, int flags) {
    return ImGui::ColorEdit4(label, col, flags);
}

// Layout / Style
extern "C" void igSetNextWindowSize(float w, float h, int cond) {
    ImGui::SetNextWindowSize(ImVec2(w, h), cond);
}

extern "C" void igSetNextWindowPos(float x, float y, int cond) {
    ImGui::SetNextWindowPos(ImVec2(x, y), cond);
}

extern "C" void igPushStyleColor(int idx, float r, float g, float b, float a) {
    ImGui::PushStyleColor(idx, ImVec4(r, g, b, a));
}

extern "C" void igPopStyleColor(int count) {
    ImGui::PopStyleColor(count);
}

extern "C" void igPushStyleVar(int idx, float val) {
    ImGui::PushStyleVar(idx, val);
}

extern "C" void igPushStyleVarVec2(int idx, float x, float y) {
    ImGui::PushStyleVar(idx, ImVec2(x, y));
}

extern "C" void igPopStyleVar(int count) {
    ImGui::PopStyleVar(count);
}

// Drawing
extern "C" void* igGetBackgroundDrawList() {
    return ImGui::GetBackgroundDrawList();
}

extern "C" void* igGetForegroundDrawList() {
    return ImGui::GetForegroundDrawList();
}

extern "C" void ImDrawList_AddLine(void* list, float x1, float y1, float x2, float y2, unsigned int col, float thickness) {
    reinterpret_cast<ImDrawList*>(list)->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), col, thickness);
}

extern "C" void ImDrawList_AddRect(void* list, float x1, float y1, float x2, float y2, unsigned int col, float rounding, int flags, float thickness) {
    reinterpret_cast<ImDrawList*>(list)->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), col, rounding, flags, thickness);
}

extern "C" void ImDrawList_AddRectFilled(void* list, float x1, float y1, float x2, float y2, unsigned int col, float rounding, int flags) {
    reinterpret_cast<ImDrawList*>(list)->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col, rounding, flags);
}

extern "C" void ImDrawList_AddCircle(void* list, float cx, float cy, float radius, unsigned int col, int num_segments, float thickness) {
    reinterpret_cast<ImDrawList*>(list)->AddCircle(ImVec2(cx, cy), radius, col, num_segments, thickness);
}

extern "C" void ImDrawList_AddCircleFilled(void* list, float cx, float cy, float radius, unsigned int col, int num_segments) {
    reinterpret_cast<ImDrawList*>(list)->AddCircleFilled(ImVec2(cx, cy), radius, col, num_segments);
}

extern "C" void ImDrawList_AddText(void* list, float x, float y, unsigned int col, const char* text) {
    reinterpret_cast<ImDrawList*>(list)->AddText(ImVec2(x, y), col, text);
}

extern "C" void ImDrawList_AddBezierCubic(void* list, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y, unsigned int col, float thickness, int num_segments) {
    reinterpret_cast<ImDrawList*>(list)->AddBezierCubic(ImVec2(p1x, p1y), ImVec2(p2x, p2y), ImVec2(p3x, p3y), ImVec2(p4x, p4y), col, thickness, num_segments);
}

// Helpers for Item Rect (using out-parameters)
extern "C" void igGetItemRectMin(float* out_x, float* out_y) {
    ImVec2 min = ImGui::GetItemRectMin();
    *out_x = min.x;
    *out_y = min.y;
}

extern "C" void igGetItemRectMax(float* out_x, float* out_y) {
    ImVec2 max = ImGui::GetItemRectMax();
    *out_x = max.x;
    *out_y = max.y;
}

extern "C" void igGetItemRectSize(float* out_x, float* out_y) {
    ImVec2 size = ImGui::GetItemRectSize();
    *out_x = size.x;
    *out_y = size.y;
}
