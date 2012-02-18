#pragma once

// CAutoDJ command target

#include "MainBassControllerr.h"		//needed for access to the Bass controller.
#include "FusicMainDlg.h"				//needed for access to the main dialog.
#include "MainPlaylistController.h"		//needed for access to playlist functions.

#include <mysql++.h>					//needed for MySQL functionality


//CAutoDJ - A class that will control the automatic cueing of songs, along with Jingles and Sweepers after
//a defined period of time.
class CAutoDJ : public CObject
{
public:

	//public member functions:

	//constructor - keeps a reference to an instance of the MainModel class
	//and the main dialog class:
	CAutoDJ(CFusicMainModel* data = NULL,
		CFusicMainDlg * mainDlg = NULL);

	//destructor:
	virtual ~CAutoDJ();
	
	//fnCueNextSong - will get a song ID from the database and cue
	//up that song on the playlist:
	void fnCueNextSong();
private:
	//private member functions:
	//fnGetRandomNumber - generate a random number between a range:
	int fnGetRandomNumber(int low, int high);

	//private members:
	CFusicMainModel* m_data;	//pointer to an instance of the data class.
	CFusicMainDlg* m_mainDlg;	//pointer to an instance of the main dlg class.

	int m_JingleCountdown;		//countdown for jingle insertion.
	int m_SweeperCountdown;		//countdown for sweeper insertion.
};


