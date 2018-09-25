#pragma once

class CHostsConfig
{
public:
	CHostsConfig(void);
	~CHostsConfig(void);

	CString GetCurrentHost();
	void Update(CString& strConfigVersion, CString& strConfigUrl);
	BOOL SetNextHost();

private:
	void Load();
	void Save();
	void DecodeHosts();
	void UpdateProg(CString strVersion, CString strUrl);

	CList<CString,CString> 	m_aHosts;
	POSITION m_HostsPos;
	CString m_strCryptedHosts;
};
