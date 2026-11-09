#include "system_stats.h"
#include "win_utils.h"
#include <cstdio>

namespace sm
{
    bool MeasureCpu(double& cpu)
    {
        FILETIME idle1, kernel1, user1, idle2, kernel2, user2;
        if (!GetSystemTimes(&idle1, &kernel1, &user1)) return false;
        Sleep(500);
        if (!GetSystemTimes(&idle2, &kernel2, &user2)) return false;

        ULONGLONG idle   = ToU64(idle2)   - ToU64(idle1);
        ULONGLONG kernel = ToU64(kernel2) - ToU64(kernel1);
        ULONGLONG user   = ToU64(user2)   - ToU64(user1);
        ULONGLONG total  = kernel + user;

        cpu = (total == 0) ? 0.0 : (double)(total - idle) / (double)total * 100.0;
        if (cpu < 0.0) cpu = 0.0;
        if (cpu > 100.0) cpu = 100.0;
        return true;
    }

    bool MemoryText(std::wstring& out)
    {
        MEMORYSTATUSEX m = {};
        m.dwLength = sizeof(m);
        if (!GlobalMemoryStatusEx(&m)) return false;

        const double gb = 1024.0 * 1024.0 * 1024.0;
        wchar_t buf[256];
        swprintf_s(buf, L"Total: %.2f GB, Free: %.2f GB",
                   (double)m.ullTotalPhys / gb, (double)m.ullAvailPhys / gb);
        out = buf;
        return true;
    }

    HRESULT DiskFreeGb(double& gb)
    {
        gb = 0.0;
        ULARGE_INTEGER freeBytes, totalBytes, totalFree;
        if (!GetDiskFreeSpaceExW(L"C:\\", &freeBytes, &totalBytes, &totalFree))
            return HRESULT_FROM_WIN32(GetLastError());
        gb = (double)freeBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
        return S_OK;
    }

    std::wstring UptimeText()
    {
        ULONGLONG sec = GetTickCount64() / 1000;
        wchar_t buf[256];
        swprintf_s(buf, L"%llu days, %llu hours, %llu minutes",
                   sec / 86400, (sec % 86400) / 3600, (sec % 3600) / 60);
        return buf;
    }
}