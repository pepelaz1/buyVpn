#include "StdAfx.h"
#include "Common.h"
#include "Configurations.h"

CConfigurations::CConfigurations(void)
{
	PopulateCountriesMap();
}

CConfigurations::~CConfigurations(void)
{
}

void CConfigurations::Update(LPCTSTR lpszType, CString language)
{
	m_Language = language;
	CString strDir = GetRunningDir();
	strDir.Append(TEXT("config"));

	m_Configurations.RemoveAll();
	m_Names.RemoveAll();

	SearchConfigFiles(strDir, lpszType);
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

void CConfigurations::SearchConfigFiles(LPCTSTR lpszDir, LPCTSTR lpszType)
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
					if (strSubDir.Find(lpszType) != -1)
						SearchConfigFiles(strSubDir, lpszType);
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
					//if (strName.Find(lpszType) != -1 || strName.Find(L"UNBLOCK") != -1)
					//{
						CString strValue;
						int n = 1;

						while (m_Configurations.Lookup(GetHash(strName), strValue))
						{
							strName.Format(TEXT("%s(%d)"), szName, n++);
						}

						
						//strName.Append(L" ru");
						strName = ConstructName(strName);

						m_Configurations.SetAt(GetHash(strName), strPath);
						m_Names.AddTail(strName);
					//}
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


CString  CConfigurations::ConstructName(CString name)
{
	CString result = L"";
	CString prefix = m_Language.MakeLower().Left(2);
	//result.Append(prefix);
	//result.Append(L" ");
	result.Append(name);

	//CString countryCode = name.Right(2);
	int idx = name.ReverseFind(' ');
	CString countryCode = name.Right(name.GetLength() - idx - 1);
	CString code = prefix;
	code.Append(L"-");
	code.Append(countryCode);
	
	CString country;
	if (m_Countries.Lookup(code, country))
	{
		idx = result.ReverseFind(' ');
		result = result.Left(idx + 1);
		result.Append(country);
	}
	return result;
}

void CConfigurations::PopulateCountriesMap() 
{
	m_Countries.SetAt(L"ru-AT", L"�������");
	m_Countries.SetAt(L"en-AT", L"Austria");

	m_Countries.SetAt(L"ru-AU", L"���������");
	m_Countries.SetAt(L"en-AU", L"Australia");

	m_Countries.SetAt(L"ru-BG", L"�������� 1");
	m_Countries.SetAt(L"en-BG", L"Bulgaria 1");

	m_Countries.SetAt(L"ru-BG2", L"�������� 2");
	m_Countries.SetAt(L"en-BG2", L"Bulgaria 2");

	m_Countries.SetAt(L"ru-BR", L"��������");
	m_Countries.SetAt(L"en-BR", L"Brazil");

	m_Countries.SetAt(L"ru-CA", L"������");
	m_Countries.SetAt(L"en-CA", L"Canada");

	m_Countries.SetAt(L"ru-CH", L"���������");
	m_Countries.SetAt(L"en-CH", L"Switzerland");

	m_Countries.SetAt(L"ru-CZ", L"�����");
	m_Countries.SetAt(L"en-CZ", L"Czech Republic");

	m_Countries.SetAt(L"ru-DE", L"�������� 1");
	m_Countries.SetAt(L"en-DE", L"Germany 1");

	m_Countries.SetAt(L"ru-DE2", L"�������� 2");
	m_Countries.SetAt(L"en-DE2", L"Germany 2");

	m_Countries.SetAt(L"ru-EE", L"�������");
	m_Countries.SetAt(L"en-EE", L"Estonia");

	m_Countries.SetAt(L"ru-EE", L"�������");
	m_Countries.SetAt(L"en-EE", L"Estonia");

	m_Countries.SetAt(L"ru-FR", L"������� 1");
	m_Countries.SetAt(L"en-FR", L"France 1");

	m_Countries.SetAt(L"ru-FR2", L"������� 2");
	m_Countries.SetAt(L"en-FR2", L"France 2");

	m_Countries.SetAt(L"ru-FR3", L"������� 3");
	m_Countries.SetAt(L"en-FR3", L"France 3");

	m_Countries.SetAt(L"ru-IT", L"������");
	m_Countries.SetAt(L"en-IT", L"Italy");

	m_Countries.SetAt(L"ru-JP", L"������");
	m_Countries.SetAt(L"en-JP", L"Japan");

	m_Countries.SetAt(L"ru-KR", L"�����");
	m_Countries.SetAt(L"en-KR", L"Korea");

	m_Countries.SetAt(L"ru-LU", L"����������");
	m_Countries.SetAt(L"en-LU", L"Luxembourg");

	m_Countries.SetAt(L"ru-LU", L"����������");
	m_Countries.SetAt(L"en-LU", L"Luxembourg");

	m_Countries.SetAt(L"ru-LV", L"������ 1");
	m_Countries.SetAt(L"en-LV", L"Latvia 1");

	m_Countries.SetAt(L"ru-LV2", L"������ 2");
	m_Countries.SetAt(L"en-LV2", L"Latvia 2");

	m_Countries.SetAt(L"ru-MY", L"��������");
	m_Countries.SetAt(L"en-MY", L"Malaysia");

	m_Countries.SetAt(L"ru-NL", L"���������� 1");
	m_Countries.SetAt(L"en-NL", L"Netherlands 1");

	m_Countries.SetAt(L"ru-NL2", L"���������� 2");
	m_Countries.SetAt(L"en-NL2", L"Netherlands 2");

	m_Countries.SetAt(L"ru-NL3", L"���������� 3");
	m_Countries.SetAt(L"en-NL3", L"Netherlands 3");

	m_Countries.SetAt(L"ru-PA", L"������");
	m_Countries.SetAt(L"en-PA", L"Panama");

	m_Countries.SetAt(L"ru-PL", L"������");
	m_Countries.SetAt(L"en-PL", L"Poland");

	m_Countries.SetAt(L"ru-RO", L"�������");
	m_Countries.SetAt(L"en-RO", L"Romania");

	m_Countries.SetAt(L"ru-RU", L"������");
	m_Countries.SetAt(L"en-RU", L"Russia");

	m_Countries.SetAt(L"ru-SE", L"������");
	m_Countries.SetAt(L"en-SE", L"Sweden");

	m_Countries.SetAt(L"ru-SG", L"��������");
	m_Countries.SetAt(L"en-SG", L"Singapore");

	m_Countries.SetAt(L"ru-TR", L"������");
	m_Countries.SetAt(L"en-TR", L"Turkey");

	m_Countries.SetAt(L"ru-UA", L"�������");
	m_Countries.SetAt(L"en-UA", L"Ukraine");

	m_Countries.SetAt(L"ru-UK", L"�������������� 1");
	m_Countries.SetAt(L"en-UK", L"United Kingdom 1");

	m_Countries.SetAt(L"ru-UK2", L"�������������� 2");
	m_Countries.SetAt(L"en-UK2", L"United Kingdom 2");

	m_Countries.SetAt(L"ru-UK3", L"�������������� 3");
	m_Countries.SetAt(L"en-UK3", L"United Kingdom 3");

	m_Countries.SetAt(L"ru-US1", L"��� 1");
	m_Countries.SetAt(L"en-US1", L"USA 1");

	m_Countries.SetAt(L"ru-US2", L"��� 2");
	m_Countries.SetAt(L"en-US2", L"USA 2");

	m_Countries.SetAt(L"ru-US3", L"��� 3");
	m_Countries.SetAt(L"en-US3", L"USA 3");

	m_Countries.SetAt(L"ru-US4", L"��� 4");
	m_Countries.SetAt(L"en-US4", L"USA 4");

	m_Countries.SetAt(L"ru-US5", L"��� 5");
	m_Countries.SetAt(L"en-US5", L"USA 5");
}

