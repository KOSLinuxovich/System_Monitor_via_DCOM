#ifndef REPORT_H
#define REPORT_H

#include <windows.h>
#include <string>

namespace sm
{
    std::wstring BuildReportText();
    HRESULT WriteUtf8File(const std::wstring& path, const std::wstring& text);
}

#endif