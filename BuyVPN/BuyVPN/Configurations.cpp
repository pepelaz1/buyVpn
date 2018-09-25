#include "StdAfx.h"
#include "Common.h"
#include "Configurations.h"

CConfigurations::CConfigurations(void)
{
}

CConfigurations::~CConfigurations(void)
{
}

void CConfigurations::Update()
{
	CString strDir = GetRunningDir();
	strDir.Append(TEXT("config"));

	m_Configurations.RemoveAll();
	m_Names.RemoveAll();

	SearchConfigFiles(strDir);
}

CString CConfigurations::GetPathByName(LPCTSTR lpszConfigName)
{
	CString strValue;
	if (!m_Configurations.Lookup(GetHash(lpszConfigName), strValue))
	{
		strValue.Empty();
	}

	return strValue;
}

void CConfigurations::SearchConfigFiles(LPCTSTR lpszDir)
{
	HANDLE hFindFile = NULL;
	WIN32_FIND_DATA ffd;

	CString strSearchFilename = lpszDir;
	strSearchFilename.Append(TEXT("\\*"));


	hFindFile = FindFirstFile(strSearchFilename, &ffd);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (lstrcmp(ffd.cFileName, TEXT("."))
					&& lstrcmp(ffd.cFileName, TEXT("..")))
				{
					CString strSubDir = lpszDir;
					strSubDir.AppendChar('\\');
					strSubDir.Append(ffd.cFileName);
					SearchConfigFiles(strSubDir);
				}
			}
			else
			{
				TCHAR szName[MAX_PATH];
				TCHAR szExt[MAX_PATH];

				_tsplitpath(ffd.cFileName, NULL, NULL, szName, szExt);
				_tcslwr(szExt);
				if (!lstrcmp(szExt, TEXT(".ovpn")))
				{
					CString strPath;
					strPath.Format(TEXT("%s\\%s"), lpszDir, ffd.cFileName);
					CString strName(szName);
					CString strValue;
					int n = 1;

					while (m_Configurations.Lookup(GetHash(strName), strValue))
					{
						strName.Format(TEXT("%s(%d)"), szName, n++);
					}

					m_Configurations.SetAt(GetHash(strName), strPath);
					m_Names.AddTail(strName);
				}
			}
		} while (FindNextFile(hFindFile, &ffd));
	}


	if (hFindFile && hFindFile != INVALID_HANDLE_VALUE)
		FindClose(hFindFile);
}

CList<CString,CString>* CConfigurations::GetNameList()
{
	return &m_Names;
}

UINT CConfigurations::GetHash(LPCTSTR lpszConfigName)
{
	UINT nHash = 0;
	while (*lpszConfigName)
		nHash = (nHash << 5) + nHash + *lpszConfigName++;
	return nHash;
}
