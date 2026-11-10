#include "process.h"
#include "text_utils.h"
#include "win_utils.h"
#include <psapi.h>
#include <cstdio>

#pragma comment(lib, "psapi.lib")

namespace sm
{
    namespace
    {
        const wchar_t* PriorityName(DWORD p)
        {
            switch (p)
            {
            case IDLE_PRIORITY_CLASS:         return L"Idle";
            case BELOW_NORMAL_PRIORITY_CLASS: return L"Below normal";
            case NORMAL_PRIORITY_CLASS:       return L"Normal";
            case ABOVE_NORMAL_PRIORITY_CLASS: return L"Above normal";
            case HIGH_PRIORITY_CLASS:         return L"High";
            case REALTIME_PRIORITY_CLASS:     return L"Realtime";
            default:                          return L"Unknown";
            }
        }
    }

    HRESULT BuildProcessInfo(DWORD pid, std::wstring& out)
    {
        HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | SYNCHRONIZE, FALSE, pid);
        if (!h) return OpenErrorToHr(GetLastError());

        if (WaitForSingleObject(h, 0) == WAIT_OBJECT_0)
        {
            CloseHandle(h);
            return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
        }

        std::vector<ProcEntry> procs;
        CollectProcesses(procs, false);
        const ProcEntry* me = nullptr;
        for (const ProcEntry& p : procs)
        {
            if (p.pid == pid)
            {
                me = &p;
                break;
            }
        }

        std::wstring info;
        wchar_t buf[512];

        swprintf_s(buf, L"Process: %ls (PID %lu)\n", me ? me->name.c_str() : L"?", pid);
        info += buf;

        wchar_t path[MAX_PATH * 2] = {};
        DWORD plen = (DWORD)(sizeof(path) / sizeof(path[0]));
        if (QueryFullProcessImageNameW(h, 0, path, &plen))
            info += L"Path: " + std::wstring(path) + L"\n";

        if (me)
        {
            swprintf_s(buf, L"Parent PID: %lu | Threads: %lu", me->ppid, me->threads);
            info += buf;
        }

        DWORD handles = 0;
        if (GetProcessHandleCount(h, &handles))
        {
            swprintf_s(buf, L" | Handles: %lu", handles);
            info += buf;
        }
        info += L"\n";

        BOOL wow = FALSE;
        IsWow64Process(h, &wow);
        swprintf_s(buf, L"Bitness: %ls | Priority: %ls\n",
                   wow ? L"32-bit" : L"64-bit", PriorityName(GetPriorityClass(h)));
        info += buf;

        PROCESS_MEMORY_COUNTERS_EX pmc = {};
        pmc.cb = sizeof(pmc);
        if (GetProcessMemoryInfo(h, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc)))
        {
            info += L"Memory: working set " + FormatBytes(pmc.WorkingSetSize)
                  + L", peak " + FormatBytes(pmc.PeakWorkingSetSize)
                  + L", private " + FormatBytes(pmc.PrivateUsage) + L"\n";
        }

        FILETIME create, exitTime, kernel, user;
        if (GetProcessTimes(h, &create, &exitTime, &kernel, &user))
        {
            double cpuSec = (double)(ToU64(kernel) + ToU64(user)) / 10000000.0;
            swprintf_s(buf, L"CPU time: %.2f s\nStarted: %ls", cpuSec, FileTimeToLocalText(create).c_str());
            info += buf;
        }

        CloseHandle(h);
        out = info;
        return S_OK;
    }
}