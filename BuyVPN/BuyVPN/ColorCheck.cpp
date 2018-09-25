// ColorCheck.cpp : implementation file
//
// Copyright (c) 2002 Radu Naiu
//
// Distribute and use freely, except:
// a) Don't alter or remove this notice.
// b) Mark the changes you make.
// This file is provided "as is" with no expressed or implied warranty.
// Use at your own risk. 
//
// History:
//
//      v1.0  24.01.2002  Initial release
//		v1.1  28.01.2002  BUG corrected: default checked state afeter initialization handled  

#include "stdafx.h"
#include "ColorCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorCheck
IMPLEMENT_DYNAMIC(CColorCheck, CButton)

CColorCheck::CColorCheck()
{
	m_checkFlag = FALSE;
	oldState = 0;
	oldAction = 0;
	m_clrBox = RGB(255,255,255);
	m_clrBoxDisabled = RGB(150,150,10);
	m_clrArrow = RGB(0,0,0);
	m_clrText = RGB(255,255,255);
	m_clrTextBk = RGB(0x3a,0x3a,0x3a);
	m_clrTextDisabled = RGB(150,150,150);
}

CColorCheck::~CColorCheck()
{
}


BEGIN_MESSAGE_MAP(CColorCheck, CButton)
	//{{AFX_MSG_MAP(CColorCheck)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCheck message handlers

void CColorCheck::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rc;
	CRect rect(13,1,11,11);
	CRect focusRect, btnRect;
	focusRect.CopyRect(&lpDrawItemStruct->rcItem); 
	
	CSize captionSize;
	BOOL drawFocus = FALSE;

 
	// Retrieve the button's caption
	//
    CString strText;
    GetWindowText(strText);

	//
	// Set the Text Position more to the left
	//
	captionSize = pDC->GetTextExtent(strText);
	btnRect.SetRect(18,0,16 + captionSize.cx + 4, 15);

	//
	// Set the focus rectangle to just past the border decoration
	//
	focusRect.SetRect(16,0,16 + captionSize.cx + 8, 15);

	UINT state = lpDrawItemStruct->itemState;
	UINT action = lpDrawItemStruct->itemAction;

	if (action == ODA_DRAWENTIRE)
	{
		pDC->SelectStockObject(NULL_BRUSH);

		//draw the caption
		pDC->FillSolidRect(focusRect, m_clrTextBk);
		DrawCheckCaption(pDC, btnRect, strText, state & ODS_DISABLED);
		
		//draw the borders
		CPen pen(PS_SOLID, 1, m_clrArrow);
		pDC->SelectObject(&pen);

		pDC->Rectangle(0, 0, 14, 14);
		pDC->Rectangle(1, 1, 13, 13);

		pen.DeleteObject();	
	}

	if (action == ODA_DRAWENTIRE
		|| action == ODA_SELECT)
	{
		pDC->FillSolidRect(2, 2, 10, 10, state & ODS_DISABLED ? m_clrBoxDisabled : m_clrBox);

		if (m_checkFlag == 1)
		{
			// draw the arrow
			CPen pen(PS_SOLID, 1, m_clrArrow);//BLACK_PEN);
			pDC->SelectObject(&pen);

			pDC->MoveTo(4,7);
			pDC->LineTo(7,11);
			pDC->LineTo(10,3);

			pDC->MoveTo(4,8);
			pDC->LineTo(7,9);
			pDC->LineTo(9,4);

			pen.DeleteObject();	//release
		}

	}

	if((action == ODA_FOCUS) && (( state & ODS_FOCUS ) ^ ( oldState & ODS_FOCUS )) 
		|| ( (action != ODA_FOCUS) && ( state & ODS_FOCUS ) ) )
	{	
		DrawFocusRect(lpDrawItemStruct->hDC, (LPRECT)&focusRect);
	}

	oldState = state;
	oldAction = action;
}

void CColorCheck::DrawCheckCaption(CDC *pDC, CRect rect, const CString& strText, BOOL bDisabled)
{
	COLORREF clrPrevText = pDC->SetTextColor(bDisabled ? m_clrTextDisabled : m_clrText);
    COLORREF clrPrevBk = pDC->SetBkColor(m_clrTextBk);
	pDC->DrawText(strText, rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(clrPrevText);
	pDC->SetBkColor(clrPrevBk);
}


void CColorCheck::SetCheck(int nCheck)
{
//	nCheck = GetCheck();
	if (nCheck == 1)
		m_checkFlag = 1;
	else
		m_checkFlag = 0;
}

BOOL CColorCheck::GetCheck()
{
	if (m_checkFlag == 1)
		return 1;	
	else
		return 0;
}

COLORREF CColorCheck::SetBkColor(COLORREF boxColor)
{
	m_clrBox = boxColor;
	return m_clrBox;
}

COLORREF CColorCheck::SetArrowColor(COLORREF arrowColor)
{
	m_clrArrow = arrowColor;
	return m_clrArrow;
}

COLORREF CColorCheck::SetMyTextColor(COLORREF textColor)
{
	m_clrText = textColor;
	return m_clrText;
}

BOOL CColorCheck::OnClicked()
{
	if (m_checkFlag == 1)
	{
		m_checkFlag = 0;
	}
	else
	{
		m_checkFlag = 1;
	}

	return FALSE;
}