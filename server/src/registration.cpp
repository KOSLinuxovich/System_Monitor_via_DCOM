#include "globals.h"
#include <olectl.h>
#include <cwchar>

namespace
{
    struct ClassInfo
    {
        const wchar_t* clsid;
        const wchar_t* progId;
        const wchar_t* name;
    };

    const ClassInfo kClasses[] =
    {
        { L"{BDB79690-6A8F-4F29-A5EB-C35BEEE287B4}", L"SystemMonitor.Monitor.1",   L"System Monitor (Lab 1)" },
        { L"{89788D68-E5F7-4ED6-B513-6587B98FEB33}", L"SystemMonitor.MonitorV2.1", L"System Monitor V2 (Lab 2)" },
        { L"{80ED9BC2-480D-4C98-A5DC-0240E9BB2B7A}", L"SystemMonitor.MonitorV3.1", L"System Monitor V3 (Lab 3)" }
    };

    LSTATUS SetStr(const wchar_t* subKey, const wchar_t* valueName, const wchar_t* value)
    {
        return RegSetKeyValueW(HKEY_CLASSES_ROOT, subKey, valueName, REG_SZ,
                               value, (DWORD)((wcslen(value) + 1) * sizeof(wchar_t)));
    }

    LSTATUS RegisterOne(const ClassInfo& c, const wchar_t* dllPath)
    {
        wchar_t key[256];
        LSTATUS rc;

        swprintf_s(key, L"CLSID\\%s", c.clsid);
        if ((rc = SetStr(key, nullptr, c.name)) != ERROR_SUCCESS) return rc;

        swprintf_s(key, L"CLSID\\%s\\InProcServer32", c.clsid);
        if ((rc = SetStr(key, nullptr, dllPath)) != ERROR_SUCCESS) return rc;
        if ((rc = SetStr(key, L"ThreadingModel", L"Both")) != ERROR_SUCCESS) return rc;

        swprintf_s(key, L"CLSID\\%s\\ProgID", c.clsid);
        if ((rc = SetStr(key, nullptr, c.progId)) != ERROR_SUCCESS) return rc;

        swprintf_s(key, L"%s", c.progId);
        if ((rc = SetStr(key, nullptr, c.name)) != ERROR_SUCCESS) return rc;

        swprintf_s(key, L"%s\\CLSID", c.progId);
        return SetStr(key, nullptr, c.clsid);
    }
}

STDAPI DllRegisterServer(void)
{
    wchar_t dllPath[MAX_PATH];
    if (GetModuleFileNameW(g_hModule, dllPath, MAX_PATH) == 0)
        return HRESULT_FROM_WIN32(GetLastError());

    for (const ClassInfo& c : kClasses)
    {
        LSTATUS rc = RegisterOne(c, dllPath);
        if (rc != ERROR_SUCCESS) return HRESULT_FROM_WIN32(rc);
    }
    return S_OK;
}

STDAPI DllUnregisterServer(void)
{
    for (const ClassInfo& c : kClasses)
    {
        wchar_t key[256];
        swprintf_s(key, L"CLSID\\%s", c.clsid);
        RegDeleteTreeW(HKEY_CLASSES_ROOT, key);
        RegDeleteTreeW(HKEY_CLASSES_ROOT, c.progId);
    }
    return S_OK;
}