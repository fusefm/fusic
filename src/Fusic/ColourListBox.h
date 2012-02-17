#pragma once

typedef COLORREF COLOURREF;
// CColourListBox

class CColourListBox : public CListBox
{
	DECLARE_DYNAMIC(CColourListBox)

public:
	CColourListBox();
	virtual ~CColourListBox();
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);

	void fnSetBackColour(COLOURREF col);
	void fnSetBackColour(int red, int green, int blue);

	void fnSetTextColour(COLOURREF col);
	void fnSetTextColour(int red, int green, int blue);

	void fnSetHighlightColour(COLOURREF col);
	void fnSetHighlightColour(int red, int green, int blue);

	void fnSetAltColour1(COLOURREF col);
	void fnSetAltColour1(int red, int green, int blue);

	void fnSetAltColour2(COLOURREF col);
	void fnSetAltColour2(int red, int green, int blue);
private:
	COLOURREF m_bgColour;
	COLOURREF m_TextColour;
	COLOURREF m_highlightColour;
	COLOURREF m_altColour1;
	COLOURREF m_altColour2;
	CBrush brush;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


