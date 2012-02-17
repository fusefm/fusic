// FusicProgress.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicProgress.h"


// CFusicProgress

IMPLEMENT_DYNAMIC(CFusicProgress, CProgressCtrl)

CFusicProgress::CFusicProgress()
{
	counter = 0;
	m_ActuallyEraseBg = false;
}
void CFusicProgress::fnSetTotalTime(double time)
{
	//for more resolution:
	time = time * 100;

	//Set the progress bar range:
	SetRange32(0, (int)time);

	//also set the local variable:
	m_totalTime = time;

	//also erase the bg to show this:
	m_ActuallyEraseBg = true;
}

void CFusicProgress::fnSetTimeRemain(double time, bool colourise)
{
	//for more resolution:
	time = time * 100;
	double timeremain = time;
	double timeLeft = m_totalTime - time;

	if(timeremain > 3000)
	{
		SetBarColor(RGB(0,255,0));
	}
	else if (timeremain <= 3000 && timeremain >= 1000)
	{
		SetBarColor(RGB(255,255,0));
	}
	else
	{
		//do some nice flashing:
		counter += 1;
		if(counter < 20)
		{
			SetBarColor(RGB(255,0,0));
		}
		else if (counter >= 20)
		{
			SetBarColor(RGB(0,0,0));
			if(counter == 30)
			{
				counter = 0;
			}
		}
	}
	if(colourise == false)
	{
		SetBarColor(RGB(0,255,0));
	}
	SetPos((int)timeLeft);
}

void CFusicProgress::fnReset()
{
	SetBarColor(RGB(0,0,0));
}

CFusicProgress::~CFusicProgress()
{
}


BEGIN_MESSAGE_MAP(CFusicProgress, CProgressCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CFusicProgress::OnEraseBkgnd( CDC* pDC ) 
{
	if(m_ActuallyEraseBg == true)
	{
		m_ActuallyEraseBg = false;
		return CProgressCtrl::OnEraseBkgnd(pDC);
	}
	return true;
}
// CFusicProgress message handlers


