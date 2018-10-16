#include "stdafx.h"
#include "Common.h"
#include "Config.h"
#include "RunThread.h"
#include "BuyVPNDlg.h"
#include "HttpWorker.h"

CRunThread::CRunThread(COptions* pOptions, CHostsConfig* pHostConfig, CConfigurations* pConfigurations, CNetAdapters* pNetAdapters, CBuyVPNDlg* pMainDlg)
{
	m_pOptions = pOptions;
	m_pHostConfig = pHostConfig;
	m_pConfigurations = pConfigurations;
	m_pNetAdapters = pNetAdapters;
	m_pMainDlg = pMainDlg;

	m_hOvpnProcess = NULL;
	m_hExitEvent = NULL;
	m_hThread = NULL;
	m_strExitEventName.Format(TEXT("%s_%x%08x"), VC_PROGRAM_NAME, GetCurrentProcessId(), GetCurrentThreadId());
	m_fStopped = FALSE;
	m_fConnected = FALSE;
}

CRunThread::~CRunThread(void)
{
	Stop();
}

VOID CRunThread::Run()
{
	m_fStopped = FALSE;
	m_fConnected = FALSE;

	if (m_hThread != NULL && m_hThread != INVALID_HANDLE_VALUE)
	{
		Stop();
	}

	m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
}

VOID CRunThread::Stop()
{
	m_fStopped = TRUE;

	SetEvent(m_hExitEvent);

	if (WaitForSingleObject(m_hThread, 5000) != WAIT_OBJECT_0)
	{
		TerminateThread(m_hThread, -1);
		TerminateProcess(m_hOvpnProcess, -1);
		Close();
	}
}

