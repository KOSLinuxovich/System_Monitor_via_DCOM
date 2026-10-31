#ifndef SYSTEM_STATS_H
#define SYSTEM_STATS_H

#include <windows.h>
#include <string>

namespace sm
{
    bool MeasureCpu(double& cpu);
    bool MemoryText(std::wstring& out);
    HRESULT DiskFreeGb(double& gb);
    std::wstring UptimeText();
}

#endif