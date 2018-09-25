#if !defined(AFX_COLORCHECK_H__A897D857_09A3_11D6_8CAD_00500439B545__INCLUDED_)
#define AFX_COLORCHECK_H__A897D857_09A3_11D6_8CAD_00500439B545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorCheck window

class CColorCheck : public CButton
{
DECLARE_DYNAMIC(CColorCheck)

// Construction
public:

	CColorCheck();
	virtual ~CColorCheck();
	
	virtual void SetCheck(int nCheck);
	virtual int GetCheck();
	COLORREF SetBkColor(COLORREF color);
	COLORREF SetArrowColor(COLORREF newColor);
	COLORREF SetMyTextColor(COLORREF txtColor);

protected:
	void DrawCheckCaption(CDC *pDC, CRect rect, const CString& strText, BOOL bDisabled);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItem);	//override the standard function (no WM_DRAWITEM !!!)
	afx_msg BOOL OnClicked();
	//{{AFX_MSG(CColorCheck)
	//}}AFX_MSG

private:
	BOOL m_checkFlag;
	UINT oldAction;
	UINT oldState;
	COLORREF m_clrBox;
	COLORREF m_clrArrow;
	COLORREF m_clrText;
	COLORREF m_clrTextBk;
	COLORREF m_clrTextDisabled;
	COLORREF m_clrBoxDisabled;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCHECK_H__A897D857_09A3_11D6_8CAD_00500439B545__INCLUDED_)
