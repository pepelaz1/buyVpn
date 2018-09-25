#include "stdafx.h"
#include "Common.h"

CString GetRunningDir()
{
	static CString strRunningDir = TEXT("");
	if (strRunningDir.IsEmpty())
	{
		TCHAR sFilename[_MAX_PATH];
		TCHAR sDrive[_MAX_DRIVE];
		TCHAR sDir[_MAX_DIR];
		TCHAR sFname[_MAX_FNAME];
		TCHAR sExt[_MAX_EXT];
		GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);
		_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

		strRunningDir = (CString(sDrive) + CString(sDir));
		int nLen = strRunningDir.GetLength();
		if (strRunningDir.GetAt(nLen-1) != _T('\\'))
			strRunningDir += _T("\\");
	}

	return strRunningDir;
}

CString GetLogFilename()
{
	static CString strLogFilename = TEXT("");
	if (strLogFilename.IsEmpty())
	{
		strLogFilename = GetRunningDir() + TEXT("log\\openvpn.log");
	}

	return strLogFilename;
}

CString GetDir(LPCTSTR szFilename)
{
	CString strDir;

	if (!szFilename)
		return strDir;

	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];
	_tsplitpath(szFilename, sDrive, sDir, sFname, sExt);

	strDir = (CString(sDrive) + CString(sDir));
	int nLen = strDir.GetLength();
	if (nLen > 0)
		if (strDir.GetAt(nLen-1) != _T('\\'))
			strDir += _T("\\");


	return strDir;
}