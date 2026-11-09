#include "system_info.h"
#include "system_stats.h"
#include "text_utils.h"
#include "win_utils.h"
#include <cstdio>

namespace sm
{
    namespace
    {
        typedef LONG(WINAPI* RtlGetVersionFn)(PRTL_OSVERSIONINFOW);

        RTL_OSVERSIONINFOW QueryOsVersion()
        {
            RTL_OSVERSIONINFOW vi = {};
            vi.dwOSVersionInfoSize = sizeof(vi);
            HMODULE nt = GetModuleHandleW(L"ntdll.dll");
            RtlGetVersionFn fn = nt ? reinterpret_cast<RtlGetVersionFn>(GetProcAddress(nt, "RtlGetVersion")) : nullptr;
            if (fn) fn(&vi);
            return vi;
        }

        std::wstring OsLine()
        {
            const wchar_t* key = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
            std::wstring product = RegString(HKEY_LOCAL_MACHINE, key, L"ProductName");
            std::wstring display = RegString(HKEY_LOCAL_MACHINE, key, L"DisplayVersion");
            RTL_OSVERSIONINFOW vi = QueryOsVersion();

            if (vi.dwBuildNumber >= 22000 && product.compare(0, 10, L"Windows 10") == 0)
                product.replace(0, 10, L"Windows 11");

            wchar_t buf[256];
            swprintf_s(buf, L"OS: %ls %ls (build %lu)\n", product.c_str(), display.c_str(), vi.dwBuildNumber);
            return buf;
        }

        std::wstring CpuLine()
        {
            std::wstring cpu = RegString(HKEY_LOCAL_MACHINE,
                L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"ProcessorNameString");
            size_t first = cpu.find_first_not_of(L' ');
            if (first == std::wstring::npos) return L"";
            return L"CPU: " + cpu.substr(first) + L"\n";
        }

        std::wstring ArchLine()
        {
            SYSTEM_INFO si = {};
            GetNativeSystemInfo(&si);
            const wchar_t* arch = L"Unknown";
            if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) arch = L"x64";
            else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) arch = L"x86";
            else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64) arch = L"ARM64";

            wchar_t buf[128];
            swprintf_s(buf, L"Architecture: %ls | Logical processors: %lu\n", arch, si.dwNumberOfProcessors);
            return buf;
        }

        std::wstring RamLine()
        {
            MEMORYSTATUSEX m = {};
            m.dwLength = sizeof(m);
            if (!GlobalMemoryStatusEx(&m)) return L"";

            wchar_t buf[256];
            swprintf_s(buf, L"RAM: %ls total, %ls free (%lu%% in use)\n",
                FormatBytes(m.ullTotalPhys).c_str(), FormatBytes(m.ullAvailPhys).c_str(), m.dwMemoryLoad);
            return buf;
        }

        std::wstring IdentityLines()
        {
            std::wstring r;
            wchar_t comp[MAX_COMPUTERNAME_LENGTH + 1] = {};
            DWORD cl = MAX_COMPUTERNAME_LENGTH + 1;
            if (GetComputerNameW(comp, &cl)) r += L"Computer: " + std::wstring(comp) + L"\n";

            wchar_t user[257] = {};
            DWORD ul = 257;
            if (GetUserNameW(user, &ul)) r += L"User: " + std::wstring(user) + L"\n";
            return r;
        }
    }

    std::wstring BuildSystemInfoText()
    {
        return IdentityLines() + OsLine() + CpuLine() + ArchLine() + RamLine() + L"Uptime: " + UptimeText();
    }
}