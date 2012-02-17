// StopAllButton.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "StopAllButton.h"
#include "FusicCartsDlg.h"
#include "globals.h"

// CStopAllButton
//get access to global callback map:
extern T_mapCallback g_mapCartCallbacks;

IMPLEMENT_DYNAMIC(CStopAllButton, CButton)

CStopAllButton::CStopAllButton()
{
	dlg = NULL;
}

CStopAllButton::~CStopAllButton()
{
}


BEGIN_MESSAGE_MAP(CStopAllButton, CButton)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CStopAllButton message handlers

//public member function implimentation:
//fnSetCartsDialogPointer - set the dialog pointer so that access to
//the dialog vectors:
void CStopAllButton::fnSetCartsDialogPointer(void *pDlg)
{
	dlg = pDlg;
}

void CStopAllButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFusicCartsDlg* pDialog = (CFusicCartsDlg*)dlg;
	//ensure we have a valid dialog pointer:
	if(pDialog == NULL)
	{
		return;
	}

	//stop the cart buttons playing:
	for(mapCart::iterator i = pDialog->m_mapBtnCartTop.begin(); i != pDialog->m_mapBtnCartTop.end();
		i++)
	{
		i->second->fnStop(true);
	}
	for(mapCart::iterator i = pDialog->m_mapBtnCartBottom.begin(); i != pDialog->m_mapBtnCartBottom.end();
		i++)
	{
		i->second->fnStop(true);
	}
	
	//stop streams on other pages:
	for(std::vector<SNavigationStreams>::iterator i = pDialog->m_vecNavStreamsBottom.begin();
		i != pDialog->m_vecNavStreamsBottom.end(); i++)
	{
		//fade out the cart (500 ms):
		BASS_ChannelSlideAttribute(i->stream, BASS_ATTRIB_VOL, 0, 500);

		//remove old end sync:
		BASS_ChannelRemoveSync(i->stream, i->syncHandle);

		//create a new sync at the end of the slide:
		BASS_ChannelSetSync(i->stream, BASS_SYNC_SLIDE, 0, &CCallbackButton::fnSlideCallback,
			NULL);
	}
	
	for(std::vector<SNavigationStreams>::iterator i = pDialog->m_vecNavStreamsTop.begin();
		i != pDialog->m_vecNavStreamsTop.end(); i++)
	{
		//fade out the cart (500 ms):
		BASS_ChannelSlideAttribute(i->stream, BASS_ATTRIB_VOL, 0, 500);

		//remove old end sync:
		BASS_ChannelRemoveSync(i->stream, i->syncHandle);

		//create a new sync at the end of the slide:
		BASS_ChannelSetSync(i->stream, BASS_SYNC_SLIDE, 0, &CCallbackButton::fnSlideCallback,
			NULL);
	}

	//reset the nav buttons:
	for(mapNav::iterator i = pDialog->m_mapBtnNavTop.begin(); i != pDialog->m_mapBtnNavTop.end();
		i++)
	{
		i->second->fnResetButton();
	}

	for(mapNav::iterator i = pDialog->m_mapBtnNavBottom.begin(); i != pDialog->m_mapBtnNavBottom.end();
		i++)
	{
		i->second->fnResetButton();
	}

	//clear out vectors and callback maps:
	g_mapCartCallbacks.clear();
	pDialog->m_vecNavStreamsBottom.clear();
	pDialog->m_vecNavStreamsTop.clear();


	//do with windows callback:
	CColourButton::OnLButtonDown(nFlags, point);
}
