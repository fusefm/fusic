// BackgroundController.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "BackgroundController.h"


// CBackgroundController

IMPLEMENT_DYNAMIC(CBackgroundController, CStatic)

CBackgroundController::CBackgroundController()
{
	m_transparent = false;
	m_bgColour = RGB(0,0,0);
}

CBackgroundController::~CBackgroundController()
{
}


BEGIN_MESSAGE_MAP(CBackgroundController, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CBackgroundController::fnSetBgColour(COLOURREF col)
{
	m_bgColour = col;
	RedrawWindow();
}

// CBackgroundController message handlers



void CBackgroundController::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//all we want to do is just set it to the BG colour:
	if(m_transparent == true)
	{
		dc.SetBkMode(TRANSPARENT);
	}
	else
	{
		CRect rt;
		GetClientRect(&rt);
		dc.FillSolidRect(rt, m_bgColour);
	}
}
