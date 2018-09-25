#pragma once

class CHttpWorker
{
public:
	CHttpWorker(void);
	~CHttpWorker(void);

	static BOOL LoadPage(LPCTSTR lpszUrl, LPCTSTR lpszReferer, CStringA& strAnswer);
	static BOOL LoadPage(LPCTSTR lpszHost, LPCTSTR lpszAddress, LPCTSTR lpszReferer, CStringA& strAnswer);
	static BOOL LoadPage(LPCTSTR lpszHost, INTERNET_PORT nPort, LPCTSTR lpszAddress, LPCTSTR lpszReferer, CStringA& strAnswer, DWORD& dwStatusCode);
	static BOOL LoadFile(LPCTSTR lpszUrl, LPCTSTR lpszFilename);
	static BOOL SendPost(LPCTSTR lpszUrl, LPCTSTR lpszReferer, CStringA strPostData, CStringA& strAnswer);
};
