#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include <windows.h>
#include <oleauto.h>
#include <string>
#include <vector>

namespace sm
{
    std::string WideToUtf8(const std::wstring& w);
    std::wstring Utf8ToWide(const std::string& s);
    HRESULT ToBstr(const std::wstring& s, BSTR* out);
    HRESULT MakeBstrArray(const std::vector<std::wstring>& items, SAFEARRAY** out);
    std::wstring FormatBytes(ULONGLONG bytes);
    std::wstring ToCrlf(const std::wstring& s);
}

#endif