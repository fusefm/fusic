#pragma once
#include "ColourStatic.h"
#include "ColourComboBox.h"
#include "ColourButton.h"
#include "FusicMainModel.h"
#include "mysql++.h"

struct s_playlistState
{
	CString title;
	CString artist;
	CString intro;
	CString mixout;
	bool halt;
	bool jingle;
	int sweeperIndex;
	int fileID;
	bool empty;
};

//CMainPlaylistController - A class that controls each playlist element
//within the playlist chain. Controls cued files, sweepers and jingles.

class CMainPlaylistController
{
public:
	//constructor - creates references to objects that the playlist
	//controller needs to control:
	CMainPlaylistController(int playlistPosistion,
								CColourStatic* title,
								CColourStatic* artist,
								CColourButton* halt,
								CColourComboBox* sweep,
								CFusicMainModel* data,
								CColourStatic* intro,
								CColourStatic* mixout);
	//destructor:
	~CMainPlaylistController(void);
public:
	//public member functions:

	// fnIsEmpty - return weather the playlist element has a song currently assigned to it:
	bool fnIsEmpty(void);

	// fnCueSong - will cue a song in this playlist element:
	BOOL fnCueSong(int fileID,int sweepIndex = -1, bool jingle = false);

	//fnCueSongWithSweeper - cue a song with a sweeper assigned to it:
	BOOL fnCueSongWithSweeper(int fileID, int sweeperID);

	//fnSetHalt - sets the halt flag for the current element:
	void fnSetHalt();

	//fnNeedToHalt - returns weather the halt flag is set for this element:
	BOOL fnNeedToHalt();

	//fnResetHalt - resets the halt flag for this element:
	void fnResetHalt();

	//fnGetFileID - return the file ID for the current song that is being cued by this playlist element
	int fnGetFileID(void);

	//fnClear - clear the currently cued song (if any):
	void fnClear(void);

	//fnGetSweeperFileID - return the FileID for the sweeper that is selected:
	int fnGetSweeperFileID(void);

	//fnIsJingle - return weather the currently cued track is a jingle: 
	bool fnIsJingle();

	//fnGetState - will return a structure contatining state information for the current 
	//playlist element:
	s_playlistState fnGetState(bool erase);


	//fnSetState - will set the state of the current playlist element as defined by the
	//s_playlistState structure:
	void fnSetState(s_playlistState plState);

	int m_duration;
private:
	//private members:

	int m_myPosistion;				//the posistion of the playlist element (0 - next)
	int m_fileID;					//the fileID of the file in the database that the playlist points to.
	CColourStatic* m_ctlTitle;		//The title control of the playlist.
	CColourStatic* m_ctlArtist;		//The artist control of the playlist.
	CColourComboBox* m_ctlSweep;	//The combo Control of the playlist.
	CColourButton* m_ctlHalt;		//The halt button of the playlist.
	CFusicMainModel* m_data;		//a reference to the data class so we can preform queries on SQL for data.
	CColourStatic* m_ctlIntro;
	CColourStatic* m_ctlMixout;
	bool m_jingle;					//weather the currently cued track is a jingle.
	bool isEmpty;					// isEmpty - weather the playlist element has a song cued up in it:
	std::map<int, int> m_mapFileIDFromIdex;	//a map of sweeper file ID's to the index in the combo box.
	BOOL m_halt;					//the halt flag for this element.
};
