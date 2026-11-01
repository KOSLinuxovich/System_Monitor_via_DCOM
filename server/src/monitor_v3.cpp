#include "monitor.h"
#include "network.h"
#include "process.h"
#include "report.h"
#include "system_info.h"
#include "text_utils.h"

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetProcessList(SAFEARRAY** pProcesses)
{
    if (pProcesses == nullptr) return E_POINTER;
    *pProcesses = nullptr;

    try
    {
        std::vector<sm::ProcEntry> procs;
        if (!sm::CollectProcesses(procs, true)) return E_FAIL;
        return sm::MakeBstrArray(sm::FormatProcessLines(procs), pProcesses);
    }
    catch (...)
    {
        return E_OUTOFMEMORY;
    }
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetProcessInfo(long pid, BSTR* pInfo)
{
    if (pInfo == nullptr) return E_POINTER;
    *pInfo = nullptr;
    if (pid <= 0) return E_INVALIDARG;

    try
    {
        std::wstring text;
        HRESULT hr = sm::BuildProcessInfo((DWORD)pid, text);
        if (FAILED(hr)) return hr;
        return sm::ToBstr(text, pInfo);
    }
    catch (...)
    {
        return E_OUTOFMEMORY;
    }
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetNetworkStats(SAFEARRAY** pNetworks)
{
    if (pNetworks == nullptr) return E_POINTER;
    *pNetworks = nullptr;

    try
    {
        std::vector<std::wstring> lines;
        if (!sm::CollectNetwork(lines)) return E_FAIL;
        return sm::MakeBstrArray(lines, pNetworks);
    }
    catch (...)
    {
        return E_OUTOFMEMORY;
    }
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetSystemInfo(BSTR* pInfo)
{
    if (pInfo == nullptr) return E_POINTER;
    *pInfo = nullptr;

    try
    {
        return sm::ToBstr(sm::BuildSystemInfoText(), pInfo);
    }
    catch (...)
    {
        return E_OUTOFMEMORY;
    }
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::KillProcess(long pid)
{
    if (pid <= 0 || pid == 4) return E_INVALIDARG;
    return sm::TerminateProcessById((DWORD)pid);
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::ExportReport(BSTR filePath)
{
    if (filePath == nullptr || *filePath == L'\0') return E_INVALIDARG;

    try
    {
        std::wstring path(filePath, SysStringLen(filePath));
        return sm::WriteUtf8File(path, sm::BuildReportText());
    }
    catch (...)
    {
        return E_OUTOFMEMORY;
    }
}