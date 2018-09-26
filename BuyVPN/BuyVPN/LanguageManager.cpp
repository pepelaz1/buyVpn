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
