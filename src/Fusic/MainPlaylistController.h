#pragma once
#include "ColourStatic.h"
#include "ColourComboBox.h"
#include "FusicMainModel.h"
#include "mysql++.h"

class CMainPlaylistController
{
public:
	CMainPlaylistController(int playlistPosistion,
								CColourStatic* title,
								CColourStatic* artist,
								CColourComboBox* sweep,
								CFusicMainModel* data);

	~CMainPlaylistController(void);
private:

	//private members:
	int m_myPosistion;	//the posistion of the playlist element (0 - next)
	int m_fileID;		//the fileID of the file in the database that the playlist points to.
	CColourStatic* m_ctlTitle;	//The title control of the playlist.
	CColourStatic* m_ctlArtist;	//The artist control of the playlist.
	CColourComboBox* m_ctlSweep;	//The combo Control of the playlist.
	// isEmpty - weather the playlist element has a song cued up in it:
	bool isEmpty;
public:
	// fnIsEmpty - return weather the playlist element has a song currently assigned to it:
	bool fnIsEmpty(void);
	// fnCueSong - will cue a song in this playlist element:
	BOOL fnCueSong(int fileID,int sweepIndex = -1);
	void fnSetHalt();
	BOOL fnNeedToHalt();
	void fnResetHalt();
private:
	// m_data - a reference to the data class so we can preform queries on SQL for data
	CFusicMainModel* m_data;
	std::map<int, int> m_mapFileIDFromIdex;
	BOOL m_halt;
public:
	// fnGetFileID - return the file ID for the current song that is being cued by this playlist element
	int fnGetFileID(void);
	// fnClear - clear the currently cued song (if any):
	void fnClear(void);
	int fnGetSweepIndex(void);
	int fnGetSweeperFileID(void);
	
};
