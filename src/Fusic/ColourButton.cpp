// ColourButton.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "ColourButton.h"


// CColourButton

IMPLEMENT_DYNAMIC(CColourButton, CButton)

CColourButton::CColourButton()
{
	//reset all members:
	m_bolChangeColoursClick = false;
	m_bolFlashNextRedraw = false;
	m_culBack = RGB(0,0,0);
	m_culFont = RGB(0,0,0);
	m_culEdge = RGB(0,0,0);
	m_culFlash = RGB(0,0,0);
	m_culOrigonalBack = RGB(0,0,0);
	m_strOrginalButtonText = "";
}

CColourButton::~CColourButton()
{
}


BEGIN_MESSAGE_MAP(CColourButton, CButton)
END_MESSAGE_MAP()


//public member functions implimentation:


//fnSetBackColour - set the back colour of the button, overloaded for both ints and
//COLOURREF's, bolSetAsOrigonal set equal to true will store the specified colour as an
//origonal colour for the button.
void CColourButton::fnSetBackColour(int intRed, int intGreen, int intBlue, bool bolSetAsOrigonal)
{
	//check we have a valid 8-bit colour:
	if(intRed < 0 || intRed > 255)
	{
		return;
	}
	if(intGreen < 0 || intGreen > 255)
	{
		return;
	}
	if(intBlue < 0 || intBlue > 255)
	{
		return;
	}

	//we have valid colors:
	m_culBack = RGB(intRed, intGreen, intBlue);

	//see if we need to set the origonal colour:
	if(bolSetAsOrigonal == true)
	{
		m_culOrigonalBack = RGB(intRed, intGreen, intBlue);
	}

	//need to update the window:
	RedrawWindow();
}
void CColourButton::fnSetBackColour(COLOURREF culBack, bool bolSetAsOrigonal)
{
	//set the colour:
	m_culBack = culBack;

	if(bolSetAsOrigonal == true)
	{
		m_culOrigonalBack = culBack;
	}

	RedrawWindow();
}

//fnSetFontColour - set the font colour of the button, overloaded for both ints and
//COLOURREF's.
void CColourButton::fnSetFontColour(int intRed, int intGreen, int intBlue)
{
	//check we have a valid 8-bit colour:
	if(intRed < 0 || intRed > 255)
	{
		return;
	}
	if(intGreen < 0 || intGreen > 255)
	{
		return;
	}
	if(intBlue < 0 || intBlue > 255)
	{
		return;
	}

	//we have valid colors:
	m_culFont = RGB(intRed, intGreen, intBlue);

	//need to update the window:
	RedrawWindow();
}
void CColourButton::fnSetFontColour(COLORREF culFont)
{
	//set the font colour:
	m_culFont = culFont;

	//need to update the window:
	RedrawWindow();
}

//fnSetEdgeColour - set the edge colour of the button, overloaded for both ints and
//COLOURREF's.
void CColourButton::fnSetEdgeColour(int intRed, int intGreen, int intBlue)
{
	//check we have a valid 8-bit colour:
	if(intRed < 0 || intRed > 255)
	{
		return;
	}
	if(intGreen < 0 || intGreen > 255)
	{
		return;
	}
	if(intBlue < 0 || intBlue > 255)
	{
		return;
	}

	//we have valid colors:
	m_culEdge = RGB(intRed, intGreen, intBlue);

	//need to update the window:
	RedrawWindow();
}

void CColourButton::fnSetEdgeColour(COLOURREF culEdge)
{
	//set the colour:
	m_culEdge = culEdge;

	//need to update the window:
	RedrawWindow();
}

//fnSetFlasColour - set the flash colour of the button, overloaded for both ints and
//COLOURREF's.
void CColourButton::fnSetFlashColour(int intRed, int intGreen, int intBlue)
{
	//check we have a valid 8-bit colour:
	if(intRed < 0 || intRed > 255)
	{
		return;
	}
	if(intGreen < 0 || intGreen > 255)
	{
		return;
	}
	if(intBlue < 0 || intBlue > 255)
	{
		return;
	}

	//we have valid colors:
	m_culFlash = RGB(intRed, intGreen, intBlue);
}

