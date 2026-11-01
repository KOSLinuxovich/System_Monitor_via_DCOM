#include "process.h"

namespace sm
{
    HRESULT TerminateProcessById(DWORD pid)
    {
        if (pid == GetCurrentProcessId()) return E_ACCESSDENIED;

        HANDLE h = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION | SYNCHRONIZE, FALSE, pid);
        if (!h) return OpenErrorToHr(GetLastError());

        BOOL critical = FALSE;
        if (IsProcessCritical(h, &critical) && critical)
        {
            CloseHandle(h);
            return E_ACCESSDENIED;
        }

        if (WaitForSingleObject(h, 0) == WAIT_OBJECT_0)
        {
            CloseHandle(h);
            return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
        }

        if (!TerminateProcess(h, 1))
        {
            HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
            CloseHandle(h);
            return hr;
        }

        DWORD w = WaitForSingleObject(h, 3000);
        CloseHandle(h);
        return (w == WAIT_OBJECT_0) ? S_OK : HRESULT_FROM_WIN32(ERROR_TIMEOUT);
    }
}