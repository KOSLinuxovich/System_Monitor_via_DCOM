#ifndef INTERFACE_H
#define INTERFACE_H

#include <windows.h>
#include <unknwn.h>
#include <oleauto.h>
#include "guids.h"

class ISystemMonitor : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetCpuUsage(double* pCpu) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetMemoryInfo(BSTR* pMemory) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDiskFree(double* pDisk) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUptime(BSTR* pUptime) = 0;
};

class ISystemMonitorV2 : public ISystemMonitor
{
public:
    virtual HRESULT STDMETHODCALLTYPE SaveSnapshot() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHistory(long count, BSTR* pHistory) = 0;
};

class ISystemMonitorV3 : public ISystemMonitorV2
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetProcessList(SAFEARRAY** pProcesses) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProcessInfo(long pid, BSTR* pInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetNetworkStats(SAFEARRAY** pNetworks) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSystemInfo(BSTR* pInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE KillProcess(long pid) = 0;
    virtual HRESULT STDMETHODCALLTYPE ExportReport(BSTR filePath) = 0;
};

#endif