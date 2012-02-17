// CallbackButton.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "CallbackButton.h"
#include "globals.h"			//needed for global variable access.

// CCallbackButton

IMPLEMENT_DYNAMIC(CCallbackButton, CButton)

CCallbackButton::CCallbackButton()
{

}

CCallbackButton::~CCallbackButton()
{
}


BEGIN_MESSAGE_MAP(CCallbackButton, CButton)
END_MESSAGE_MAP()


//need access to the callback map:
extern T_mapCallback g_mapCartCallbacks;

//puiblic member funtions:

// fnSlideCallback - function is called when the jingle stops
// sliding (fade out to volume level 0):
void CALLBACK CCallbackButton::fnSlideCallback(HSYNC handle, DWORD channel, 
														DWORD data, void* user)
{
	//ensure that the jingle has stopped playing:
	BASS_ChannelStop(channel);

	//free up all stream resources:
	BASS_StreamFree(channel);
}	

// fnEndCallback - function is called when the jingle stops playing:
void CALLBACK CCallbackButton::fnEndCallback(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	//get the button that is currently handleing the jingle:
	CCallbackButton* btnCallbackTarget = g_mapCartCallbacks[handle];

	//ensure that the jingle has stopped playing:
	BASS_ChannelStop(channel);

	//free up all stream resources:
	BASS_StreamFree(channel);

	//remove the callback from the map:
	for(T_mapCallback::iterator i = g_mapCartCallbacks.begin();
		i != g_mapCartCallbacks.end(); i++)
	{
		if(i->first == handle)
		{
			g_mapCartCallbacks.erase(i);
			break;
		}
	}


	//call the button specific overide:
	btnCallbackTarget->fnDoButtonCallack(channel);
}