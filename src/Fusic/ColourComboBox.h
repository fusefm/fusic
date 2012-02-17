#pragma once

typedef COLORREF COLOURREF;
// CColourComboBox

class CColourComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CColourComboBox)

public:
	CColourComboBox();
	virtual ~CColourComboBox();

	void fnSetBackColour(COLOURREF col);
	void fnSetBackColour(int red, int green, int blue);

	void fnSetTextColour(COLOURREF col);
	void fnSetTextColour(int red, int green, int blue);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_colorText;
    COLORREF m_colorBack;
};


