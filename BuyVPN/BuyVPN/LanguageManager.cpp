#include "stdafx.h"
#include "LanguageManager.h"


CLanguageManager::CLanguageManager()
{
	PopulateMaps();
}


CLanguageManager::~CLanguageManager()
{
}

void CLanguageManager::PopulateMaps()
{
	m_English.SetAt(L"Login", L"Login:");
	m_Russian.SetAt(L"Login", L"�����:");

	m_English.SetAt(L"Password", L"Password:");
	m_Russian.SetAt(L"Password", L"������:");

	m_English.SetAt(L"SaveLogin", L"Remember login and password");
	m_Russian.SetAt(L"SaveLogin", L"��������� ����� � ������");

	m_English.SetAt(L"MemberArea", L"Member area");
	m_Russian.SetAt(L"MemberArea", L"������ �������");

	m_English.SetAt(L"SelectConfiguration", L"Select configuration:");
	m_Russian.SetAt(L"SelectConfiguration", L"����� ������������:");

	m_English.SetAt(L"NetAdapter", L"Turn off the network when VPN disconnected suddenly\n(prevent traffic leakage) :");
	m_Russian.SetAt(L"NetAdapter", L"��������� ��� ���� ����� VPN ���������� ��������\n(������������� ������ ������� ���� vpn) :");

	m_English.SetAt(L"CheckAccount", L"Check account state:");
	m_Russian.SetAt(L"CheckAccount", L"��������� ��������� ��������:");

	m_English.SetAt(L"Balance", L"Balance: ");
	m_Russian.SetAt(L"Balance", L"������: ");

	m_English.SetAt(L"Expires", L"Expires: ");
	m_Russian.SetAt(L"Expires", L"��������: ");

	m_English.SetAt(L"Connect", L"Connect");
	m_Russian.SetAt(L"Connect", L"������������");

	m_English.SetAt(L"Disconnected", L"VPN disconnected");
	m_Russian.SetAt(L"Disconnected", L"VPN ��������");

	m_English.SetAt(L"Disconnect", L"Disconnect");
	m_Russian.SetAt(L"Disconnect", L"�����������");

	m_English.SetAt(L"Connecting", L"Connecting...");
	m_Russian.SetAt(L"Connecting", L"�����������...");

	m_English.SetAt(L"Connected", L"VPN connected");
	m_Russian.SetAt(L"Connected", L"VPN ���������");

	m_English.SetAt(L"Disconnecting", L"Disconnecting...");
	m_Russian.SetAt(L"Disconnecting", L"...");

	m_English.SetAt(L"SelectVpnChains", L"Select VPN chains:");
	m_Russian.SetAt(L"SelectVpnChains", L"����� VPN �������:");

	m_English.SetAt(L"Edit", L"Edit");
	m_Russian.SetAt(L"Edit", L"�������������");

	m_English.SetAt(L"ShowLog", L"Show log");
	m_Russian.SetAt(L"ShowLog", L"�������� ���");

	m_English.SetAt(L"DisableNetwork", L"Attention, VPN is disconnected! Selected network disabled.");
	m_Russian.SetAt(L"DisableNetwork", L"��������, VPN ����������! ��������� ���� ��������");

	m_English.SetAt(L"DisableNone", L"Attention, VPN is disconnected!");
	m_Russian.SetAt(L"DisableNone", L"��������, VPN ����������!");
	

	m_pCurrentMap = &m_English;
}

void CLanguageManager::SetLanguage(Language language)
{
	if (language == English)
		m_pCurrentMap = &m_English;
	else if (language == Russian)
		m_pCurrentMap = &m_Russian;
}

CString CLanguageManager::GetText(CString identifier)
{
	return (*m_pCurrentMap)[identifier];
}
