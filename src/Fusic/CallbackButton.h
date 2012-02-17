#pragma once

#include "ColourButton.h"		//needed for Colour Button subclass.
#include "bass.h"				//needed for bass library functions.
//#include "globals.h"			//needed for global variable access.
// CCallbackButton

class CCallbackButton : public CColourButton
{
	DECLARE_DYNAMIC(CCallbackButton)

public:
	CCallbackButton();
	virtual ~CCallbackButton();

	//public member functions:

	// fnSlideCallback - function is called when the jingle stops
	// sliding (fade out to volume level 0):
	static void CALLBACK fnSlideCallback(HSYNC handle, DWORD channel, DWORD data, void* user);

	// fnEndCallback - function is called when the jingle stops playing:
 	static void CALLBACK fnEndCallback(HSYNC handle, DWORD channel, DWORD data, void* user);

	//fnDoButtonCallback - this allows overide implimentation to reset buttons after a jingle
	//has sopted playing that has been assigned to that button:
	virtual void fnDoButtonCallack(HSTREAM channel)
	{
	}

protected:
	DECLARE_MESSAGE_MAP()
};


