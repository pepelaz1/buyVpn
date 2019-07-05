#pragma once

class CNetAdapters
{
public:
	CNetAdapters(void);
	~CNetAdapters(void);

	void Update(BOOL noTap = FALSE);
	CList<CString,CString>* GetNameList();
	void StopAdapter(LPCTSTR lpszAdapterName);

private:
	void EnumRasConnections();
	void EnumNetConnections(BOOL noTap = FALSE);
	void StopRasConnection(LPCTSTR lpszAdapterName);
	void StopNetConnection(LPCTSTR lpszAdapterName);

	CList<CString,CString> m_Names;
};
