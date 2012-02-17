#include "StdAfx.h"
#include "MainPlaylistController.h"

CMainPlaylistController::CMainPlaylistController(int playlistPosistion,
												 CColourStatic* title,
												 CColourStatic* artist,
												 CColourComboBox* sweep,
												 CFusicMainModel* data)
{
	//set the posistion:
	m_myPosistion = playlistPosistion;
	//we will start with everything empty:
	m_fileID = -1;
	
	//set all controls to the empty state:
	title->fnSetTextColour(120,120,120);
	title->SetWindowTextA("Empty");
	artist->fnSetTextColour(120,120,120);
	artist->SetWindowTextA("Empty");

	//store the control addresses:
	m_ctlArtist = artist;
	m_ctlTitle = title;
	m_ctlSweep = sweep;
	m_halt = false;

	isEmpty = true;

	//setup the data source:
	m_data = data;

	//ensure that there are no sweepers in the combobox:
	sweep->Clear();
}

CMainPlaylistController::~CMainPlaylistController(void)
{
}

// fnIsEmpty - return weather the playlist element has a song 
// currently assigned to it:
bool CMainPlaylistController::fnIsEmpty(void)
{
	return isEmpty;
}

// fnCueSong - will cue a song in this playlist element:
BOOL CMainPlaylistController::fnCueSong(int fileID, int sweepIndex)
{
	CString title;
	CString artist;
	//firstly we need to get all file info for that file:
	mysqlpp::StoreQueryResult res = m_data->fnGetSongInfoForFileID(fileID);

	//also find sweepers that we can use:
	mysqlpp::StoreQueryResult resSweepers = m_data->fnGetSweepersForSongID(fileID);

	if(res.size() != 1)
	{
		return false;
	}
	//get info
	title = (CString)res[0]["File_Title"];
	artist = (CString)res[0]["File_Artist"];

	//setup the conrtols:
	m_ctlTitle->SetWindowText(title);
	m_ctlTitle->fnSetTextColour(255,255,255);
	m_ctlTitle->RedrawWindow();

	m_ctlArtist->SetWindowTextA(artist);
	m_ctlArtist->fnSetTextColour(255,255,255);
	m_ctlArtist->RedrawWindow();

	for(size_t i = 0; i < resSweepers.size(); i ++)
	{
		int nIndex = 0;
		nIndex = m_ctlSweep->AddString((CString)resSweepers[i]["File_Title"]);
		m_mapFileIDFromIdex[nIndex] = (int)resSweepers[i]["File_ID"];
	}
	if(sweepIndex != -1)
	{

		m_ctlSweep->SetCurSel(sweepIndex);
	}
	//also store the file ID:
	m_fileID = fileID;

	isEmpty = false;

	return 0;
}

// fnGetFileID - return the file ID for the current song that is being cued by this playlist element
int CMainPlaylistController::fnGetFileID(void)
{
	return m_fileID;
}

int CMainPlaylistController::fnGetSweeperFileID(void)
{
	if(m_ctlSweep->GetCurSel() == LB_ERR)
	{
		return -1;
	}

	return m_mapFileIDFromIdex[m_ctlSweep->GetCurSel()];
}

void CMainPlaylistController::fnSetHalt()
{
	m_halt = true;
}

BOOL CMainPlaylistController::fnNeedToHalt()
{
	return m_halt;
}

void CMainPlaylistController::fnResetHalt()
{
	m_halt = false;
}

int CMainPlaylistController::fnGetSweepIndex(void)
{
	if(m_ctlSweep->GetCurSel() == LB_ERR)
	{
		return -1;
	}

	return m_ctlSweep->GetCurSel();
}

// fnClear - clear the currently cued song (if any):
void CMainPlaylistController::fnClear(void)
{
	m_ctlArtist->fnSetTextColour(120,120,120);
	m_ctlArtist->SetWindowTextA("Empty");
	m_ctlTitle->fnSetTextColour(120,120,120);
	m_ctlTitle->SetWindowTextA("Empty");
	m_ctlSweep->ResetContent();
	m_halt = false;
	isEmpty = true;

	m_fileID = -1;
}
