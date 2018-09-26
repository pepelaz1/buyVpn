#include "StdAfx.h"
#include "Config.h"
#include "Options.h"

COptions::COptions(void)
{
	m_fSavePassword = TRUE;
	m_fCheckAccount = TRUE;
	m_fLaunchOnStart = FALSE;
	m_strConfigVer = "0.0";
	m_strNetAdapter = TEXT("None");
}

COptions::~COptions(void)
{
}

BOOL COptions::Load()
{
	BOOL fRes = FALSE;
	TCHAR szBuffer[1024];
	DWORD dwValue = 0;
	DWORD dwSize;
	HKEY hKey;

	
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\") VC_PROGRAM_NAME, 0, NULL, 0, KEY_READ, NULL, &hKey, NULL) == ERROR_SUCCESS)
	{
		dwSize = sizeof(szBuffer);
		ZeroMemory(szBuffer, dwSize);
		RegQueryValueEx(hKey, TEXT("Username"), 0, NULL, (BYTE*)szBuffer, &dwSize);
		m_strUsername = szBuffer;

		dwSize = sizeof(szBuffer);
		ZeroMemory(szBuffer, dwSize);
		RegQueryValueEx(hKey, TEXT("Password"), 0, NULL, (BYTE*)szBuffer, &dwSize);
		m_strPassword = DecodePassword(szBuffer);

		dwSize = sizeof(DWORD);
		dwValue = m_fSavePassword ? 1 : 0;
		RegQueryValueEx(hKey, TEXT("SavePass"), 0, NULL, (BYTE*)&dwValue, &dwSize);
		m_fSavePassword = dwValue;

		dwSize = sizeof(szBuffer);
		ZeroMemory(szBuffer, dwSize);
		RegQueryValueEx(hKey, TEXT("Configuration"), 0, NULL, (BYTE*)szBuffer, &dwSize);
		m_strConfiguration = szBuffer;

		dwSize = sizeof(szBuffer);
		ZeroMemory(szBuffer, dwSize);
		RegQueryValueEx(hKey, TEXT("Folder"), 0, NULL, (BYTE*)szBuffer, &dwSize);
		m_strFolder = szBuffer;

		dwSize = sizeof(szBuffer);
		ZeroMemory(szBuffer, dwSize);
		RegQueryValueEx(hKey, TEXT("NetAdapter"), 0, NULL, (BYTE*)szBuffer, &dwSize);
		m_strNetAdapter = szBuffer;

		dwSize = sizeof(DWORD);
		dwValue = m_fCheckAccount ? 1 : 0;
		RegQueryValueEx(hKey, TEXT("CheckAccount"), 0, NULL, (BYTE*)&dwValue, &dwSize);
		m_fCheckAccount = dwValue;

		dwSize = sizeof(DWORD);
		dwValue = m_fLaunchOnStart ? 1 : 0;
		RegQueryValueEx(hKey, TEXT("LaunchOnStart"), 0, NULL, (BYTE*)&dwValue, &dwSize);
		m_fLaunchOnStart = dwValue;

		dwSize = sizeof(szBuffer);
		ZeroMemory(szBuffer, dwSize);
		RegQueryValueEx(hKey, TEXT("ConfigVersion"), 0, NULL, (BYTE*)szBuffer, &dwSize);
		m_strConfigVer = szBuffer;

		dwSize = sizeof(szBuffer);
		ZeroMemory(szBuffer, dwSize);
		RegQueryValueEx(hKey, TEXT("Language"), 0, NULL, (BYTE*)szBuffer, &dwSize);
		m_strLanguage = szBuffer;

		fRes = TRUE;
		RegCloseKey(hKey);
	}

	return fRes;
}

BOOL COptions::Save(DWORD dwOptionsFlag)
{
	BOOL fRes = FALSE;
	TCHAR szBuffer[1024];
	HKEY hKey;

	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\") VC_PROGRAM_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
	{
		if (dwOptionsFlag & VCOF_USERNAME)
		{
			RegSetValueEx(hKey, TEXT("Username"), 0, REG_SZ, (const BYTE*)(const TCHAR*)m_strUsername, (m_strUsername.GetLength() + 1)*sizeof(TCHAR));
		}
		
		if (dwOptionsFlag & VCOF_PASSWORD)
		{
			ZeroMemory(szBuffer, sizeof(szBuffer));
			lstrcpy(szBuffer, m_strPassword);
			DecodePassword(szBuffer);
			RegSetValueEx(hKey, TEXT("Password"), 0, REG_SZ, (const BYTE*)szBuffer, (m_strPassword.GetLength() + 1)*sizeof(TCHAR));
		}
		
		if (dwOptionsFlag & VCOF_SAVEPASS)
		{
			RegSetValueEx(hKey, TEXT("SavePass"), 0, REG_DWORD, (const BYTE*)&m_fSavePassword, sizeof(DWORD));
		}

		if (dwOptionsFlag & VCOF_CONFIGURATION)
		{
			RegSetValueEx(hKey, TEXT("Configuration"), 0, REG_SZ, (const BYTE*)(const TCHAR*)m_strConfiguration, (m_strConfiguration.GetLength() + 1)*sizeof(TCHAR));
		}

		if (dwOptionsFlag & VCOF_FOLDER)
		{
			RegSetValueEx(hKey, TEXT("Folder"), 0, REG_SZ, (const BYTE*)(const TCHAR*)m_strFolder, (m_strFolder.GetLength() + 1) * sizeof(TCHAR));
		}

		if (dwOptionsFlag & VCOF_NETADAPTER)
		{
			RegSetValueEx(hKey, TEXT("NetAdapter"), 0, REG_SZ, (const BYTE*)(const TCHAR*)m_strNetAdapter, (m_strNetAdapter.GetLength() + 1)*sizeof(TCHAR));
		}

		if (dwOptionsFlag & VCOF_CHECKACCOUNT)
		{
			RegSetValueEx(hKey, TEXT("CheckAccount"), 0, REG_DWORD, (const BYTE*)&m_fCheckAccount, sizeof(DWORD));
		}

		if (dwOptionsFlag & VCOF_LAUNCHSTART)
		{
			RegSetValueEx(hKey, TEXT("LaunchOnStart"), 0, REG_DWORD, (const BYTE*)&m_fLaunchOnStart, sizeof(DWORD));
		}

		if (dwOptionsFlag & VCOF_CONFIGVERSION)
		{
			RegSetValueEx(hKey, TEXT("ConfigVersion"), 0, REG_SZ, (const BYTE*)(const TCHAR*)m_strConfigVer, (m_strConfigVer.GetLength() + 1)*sizeof(TCHAR));
		}

		if (dwOptionsFlag & VCOF_LANGUAGE)
		{
			RegSetValueEx(hKey, TEXT("Language"), 0, REG_SZ, (const BYTE*)(const TCHAR*)m_strLanguage, (m_strLanguage.GetLength() + 1) * sizeof(TCHAR));
		}


		RegCloseKey(hKey);

		fRes = TRUE;
	}

	return fRes;
}

TCHAR* COptions::DecodePassword(TCHAR* pass)
{
	TCHAR* p = pass;
	while (*p)
	{
		*p ^= 202;
		p++;
	}

	return pass;
}