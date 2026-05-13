#include "winmm.h"
#include "ns_overlay.h"

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
            
            // Инициализация библиотеки с авто-определением рендера
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
            // Очистка ресурсов и отцепление хуков
            NsUnload();
        }
            break;
    }
    return TRUE;
}
