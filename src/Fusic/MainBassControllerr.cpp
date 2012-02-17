// MainBassControllerr.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "MainBassControllerr.h"


// CMainBassControllerr

CMainBassControllerr::CMainBassControllerr(CFusicMainDlg* dlg)
{
	//default values:
	m_CurrentStream = 0;
	m_CurrentStreamLength = 0;
	m_CurrentSyncEnd = 0;
	m_CurrentlyPlaying = false;
	m_CurrentSweeperLength = 0;
	m_aboutToPlaySweeper = false;
	m_paused = false;

	//initaliase bass:
	BASS_Init(g_intMusicDev, 44100, 0, 0, NULL);
	m_MainDlg = dlg;
}

CMainBassControllerr::~CMainBassControllerr()
{
	if(fnPlaying())
	{
		BASS_ChannelStop(m_CurrentStream);
		BASS_StreamFree(m_CurrentStream);
	}

	//BASS_Free();
}


// CMainBassControllerr member functions

// fnStart - start a file playing:
double CMainBassControllerr::fnStart(CString fileLocation)
{

	//ensure the correct dev is set:
	BASS_SetDevice(g_intMusicDev);

	//if we are paused, clear the current song:
	if(m_paused == TRUE)
	{
		fnReset();
	}

	//create the stream:
	m_CurrentStream = BASS_StreamCreateFile(false, fileLocation, 0,
		0,0);
	if(m_CurrentStream == 0)
	{
		return 0;
	}
	
	//find the length of the stream:
	m_CurrentStreamLength = BASS_ChannelBytes2Seconds(m_CurrentStream,
		BASS_ChannelGetLength(m_CurrentStream, BASS_POS_BYTE));
	
	//begin playing:
	if(!BASS_ChannelPlay(m_CurrentStream, false))
	{
		return 0;
	}
	
	//setup a sync for the end:
	m_CurrentSyncEnd = BASS_ChannelSetSync(m_CurrentStream,
		BASS_SYNC_END, 0, CFusicMainDlg::fnSyncCallbackEnd, m_MainDlg);
	
	//we are now playing:
	m_CurrentlyPlaying = true;

	//done:
	return m_CurrentStreamLength;
}

// fnStart - start a file playing:
double CMainBassControllerr::fnStartWithSweeper(CString fileLocation,
												CString sweeperLocation,
												double introTime)
{
	//ensure the correct dev is set:
	BASS_SetDevice(g_intMusicDev);

	//if we are paused, clear the current song:
	if(m_paused == TRUE)
	{
		fnReset();
	}

	//create the stream:
	m_CurrentStream = BASS_StreamCreateFile(false, fileLocation, 0,
		0,0);
	if(m_CurrentStream == 0)
	{
		return 0;
	}

	//create the sweeper stream:
	m_CurrentSweeperStream = BASS_StreamCreateFile(false, sweeperLocation, 0,
		0,0);

	//find the length of the stream:
	m_CurrentSweeperLength = BASS_ChannelBytes2Seconds(m_CurrentSweeperStream,
		BASS_ChannelGetLength(m_CurrentSweeperStream, BASS_POS_BYTE));
	
	double callbackFadeDownPos = introTime - m_CurrentSweeperLength - 0.6;
	double callbackPlaySweep = introTime - m_CurrentSweeperLength - 0.3;
	double callbackFadeUpPos = introTime - 0.3;
	
	//find the length of the stream:
	m_CurrentStreamLength = BASS_ChannelBytes2Seconds(m_CurrentStream,
		BASS_ChannelGetLength(m_CurrentStream, BASS_POS_BYTE));
	
	//begin playing:
	if(!BASS_ChannelPlay(m_CurrentStream, false))
	{
		return 0;
	}

	//setup a sync for the fade down:
	m_CurrentSyncFadeDownSweeper = BASS_ChannelSetSync(m_CurrentStream,
		BASS_SYNC_POS, BASS_ChannelSeconds2Bytes(m_CurrentStream, 
		callbackFadeDownPos),CMainBassControllerr::fnCallbackFadeDownSweeper, this);

	BASS_ChannelSetSync(m_CurrentStream,
		BASS_SYNC_POS, BASS_ChannelSeconds2Bytes(m_CurrentStream, 
		callbackPlaySweep),CMainBassControllerr::fnCallbackPlaySweeper, this);
	
	BASS_ChannelSetSync(m_CurrentStream,
		BASS_SYNC_POS, BASS_ChannelSeconds2Bytes(m_CurrentStream, 
		callbackFadeUpPos),CMainBassControllerr::fnCallbackFadeUpSweeper, this);

	
	//setup a sync for the end:
	m_CurrentSyncEnd = BASS_ChannelSetSync(m_CurrentStream,
		BASS_SYNC_END, 0, CFusicMainDlg::fnSyncCallbackEnd, m_MainDlg);
	
	//we are now playing:
	m_CurrentlyPlaying = true;

	//we are going to play a sweeper (lock the pause and stop buttons):
	m_aboutToPlaySweeper = true;

	//done:
	return m_CurrentStreamLength;
}

