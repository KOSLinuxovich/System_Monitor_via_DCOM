#include "win_utils.h"
#include "globals.h"
#include <cstdio>

namespace sm
{
    ULONGLONG ToU64(const FILETIME& ft)
    {
        ULARGE_INTEGER u;
        u.LowPart = ft.dwLowDateTime;
        u.HighPart = ft.dwHighDateTime;
        return u.QuadPart;
    }

    std::wstring FileTimeToLocalText(const FILETIME& ft)
    {
        FILETIME local;
        SYSTEMTIME st;
        if (!FileTimeToLocalFileTime(&ft, &local) || !FileTimeToSystemTime(&local, &st)) return L"?";
        wchar_t buf[64];
        swprintf_s(buf, L"%04u-%02u-%02u %02u:%02u:%02u",
                   st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        return buf;
    }

    std::wstring RegString(HKEY root, const wchar_t* subKey, const wchar_t* value)
    {
        wchar_t buf[256] = {};
        DWORD sz = sizeof(buf);
        if (RegGetValueW(root, subKey, value, RRF_RT_REG_SZ, nullptr, buf, &sz) == ERROR_SUCCESS)
            return buf;
        return L"";
    }

    std::wstring ModuleDirectory()
    {
        wchar_t path[MAX_PATH] = {};
        DWORD len = GetModuleFileNameW(g_hModule, path, MAX_PATH);
        std::wstring p(path, len);
        size_t pos = p.find_last_of(L"\\/");
        return pos == std::wstring::npos ? std::wstring() : p.substr(0, pos + 1);
    }
}