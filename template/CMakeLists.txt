cmake_minimum_required(VERSION 3.15)
project(MyInjectLibrary VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определение типа библиотеки (DLL)
add_library(${PROJECT_NAME} SHARED
    dllmain.cpp
    # Добавьте сюда другие файлы вашего проекта
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Линковка необходимых библиотек Windows
target_link_libraries(${PROJECT_NAME} PRIVATE
    winmm
    d3d9
    d3d10
    d3d11
    d3d12
    dxgi
    opengl32
    vulkan
    user32
)

# Настройки для релизной сборки
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib"
)
