#pragma once

enum Language
{
	English = 0,
	Russian
};

class CLanguageManager
{
private:
	 CMapStringToString m_English;
	 CMapStringToString m_Russian;
	 CMapStringToString *m_pCurrentMap;

	void PopulateMaps();
public:
	CLanguageManager();
	virtual ~CLanguageManager();
	void SetLanguage(Language language);
	CString GetText(CString identifier);
};