VOID CRunThread::Work()
{
	if (m_pOptions->m_strConfiguration.IsEmpty())
	{
		MessageBox(NULL, TEXT("Invalid configuration"), TEXT("Error"), MB_OK);
		Close();
		return;
	}

	DeleteFile(GetLogFilename());

	if (m_pOptions->m_fCheckAccount && !DoAuth())
	{
		Close();
		return;
	}

	if (m_hExitEvent == NULL || m_hExitEvent == INVALID_HANDLE_VALUE)
	{
		m_hExitEvent = CreateEvent(NULL, TRUE, FALSE, m_strExitEventName);
	}

	CString strConfigFilename = m_pConfigurations->GetPathByName(m_pOptions->m_strConfiguration);
	CString strConfigDir = GetDir(strConfigFilename).TrimRight('\\');
	//CString strCaFilename = strConfigDir + TEXT("\\ca.crt");
	//CString strCertFilename = strConfigDir + TEXT("\\client.crt");
	//CString strKeyFilename = strConfigDir + TEXT("\\client.key");
	//CString strTaFilename = strConfigDir + TEXT("\\ta.key");
	CString strCommandLine;
	if (!m_pOptions->m_strUsername.IsEmpty() && !m_pOptions->m_strPassword.IsEmpty())
	{
		CreateAuthFile();
		strCommandLine.Format(TEXT("openvpn.exe --config \"%s\" --service %s 0 --auth-user-pass \"%s\" --cd \"%s\""),
			strConfigFilename, m_strExitEventName, m_strAuthFilename, strConfigDir);
	}
	else
	{
		strCommandLine.Format(TEXT("openvpn.exe --config \"%s\" --service %s 0 --cd \"%s\""),
			strConfigFilename, m_strExitEventName, strConfigDir);
	}

	TCHAR szCommandLine[2048];
	lstrcpy(szCommandLine, strCommandLine);

	CString strExeFilename = GetRunningDir() + TEXT("openvpn.exe");

		
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	HANDLE hStdInRead = NULL;
	HANDLE hStdInWrite = NULL;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL fProcessStarted = FALSE;

	ZeroMemory(&pi, sizeof(pi));

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = TRUE;

	do 
	{
		if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
		{
			break;
		}
		if (!SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE))
		{
			break;
		}

		if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0))
		{
			break;
		}
		if (!SetHandleInformation(hStdInRead, HANDLE_FLAG_INHERIT, 0))
		{
			break;
		}
		
		GetStartupInfo(&si);
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdInput = NULL;
		si.hStdOutput = hStdInWrite;
		si.hStdError = hStdInWrite;
		if (!CreateProcess(strExeFilename, szCommandLine, NULL, NULL, TRUE,
							CREATE_NO_WINDOW, NULL, strConfigDir, &si, &pi))
		{
			break;
		}

		m_hOvpnProcess = pi.hProcess;
		fProcessStarted = TRUE;
		m_pMainDlg->PostMessage(WM_CHANGESTATE, NULL, VVC_STATE_CONNECTING);

		char szLine[1024];
		DWORD dwSize = sizeof(szLine);
		DWORD dwRead = 0;
		DWORD dwLen = 0;

		while(TRUE)
		{
			dwSize = sizeof(szLine);
			dwLen = 0;

			if (PeekNamedPipe(hStdInRead, szLine, dwSize - 2, &dwRead, NULL, NULL))
			{
				char* p = (char*)memchr(szLine, '\r', dwRead);
				if (p)
				{
					dwLen = (DWORD)(p - szLine) + 2;
				}
				else if (dwSize - 2 == dwRead)
				{
					dwLen = dwRead;
				}

				if (dwLen > 0)
				{
					if (ReadFile(hStdInRead, szLine, dwLen, &dwRead, NULL))
					{
						szLine[dwRead - 2] = '\n';
						szLine[dwRead - 1] = '\0';
						if (!AnalyzeLogLine(szLine))
						{
							TerminateProcess(m_hOvpnProcess, -1);
						}
					}
				}
			}

			if (WaitForSingleObject(m_hOvpnProcess, 100) != WAIT_TIMEOUT)
			{
				if (!m_fStopped && m_fConnected
					&& !m_pOptions->m_strNetAdapter.IsEmpty() && m_pOptions->m_strNetAdapter.CompareNoCase(TEXT("None")) != 0)
				{
					
					m_pNetAdapters->StopAdapter(m_pOptions->m_strNetAdapter);
					::MessageBox(m_pMainDlg->GetSafeHwnd(), m_pMainDlg->getLangManager()->GetText(L"DisableNetwork"), VC_PROGRAM_NAME, MB_OK);
					//::MessageBox(m_pMainDlg->GetSafeHwnd(), VC_DISABLE_NETWORK_TEXT, VC_PROGRAM_NAME, MB_OK);
				}
				
				break;
			}
		}


	} while (FALSE);

	if (hStdInWrite && hStdInWrite != INVALID_HANDLE_VALUE)
		CloseHandle(hStdInWrite);
	if (hStdInRead && hStdInRead != INVALID_HANDLE_VALUE)
		CloseHandle(hStdInRead);
	if (pi.hThread && pi.hThread != INVALID_HANDLE_VALUE)
		CloseHandle(pi.hThread);
	if (pi.hProcess && pi.hProcess != INVALID_HANDLE_VALUE)
	{
		CloseHandle(pi.hProcess);
		m_hOvpnProcess = NULL;
	}

	if (!fProcessStarted)
	{
		MessageBox(NULL, TEXT("Could not start OpenVPN"), TEXT("Error"), MB_OK);
	}
}

DWORD WINAPI CRunThread::ThreadProc(LPVOID lpParameter)
{
	CRunThread* pRunThread = (CRunThread*)lpParameter;
	pRunThread->Work();
	pRunThread->Close();

	return 0;
}

VOID CRunThread::CreateAuthFile()
{
	TCHAR szTmp[MAX_PATH];

	GetTempPath(MAX_PATH, szTmp);
	m_strAuthFilename = szTmp;
	GetTempFileName(m_strAuthFilename, TEXT("vvc"), 0, szTmp);
	m_strAuthFilename = szTmp;

	HANDLE hFile = CreateFile(m_strAuthFilename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CStringA strUsername = (CStringA)m_pOptions->m_strUsername;
		CStringA strPassword = (CStringA)m_pOptions->m_strPassword;
		DWORD dwWritten = 0;

		WriteFile(hFile, strUsername, strUsername.GetLength(), &dwWritten, NULL);
		WriteFile(hFile, "\n", 1, &dwWritten, NULL);
		WriteFile(hFile, strPassword, strPassword.GetLength(), &dwWritten, NULL);

		CloseHandle(hFile);
	}
}

