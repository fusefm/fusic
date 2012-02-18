#pragma once
#include "bass.h"

//forward decloration of the dialog class:
class CFusicMainDlg;

#include "FusicMainDlg.h"
extern int g_intMusicDev;

// CMainBassControllerr command target

class CMainBassControllerr : public CObject
{
public:
	//constructor:
	CMainBassControllerr(CFusicMainDlg* dlg);

	//destructor:
	virtual ~CMainBassControllerr();


	// fnStart - start a file playing and return
	//the length of the file:
	double fnStart(CString fileLocation, double fadeIn, double fadeOut);

	//fnReset - this will reset the bass class so when
	//the stream stops:
	void fnReset();

	// fnGetTimeRemaining - Returns the time remaining for the currently playing song:
	double fnGetTimeRemaining(void);

	// fnGetTimeElapsed - will retrun the number of seconds that the song has been playing:
	double fnGetTimeElapsed(void);

	// fnPlaying - return weather a stream is currently playing:
	BOOL fnPlaying();

	// fnFadeOut - fade out the currently playing stream over the duration
	// passed (in miliseconds):
	void fnFadeOut(int duration, bool stop);
	double fnStartWithSweeper(CString fileLocation, CString sweeperLocation,
		double introTime, double fadeIn, double fadeOut);
	double fnGetSweeperLength();
	HSTREAM fnGetCurrentStream();
	void fnSetSweeperLength(double d);
	void fnPause(double fadeDownTime, double fadeUpTime);
	void fnStop(double fadeDownTime);
	HSTREAM fnGetCurrentSweeperStream();
	BOOL m_aboutToPlaySweeper;
	CFusicMainDlg* fnGetMainDialog();
	bool isInIntroTime();
	bool m_currentlyInIntroTime();
	bool m_CurrentlyPlaying;
	bool m_fadeing;
	double m_currentMixOutPoint;
private:
	// m_CurrentStreamLength - The length of the current stream:
	double m_CurrentStreamLength;
	double m_CurrentSweeperLength;
	double m_CurrentIntroTime;

	//fnCallbackFadeOut - called when a fadeout has finished so we need
	//to close the stream to the file:
	static void CALLBACK fnCallbackFadeOut(HSYNC handle, DWORD channel, 
													DWORD data, void* user);
	static void CALLBACK fnCallbackFadeDownSweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user);
	static void CALLBACK fnCallbackPlaySweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user);
	static void CALLBACK fnCallbackFadeUpSweeper(HSYNC handle, DWORD channel, 
													DWORD data, void* user);
	static void CALLBACK fnCallbackPause(HSYNC handle, DWORD channel, 
													DWORD data, void* user);
	static void CALLBACK fnCallbackMixOut(HSYNC handle, DWORD channel, 
													DWORD data, void* user);
	// m_CurrentStream - The stream for the currently playing file:
	HSTREAM m_CurrentStream;
	
	HSTREAM m_CurrentSweeperStream;
	
	// m_CurrentSyncEnd - the sync handle for the end of the stream:
	HSYNC m_CurrentSyncEnd;

	HSYNC m_CurrentSyncFadeDownSweeper;
	HSYNC m_CurrentSyncFadeUpSweeper;
	HSYNC m_CurrentSyncFadeDownStartSweeper;

	BOOL m_paused;

	//m_MainDlgPointer - A pointer to the main dialog, used for callbacks:
	CFusicMainDlg* m_MainDlg;
};


