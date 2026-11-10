#include "process.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <cstdio>

#pragma comment(lib, "psapi.lib")

namespace sm
{
    bool CollectProcesses(std::vector<ProcEntry>& out, bool withMemory)
    {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) return false;

        PROCESSENTRY32W pe = {};
        pe.dwSize = sizeof(pe);
        if (Process32FirstW(snap, &pe))
        {
            do
            {
                ProcEntry e;
                e.pid = pe.th32ProcessID;
                e.ppid = pe.th32ParentProcessID;
                e.threads = pe.cntThreads;
                e.name = pe.szExeFile;

                if (withMemory && e.pid != 0)
                {
                    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, e.pid);
                    if (h)
                    {
                        PROCESS_MEMORY_COUNTERS pmc = {};
                        pmc.cb = sizeof(pmc);
                        if (GetProcessMemoryInfo(h, &pmc, sizeof(pmc)))
                            e.wsKB = pmc.WorkingSetSize / 1024;
                        CloseHandle(h);
                    }
                }
                out.push_back(e);
            } while (Process32NextW(snap, &pe));
        }
        CloseHandle(snap);
        return true;
    }

    std::vector<std::wstring> FormatProcessLines(const std::vector<ProcEntry>& procs)
    {
        std::vector<std::wstring> lines;
        lines.reserve(procs.size());
        for (const ProcEntry& p : procs)
        {
            wchar_t buf[512];
            swprintf_s(buf, L"%lu|%ls|%lu|%lu|%llu", p.pid, p.name.c_str(), p.threads, p.ppid, p.wsKB);
            lines.push_back(buf);
        }
        return lines;
    }

    HRESULT OpenErrorToHr(DWORD err)
    {
        if (err == ERROR_INVALID_PARAMETER) err = ERROR_NOT_FOUND;
        return HRESULT_FROM_WIN32(err);
    }
}