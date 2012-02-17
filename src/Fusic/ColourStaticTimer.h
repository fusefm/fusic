#pragma once
#include "ColourStatic.h"
typedef COLORREF COLOURREF;
// CColourStaticTimer

class CColourStaticTimer : public CColourStatic
{
	DECLARE_DYNAMIC(CColourStaticTimer)

public:
	CColourStaticTimer();
	virtual ~CColourStaticTimer();

	//public funcitons:
	//fnSetCounterValue - will set the value of the count down timer
	//in mm:SS and also check for colours dependant on the time remaining:
	void fnSetCounterValue(double time);

protected:
	DECLARE_MESSAGE_MAP()

private:
	//private member vairables
	//colours used:
	double m_EarlyWarnTime;
	double m_WarnTime;
	double m_curTime;
	COLOURREF m_colCountdown;
	COLOURREF m_colCountdownEarlyWarn;
	COLOURREF m_colCountDownWarn;
	COLOURREF m_colIdle;
	CString oldString;
};