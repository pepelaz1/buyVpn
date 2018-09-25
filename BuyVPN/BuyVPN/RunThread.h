#pragma once

#include "Options.h"
#include "Configurations.h"
#include "NetAdapters.h"
#include "HostsConfig.h"

class CBuyVPNDlg;

class CRunThread
{
public:
	CRunThread(COptions* pOptions, CHostsConfig* pHostConfig,  CConfigurations* pConfigurations, CNetAdapters* pNetAdapters, CBuyVPNDlg* pMainDlg);
	~CRunThread(void);

	VOID Run();
	VOID Stop();

private:
	VOID Work();
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	VOID CreateAuthFile();
	VOID Close();
	BOOL AnalyzeLogLine(LPCSTR szLogLine);
	BOOL DoAuth();

	HANDLE m_hOvpnProcess;
	CString m_strExitEventName;
	HANDLE m_hExitEvent;
	HANDLE m_hThread;
	COptions* m_pOptions;
	CString m_strAuthFilename;
	CConfigurations* m_pConfigurations;
	CNetAdapters* m_pNetAdapters;
	CBuyVPNDlg* m_pMainDlg;
	CHostsConfig* m_pHostConfig;
	BOOL m_fStopped;
	BOOL m_fConnected;
};
