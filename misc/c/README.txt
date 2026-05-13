Dear ImGui C API Wrapper
========================

This directory contains C-compatible wrappers for the Dear ImGui C++ API.

Files:
------
- imgui_c_api.h    : Header file with C function declarations
- imgui_c_api.cpp  : Implementation of C wrapper functions

Usage:
------
1. Compile imgui_c_api.cpp along with the standard Dear ImGui source files
2. Include imgui_c_api.h in your C code
3. Link against the resulting static library

Example:
--------
#include "imgui_c_api.h"

int main() {
    igCreateContext(NULL);
    // ... use Dear ImGui via C API (igBegin, igButton, etc.)
    igDestroyContext(NULL);
    return 0;
}

Naming convention:
------------------
All C wrapper functions are prefixed with 'ig' (for ImGui) to avoid naming conflicts.
For example:
- ImGui::Begin()     -> igBegin()
- ImGui::Button()    -> igButton()
- ImGui::GetIO()     -> igGetIO()

Functions that have overloaded versions in C++ get suffixes:
- igBeginChild_Str() - takes const char* id
- igBeginChild_ID()  - takes ImGuiID id
- igPushStyleColor_U32() - takes ImU32 color
- igPushStyleColor_Vec4() - takes ImVec4 color
