#include "StdAfx.h"
#include "Config.h"
#include "HttpWorker.h"
#include "HostsConfig.h"

CHostsConfig::CHostsConfig(void)
{
	Load();
	m_HostsPos = m_aHosts.GetHeadPosition();
}

CHostsConfig::~CHostsConfig(void)
{
}

CString CHostsConfig::GetCurrentHost()
{
	CString str;
	if (!m_HostsPos)
	{
		m_HostsPos = m_aHosts.GetHeadPosition();
	}
	if (m_HostsPos)
	{
		str = m_aHosts.GetAt(m_HostsPos);
	}

	if (str.IsEmpty())
	{
		str = (CString)VC_DEFAULT_HOST;
	}

	str.TrimRight('/');

	return str;
}

BOOL CHostsConfig::SetNextHost()
{
	if (!m_HostsPos)
	{
		m_HostsPos = m_aHosts.GetHeadPosition();
		return FALSE;
	}
	m_aHosts.GetNext(m_HostsPos);
	if (m_HostsPos)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CHostsConfig::Load()
{
	TCHAR szBuffer[1024];
	DWORD dwSize = sizeof(szBuffer);
	HKEY hKey;

	ZeroMemory(szBuffer, dwSize);
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\") VC_PROGRAM_NAME, 0, NULL, 0, KEY_READ, NULL, &hKey, NULL) == ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey, TEXT("Hosts"), 0, NULL, (BYTE*)szBuffer, &dwSize);
		m_strCryptedHosts = szBuffer;

		DecodeHosts();

		RegCloseKey(hKey);
	}
}

void CHostsConfig::Save()
{
	if (m_strCryptedHosts.IsEmpty())
	{
		return;
	}

	HKEY hKey;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\") VC_PROGRAM_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
	{
		RegSetValueEx(hKey, TEXT("Hosts"), 0, REG_SZ, (const BYTE*)(const TCHAR*)m_strCryptedHosts, (m_strCryptedHosts.GetLength() + 1)*sizeof(TCHAR));
		RegCloseKey(hKey);
	}
}

CString Base64dec(CStringA strInput)
{
	const unsigned char* base64 = (const unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	
	int n = strInput.GetLength();
	unsigned char* src = new unsigned char[n];
	for (int i = 0; i < n; i++)
	{
		src[i] = (unsigned char)strInput[i];
	}

	while (n > 0 && src[n-1] == '=') 
	{
		src[n-1] = 0;
		n--;
	}

	//map base64 ASCII character to 6 bit value
	for(int i = 0; i < n; i++)   
	{  
		unsigned char* p = (unsigned char*)strchr((const char*)base64, (int)src[i]);
		if(!p)
			break;
		src[i] = (unsigned char)(p - (unsigned char*)base64);
	}

	int size = n * 3 / 4 + 4;
	unsigned char* dst = new unsigned char[size];
	ZeroMemory(dst, size);
	for(int i = 0, j = 0; i < n; i += 4,j += 3) 
	{
		dst[j] = (src[i]<<2) + ((src[i+1]&0x30)>>4);
		dst[j+1] = ((src[i+1]&0x0F)<<4) + ((src[i+2]&0x3C)>>2);
		dst[j+2] = ((src[i+2]&0x03)<<6) + src[i+3];
	}
	
	CString strOutput = (CString)(CStringA)(const char*)dst;
	
	delete[] src;
	delete[] dst;
	
	return strOutput;
}

void CHostsConfig::DecodeHosts()
{
	if (m_strCryptedHosts.IsEmpty())
	{
		return;
	}

	CString strEncodedHosts = Base64dec((CStringA)m_strCryptedHosts);

	if (strEncodedHosts.IsEmpty())
	{
		return;
	}

	m_aHosts.RemoveAll();
	CString strHost;
	for (int i = 0; AfxExtractSubString(strHost, strEncodedHosts, i, TEXT(';')); ++i)
	{
		m_aHosts.AddTail(strHost);
	}

	m_HostsPos = m_aHosts.GetHeadPosition();
}

void CHostsConfig::Update(CString& strConfigVersion, CString& strConfigUrl)
{
	CStringA strScriptAnswer;
	CStringA strNewCryptedHosts;
	CStringA strProgVersion;
	CStringA strProgUrl;
	CStringA strConfigVersionA;
	CStringA strConfigUrlA;
	CString strUrl;
	strUrl.Format(TEXT("http://%s%s"), GetCurrentHost(), VC_SCRIPT_ADDRESS);
	if (CHttpWorker::LoadPage(strUrl, NULL, strScriptAnswer))
	{
		if (strScriptAnswer.IsEmpty())
		{
			return;
		}

		int iPos = strScriptAnswer.Find('\n');
		if (iPos == -1 || iPos == 0)
		{
			return;
		}
		strNewCryptedHosts = strScriptAnswer.Left(iPos);
		++iPos;
		
		int iPos2 = strScriptAnswer.Find('\n', iPos);
		if (iPos2 == -1)
		{
			return;
		}
		strProgVersion = strScriptAnswer.Mid(iPos, iPos2-iPos);

		iPos = iPos2 + 1;
		iPos2 = strScriptAnswer.Find('\n', iPos);
		if (iPos2 == -1)
		{
			return;
		}
		strProgUrl = strScriptAnswer.Mid(iPos, iPos2-iPos);

		iPos = iPos2 + 1;
		iPos2 = strScriptAnswer.Find('\n', iPos);
		if (iPos2 == -1)
		{
			return;
		}
		strConfigVersionA = strScriptAnswer.Mid(iPos, iPos2-iPos);

		iPos = iPos2 + 1;
		iPos2 = strScriptAnswer.Find('\n', iPos);
		if (iPos2 == -1)
		{
			return;
		}
		strConfigUrlA = strScriptAnswer.Mid(iPos, iPos2-iPos);

		UpdateProg((CString)strProgVersion, Base64dec(strProgUrl));

		strConfigVersion = (CString)strConfigVersionA;
		strConfigUrl = Base64dec(strConfigUrlA);

		if (!strNewCryptedHosts.IsEmpty() 
			&& (CString)strNewCryptedHosts != m_strCryptedHosts)
		{
			m_strCryptedHosts = (CString)strNewCryptedHosts;
			Save();
			DecodeHosts();
		}
	}
	else
	{
		if (SetNextHost())
		{
			Update(strConfigVersion, strConfigUrl);
		}
	}
}

void CHostsConfig::UpdateProg(CString strVersion, CString strUrl)
{
	if (strVersion.IsEmpty() || strUrl.IsEmpty())
	{
		return;
	}

	if (strVersion.CompareNoCase(VC_CURRENT_VERSION) != 0)
	{
		if (strUrl.Find(TEXT("http")) == 0)
		{
			if (::MessageBox(NULL, TEXT("New version is available. Download and install?"), VC_PROGRAM_NAME TEXT(" update"), MB_YESNO) == IDYES)
			{
				TCHAR szTempFilename[MAX_PATH];
				GetTempPath(MAX_PATH, szTempFilename);
				lstrcat(szTempFilename, VC_PROGRAM_NAME TEXT("_Intall.exe"));
				if (CHttpWorker::LoadFile(strUrl, szTempFilename))
				{
					ShellExecute(NULL, NULL, szTempFilename, NULL, NULL, SW_SHOWDEFAULT);
					ExitProcess(0);
				}
			}
		}
	}
}
