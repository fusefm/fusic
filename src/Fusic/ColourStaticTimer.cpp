// ColourStaticTimer.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include <math.h>
#include "ColourStaticTimer.h"


// CColourStaticTimer

IMPLEMENT_DYNAMIC(CColourStaticTimer, CStatic)

CColourStaticTimer::CColourStaticTimer()
{
	//default time of zero:
	m_curTime = 0;
	//set default text:
	//SetWindowText("00:00");

	//setdefaults:
	m_colBgColour = RGB(0,0,0);
	m_colCountdown = RGB(255,255,255);
	m_colCountdownEarlyWarn = RGB(255,255,0);
	m_colCountDownWarn = RGB(255,0,0);
	m_colIdle = RGB(120,120,120);

	m_WarnTime = 10;
	m_EarlyWarnTime = 30;
	oldString = "00:00";

}

CColourStaticTimer::~CColourStaticTimer()
{
}


BEGIN_MESSAGE_MAP(CColourStaticTimer, CStatic)
END_MESSAGE_MAP()



// CColourStaticTimer message handlers

//fnSetCounterValue - will set the value of the count down timer
//in mm:SS and also check for colours dependant on the time remaining:
void CColourStaticTimer::fnSetCounterValue(double time)
{
	//stop "-" signs:
	if(time < 0)
	{
		time = 0;
	}
	//set the time:
	m_curTime = time;

	//make sure that the right text colour is set:
	if(time > m_EarlyWarnTime)
	{
		fnSetTextColour(m_colCountdown);
	}
	else if ((time <= m_EarlyWarnTime) && (time >= m_WarnTime))
	{
		fnSetTextColour(m_colCountdownEarlyWarn);
	}
	else if ((time < m_WarnTime) && (time > 0))
	{
		fnSetTextColour(m_colCountDownWarn);
	}
	else
	{
		fnSetTextColour(m_colIdle);
	}

	//also set the text of the window:
	CString strTime;

	//get MS:
	int s = (int)ceil(time);    // convert to seconds - round to nearest second

	int Hours   = s / 3600 % 24;
	int Minutes = s % 3600 / 60;
	//also add hours to mins:
	Minutes += (Hours * 60);

	int Seconds = s % 60;

	strTime.Format("%02d:%02d",Minutes, Seconds);

	//only redraw if we have to (stops flickering):
	if(oldString != strTime)
	{
		oldString = strTime;

		SetWindowText(strTime);

		//also update the window:
		UpdateWindow();
	}
}


