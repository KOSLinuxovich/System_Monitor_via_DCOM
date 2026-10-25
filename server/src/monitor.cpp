#include "monitor.h"
#include "globals.h"
#include "system_stats.h"
#include "text_utils.h"

CSystemMonitor::CSystemMonitor() : m_refCount(1)
{
    InterlockedIncrement(&g_objectCount);
}

CSystemMonitor::~CSystemMonitor()
{
    InterlockedDecrement(&g_objectCount);
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::QueryInterface(REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr) return E_POINTER;
    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_ISystemMonitor)
        *ppvObject = static_cast<ISystemMonitor*>(this);
    else if (riid == IID_ISystemMonitorV2)
        *ppvObject = static_cast<ISystemMonitorV2*>(this);
    else if (riid == IID_ISystemMonitorV3)
        *ppvObject = static_cast<ISystemMonitorV3*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE CSystemMonitor::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG STDMETHODCALLTYPE CSystemMonitor::Release()
{
    ULONG count = InterlockedDecrement(&m_refCount);
    if (count == 0) delete this;
    return count;
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetCpuUsage(double* pCpu)
{
    if (pCpu == nullptr) return E_POINTER;
    *pCpu = 0.0;
    return sm::MeasureCpu(*pCpu) ? S_OK : E_FAIL;
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetMemoryInfo(BSTR* pMemory)
{
    if (pMemory == nullptr) return E_POINTER;
    *pMemory = nullptr;

    std::wstring text;
    if (!sm::MemoryText(text)) return E_FAIL;
    return sm::ToBstr(text, pMemory);
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetDiskFree(double* pDisk)
{
    if (pDisk == nullptr) return E_POINTER;
    return sm::DiskFreeGb(*pDisk);
}

HRESULT STDMETHODCALLTYPE CSystemMonitor::GetUptime(BSTR* pUptime)
{
    if (pUptime == nullptr) return E_POINTER;
    *pUptime = nullptr;
    return sm::ToBstr(sm::UptimeText(), pUptime);
}