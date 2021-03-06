#pragma once
#include "dbgeng.h"
#include <boost/unordered_map.hpp>

namespace md
{

class ControlCentre;
struct VB_INFO;

class FPDebugEvent :
	public IDebugEventCallbacks
{
public:
	FPDebugEvent(void);
	~FPDebugEvent(void);

	bool initialize();
	bool finalize();

public:
	// IUnknown.
    STDMETHOD(QueryInterface)(
        THIS_
        __in REFIID InterfaceId,
        __out PVOID* Interface
        );
    STDMETHOD_(ULONG, AddRef)(
        THIS
        );
    STDMETHOD_(ULONG, Release)(
        THIS
        );

public:
	STDMETHOD(GetInterestMask)(
        THIS_
        __out PULONG Mask
        );

	STDMETHOD(Breakpoint)(
        THIS_
        __in PDEBUG_BREAKPOINT Bp
        );
	
	STDMETHOD(Exception)(
        THIS_
        __in PEXCEPTION_RECORD64 Exception,
        __in ULONG FirstChance
        );

	STDMETHOD(CreateThread)(
        THIS_
        __in ULONG64 Handle,
        __in ULONG64 DataOffset,
        __in ULONG64 StartOffset
        );

	STDMETHOD(ExitThread)(
        THIS_
        __in ULONG ExitCode
        );

	STDMETHOD(CreateProcess)(
        THIS_
        __in ULONG64 ImageFileHandle,
        __in ULONG64 Handle,
        __in ULONG64 BaseOffset,
        __in ULONG ModuleSize,
        __in_opt PCSTR ModuleName,
        __in_opt PCSTR ImageName,
        __in ULONG CheckSum,
        __in ULONG TimeDateStamp,
        __in ULONG64 InitialThreadHandle,
        __in ULONG64 ThreadDataOffset,
        __in ULONG64 StartOffset
        );

	STDMETHOD(ExitProcess)(
        THIS_
        __in ULONG ExitCode
        );

	STDMETHOD(LoadModule)(
        THIS_
        __in ULONG64 ImageFileHandle,
        __in ULONG64 BaseOffset,
        __in ULONG ModuleSize,
        __in_opt PCSTR ModuleName,
        __in_opt PCSTR ImageName,
        __in ULONG CheckSum,
        __in ULONG TimeDateStamp
        );

	STDMETHOD(UnloadModule)(
        THIS_
        __in_opt PCSTR ImageBaseName,
        __in ULONG64 BaseOffset
        );

	STDMETHOD(SystemError)(
        THIS_
        __in ULONG Error,
        __in ULONG Level
        );

	STDMETHOD(SessionStatus)(
        THIS_
        __in ULONG Status
        );

	STDMETHOD(ChangeDebuggeeState)(
        THIS_
        __in ULONG Flags,
        __in ULONG64 Argument
        );
    
    STDMETHOD(ChangeEngineState)(
        THIS_
        __in ULONG Flags,
        __in ULONG64 Argument
        );
    
    STDMETHOD(ChangeSymbolState)(
        THIS_
        __in ULONG Flags,
        __in ULONG64 Argument
        );
		

public:
	void addHardcodeBreakpoints();
	void handleSetJIT();
    void handleAnalyzeEmbedded();

	void dumpSprayResult();

private:
	void callbackAcroMalloc();
	void callbackAcroMcp();
	void callbackRtlFreeHeap();

private:
	unsigned int m_refCount;

	unsigned int m_uiParseHit;
	bool m_bResetHardcodeBP;

    // Export Embedded
    ULONG64 m_addrLoadBytesEntry;
    bool m_findLoadBytesEntry;

	ControlCentre* m_cc;
	IDebugAdvanced* m_dbgAdv;
	IDebugControl* m_dbgCtrl;
	IDebugDataSpaces* m_dbgData;
    IDebugClient* m_dbgClient;

	// AcroRd Heap Spray
	DWORD m_preAllocVBSize; // previous allocated size of virtual block
	DWORD m_preAllocVBAddr; // previous allocated address of virtual block
	DWORD m_preMcpSrc; // previous source address in memcpy
	DWORD m_preMcpDest; // previous destination address in memcpy
	DWORD m_preMcpSize; // previous size in memcpy
	boost::unordered::unordered_map<DWORD, DWORD> m_histAllocVBSize; // histogram for allocated virtual block size
	boost::unordered::unordered_map<DWORD, DWORD> m_histMcpSrc; // histogram for memcpy from source
	boost::unordered::unordered_map<DWORD, VB_INFO> m_mapVB;
	ULONG64 m_offsetRtlFreeHeap;
};

}