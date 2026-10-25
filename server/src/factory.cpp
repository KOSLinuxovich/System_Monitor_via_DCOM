#include "factory.h"
#include "monitor.h"
#include "globals.h"
#include <new>

CClassFactory::CClassFactory() : m_refCount(1) {}

HRESULT STDMETHODCALLTYPE CClassFactory::QueryInterface(REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr) return E_POINTER;
    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IClassFactory)
    {
        *ppvObject = static_cast<IClassFactory*>(this);
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CClassFactory::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG STDMETHODCALLTYPE CClassFactory::Release()
{
    ULONG count = InterlockedDecrement(&m_refCount);
    if (count == 0) delete this;
    return count;
}

HRESULT STDMETHODCALLTYPE CClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr) return E_POINTER;
    *ppvObject = nullptr;
    if (pUnkOuter != nullptr) return CLASS_E_NOAGGREGATION;

    CSystemMonitor* monitor = new (std::nothrow) CSystemMonitor();
    if (monitor == nullptr) return E_OUTOFMEMORY;

    HRESULT hr = monitor->QueryInterface(riid, ppvObject);
    monitor->Release();
    return hr;
}

HRESULT STDMETHODCALLTYPE CClassFactory::LockServer(BOOL fLock)
{
    if (fLock) InterlockedIncrement(&g_lockCount);
    else       InterlockedDecrement(&g_lockCount);
    return S_OK;
}