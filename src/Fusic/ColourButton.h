#pragma once

//just because i HATE american spellings:
typedef COLORREF COLOURREF;

// CColourButton

class CColourButton : public CButton
{
	DECLARE_DYNAMIC(CColourButton)

public:
	//public member functions:

	//fnSetBackColour - set the back colour of the button, overloaded for both ints and
	//COLOURREF's, bolSetAsOrigonal set equal to true will store the specified colour as an
	//origonal colour for the button.
	void fnSetBackColour(int intRed, int intGreen, int intBlue, bool bolSetAsOrigonal = false);
	void fnSetBackColour(COLOURREF culBack, bool bolSetAsOrigonal = false);

	//fnSetFontColour - set the font colour of the button, overloaded for both ints and
	//COLOURREF's.
	void fnSetFontColour(int intRed, int intGreen, int intBlue);
	void fnSetFontColour(COLORREF culFont);

	//fnSetEdgeColour - set the edge colour of the button, overloaded for both ints and
	//COLOURREF's.
	void fnSetEdgeColour(int intRed, int intGreen, int intBlue);
	void fnSetEdgeColour(COLOURREF culEdge);

	//fnSetFlasColour - set the flash colour of the button, overloaded for both ints and
	//COLOURREF's.
	void fnSetFlashColour(int intRed, int intGreen, int intBlue);
	void fnSetFlashColour(COLOURREF culFlash);
	

	//fnSetClickColourChange - Sets weather or not the button's back and font colurs swap on
	//a mouse click on the button.
	void fnSetClickColourChange(bool bolClickChange);

	//fnSetFlashOnNextRedraw - sets weather the button will display flash colours on next redraw.
	void fnSetFlashOnNextRedraw(bool bolFlash);

	//DrawItem - overide the draw function so we can draw our custom button.
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	//fnGetFlashState - this will return weather the button is displaying
	//flash colours or not.
	bool fnGetFlashState();
	
	//fnSetOrigonalBackColour - this will reset the colours of the button back
	//to the origonal.
	void fnSetOrigonalBackColour();

	//fnSetOrigonalText - with no paramets sets back to origonal text
	//with a string parameter sets the text.
	void fnSetOrigonalText();
	void fnSetOrigonalText(CString strText);

	//Constructor:
	CColourButton();

	//virual Destrcutor:
	virtual ~CColourButton();

protected:
	DECLARE_MESSAGE_MAP()


	//protected member functions (allow use by derived classes):

	//fnDrawLine - draws a coloured line from starting points (sx, sy) to end points
	//(ex, ey).
	void fnDrawLine(CDC *pDC, long sx, long sy, long ex, long ey, COLORREF colour);

	//fnDrawButtonEdge - Will draw a colored edge of a button.
	void fnDrawButtonEdge(CDC *pDC, COLOURREF colour, CRect rt);

	//protected member variables:
	bool m_bolFlashNextRedraw;			//weather the button will change to flash colours on next redraw.
	bool m_bolChangeColoursClick;		//weather to swap the font and bck colours when clicked.
	COLOURREF m_culBack;				//button back colour.
	COLOURREF m_culOrigonalBack;		//original back colour.
	COLOURREF m_culFont;				//button font colour.
	COLOURREF m_culEdge;				//button edge colour.
	COLOURREF m_culFlash;				//button flash colour.


private:

	//private member variables:
	CString m_strOrginalButtonText;		//the origonal Button Text.
};


