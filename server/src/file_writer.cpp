#include "report.h"
#include "text_utils.h"

namespace sm
{
    HRESULT WriteUtf8File(const std::wstring& path, const std::wstring& text)
    {
        HANDLE f = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (f == INVALID_HANDLE_VALUE) return HRESULT_FROM_WIN32(GetLastError());

        std::string data = "\xEF\xBB\xBF" + WideToUtf8(text);
        DWORD written = 0;
        BOOL ok = WriteFile(f, data.data(), (DWORD)data.size(), &written, nullptr);
        HRESULT hr = ok ? S_OK : HRESULT_FROM_WIN32(GetLastError());
        CloseHandle(f);
        return hr;
    }
}