// MainBassControllerr.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "MainBassControllerr.h"

extern bool doLog;
extern playoutSettings g_settings;

// CMainBassControllerr

CMainBassControllerr::CMainBassControllerr(CFusicMainDlg* dlg)
{
	//default values:
	m_CurrentStream = 0;
	m_CurrentStreamLength = 0;
	m_CurrentSyncEnd = 0;
	m_CurrentIntroTime = -1;
	m_CurrentlyPlaying = false;
	m_CurrentSweeperLength = 0;
	m_aboutToPlaySweeper = false;
	m_paused = false;
	m_fadeing = false;

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
double CMainBassControllerr::fnStart(CString fileLocation, 
									 double fadeIn, double fadeOut)
{
	if(doLog)
	{
		CString temp1;
		CString temp2;
		temp1.Format("%f", fadeIn);
		temp2.Format("%f", fadeOut);
		fnLogOutMsg("CMainBassControllerr::fnStart. Beggining playback of file, " +
			fileLocation);
		fnLogOutMsg("Fade in point, " + temp1);
		fnLogOutMsg("Fade out point, " + temp2);
	}

	//ensure the correct dev is set:
	BASS_SetDevice(g_intMusicDev);

	//if we are paused, clear the current song:
	if(m_paused == TRUE)
	{
		fnReset();
	}

	//intro time = 0 because it doesn't matter if we're not playing a jingle.
	m_CurrentIntroTime = 0;
	//create the stream:
	m_CurrentStream = BASS_StreamCreateFile(false, fileLocation, 0,
		0,0);
	if(doLog)
	{
		CString temp;
		temp.Format("%d", m_CurrentStream);
		fnLogOutMsg("Created stream with result, " + temp);
	}
	if(m_CurrentStream == 0)
	{
		return 0;
	}
	m_currentMixOutPoint = fadeOut;
	
	//find the length o	f the stream:
	m_CurrentStreamLength = BASS_ChannelBytes2Seconds(m_CurrentStream,
		BASS_ChannelGetLength(m_CurrentStream, BASS_POS_BYTE));

	//ensure that the fade out point isn't after the end of the song
	//otherwise we will get stuck.
	if(fadeOut >= m_CurrentStreamLength)
	{
		fadeOut = 0;
	}

	if(fadeOut >= (m_CurrentStreamLength - 
		(g_settings.mix_out_fade_out_time/1000)))
	{
		fadeOut = 0;
	}

	if(doLog)
	{
		CString temp;
		temp.Format("%f", m_CurrentStreamLength);
	}
	//set the mix in point:
	BASS_ChannelSetPosition(m_CurrentStream, 
		BASS_ChannelSeconds2Bytes(m_CurrentStream, fadeIn),
		BASS_POS_BYTE);
	
	//begin playing:
	if(!BASS_ChannelPlay(m_CurrentStream, false))
	{
		return 0;
	}
	
	if(fadeOut != 0)
	{
		//setup a sync for the mixdown:
		BASS_ChannelSetSync(m_CurrentStream, BASS_SYNC_POS,
			BASS_ChannelSeconds2Bytes(m_CurrentStream, fadeOut), 
			CMainBassControllerr::fnCallbackMixOut, m_MainDlg);
	}
	else
	{
		BASS_ChannelSetSync(m_CurrentStream, BASS_SYNC_END,
		0, CMainBassControllerr::fnCallbackMixOut, m_MainDlg);
	}

	//we are now playing:
	m_CurrentlyPlaying = true;

	//done:
	if(doLog)
		fnLogOutMsg("done.");
	return m_CurrentStreamLength;
}

// fnStart - start a file playing:
double CMainBassControllerr::fnStartWithSweeper(CString fileLocation,
												CString sweeperLocation,
												double introTime, double fadeIn,
												double fadeOut)
{
	if(doLog)
	{
		fnLogOutMsg("CMainBassControllerr::fnStartWithSweeper. Beggining playback"
			" of song, " + fileLocation + " with sweeper " + sweeperLocation);
		CString temp;
		temp.Format("%f", introTime);
		fnLogOutMsg("Intro time, " + temp );
		temp.Format("%f", fadeIn);
		fnLogOutMsg("Mix in point, " + temp);
		temp.Format("%f", fadeOut);
		fnLogOutMsg("Mix out potint, " + temp);
	}
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
	if(doLog)
	{
		CString temp;
		temp.Format("%d", m_CurrentStream);
		fnLogOutMsg("Created stream with result, " + temp);
	}
	if(m_CurrentStream == 0)
	{
		return 0;
	}

	m_currentMixOutPoint = fadeOut;

	//create the sweeper stream:
	m_CurrentSweeperStream = BASS_StreamCreateFile(false, sweeperLocation, 0,
		0,0);
	if(doLog)
	{
		CString temp;
		temp.Format("%d", m_CurrentSweeperStream);
		fnLogOutMsg("Created sweeper stream with result, " + temp);
	}

	//find the length of the stream:
	m_CurrentSweeperLength = BASS_ChannelBytes2Seconds(m_CurrentSweeperStream,
		BASS_ChannelGetLength(m_CurrentSweeperStream, BASS_POS_BYTE));
	m_CurrentIntroTime = introTime;
	double callbackFadeDownPos = 
		introTime - m_CurrentSweeperLength - 
		(double)((g_settings.sweeper_fade_down_time + g_settings.sweeper_fade_up_time)/1000);
	double callbackPlaySweep = 
		introTime - m_CurrentSweeperLength - (double)(g_settings.sweeper_fade_up_time/1000);
	double callbackFadeUpPos = introTime - (double)(g_settings.sweeper_fade_up_time/1000);
	if(doLog)
	{
		CString temp;
		temp.Format("%f", callbackFadeDownPos);
		fnLogOutMsg("Fade Down Pos, " + temp);
		temp.Format("%f", callbackPlaySweep);
		fnLogOutMsg("Play Sweeper Pos, " + temp);
		temp.Format("%f", callbackFadeUpPos);
		fnLogOutMsg("Fade Up Pos, " + temp);
	}
	
	//find the length of the stream:
	m_CurrentStreamLength = BASS_ChannelBytes2Seconds(m_CurrentStream,
		BASS_ChannelGetLength(m_CurrentStream, BASS_POS_BYTE));
	if(doLog)
	{
		CString temp;
		temp.Format("%f", m_CurrentStreamLength);
		fnLogOutMsg("Found stream length, " + temp);
	}

	if(fadeOut >= m_CurrentStreamLength)
	{
		fadeOut = 0;
	}
	
	if(fadeOut >= (m_CurrentStreamLength - 
		(g_settings.mix_out_fade_out_time/1000)))
	{
		fadeOut = 0;
	}

	//move to mix in point:
	BASS_ChannelSetPosition(m_CurrentStream, 
		BASS_ChannelSeconds2Bytes(m_CurrentStream, fadeIn), BASS_POS_BYTE);
	
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

	//setup a sync for the mixdown:
	if(fadeOut != 0)
	{
	BASS_ChannelSetSync(m_CurrentStream, BASS_SYNC_POS,
		BASS_ChannelSeconds2Bytes(m_CurrentStream, fadeOut), 
		CMainBassControllerr::fnCallbackMixOut, m_MainDlg);
	}
	else
	{
		BASS_ChannelSetSync(m_CurrentStream, BASS_SYNC_END,
			0, CMainBassControllerr::fnCallbackMixOut, m_MainDlg);
	}
	
	
	//we are now playing:
	m_CurrentlyPlaying = true;

	//we are going to play a sweeper (lock the pause and stop buttons):
	m_aboutToPlaySweeper = true;

	//done:
	return m_CurrentStreamLength;
}

void CMainBassControllerr::fnReset()
{
	if(doLog)
		fnLogOutMsg("CMainBassControllerr::fnReset called.");
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
	if(doLog)
	{
		CString temp;
		fnLogOutMsg("CMainBassControllerr::fnFadeOut called.");
		temp.Format("%i", duration);
		fnLogOutMsg("Duration, " + temp);
		if(stop == true)
			fnLogOutMsg("Stop, true");
		else
			fnLogOutMsg("Stop, false");
	}
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
		m_CurrentIntroTime = -1;
		m_CurrentlyPlaying = false;
	}
}
CFusicMainDlg* CMainBassControllerr::fnGetMainDialog()
{
	return m_MainDlg;
}

