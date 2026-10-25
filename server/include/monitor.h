#ifndef MONITOR_H
#define MONITOR_H

#include "interface.h"

class CSystemMonitor : public ISystemMonitorV3
{
private:
    LONG m_refCount;

    CSystemMonitor(const CSystemMonitor&) = delete;
    CSystemMonitor& operator=(const CSystemMonitor&) = delete;

public:
    CSystemMonitor();
    ~CSystemMonitor();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
    ULONG   STDMETHODCALLTYPE AddRef() override;
    ULONG   STDMETHODCALLTYPE Release() override;

    HRESULT STDMETHODCALLTYPE GetCpuUsage(double* pCpu) override;
    HRESULT STDMETHODCALLTYPE GetMemoryInfo(BSTR* pMemory) override;
    HRESULT STDMETHODCALLTYPE GetDiskFree(double* pDisk) override;
    HRESULT STDMETHODCALLTYPE GetUptime(BSTR* pUptime) override;

    HRESULT STDMETHODCALLTYPE SaveSnapshot() override;
    HRESULT STDMETHODCALLTYPE GetHistory(long count, BSTR* pHistory) override;

    HRESULT STDMETHODCALLTYPE GetProcessList(SAFEARRAY** pProcesses) override;
    HRESULT STDMETHODCALLTYPE GetProcessInfo(long pid, BSTR* pInfo) override;
    HRESULT STDMETHODCALLTYPE GetNetworkStats(SAFEARRAY** pNetworks) override;
    HRESULT STDMETHODCALLTYPE GetSystemInfo(BSTR* pInfo) override;
    HRESULT STDMETHODCALLTYPE KillProcess(long pid) override;
    HRESULT STDMETHODCALLTYPE ExportReport(BSTR filePath) override;
};

#endif