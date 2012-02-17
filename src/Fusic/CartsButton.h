#pragma once
#include "CallbackButton.h"
#include "bass.h"				//needed for bass functions.
#include "globals.h"			//needed for callback map.

//timer IDs:
#define IDT_CARTFLASH WM_USER + 200	//used when the cart needs to flash.
#define IDT_CARTTIME WM_USER + 201	//used when the cart needs to update time remaining.

// CCartsButton

class CCartsButton : public CCallbackButton
{
	DECLARE_DYNAMIC(CCartsButton)

public:
	CCartsButton();
	virtual ~CCartsButton();
	
	//public member functions:
	
	//fnSetupCartButton - this will setup the button for the given data.
	void fnSetupCartButton(CString strFile, COLOURREF culBackColour,
		CString strTitle, double dblDuration, bool blRefreshOnClick = false,
		bool blFuseCart = false);
	
	//fnDisableButton - this will set the button as disabled setting the appopriate
	//back colour and resets all member variables:
	void fnDisableButton();
	
	//onBnClicked - function called when the button is clicked, this will allow us to
	//play the jingle and stop the jingle if there is one playing.
	afx_msg void OnBnClicked();

	//OnTimer - this will hook all timer events, allowing us to setup timing events
	//for countdown and flash changes:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//fnDoButtonCallback - called when the button is called back.
	void fnDoButtonCallack(HSTREAM channel);

	//fnStop - this will stop a button playing and put the button back into it's
	//origonal state.
	void fnStop(bool bolStopPlay = false);

	//fnSetDefaultButton - this will set up all defaults for the button.
	//shuold only be called by the parent dialog on InitDialog.
	void fnSetDefaultButton();

	//fnPageMoveOff - this is called when a page change is happening and the button
	//needs to report back the cart that is playing (if any) and reset for the next
	//button:
	HSTREAM fnPageMoveOff(HSYNC &syncHandle);

	//DrawItem - Overide the default draw function from CColourButton
	//to allow us to write out the cart tile and duration at the 
	//top and bottom resepectivly.
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//fnPageMoveOn - this is called when a page change is happening and the button
	//needs to be set as the cart it is playing for the page has not yet
	//finished playing:
	void fnPageMoveOn(HSTREAM stream, HSYNC syncHandle);

	//fnGetCurrentStream - Return the current stream that the button is playing:
	HSTREAM fnGetCurrentStream();

	HSYNC fnGetCurrentSync();

	//OnEraseBkgnd - we overide this in order to stop redrawwindow() from redrawing the
	//button if it hasn't changed. This will help stop button flickering.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP()
private:

	//private member variables:

	CString m_strFilePath;		//the path of the cart to be plaied.
	CString m_strPrintDuration;	//the duration that is drawn at the bottom of the button.
	CString m_strODuration;		//the origonal duration that is shown after a jingles has been playing.
	HSTREAM m_streamCart;		//the stream handle of the cart that is playing.
	double m_dblStreamLength;	//the length of the stream in seconds.
	HSYNC m_syncHandle;			//the handle of the sync callback.
	bool m_bolFuseCart;			//weather the button is a fuse cart (if not then it is a user one).
	bool m_bolState;			//the state of the button (weather a jingle is assigned to that button) (true - enabled, false - disabled).
	bool m_bolRefreshOnClick;	//weather the file location should be refreshed every time the jingle is pressed (used for the news).
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


