#pragma once

#ifdef _WIN32
    #define IMGUI_C_API_EXPORT __declspec(dllexport)
#else
    #define IMGUI_C_API_EXPORT
#endif

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Simple C-compatible types matching ImGui's layout
// We use distinct names (Vec2, Vec4) to avoid conflict with ImVec2/ImVec4 from imgui.h
typedef struct { float x, y; } Vec2;
typedef struct { float x, y, z, w; } Vec4;

// --- Core API Wrappers ---

// Context
IMGUI_C_API_EXPORT void* igCreateContext(void* font_data);
IMGUI_C_API_EXPORT void igDestroyContext(void* ctx);
IMGUI_C_API_EXPORT void igSetCurrentContext(void* ctx);
IMGUI_C_API_EXPORT void* igGetCurrentContext();

// IO
IMGUI_C_API_EXPORT void* igGetIO();
IMGUI_C_API_EXPORT void igSetMousePos(float x, float y);
IMGUI_C_API_EXPORT void igSetMouseDown(int button, bool down);
IMGUI_C_API_EXPORT void igSetKey(int key, bool down);
IMGUI_C_API_EXPORT void igAddInputCharacter(unsigned int c);

// Main Loop
IMGUI_C_API_EXPORT void igNewFrame();
IMGUI_C_API_EXPORT void igRender();
IMGUI_C_API_EXPORT void* igGetDrawData();

// Windows
IMGUI_C_API_EXPORT bool igBegin(const char* name, bool* p_open, int flags);
IMGUI_C_API_EXPORT void igEnd();
IMGUI_C_API_EXPORT bool igBeginChild(const char* str_id, float w, float h, bool border, int flags);
IMGUI_C_API_EXPORT void igEndChild();

// Basic Widgets
IMGUI_C_API_EXPORT void igText(const char* fmt, ...);
IMGUI_C_API_EXPORT void igTextColored(float r, float g, float b, float a, const char* fmt, ...);
IMGUI_C_API_EXPORT void igButton(const char* label, float w, float h);
IMGUI_C_API_EXPORT bool igCheckbox(const char* label, bool* v);
IMGUI_C_API_EXPORT bool igSliderFloat(const char* label, float* v, float v_min, float v_max, const char* fmt);
IMGUI_C_API_EXPORT bool igSliderInt(const char* label, int* v, int v_min, int v_max, const char* fmt);
IMGUI_C_API_EXPORT bool igInputText(const char* label, char* buf, int buf_size, int flags);
IMGUI_C_API_EXPORT bool igInputFloat(const char* label, float* v, float step, float step_fast, const char* fmt);
IMGUI_C_API_EXPORT bool igInputInt(const char* label, int* v, int step, int step_fast, int flags);

// Colors
IMGUI_C_API_EXPORT void igColorPicker3(const char* label, float* col);
IMGUI_C_API_EXPORT void igColorPicker4(const char* label, float* col, int flags);
IMGUI_C_API_EXPORT bool igColorEdit3(const char* label, float* col, int flags);
IMGUI_C_API_EXPORT bool igColorEdit4(const char* label, float* col, int flags);

// Layout / Style
IMGUI_C_API_EXPORT void igSetNextWindowSize(float w, float h, int cond);
IMGUI_C_API_EXPORT void igSetNextWindowPos(float x, float y, int cond);
IMGUI_C_API_EXPORT void igPushStyleColor(int idx, float r, float g, float b, float a);
IMGUI_C_API_EXPORT void igPopStyleColor(int count);
IMGUI_C_API_EXPORT void igPushStyleVar(int idx, float val);
IMGUI_C_API_EXPORT void igPushStyleVarVec2(int idx, float x, float y);
IMGUI_C_API_EXPORT void igPopStyleVar(int count);

// Drawing (ImDrawList wrappers)
IMGUI_C_API_EXPORT void* igGetBackgroundDrawList(); 
IMGUI_C_API_EXPORT void* igGetForegroundDrawList(); 

IMGUI_C_API_EXPORT void ImDrawList_AddLine(void* list, float x1, float y1, float x2, float y2, unsigned int col, float thickness);
IMGUI_C_API_EXPORT void ImDrawList_AddRect(void* list, float x1, float y1, float x2, float y2, unsigned int col, float rounding, int flags, float thickness);
IMGUI_C_API_EXPORT void ImDrawList_AddRectFilled(void* list, float x1, float y1, float x2, float y2, unsigned int col, float rounding, int flags);
IMGUI_C_API_EXPORT void ImDrawList_AddCircle(void* list, float cx, float cy, float radius, unsigned int col, int num_segments, float thickness);
IMGUI_C_API_EXPORT void ImDrawList_AddCircleFilled(void* list, float cx, float cy, float radius, unsigned int col, int num_segments);
IMGUI_C_API_EXPORT void ImDrawList_AddText(void* list, float x, float y, unsigned int col, const char* text);
IMGUI_C_API_EXPORT void ImDrawList_AddBezierCubic(void* list, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float p4x, float p4y, unsigned int col, float thickness, int num_segments);

// Helpers for Item Rect (using out-parameters instead of returning ImVec2)
IMGUI_C_API_EXPORT void igGetItemRectMin(float* out_x, float* out_y);
IMGUI_C_API_EXPORT void igGetItemRectMax(float* out_x, float* out_y);
IMGUI_C_API_EXPORT void igGetItemRectSize(float* out_x, float* out_y);

#ifdef __cplusplus
}
#endif
