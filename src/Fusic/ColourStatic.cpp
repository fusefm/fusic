// ColourStatic.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "ColourStatic.h"


// CColourStatic

//public member functions:
	
//fnSetBackgroundColour - will set the bg colour of the control
//and also redraw it to apply changes:
void CColourStatic::fnSetBackgroundColour(COLORREF colBgColour)
{
	//set the colour:
	m_colBgColour = colBgColour;

	//update the control:
	RedrawWindow();
}

void CColourStatic::fnSetBackgroundColour(int intRed, int intGreen, int intBlue)
{
	COLOURREF colBgColour;

	//ensure that we have some valid colours:
	if(intRed < 0 || intRed > 255)
	{
		return;
	}

	if(intGreen < 0 || intRed > 255)
	{
		return;
	}

	if(intBlue < 0 || intBlue > 255)
	{
		return;
	}

	//we have valid colours:
	colBgColour = RGB(intRed, intGreen, intBlue);
	m_colBgColour = colBgColour;
}

//fnSetTextColour - will set the text colour of the control
//and also redraw it to apply changes:
void CColourStatic::fnSetTextColour(COLOURREF colTxtColour)
{
	//set the colour:
	m_colTxtColour = colTxtColour;

	//update the control:
	RedrawWindow();
}

void CColourStatic::fnSetTextColour(int intRed, int intGreen, int intBlue)
{
	COLOURREF colTxtColour;

	//ensure that we have some valid colours:
	if(intRed < 0 || intRed > 255)
	{
		return;
	}

	if(intGreen < 0 || intRed > 255)
	{
		return;
	}

	if(intBlue < 0 || intBlue > 255)
	{
		return;
	}

	//we have valid colours:
	colTxtColour = RGB(intRed, intGreen, intBlue);
	m_colTxtColour = colTxtColour;
}

IMPLEMENT_DYNAMIC(CColourStatic, CStatic)

CColourStatic::CColourStatic()
{
	//setup default colours:
	m_colBgColour = RGB(0,0,0);
	m_colTxtColour = RGB(0,0,0);
}

CColourStatic::~CColourStatic()
{
}


BEGIN_MESSAGE_MAP(CColourStatic, CStatic)
END_MESSAGE_MAP()



// CColourStatic message handlers



void CColourStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//Draw the static control with some colour:
	CDC dc;
	CRect rt;
	CString strText;
	
	//Get device context object
	dc.Attach(lpDrawItemStruct->hDC);

	//get the button rect:
	rt = lpDrawItemStruct->rcItem;

	//paint the background of the control:
	dc.FillSolidRect(rt, m_colBgColour);
//	dc.SetBkMode(TRANSPARENT);

	//set the text colour:
	dc.SetTextColor(m_colTxtColour);
	
	//draw out the text:
	this->GetWindowTextA(strText);
	dc.DrawText(strText, rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	//free memory:
	dc.Detach();
}

void CColourStatic::PreSubclassWindow()
{
	//ensure that the control is ownerdrawn:
	ModifyStyle(0,SS_OWNERDRAW);

	//contine to do presubclass:
	CStatic::PreSubclassWindow();
}
