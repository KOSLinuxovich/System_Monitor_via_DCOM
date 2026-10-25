#include "monitor.h"
#include "database.h"
#include "system_stats.h"
#include "text_utils.h"

HRESULT STDMETHODCALLTYPE CSystemMonitor::SaveSnapshot()
{
    try
    {
        double cpu = 0.0;
        std::wstring mem;
        if (!sm::MeasureCpu(cpu) || !sm::MemoryText(mem)) return E_FAIL;
        return sm::InsertSnapshot(cpu, mem) ? S_OK : E_FAIL;
    }
    catch (...)
    {
        return E_OUTOFMEMORY;
    }
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetHistory(long count, BSTR* pHistory)
{
    if (pHistory == nullptr) return E_POINTER;
    *pHistory = nullptr;
    if (count <= 0) return E_INVALIDARG;
    if (count > sm::kMaxHistory) count = sm::kMaxHistory;

    try
    {
        std::string rows;
        if (!sm::QueryHistory((int)count, rows)) return E_FAIL;
        return sm::ToBstr(sm::Utf8ToWide(rows), pHistory);
    }
    catch (...)
    {
        return E_OUTOFMEMORY;
    }
}