#include "report.h"
#include "database.h"
#include "network.h"
#include "process.h"
#include "system_info.h"
#include "system_stats.h"
#include "text_utils.h"
#include <algorithm>
#include <cstdio>

namespace sm
{
    namespace
    {
        std::wstring HeaderSection()
        {
            SYSTEMTIME now;
            GetLocalTime(&now);
            wchar_t buf[256];
            swprintf_s(buf, L"=== SYSTEM REPORT ===\r\nGenerated: %04u-%02u-%02u %02u:%02u:%02u\r\n\r\n",
                       now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond);
            return buf;
        }

        std::wstring SystemSection()
        {
            return L"--- System ---\r\n" + ToCrlf(BuildSystemInfoText()) + L"\r\n\r\n";
        }

        std::wstring StatsSection()
        {
            double cpu = 0.0;
            MeasureCpu(cpu);
            std::wstring mem;
            MemoryText(mem);
            double disk = 0.0;
            DiskFreeGb(disk);

            wchar_t buf[512];
            swprintf_s(buf, L"--- Current stats ---\r\nCPU: %.2f%%\r\nMemory: %ls\r\nDisk C: %.2f GB free\r\n\r\n",
                       cpu, mem.c_str(), disk);
            return buf;
        }

        std::wstring NetworkSection()
        {
            std::wstring r = L"--- Network ---\r\n";
            std::vector<std::wstring> net;
            if (CollectNetwork(net))
                for (const std::wstring& line : net) r += line + L"\r\n";
            return r + L"\r\n";
        }

        std::wstring ProcessSection()
        {
            std::wstring r = L"--- Top 10 processes by memory ---\r\n";
            std::vector<ProcEntry> procs;
            if (CollectProcesses(procs, true))
            {
                std::sort(procs.begin(), procs.end(),
                    [](const ProcEntry& a, const ProcEntry& b) { return a.wsKB > b.wsKB; });
                for (size_t i = 0; i < procs.size() && i < 10; ++i)
                {
                    wchar_t buf[256];
                    swprintf_s(buf, L"%6lu  %-32ls %10llu KB\r\n", procs[i].pid, procs[i].name.c_str(), procs[i].wsKB);
                    r += buf;
                }
            }
            return r + L"\r\n";
        }

        std::wstring HistorySection()
        {
            std::string rows;
            if (!QueryHistory(5, rows)) return L"";
            return L"--- Last 5 snapshots ---\r\n" + ToCrlf(Utf8ToWide(rows)) + L"\r\n";
        }
    }

    std::wstring BuildReportText()
    {
        return HeaderSection() + SystemSection() + StatsSection()
             + NetworkSection() + ProcessSection() + HistorySection();
    }
}