void CMainBassControllerr::fnPause(double fadeDownTime, double fadeUpTime)
{
	//we can only pause if we're not playing a sweeper:
	if(!m_fadeing)
	{
	if(!m_paused)
	{
		//need to slide down:
		double pos = fnGetTimeElapsed();
		int iFadeDownTime = (int)(fadeDownTime*1000);
		m_fadeing = true;

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
	if(doLog)
		fnLogOutMsg("CMainBassControllerr::fnCallbackFadeOut called.");
	//stop the playback:
	BASS_ChannelStop(channel);

	//clean up the stream:
	BASS_StreamFree(channel);
}

void CALLBACK CMainBassControllerr::fnCallbackFadeDownSweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	float fadeDownpercentage = (float)(g_settings.sweeper_fade_down_volume/100.0);

	if(doLog)
		fnLogOutMsg("CMainBassControllerr::fnCallbackFadeDownSweeper called.");
	CMainBassControllerr* cls = (CMainBassControllerr*)user;
	BASS_ChannelSlideAttribute(cls->fnGetCurrentStream(), BASS_ATTRIB_VOL,
		fadeDownpercentage,
		g_settings.sweeper_fade_down_time );
}

void CALLBACK CMainBassControllerr::fnCallbackPlaySweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	if(doLog)
		fnLogOutMsg("CMainBassControllerr::fnCallbackPlaySweeper called.");
	CMainBassControllerr* cls = (CMainBassControllerr*)user;
	BASS_ChannelPlay(cls->fnGetCurrentSweeperStream(), false);
}

