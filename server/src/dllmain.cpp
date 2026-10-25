#include "factory.h"
#include "globals.h"
#include <new>

HMODULE g_hModule     = nullptr;
LONG    g_objectCount = 0;
LONG    g_lockCount   = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    if (ppv == nullptr) return E_POINTER;
    *ppv = nullptr;

    if (rclsid != CLSID_SystemMonitor && rclsid != CLSID_SystemMonitorV2 && rclsid != CLSID_SystemMonitorV3)
        return CLASS_E_CLASSNOTAVAILABLE;

    CClassFactory* factory = new (std::nothrow) CClassFactory();
    if (factory == nullptr) return E_OUTOFMEMORY;

    HRESULT hr = factory->QueryInterface(riid, ppv);
    factory->Release();
    return hr;
}

STDAPI DllCanUnloadNow(void)
{
    return (g_objectCount == 0 && g_lockCount == 0) ? S_OK : S_FALSE;
}