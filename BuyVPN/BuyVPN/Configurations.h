#pragma once

class CConfigurations
{
public:
	CConfigurations(void);
	~CConfigurations(void);

	void Update(LPCTSTR lpszType);
	CString GetPathByName(LPCTSTR lpszConfigName);
	CList<CString,CString>* GetNameList();

private:
	void SearchConfigFiles(LPCTSTR lpszDir, LPCTSTR lpszType);
	UINT GetHash(LPCTSTR lpszConfigName);

	CMap<UINT,UINT,CString,CString> m_Configurations;
	CList<CString,CString> m_Names;
};
