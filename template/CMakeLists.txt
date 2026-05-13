cmake_minimum_required(VERSION 3.15)
project(MyInjectLibrary VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции для выбора бэкендов (все включены по умолчанию)
option(BUILD_D3D9 "Build with DirectX 9 support" ON)
option(BUILD_D3D10 "Build with DirectX 10 support" ON)
option(BUILD_D3D11 "Build with DirectX 11 support" ON)
option(BUILD_D3D12 "Build with DirectX 12 support" ON)
option(BUILD_OPENGL "Build with OpenGL support" ON)
option(BUILD_VULKAN "Build with Vulkan support" ON)

# Определение типа библиотеки (DLL)
add_library(${PROJECT_NAME} SHARED
    dllmain.cpp
    ns_overlay.cpp
    ns_overlay.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Пути к ImGui (если используется отдельная сборка)
    # ${CMAKE_SOURCE_DIR}/imgui
    # ${CMAKE_SOURCE_DIR}/imgui/backends
)

# Линковка необходимых библиотек Windows
target_link_libraries(${PROJECT_NAME} PRIVATE
    winmm
    user32
    kernel32
)

# Опциональная линковка графических библиотек
if(BUILD_D3D9)
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d9)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_D3D9)
endif()

if(BUILD_D3D10)
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d10 d3d10_1)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_D3D10)
endif()

if(BUILD_D3D11)
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d11)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_D3D11)
endif()

if(BUILD_D3D12)
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d12 dxgi)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_D3D12)
endif()

if(BUILD_OPENGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_OPENGL)
endif()

if(BUILD_VULKAN)
    target_link_libraries(${PROJECT_NAME} PRIVATE vulkan)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_VULKAN)
endif()

# Настройки для релизной сборки
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib"
)

# Оптимизации для релиза
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Release>:/O2 /Ob2 /DNDEBUG>
    )
endif()
