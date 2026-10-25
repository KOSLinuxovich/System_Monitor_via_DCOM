#ifndef FACTORY_H
#define FACTORY_H

#include "interface.h"

class CClassFactory : public IClassFactory
{
private:
    LONG m_refCount;

public:
    CClassFactory();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
    ULONG   STDMETHODCALLTYPE AddRef() override;
    ULONG   STDMETHODCALLTYPE Release() override;

    HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
    HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) override;
};

#endif