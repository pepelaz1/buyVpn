#include "StdAfx.h"
#include "NetAdapters.h"
#include <Ras.h>
#include <RasError.h>
#include <objbase.h>
#include <netcon.h> 

CNetAdapters::CNetAdapters(void)
{
}

CNetAdapters::~CNetAdapters(void)
{
}

void CNetAdapters::Update()
{
	m_Names.RemoveAll();
	m_Names.AddHead(CString(TEXT("None")));
	EnumNetConnections();
	EnumRasConnections();
}

void CNetAdapters::EnumNetConnections()
{
	HRESULT hr = E_FAIL;
	INetConnectionManager* pNet;
	
	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pNet);
	if (SUCCEEDED(hr))
	{
		IEnumNetConnection* pEnum;
		INetConnection* pConn;
		NETCON_PROPERTIES* pProps;
		ULONG uCount = 0;

		pNet->EnumConnections(NCME_DEFAULT, &pEnum);

		while (pEnum->Next(1, &pConn, &uCount) == S_OK)
		{
			pConn->GetProperties(&pProps);

			CString strName(pProps->pszwName);

			m_Names.AddTail(strName);

			CoTaskMemFree(pProps->pszwName);
			CoTaskMemFree(pProps->pszwDeviceName);
			CoTaskMemFree(pProps);
			pConn->Release();
		}

		pEnum->Release();
		pNet->Release();
		CoUninitialize();
	}
}

void CNetAdapters::EnumRasConnections()
{
	DWORD dwResult = 0;
	RASCONN* pRasConn = NULL;
	DWORD dwSize = 0;
	DWORD dwConnCount = 0;

	dwResult = RasEnumConnections(pRasConn, &dwSize, &dwConnCount);
	if (dwResult == ERROR_BUFFER_TOO_SMALL)
	{
		pRasConn = (RASCONN*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
		if (pRasConn == NULL)
		{
			//HeapAlloc error;
			return;
		}

		pRasConn[0].dwSize = sizeof(RASCONN);

		dwResult = RasEnumConnections(pRasConn, &dwSize, &dwConnCount);
		if (dwResult == ERROR_SUCCESS)
		{
			for (DWORD i = 0; i < dwConnCount; ++i)
			{
				CString strName(pRasConn[i].szEntryName);

				m_Names.AddTail(strName);
			}
		}

		HeapFree(GetProcessHeap(), 0, pRasConn);
		pRasConn = NULL;
	}
}

CList<CString,CString>* CNetAdapters::GetNameList()
{
	return &m_Names;
}

void CNetAdapters::StopAdapter(LPCTSTR lpszAdapterName)
{
	StopNetConnection(lpszAdapterName);
	StopRasConnection(lpszAdapterName);
}

void CNetAdapters::StopNetConnection(LPCTSTR lpszAdapterName)
{
	HRESULT hr = E_FAIL;
	INetConnectionManager* pNet;

	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pNet);
	if (SUCCEEDED(hr))
	{
		IEnumNetConnection* pEnum;
		INetConnection* pConn;
		NETCON_PROPERTIES* pProps;
		ULONG uCount = 0;

		pNet->EnumConnections(NCME_DEFAULT, &pEnum);

		while (pEnum->Next(1, &pConn, &uCount) == S_OK)
		{
			pConn->GetProperties(&pProps);
			if (!lstrcmp(pProps->pszwName, lpszAdapterName))
			{
				hr = pConn->Disconnect();
			}

			CoTaskMemFree(pProps->pszwName);
			CoTaskMemFree(pProps->pszwDeviceName);
			CoTaskMemFree(pProps);
			pConn->Release();
		}

		pEnum->Release();
		pNet->Release();
		CoUninitialize();
	}
}

void CNetAdapters::StopRasConnection(LPCTSTR lpszAdapterName)
{
	DWORD dwResult = 0;
	RASCONN* pRasConn = NULL;
	DWORD dwSize = 0;
	DWORD dwConnCount = 0;

	dwResult = RasEnumConnections(pRasConn, &dwSize, &dwConnCount);
	if (dwResult == ERROR_BUFFER_TOO_SMALL)
	{
		pRasConn = (RASCONN*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
		if (pRasConn == NULL)
		{
			return;
		}

		pRasConn[0].dwSize = sizeof(RASCONN);

		dwResult = RasEnumConnections(pRasConn, &dwSize, &dwConnCount);
		if (dwResult == ERROR_SUCCESS)
		{
			for (DWORD i = 0; i < dwConnCount; ++i)
			{
				if (!lstrcmp(pRasConn[i].szEntryName, lpszAdapterName))
				{
					RasHangUp(pRasConn[i].hrasconn);
				}
			}
		}

		HeapFree(GetProcessHeap(), 0, pRasConn);
		pRasConn = NULL;
	}
}