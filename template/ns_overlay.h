#pragma once

#include <windows.h>
#include <vector>
#include <string>

// Глобальные переменные
extern std::vector<HMODULE> g_InjectDlls;
extern HMODULE g_hCurrentModule;

// Типы рендеров
enum class RenderType {
    Unknown,
    DirectX9,
    DirectX10,
    DirectX11,
    DirectX12,
    OpenGL,
    Vulkan
};

// Инициализация библиотеки (вызывается из DllMain)
bool NsLoad();

// Очистка при выгрузке
void NsUnload();

// Получить текущий тип рендера
RenderType NsGetRenderType();

// ==========================================
// C API для экспорта в C# (P/Invoke)
// ==========================================
#ifdef __cplusplus
extern "C" {
#endif

    // Инициализация оверлея (вызывать после загрузки DLL)
    __declspec(dllexport) bool Ns_Initialize();
    
    // Начало кадра (вызывать перед рисованием)
    __declspec(dllexport) void Ns_BeginFrame();
    
    // Конец кадра (вызывать после рисования)
    __declspec(dllexport) void Ns_EndFrame();

    // Рисование окна
    // title: заголовок окна (UTF-8)
    // x, y: позиция
    // w, h: размеры
    // visible: видно ли окно
    // returns: true если окно активно (не закрыто пользователем)
    __declspec(dllexport) bool Ns_DrawWindow(const char* title, float x, float y, float w, float h, bool visible);

    // Рисование текста
    __declspec(dllexport) void Ns_DrawText(const char* text, float x, float y);

    // Проверка, был ли клик мыши
    __declspec(dllexport) bool Ns_IsMouseClicked(int button);

    // Освобождение ресурсов
    __declspec(dllexport) void Ns_Shutdown();

#ifdef __cplusplus
}
#endif
