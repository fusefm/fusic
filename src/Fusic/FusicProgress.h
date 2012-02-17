#pragma once


// CFusicProgress

class CFusicProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CFusicProgress)

public:
	CFusicProgress();
	virtual ~CFusicProgress();
	void fnSetTotalTime(double time);
	void fnSetTimeRemain(double time, bool colourise = true);
	void fnReset();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
protected:
	DECLARE_MESSAGE_MAP()
private:
	double m_totalTime;
	int counter;
	int m_nPrevPos;
	bool m_ActuallyEraseBg;
};


