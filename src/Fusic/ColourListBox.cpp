// ColourListBox.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "ColourListBox.h"
#include <iostream>

// CColourListBox

IMPLEMENT_DYNAMIC(CColourListBox, CListBox)

CColourListBox::CColourListBox()
{

	//default Colours:
	m_bgColour = RGB(0,0,0);
	m_TextColour = RGB(255,255,255);
	m_highlightColour = RGB(0,100,0);
	m_altColour1 = RGB(0,0,0);
	m_altColour2 = RGB(50,50,50);
	brush.CreateSolidBrush(RGB(0,0,0));
}

CColourListBox::~CColourListBox()
{
}


BEGIN_MESSAGE_MAP(CColourListBox, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CColourListBox message handlers

void CColourListBox::fnSetBackColour(COLOURREF col)
{
	m_bgColour = col;
}

void CColourListBox::fnSetBackColour(int red, int green, int blue)
{
	if(red < 0 || red > 255)
	{
		return;
	}
	if(green < 0 || green > 255)
	{
		return;
	}
	if(blue < 0 || blue > 255)
	{
		return;
	}

	m_bgColour = RGB(red,green,blue);
}

void CColourListBox::fnSetTextColour(COLOURREF col)
{
	m_TextColour = col;
}

void CColourListBox::fnSetTextColour(int red, int green, int blue)
{
	if(red < 0 || red > 255)
	{
		return;
	}
	if(green < 0 || green > 255)
	{
		return;
	}
	if(blue < 0 || blue > 255)
	{
		return;
	}

	m_TextColour = RGB(red, green, blue);
}

void CColourListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	CString strText;
	//get the text for the item:
	if(lpDrawItemStruct->itemID == -1)
	{
		return;
	}
	GetText(lpDrawItemStruct->itemID, strText);
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	// If this item is selected, set the background color 
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetBkColor(m_highlightColour);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, 
			m_highlightColour);
		CBrush br(RGB(255,255,255));
		dc.FrameRect(&lpDrawItemStruct->rcItem, &br);
	}
	else
	{
		//the item doesn't have focus so we need to draw
		//the alternating bg:
		if((lpDrawItemStruct->itemID + 1) % 2 == 0)
		{
			dc.SetBkColor(m_altColour1);
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, 
				m_altColour1);
		}
		else
		{
			dc.SetBkColor(m_altColour2);
			dc.FillSolidRect(&lpDrawItemStruct->rcItem,
				m_altColour2);
		}
	}

	dc.SetTextColor(m_TextColour);
	// Draw the text.
	dc.DrawText(
		strText,
		strText.GetLength(),
		&lpDrawItemStruct->rcItem,
		DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();
}

int CColourListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	LPCSTR item1 = (LPCSTR) lpCompareItemStruct->itemData1;
	LPCSTR item2 = (LPCSTR) lpCompareItemStruct->itemData2;
	return _tcscmp(item1, item2);
	// TODO:  Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
}


BOOL CColourListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//control the bg painting:
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(rt, m_bgColour);
	return TRUE;
	//return CListBox::OnEraseBkgnd(pDC);
}

void CColourListBox::OnPaint()
{
	if(GetCount() == 0)
	{
		CPaintDC dc(this);
		CRect rt;
		GetClientRect(&rt);
		dc.FillSolidRect(rt, RGB(0,0,0));
	}
	else
	{
		CListBox::OnPaint();
	}
}