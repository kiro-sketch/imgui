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
# Vulkan отключен по умолчанию, так как требует SDK
option(BUILD_VULKAN "Build with Vulkan support" OFF)

# Пути к ImGui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/..")
set(IMGUI_BACKENDS_DIR "${IMGUI_DIR}/backends")

# Определение типа библиотеки (DLL)
add_library(${PROJECT_NAME} SHARED
    dllmain.cpp
    ns_overlay.cpp
    ns_overlay.h
    winmm.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKENDS_DIR}
)

# Линковка необходимых библиотек Windows
target_link_libraries(${PROJECT_NAME} PRIVATE
    winmm
    user32
    kernel32
    d3d9
    d3d10
    d3d10_1
    d3d11
    d3d12
    dxgi
    opengl32
)

# Опциональная линковка Vulkan
if(BUILD_VULKAN)
    find_package(Vulkan QUIET)
    if(Vulkan_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
        target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_VULKAN)
    endif()
endif()

# Компиляционные определения для всех бэкендов
target_compile_definitions(${PROJECT_NAME} PRIVATE
    BUILD_D3D9
    BUILD_D3D10
    BUILD_D3D11
    BUILD_D3D12
    BUILD_OPENGL
    $<$<BOOL:${BUILD_VULKAN}>:BUILD_VULKAN>
)

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
