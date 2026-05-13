# CMakeLists.txt для сборки ImGui со статической библиотекой и C API (Windows)

cmake_minimum_required(VERSION 3.10)
project(imgui VERSION 1.90.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции сборки
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(IMGUI_BUILD_C_API "Build C API wrapper" ON)

# Основные файлы ImGui
set(IMGUI_SOURCES
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
)

set(IMGUI_HEADERS
    imgui.h
    imgui_internal.h
    imconfig.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

# Бекенды Windows - только те, что не требуют внешних зависимостей
if(WIN32)
    list(APPEND IMGUI_SOURCES
        backends/imgui_impl_win32.cpp
        backends/imgui_impl_dx9.cpp
        backends/imgui_impl_dx10.cpp
        backends/imgui_impl_dx11.cpp
        backends/imgui_impl_dx12.cpp
        backends/imgui_impl_opengl3.cpp
    )
    
    list(APPEND IMGUI_HEADERS
        backends/imgui_impl_win32.h
        backends/imgui_impl_dx9.h
        backends/imgui_impl_dx10.h
        backends/imgui_impl_dx11.h
        backends/imgui_impl_dx12.h
        backends/imgui_impl_opengl3.h
        backends/imgui_impl_opengl3_loader.h
    )
endif()

# Создаем статическую библиотеку ImGui
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

# Линковочные зависимости для Windows
if(WIN32)
    target_link_libraries(imgui PUBLIC
        d3d9
        d3d10
        d3d11
        dxgi
        user32
        shell32
        ole32
        imm32
    )
    
    # Добавляем определения для Windows
    target_compile_definitions(imgui PUBLIC
        _WIN32_WINNT=0x0A00
        WINVER=0x0A00
        NOMINMAX
        UNICODE
        _UNICODE
    )
endif()

# C API Wrapper и NS Render API
if(IMGUI_BUILD_C_API AND WIN32)
    set(IMGUI_C_API_SOURCES
        c_api/imgui_c_api.cpp
    )
    
    set(IMGUI_C_API_HEADERS
        c_api/imgui_c_api.h
    )
    
    add_library(imgui_c_api STATIC ${IMGUI_C_API_SOURCES} ${IMGUI_C_API_HEADERS})
    
    target_include_directories(imgui_c_api PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/c_api
    )
    
    target_link_libraries(imgui_c_api PUBLIC imgui)
    
    target_compile_definitions(imgui_c_api PUBLIC
        _WIN32_WINNT=0x0A00
        WINVER=0x0A00
        NOMINMAX
        UNICODE
        _UNICODE
    )

    # NS Render API - обёртка для C# с автодетектом рендерера
    set(NS_RENDER_API_SOURCES
        src/ns_render_api.cpp
        src/renderer_detector.cpp
    )
    
    set(NS_RENDER_API_HEADERS
        src/ns_render_api.h
        src/renderer_detector.h
    )
    
    add_library(ns_render_api SHARED ${NS_RENDER_API_SOURCES} ${NS_RENDER_API_HEADERS})
    
    target_include_directories(ns_render_api PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/c_api
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    
    target_link_libraries(ns_render_api PUBLIC imgui_c_api)
    
    target_compile_definitions(ns_render_api PUBLIC
        _WIN32_WINNT=0x0A00
        WINVER=0x0A00
        NOMINMAX
        UNICODE
        _UNICODE
    )
endif()

# Установка файлов
install(TARGETS imgui
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

if(IMGUI_BUILD_C_API AND WIN32)
    install(TARGETS imgui_c_api ns_render_api
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )
    install(FILES ${IMGUI_C_API_HEADERS} DESTINATION include/imgui)
endif()

install(FILES ${IMGUI_HEADERS} DESTINATION include/imgui)
install(FILES ${NS_RENDER_API_HEADERS} DESTINATION include/ns_render)

# Вывод информации о сборке
message(STATUS "ImGui version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Building for Windows: ${WIN32}")
message(STATUS "Building C API: ${IMGUI_BUILD_C_API}")
