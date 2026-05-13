#pragma once

#include <windows.h>
#include <vector>

// Глобальные переменные для управления инжектом

extern HMODULE g_hCurrentModule;

// Функция загрузки библиотеки (реализуется пользователем)
BOOL NsLoad();



