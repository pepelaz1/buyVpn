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
	m_Russian.SetAt(L"Login", L"Логин:");

	m_English.SetAt(L"Password", L"Password:");
	m_Russian.SetAt(L"Password", L"Пароль:");

	m_English.SetAt(L"SaveLogin", L"Remember login and password");
	m_Russian.SetAt(L"SaveLogin", L"Запомнить логин и пароль");

	m_English.SetAt(L"MemberArea", L"Member area");
	m_Russian.SetAt(L"MemberArea", L"Личный кабинет");

	m_English.SetAt(L"SelectConfiguration", L"Select configuration:");
	m_Russian.SetAt(L"SelectConfiguration", L"Выбор конфигурации:");

	m_English.SetAt(L"NetAdapter", L"Turn off the network when VPN disconnected suddenly\n(prevent traffic leakage) :");
	m_Russian.SetAt(L"NetAdapter", L"Отключить эту сеть когда VPN отключился внезапно\n(предотвратить утечку трафика мимо vpn) :");

	m_English.SetAt(L"CheckAccount", L"Check account state:");
	m_Russian.SetAt(L"CheckAccount", L"Проверять состояние аккаунта:");

	m_English.SetAt(L"Balance", L"Balance: ");
	m_Russian.SetAt(L"Balance", L"Баланс: ");

	m_English.SetAt(L"Expires", L"Expires: ");
	m_Russian.SetAt(L"Expires", L"Истекает: ");

	m_English.SetAt(L"Connect", L"Connect");
	m_Russian.SetAt(L"Connect", L"Подключиться");

	m_English.SetAt(L"Disconnected", L"VPN disconnected");
	m_Russian.SetAt(L"Disconnected", L"VPN отключен");

	m_English.SetAt(L"Disconnect", L"Disconnect");
	m_Russian.SetAt(L"Disconnect", L"Отключиться");

	m_English.SetAt(L"Connecting", L"Connecting...");
	m_Russian.SetAt(L"Connecting", L"Подключение...");

	m_English.SetAt(L"Connected", L"VPN connected");
	m_Russian.SetAt(L"Connected", L"VPN подключен");

	m_English.SetAt(L"Disconnecting", L"Disconnecting...");
	m_Russian.SetAt(L"Disconnecting", L"...");

	m_English.SetAt(L"SelectVpnChains", L"Select VPN chains:");
	m_Russian.SetAt(L"SelectVpnChains", L"Выбор VPN цепочек:");

	m_English.SetAt(L"Edit", L"Edit");
	m_Russian.SetAt(L"Edit", L"Редактировать");

	m_English.SetAt(L"ShowLog", L"Show log");
	m_Russian.SetAt(L"ShowLog", L"Показать лог");

	m_English.SetAt(L"DisableNetwork", L"Attention, VPN is disconnected! Selected network disabled.");
	m_Russian.SetAt(L"DisableNetwork", L"Внимание, VPN разъединен! Выбранная сеть отлючена");

	m_English.SetAt(L"DisableNone", L"Attention, VPN is disconnected!");
	m_Russian.SetAt(L"DisableNone", L"Внимание, VPN разъединен!");
	

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