void CALLBACK CMainBassControllerr::fnCallbackPause(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	if(doLog)
		fnLogOutMsg("CMainBassControllerr::fnCallbackPause called.");
	CMainBassControllerr* cls = (CMainBassControllerr*)user;
	BASS_ChannelPause(cls->fnGetCurrentStream());
	cls->m_fadeing = false;

}


void CALLBACK CMainBassControllerr::fnCallbackFadeUpSweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	if(doLog)
		fnLogOutMsg("CMainBassControllerr::fnCallbackFadeUpSweeper called.");
	CMainBassControllerr* cls = (CMainBassControllerr*)user;
	BASS_ChannelStop(cls->fnGetCurrentSweeperStream());
	BASS_StreamFree(cls->fnGetCurrentSweeperStream());
	BASS_ChannelSlideAttribute(cls->fnGetCurrentStream(), BASS_ATTRIB_VOL,
		(float)1.0, g_settings.sweeper_fade_up_time);
	cls->fnSetSweeperLength(0);
	cls->m_aboutToPlaySweeper = false;
}

void CALLBACK CMainBassControllerr::fnCallbackMixOut(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	if(doLog)
		fnLogOutMsg("CMainBassControllerr::fnCallbackMixOut called.");

	CFusicMainDlg* dlg = (CFusicMainDlg*)user;
	dlg->m_bass->m_CurrentlyPlaying = false;
	dlg->fnSyncCallbackEnd(handle, channel, data, user);
}

bool CMainBassControllerr::isInIntroTime()
{
	if(m_CurrentlyPlaying == false)
	{
		return false;
	}
	if(m_CurrentIntroTime == -1)
	{
		return false;
	}
	if(m_CurrentIntroTime >= fnGetTimeElapsed())
	{
		return true;
	}
	return false;
}