void CMainBassControllerr::fnReset()
{
	//free the current stream:
	BASS_StreamFree(m_CurrentStream);
	
	//reset the current current stream:
	m_CurrentStream = 0;

	//we arn't playing:
	m_CurrentlyPlaying = false;
}

BOOL CMainBassControllerr::fnPlaying()
{
	return m_CurrentlyPlaying;
}

// fnGetTimeRemaining - Returns the time remaining for the currently playing song:
double CMainBassControllerr::fnGetTimeRemaining(void)
{
	if(m_CurrentStream == 0)
	{
		//if nothing is playing return 0:
		return 0;
	}
	return m_CurrentStreamLength - BASS_ChannelBytes2Seconds(m_CurrentStream,
		BASS_ChannelGetPosition(m_CurrentStream, BASS_POS_BYTE));
}

double CMainBassControllerr::fnGetTimeElapsed(void)
{
	if(m_CurrentStream == 0)
	{
		//if nothing is playing return 0:
		return 0;
	}

	return  BASS_ChannelBytes2Seconds(m_CurrentStream,
		BASS_ChannelGetPosition(m_CurrentStream, BASS_POS_BYTE));
}

// fnFadeOut - fade out the currently playing stream over the duration
// passed (in miliseconds):
void CMainBassControllerr::fnFadeOut(int duration, bool stop)
{
	//do some sliding:
	BASS_ChannelSlideAttribute(m_CurrentStream, BASS_ATTRIB_VOL,
		0, duration);

	//remove the unneccerry sync:
	BASS_ChannelRemoveSync(m_CurrentStream, m_CurrentSyncEnd);

	//set the callback back:
	HSYNC peer = BASS_ChannelSetSync(m_CurrentStream, BASS_SYNC_SLIDE,
		0, &fnCallbackFadeOut,0);

	if(stop == true)
	{
		m_CurrentlyPlaying = false;
	}
}

void CMainBassControllerr::fnPause(double fadeDownTime, double fadeUpTime)
{
	//we can only pause if we're not playing a sweeper:
	if(!m_aboutToPlaySweeper)
	{
		if(!m_paused)
		{
			//need to slide down:
			double pos = fnGetTimeElapsed();
			int iFadeDownTime = (int)(fadeDownTime*1000);

			pos += fadeDownTime;

			BASS_ChannelSlideAttribute(m_CurrentStream, BASS_ATTRIB_VOL,
				0, iFadeDownTime);
			BASS_ChannelSetSync(m_CurrentStream, BASS_SYNC_POS,
				BASS_ChannelSeconds2Bytes(
				fnGetCurrentStream(), pos), &CMainBassControllerr::fnCallbackPause, this);
			m_paused = true;
		}
		else
		{
			int iFadeUpTime = (int)(fadeDownTime*1000);
			BASS_ChannelSlideAttribute(m_CurrentStream, BASS_ATTRIB_VOL,
				1, iFadeUpTime);
			BASS_ChannelPlay(fnGetCurrentStream(), false);
			m_paused = false;
		}
	}
}

void CMainBassControllerr::fnStop(double fadeDownTime)
{
	fnFadeOut((int)(fadeDownTime*1000), true);
}
HSTREAM CMainBassControllerr::fnGetCurrentStream()
{
	return m_CurrentStream;
}
HSTREAM CMainBassControllerr::fnGetCurrentSweeperStream()
{
	return m_CurrentSweeperStream;
}

void CMainBassControllerr::fnSetSweeperLength(double d)
{
	m_CurrentSweeperLength = d;
}

double CMainBassControllerr::fnGetSweeperLength()
{
	return m_CurrentSweeperLength;
}

//fnCallbackFadeOut - called when a fadeout has finished so we need
//to close the stream to the file:
void CALLBACK CMainBassControllerr::fnCallbackFadeOut(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	//stop the playback:
	BASS_ChannelStop(channel);

	//clean up the stream:
	BASS_StreamFree(channel);
}

void CALLBACK CMainBassControllerr::fnCallbackFadeDownSweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	CMainBassControllerr* cls = (CMainBassControllerr*)user;
	BASS_ChannelSlideAttribute(cls->fnGetCurrentStream(), BASS_ATTRIB_VOL,
		(float)0.4, 300);
}

void CALLBACK CMainBassControllerr::fnCallbackPlaySweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	CMainBassControllerr* cls = (CMainBassControllerr*)user;
	BASS_ChannelPlay(cls->fnGetCurrentSweeperStream(), false);
}

void CALLBACK CMainBassControllerr::fnCallbackPause(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	CMainBassControllerr* cls = (CMainBassControllerr*)user;
	BASS_ChannelPause(cls->fnGetCurrentStream());

}


void CALLBACK CMainBassControllerr::fnCallbackFadeUpSweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	CMainBassControllerr* cls = (CMainBassControllerr*)user;
	BASS_ChannelStop(cls->fnGetCurrentSweeperStream());
	BASS_StreamFree(cls->fnGetCurrentSweeperStream());
	BASS_ChannelSlideAttribute(cls->fnGetCurrentStream(), BASS_ATTRIB_VOL,
		(float)1.0, 300);
	cls->fnSetSweeperLength(0);
	cls->m_aboutToPlaySweeper = false;
}