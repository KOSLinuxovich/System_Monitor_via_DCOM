#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

static const CLSID CLSID_SystemMonitor = 
{ 
    0xBDB79690, 0x6A8F, 0x4F29, { 0xA5, 0xEB, 0xC3, 0x5B, 0xEE, 0xE2, 0x87, 0xB4 } 
};

static const IID IID_ISystemMonitor = 
{ 
    0x78A5FD98, 0x4D87, 0x48F0, { 0x8D, 0x31, 0x42, 0x3C, 0x5D, 0xFF, 0x95, 0xCB } 
};

typedef struct ISystemMonitorVtbl 
{
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(void* thisPtr, REFIID riid, void** ppvObject);
    ULONG (STDMETHODCALLTYPE *AddRef)(void* thisPtr);
    ULONG (STDMETHODCALLTYPE *Release)(void* thisPtr);
    HRESULT (STDMETHODCALLTYPE *GetCpuUsage)(void* thisPtr, double* pCpu);
    HRESULT (STDMETHODCALLTYPE *GetMemoryInfo)(void* thisPtr, BSTR* pMemory);
    HRESULT (STDMETHODCALLTYPE *GetDiskFree)(void* thisPtr, double* pDisk);
    HRESULT (STDMETHODCALLTYPE *GetUptime)(void* thisPtr, BSTR* pUptime);
} ISystemMonitorVtbl;

typedef struct ISystemMonitor 
{
    ISystemMonitorVtbl* lpVtbl;
} ISystemMonitor;

int main() 
{
    HRESULT hr;
    ISystemMonitor* pMonitor = NULL;

    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) 
    {
        printf("Error CoInitializeEx: 0x%08X\n", hr);
        return 1;
    }

    hr = CoCreateInstance(&CLSID_SystemMonitor, NULL, CLSCTX_INPROC_SERVER, &IID_ISystemMonitor, (void**)&pMonitor);
    if (FAILED(hr)) 
    {
        printf("Error CoCreateInstance: 0x%08X\n", hr);
        CoUninitialize();
        return 1;
    }

    double cpu = 0.0;
    double disk = 0.0;
    BSTR mem = NULL;
    BSTR uptime = NULL;

    if (SUCCEEDED(pMonitor->lpVtbl->GetCpuUsage(pMonitor, &cpu))) 
    {
        printf("CPU Usage: %.2f %%\n", cpu);
    }

    if (SUCCEEDED(pMonitor->lpVtbl->GetMemoryInfo(pMonitor, &mem))) 
    {
        wprintf(L"Memory: %s\n", mem);
        SysFreeString(mem);
    }

    if (SUCCEEDED(pMonitor->lpVtbl->GetDiskFree(pMonitor, &disk))) 
    {
        printf("Disk C: Free %.2f GB\n", disk);
    }

    if (SUCCEEDED(pMonitor->lpVtbl->GetUptime(pMonitor, &uptime))) 
    {
        wprintf(L"Uptime: %s\n", uptime);
        SysFreeString(uptime);
    }

    pMonitor->lpVtbl->Release(pMonitor);
    CoUninitialize();

    return 0;
}