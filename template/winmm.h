#pragma once

#include <windows.h>
#include <vector>

// Глобальные переменные для управления инжектом
extern std::vector<HMODULE> g_InjectDlls;
extern HMODULE g_hCurrentModule;

// Функция загрузки библиотеки (реализуется пользователем)
BOOL NsLoad();

// Функция выгрузки инжектированных DLL (опционально)
void UnloadInjectDlls(std::vector<HMODULE>& dlls);

#endif // WINMM_H