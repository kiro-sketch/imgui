#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// Типы для C API
typedef enum
{
    NS_RENDERER_UNKNOWN = 0,
    NS_RENDERER_DIRECTX9,
    NS_RENDERER_DIRECTX10,
    NS_RENDERER_DIRECTX11,
    NS_RENDERER_DIRECTX12,
    NS_RENDERER_OPENGL,
    NS_RENDERER_VULKAN,
    NS_RENDERER_SOFTWARE
} NsRendererType;

// Инициализация системы рендеринга (вызывается один раз при загрузке)
__declspec(dllexport) int NsInitialize();

// Получение текущего типа рендерера
__declspec(dllexport) NsRendererType NsGetRendererType();

// Получение имени рендерера в виде строки
__declspec(dllexport) const char* NsGetRendererName();

// === API для рисования окон (не зависит от бекенда) ===

// Начало фрейма рисования
__declspec(dllexport) void NsBeginFrame();

// Конец фрейма рисования
__declspec(dllexport) void NsEndFrame();

// Создание окна
// Возвращает ID окна (int), который используется для дальнейших операций
__declspec(dllexport) int NsCreateWindow(const char* title, float x, float y, float width, float height);

// Удаление окна по ID
__declspec(dllexport) void NsDestroyWindow(int windowId);

// Установка позиции окна
__declspec(dllexport) void NsSetWindowPosition(int windowId, float x, float y);

// Установка размера окна
__declspec(dllexport) void NsSetWindowSize(int windowId, float width, float height);

// Установка заголовка окна
__declspec(dllexport) void NsSetWindowTitle(int windowId, const char* title);

// Проверка, закрыто ли окно пользователем
__declspec(dllexport) int NsIsWindowClosed(int windowId);

// Проверка, активно ли окно (в фокусе)
__declspec(dllexport) int NsIsWindowFocused(int windowId);

// Получение позиции окна
__declspec(dllexport) void NsGetWindowPosition(int windowId, float* outX, float* outY);

// Получение размера окна
__declspec(dllexport) void NsGetWindowSize(int windowId, float* outWidth, float* outHeight);

// === Элементы управления внутри окон ===

// Отображение текста
__declspec(dllexport) void NsText(int windowId, const char* text);

// Отображение кнопки
// Возвращает 1, если кнопка была нажата в этом фрейме, иначе 0
__declspec(dllexport) int NsButton(int windowId, const char* label);

// Отображение чекбокса
// value - указатель на значение (int: 0 или 1)
// Возвращает 1, если значение изменилось, иначе 0
__declspec(dllexport) int NsCheckbox(int windowId, const char* label, int* value);

// Отображение слайдера (float)
// Возвращает 1, если значение изменилось, иначе 0
__declspec(dllexport) int NsSliderFloat(int windowId, const char* label, float* value, float min, float max);

// Отображение слайдера (int)
// Возвращает 1, если значение изменилось, иначе 0
__declspec(dllexport) int NsSliderInt(int windowId, const char* label, int* value, int min, int max);

// Отображение поля ввода текста
// buffer - буфер для ввода, bufferSize - размер буфера
// Возвращает 1, если текст изменился, иначе 0
__declspec(dllexport) int NsInputText(int windowId, const char* label, char* buffer, int bufferSize);

// Разделитель
__declspec(dllexport) void NsSeparator(int windowId);

// Отступ
__declspec(dllexport) void NsSpacing(int windowId);

// Очистка всех окон
__declspec(dllexport) void NsClearWindows();

// Завершение работы системы
__declspec(dllexport) void NsShutdown();

#ifdef __cplusplus
}
#endif
