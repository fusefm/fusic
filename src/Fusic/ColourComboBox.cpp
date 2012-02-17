// ColourComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "ColourComboBox.h"


// CColourComboBox

IMPLEMENT_DYNAMIC(CColourComboBox, CComboBox)

CColourComboBox::CColourComboBox()
{
	//default colours:
	m_colorBack = RGB(0,0,0);
	m_colorText = RGB(255,255,255);
}

CColourComboBox::~CColourComboBox()
{
}


BEGIN_MESSAGE_MAP(CColourComboBox, CComboBox)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CColourComboBox message handlers

HBRUSH CColourComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CPaintDC dc(this);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkColor(RGB(0, 0, 0));
	return ::CreateSolidBrush(RGB(0,0,0));
}

void CColourComboBox::fnSetBackColour(COLOURREF col)
{
	m_colorBack = col;
}

void CColourComboBox::fnSetBackColour(int red, int green, int blue)
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

	m_colorBack = RGB(red,green,blue);
}

void CColourComboBox::fnSetTextColour(COLOURREF col)
{
	m_colorText = col;
}

void CColourComboBox::fnSetTextColour(int red, int green, int blue)
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
	m_colorText = RGB(red,green,blue);
}
