#include "winmm.h"

std::vector<HMODULE> g_InjectDlls{};
HMODULE g_hCurrentModule = NULL;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            g_hCurrentModule = hModule;
            DisableThreadLibraryCalls(hModule);
            
            // TODO: Инициализация вашей библиотеки
            if (!NsLoad())
                return FALSE;
        }
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
        {
            // TODO: Очистка ресурсов
            // UnloadInjectDlls(g_InjectDlls);
        }
            break;
    }
    return TRUE;
}