void CColourButton::fnSetFlashColour(COLOURREF culFlash)
{
	//set the colour:
	m_culFlash = culFlash;
}


//fnSetClickColourChange - Sets weather or not the button's back and font colurs swap on
//a mouse click on the button.
void CColourButton::fnSetClickColourChange(bool bolClickChange)
{
	//set the option:
	m_bolChangeColoursClick = bolClickChange;
}


//fnSetFlashOnNextRedraw - sets weather the button will display flash colours on next redraw.
void CColourButton::fnSetFlashOnNextRedraw(bool bolFlash)
{
	//set the option:
	m_bolFlashNextRedraw = bolFlash;
	
	//redraw it:
	RedrawWindow();
}


bool CColourButton::fnGetFlashState()
{
	return m_bolFlashNextRedraw;
}


//DrawItem - overide the draw function so we can draw our custom button.
void CColourButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//Redraw the button to give a coloured apperance.
	CDC dc;
	CRect rt;

	//Get device context object
	dc.Attach(lpDrawItemStruct->hDC);     

	//get the button rect:
	rt = lpDrawItemStruct->rcItem;        

	//set the custom back colour weathr it be flash or otherwise.
	if(m_bolFlashNextRedraw == true)
	{
		dc.FillSolidRect(rt, m_culFlash);
	}
	else
	{
		dc.FillSolidRect(rt, m_culBack);
	}

	//set the font colour:
	dc.SetTextColor(m_culFont);

	//get the state of the button:
	UINT state = lpDrawItemStruct->itemState;

	if ( (state & ODS_SELECTED) )            
	{
		// If it is pressed
		if(m_bolChangeColoursClick == true)
		{
			//change some colours round:
			dc.FillSolidRect(rt, m_culFont);
			dc.SetTextColor(m_culBack);
		}
		//draw the edge of the button:
		fnDrawButtonEdge(&dc, m_culEdge, rt);
	}
	else 
	{
		//draw the edge of the button:
		fnDrawButtonEdge(&dc, m_culEdge, rt);
	}

	// Get the caption which have been set and draw it:
	CString strTemp;
	GetWindowText(strTemp);
	dc.DrawText(strTemp, rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//free memory:
	dc.Detach();
}

//fnSetOrigonalBackColour - this will reset the colours of the button back
//to the origonal.
void CColourButton::fnSetOrigonalBackColour()
{
	fnSetBackColour(m_culOrigonalBack);
	RedrawWindow();
}

//fnSetOrigonalText - with no paramets sets back to origonal text
//with a string parameter sets the text.
void CColourButton::fnSetOrigonalText()
{
	SetWindowText(m_strOrginalButtonText);
}
void CColourButton::fnSetOrigonalText(CString strText)
{
	m_strOrginalButtonText = strText;
	SetWindowText(strText);

}


//protected member functions implimentation:

//fnDrawLine - draws a coloured line from starting points (sx, sy) to end points
//(ex, ey).
void CColourButton::fnDrawLine(CDC *pDC, long sx, long sy, long ex, long ey, COLORREF colour)
{
	//define pens:
	CPen newPen;
	CPen *oldPen;

	//create pen:
	newPen.CreatePen(PS_SOLID, 1, colour);
	//ensure we dont loose the old pen (with the old colour)
	oldPen = pDC->SelectObject(&newPen);

	//draw the line.
	pDC->MoveTo(sx, sy);
	pDC->LineTo(ex, ey);
	//set back to old pen:
	pDC->SelectObject(oldPen);

	//clean up ram:
	newPen.DeleteObject();	
}


//fnDrawButtonEdge - Will draw a colored edge of a button.
void CColourButton::fnDrawButtonEdge(CDC *pDC, COLOURREF colour, CRect rt)
{
	//across the top:
	fnDrawLine(pDC, rt.left, rt.top, rt.right, rt.top, colour);

	//down the left:
	fnDrawLine(pDC, rt.left, rt.top, rt.left, rt.bottom, colour);

	//along the bottom:
	fnDrawLine(pDC, rt.left, rt.bottom -1, rt.right, rt.bottom -1, colour);

	//up the right:
	fnDrawLine(pDC, rt.right -1, rt.bottom, rt.right - 1, rt.top, colour);
}
