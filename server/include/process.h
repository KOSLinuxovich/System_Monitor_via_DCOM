#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>
#include <string>
#include <vector>

namespace sm
{
    struct ProcEntry
    {
        DWORD pid = 0;
        DWORD ppid = 0;
        DWORD threads = 0;
        std::wstring name;
        ULONGLONG wsKB = 0;
    };

    bool CollectProcesses(std::vector<ProcEntry>& out, bool withMemory);
    std::vector<std::wstring> FormatProcessLines(const std::vector<ProcEntry>& procs);
    HRESULT OpenErrorToHr(DWORD err);
    HRESULT BuildProcessInfo(DWORD pid, std::wstring& out);
    HRESULT TerminateProcessById(DWORD pid);
}

#endif