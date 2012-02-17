#pragma once

#include "CallbackButton.h"	//needed for CColourButton  subclass
//#include "FusicCartsDlg.h"
// CNavigationButton

#define IDT_FLASNAV WM_USER + 123

class CNavigationButton : public CCallbackButton
{
	DECLARE_DYNAMIC(CNavigationButton)

public:
	CNavigationButton();
	virtual ~CNavigationButton();

	//public member functions:
	
	//fnSetupNavButtonForPage - this will set up the nav button for a specific page:
	void fnSetupNavButtonForPage(int intPageNumber, int intNavButtonPosistion,
		void* dlg);

	//OnBnClicked - called when the nav button is clicked, here we start
	//to set up all the buttons for the next page as well as moving callbacks
	//and increaseing the reference count on the old nav button.
	afx_msg void OnBnClicked();

	//resets the reference counter and stops the button from flashing if it is doing so
	//used when the stop all button is pressed and the jingles have been stopped withiout
	//notifying the navbutton:
	void fnResetButton();

	//fnIncrasePlayCount - increase the number of carts that are playing on that
	//page.
	void fnIncreasePlayCount();

	//fnDecreasePlayCount - Decrease the number of carts that are playing on that
	//page.
	void fnDecreasePlayCount();

	//fnDoButtonCallback - impliment the callback for Navigation buttons:
	void fnDoButtonCallack(HSTREAM channel);

	//OnTimer - Hook into the timer function and make the button flash:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	DECLARE_MESSAGE_MAP()

private:
	//private member variables:
	int m_intNavPage;			//the page number that nav button points to.
	int m_intNavPosistion;		//the posistion of the nav button (fuse or user).
	int m_intCartsOnPagePlaying;//the number of carts on the page that are currenly playing.
	void* m_pCartsDlg;			//a pointer to the carts dialog class.
};


