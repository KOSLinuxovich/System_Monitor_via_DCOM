#include "text_utils.h"
#include <cstdio>

namespace sm
{
    std::string WideToUtf8(const std::wstring& w)
    {
        if (w.empty()) return std::string();
        int n = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), nullptr, 0, nullptr, nullptr);
        if (n <= 0) return std::string();
        std::string s(n, '\0');
        WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), &s[0], n, nullptr, nullptr);
        return s;
    }

    std::wstring Utf8ToWide(const std::string& s)
    {
        if (s.empty()) return std::wstring();
        int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
        if (n <= 0) return std::wstring();
        std::wstring w(n, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &w[0], n);
        return w;
    }

    HRESULT ToBstr(const std::wstring& s, BSTR* out)
    {
        *out = SysAllocStringLen(s.c_str(), (UINT)s.size());
        return *out ? S_OK : E_OUTOFMEMORY;
    }

    HRESULT MakeBstrArray(const std::vector<std::wstring>& items, SAFEARRAY** out)
    {
        SAFEARRAY* psa = SafeArrayCreateVector(VT_BSTR, 0, (ULONG)items.size());
        if (!psa) return E_OUTOFMEMORY;

        for (LONG i = 0; i < (LONG)items.size(); ++i)
        {
            BSTR b = SysAllocStringLen(items[i].c_str(), (UINT)items[i].size());
            if (!b)
            {
                SafeArrayDestroy(psa);
                return E_OUTOFMEMORY;
            }
            HRESULT hr = SafeArrayPutElement(psa, &i, b);
            SysFreeString(b);
            if (FAILED(hr))
            {
                SafeArrayDestroy(psa);
                return hr;
            }
        }
        *out = psa;
        return S_OK;
    }

    std::wstring FormatBytes(ULONGLONG bytes)
    {
        static const wchar_t* units[] = { L"B", L"KB", L"MB", L"GB", L"TB" };
        double v = (double)bytes;
        int i = 0;
        while (v >= 1024.0 && i < 4)
        {
            v /= 1024.0;
            ++i;
        }
        wchar_t buf[64];
        if (i == 0) swprintf_s(buf, L"%llu B", bytes);
        else        swprintf_s(buf, L"%.2f %ls", v, units[i]);
        return buf;
    }

    std::wstring ToCrlf(const std::wstring& s)
    {
        std::wstring o;
        o.reserve(s.size() + 16);
        for (wchar_t c : s)
        {
            if (c == L'\n') o += L"\r\n";
            else            o += c;
        }
        return o;
    }
}