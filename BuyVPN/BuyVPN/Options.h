#pragma once

#define VCOF_USERNAME		0x00000001
#define VCOF_PASSWORD		0x00000002
#define VCOF_SAVEPASS		0x00000004
#define VCOF_CONFIGURATION	0x00000008
#define VCOF_SAFENET		0x00000010
#define VCOF_NETADAPTER		0x00000020
#define VCOF_CHECKACCOUNT	0x00000040
#define VCOF_LAUNCHSTART	0x00000080
#define VCOF_CONFIGVERSION	0x00000100
#define VCOF_FOLDER			0x00000200
#define VCOF_ALL			0xFFFFFFFF

class COptions
{
public:
	COptions(void);
	~COptions(void);

	BOOL Load();
	BOOL Save(DWORD dwOptionsFlag);

	CString m_strUsername;
	CString m_strPassword;
	BOOL m_fSavePassword;
	CString m_strConfiguration;
	CString m_strFolder;
	CString m_strNetAdapter;
	BOOL m_fCheckAccount;
	BOOL m_fLaunchOnStart;
	CString m_strConfigVer;

private:
	TCHAR* DecodePassword(TCHAR* pass);
};
