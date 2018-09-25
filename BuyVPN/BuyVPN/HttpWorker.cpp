#include "StdAfx.h"
#include "HttpWorker.h"

CHttpWorker::CHttpWorker(void)
{
}

CHttpWorker::~CHttpWorker(void)
{
}

BOOL CHttpWorker::LoadPage(LPCTSTR lpszUrl, LPCTSTR lpszReferer, CStringA& strAnswer)
{
	CString strHost;
	CString strAddress;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	DWORD dwStatusCode = 0;

	if (!AfxParseURL(lpszUrl, dwServiceType, strHost, strAddress, nPort ))
	{
		return FALSE;
	}

	return LoadPage(strHost, nPort, strAddress, lpszReferer, strAnswer, dwStatusCode);
}

BOOL CHttpWorker::LoadPage(LPCTSTR lpszHost, LPCTSTR lpszAddress, LPCTSTR lpszReferer, CStringA& strAnswer)
{
	DWORD dwStatusCode = 0;

	return LoadPage(lpszHost, 80, lpszAddress, lpszReferer, strAnswer, dwStatusCode);
}

BOOL CHttpWorker::LoadPage(LPCTSTR lpszHost, INTERNET_PORT nPort, LPCTSTR lpszAddress, LPCTSTR lpszReferer, CStringA& strAnswer, DWORD& dwStatusCode)
{
	BOOL bRes = FALSE;
	CInternetSession session;
	CHttpConnection* pHttpCon = NULL;
	CHttpFile* pFile = NULL;

	strAnswer.Empty();
	dwStatusCode = -1;

	try
	{
		pHttpCon = session.GetHttpConnection(lpszHost, nPort);
		if (pHttpCon)
		{
			DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT;
			if (nPort == 443)
				dwFlags |= INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
			pFile = pHttpCon->OpenRequest(TEXT("GET"), lpszAddress, lpszReferer, 1, NULL, NULL, dwFlags);
			if (pFile && pFile->SendRequest())
			{
				pFile->QueryInfoStatusCode(dwStatusCode);
				if (dwStatusCode == HTTP_STATUS_OK)
				{
					char szBuffer[2048];
					UINT uiCount = 0;
					while (uiCount = pFile->Read(szBuffer, sizeof(szBuffer)-1))
					{
						szBuffer[uiCount] = 0;
						strAnswer.Append(szBuffer, uiCount);
					}

					bRes = TRUE;
				}				
			}
		}
	}
	catch (CInternetException*)
	{
		bRes = FALSE;
	}

	if (pFile)
	{
		pFile->Close();
		delete pFile;
	}
	if (pHttpCon)
	{
		pHttpCon->Close();
		delete pHttpCon;
	}

	session.Close();

	return bRes;
}

BOOL CHttpWorker::LoadFile(LPCTSTR lpszUrl, LPCTSTR lpszFilename)
{
	BOOL bRes = FALSE;
	CInternetSession session;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	DWORD dwStatusCode = 0;
	CString strHost;
	CString strAddress;
	INTERNET_PORT nPort = 80;
	DWORD dwServiceType;


	if (!AfxParseURL(lpszUrl, dwServiceType, strHost, strAddress, nPort ))
	{
		return FALSE;
	}

	try
	{
		pHttpConnection = session.GetHttpConnection(strHost, nPort);
		if (pHttpConnection)
		{
			DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT;
			if (nPort == 443)
				dwFlags |= INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
			pHttpFile = pHttpConnection->OpenRequest(TEXT("GET"), strAddress, NULL, 1, NULL, NULL, dwFlags);
			if (pHttpFile && pHttpFile->SendRequest())
			{
				pHttpFile->QueryInfoStatusCode(dwStatusCode);
				if (dwStatusCode == HTTP_STATUS_OK)
				{
					CFile* pFile = new CFile(lpszFilename, CFile::modeCreate | CFile::modeReadWrite);
					if (pFile)
					{
						char szBuffer[2048];
						UINT uiCount = 0;
						while (uiCount = pHttpFile->Read(szBuffer, sizeof(szBuffer)))
						{
							pFile->Write(szBuffer, uiCount);
						}

						pFile->Close();
						delete pFile;

						bRes = TRUE;
					}					
				}				
			}
		}
	}
	catch (CInternetException*)
	{
		bRes = FALSE;
	}

	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConnection)
	{
		pHttpConnection->Close();
		delete pHttpConnection;
	}

	session.Close();

	return bRes;
}

BOOL CHttpWorker::SendPost(LPCTSTR lpszUrl, LPCTSTR lpszReferer, CStringA strPostData, CStringA& strAnswer)
{
	BOOL bRes = FALSE;
	CString strHost;
	CString strAddress;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	DWORD dwStatusCode = 0;
	CInternetSession session;
	CHttpConnection* pHttpCon = NULL;
	CHttpFile* pFile = NULL;
	CString strHeaders = TEXT("Content-Type: application/x-www-form-urlencoded");

	if (!AfxParseURL(lpszUrl, dwServiceType, strHost, strAddress, nPort ))
	{
		return FALSE;
	}

	strAnswer.Empty();
	dwStatusCode = -1;

	try
	{
		pHttpCon = session.GetHttpConnection(strHost, nPort);
		if (pHttpCon)
		{
			DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT;
			if (nPort == 443)
				dwFlags |= INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
			pFile = pHttpCon->OpenRequest(TEXT("POST"), strAddress, lpszReferer, 1, NULL, NULL, dwFlags);
			if (pFile && pFile->SendRequest(strHeaders, (LPVOID)(LPCSTR)strPostData, strPostData.GetLength()))
			{
				pFile->QueryInfoStatusCode(dwStatusCode);
				if (dwStatusCode == HTTP_STATUS_OK)
				{
					char szBuffer[2048];
					UINT uiCount = 0;
					while (uiCount = pFile->Read(szBuffer, sizeof(szBuffer)-1))
					{
						szBuffer[uiCount] = 0;
						strAnswer.Append(szBuffer, uiCount);
					}

					bRes = TRUE;
				}				
			}
		}
	}
	catch (CInternetException*)
	{
		bRes = FALSE;
	}

	if (pFile)
	{
		pFile->Close();
		delete pFile;
	}
	if (pHttpCon)
	{
		pHttpCon->Close();
		delete pHttpCon;
	}

	session.Close();

	return bRes;
}
