#pragma once

class CNetAdapters
{
public:
	CNetAdapters(void);
	~CNetAdapters(void);

	void Update();
	CList<CString,CString>* GetNameList();
	void StopAdapter(LPCTSTR lpszAdapterName);

private:
	void EnumRasConnections();
	void EnumNetConnections();
	void StopRasConnection(LPCTSTR lpszAdapterName);
	void StopNetConnection(LPCTSTR lpszAdapterName);

	CList<CString,CString> m_Names;
};