VOID CRunThread::Close()
{
	CloseHandle(m_hExitEvent);
	if (m_hThread && m_hThread != INVALID_HANDLE_VALUE)
		CloseHandle(m_hThread);
	if (m_hOvpnProcess && m_hOvpnProcess != INVALID_HANDLE_VALUE)
		CloseHandle(m_hOvpnProcess);
	m_hExitEvent = NULL;
	m_hThread = NULL;
	m_hOvpnProcess = NULL;

	DeleteFile(m_strAuthFilename);

	m_pMainDlg->PostMessage(WM_CHANGESTATE, NULL, VVC_STATE_DISCONNECTED);
}

BOOL CRunThread::AnalyzeLogLine(LPCSTR szLogLine)
{
	CStringA strLogLine(szLogLine);

	FILE* pFile = _tfopen(GetLogFilename(), TEXT("at"));
	if (pFile)
	{
		fwrite((const void*)(const char*)strLogLine, 1, strLogLine.GetLength(), pFile);
		fclose(pFile);
	}

	strLogLine.MakeUpper();
	//if (strLogLine.Find("Initialization Sequence Completed") != -1)
	if (strLogLine.Find("INITIALIZATION SEQUENCE COMPLETED") != -1)
	{
		WCHAR buf[100];
		SYSTEMTIME st;
		GetLocalTime(&st); // Local time
		wsprintfW(buf, L"Connected at %.2u:%.2u:%.2u ", st.wHour, st.wMinute, st.wSecond); // 24h format

		OutputDebugString(buf);
		//m_pMainDlg->MessageBox(buf, L"Test", MB_OK);
		m_pMainDlg->PostMessage(WM_CHANGESTATE, NULL, VVC_STATE_CONNECTED);
		m_fConnected = TRUE;
	}
	else if ((strLogLine.Find("RESTART") != -1 && strLogLine.Find("PING-RESTART") == -1) 
			|| (strLogLine.Find("RESTARTING") != -1))
	{
		m_pMainDlg->PostMessage(WM_DISABLENONE, NULL, NULL);
		return FALSE;
	}

	return TRUE;
}

BOOL CRunThread::DoAuth()
{
	CStringA strPostData;
	strPostData.Format("user=%s&passwd=%s&logined=%%E2%%EE%%E9%%F2%%E8", (CStringA)m_pOptions->m_strUsername, (CStringA)m_pOptions->m_strPassword);
	CStringA strAnswer;

	while (TRUE)
	{
		CString strUrl;
		strUrl.Format(TEXT("https://%s%s"), m_pHostConfig->GetCurrentHost(), VC_AUTH_ADDRESS);

		if (CHttpWorker::SendPost(strUrl, strUrl, strPostData, strAnswer)
			&& strAnswer.Find("<title>User Space</title>") != -1)
		{
			break;
		}

		if (!m_pHostConfig->SetNextHost())
		{
			MessageBox(NULL, TEXT("Could not connect to server"), TEXT("Error"), MB_OK);
			return FALSE;
		}
	}

	if (strAnswer.Find("<div class='text_msg'>Неправильный пароль</div>") != -1)
	{
		MessageBox(NULL, TEXT("Invalid username/password"), TEXT("Error"), MB_OK);
		return FALSE;
	}

	if (m_pOptions->m_fCheckAccount)
	{
		CString strBalance;
		CString strExpires;
		CStringA strFind(VC_BALANCE_BEGIN);
		int iPos = strAnswer.Find(strFind);
		int iPos2 = -1;
		if (iPos != -1)
		{
			iPos += strFind.GetLength();
			iPos2 = strAnswer.Find(VC_BALANCE_END, iPos);
			if (iPos2 != -1)
			{
				strBalance = (CString)strAnswer.Mid(iPos, iPos2-iPos);
			}
		}

		strFind = VC_EXPIRES_BEGIN;
		iPos = strAnswer.Find(strFind);
		if (iPos != -1)
		{
			iPos += strFind.GetLength();
			iPos2 = strAnswer.Find(VC_EXPIRES_END, iPos);
			if (iPos2 != -1)
			{
				strExpires = (CString)strAnswer.Mid(iPos, iPos2-iPos);
			}
		}

		m_pMainDlg->SetAccountInfo(strBalance, strExpires);
		m_pMainDlg->PostMessage(WM_UPDATEACCINFO, NULL, NULL);
	}

	return TRUE;
}
