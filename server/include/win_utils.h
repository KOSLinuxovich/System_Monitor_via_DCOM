#ifndef WIN_UTILS_H
#define WIN_UTILS_H

#include <windows.h>
#include <string>

namespace sm
{
    ULONGLONG ToU64(const FILETIME& ft);
    std::wstring FileTimeToLocalText(const FILETIME& ft);
    std::wstring RegString(HKEY root, const wchar_t* subKey, const wchar_t* value);
    std::wstring ModuleDirectory();
}

#endif