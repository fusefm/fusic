#pragma once

typedef COLORREF COLOURREF;

// CBackgroundController

class CBackgroundController : public CStatic
{
	DECLARE_DYNAMIC(CBackgroundController)

public:
	CBackgroundController();
	virtual ~CBackgroundController();
	void fnSetBgColour(COLOURREF col);
protected:
	DECLARE_MESSAGE_MAP()
private:
	bool m_transparent;
	COLOURREF m_bgColour;
public:
	afx_msg void OnPaint();
};


