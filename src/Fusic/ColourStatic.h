#pragma once

//CColourStatic - Extends the MFC Static control to support different
//background and text colours.

//just because I HATE american spellings:
typedef COLORREF COLOURREF;

class CColourStatic : public CStatic
{
	DECLARE_DYNAMIC(CColourStatic)

public:
	CColourStatic();
	virtual ~CColourStatic();

	//public member functions:

	//fnSetBackgroundColour - will set the bg colour of the control
	//and also redraw it to apply changes:
	void fnSetBackgroundColour(COLORREF colBgColour);
	void fnSetBackgroundColour(int intRed, int intGreen, int intBlue);

	//fnSetTextColour - will set the text colour of the control
	//and also redraw it to apply changes:
	void fnSetTextColour(COLOURREF colTxtColour);
	void fnSetTextColour(int intRed, int intGreen, int intBlue);

protected:
	DECLARE_MESSAGE_MAP()

	//protected member variables:
	COLOURREF	m_colBgColour;		//the background colour of the static control.
	COLOURREF	m_colTxtColour;		//the text colour of the static control.
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	virtual void PreSubclassWindow();
};


