#include "StdAfx.h"
#include "MainPlaylistController.h"

//CMainPlaylistController - A class that controls each playlist element
//within the playlist chain. Controls cued files, sweepers and jingles.

extern int g_intShowID;

//constructor - creates references to objects that the playlist
//controller needs to control:
CMainPlaylistController::CMainPlaylistController(int playlistPosistion,
												 CColourStatic* title,
												 CColourStatic* artist,
												 CColourButton* halt,
												 CColourComboBox* sweep,
												 CFusicMainModel* data,
												 CColourStatic* intro,
												 CColourStatic* mixout)
{
	//set the posistion of the playlist element:
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
	m_ctlIntro = intro;
	m_ctlMixout = mixout;
	m_ctlHalt = halt;
	m_halt = false;
	m_jingle = false;

	m_ctlIntro->SetWindowTextA("00:00");
	m_ctlIntro->fnSetTextColour(120,120,120);
	m_ctlMixout->SetWindowTextA("00:00");
	m_ctlMixout->fnSetTextColour(120,120,120);

	isEmpty = true;

	//setup the data source:
	m_data = data;

	//ensure that there are no sweepers in the combobox:
	sweep->ResetContent();
}

//destructor:
CMainPlaylistController::~CMainPlaylistController(void)
{
}

//public member implimentation:

// fnIsEmpty - return weather the playlist element has a song 
// currently assigned to it:
bool CMainPlaylistController::fnIsEmpty(void)
{
	return isEmpty;
}

//fnCueSong - will cue a song in this playlist element:
BOOL CMainPlaylistController::fnCueSong(int fileID, int sweepIndex, bool jingle)
{
	CString title;
	CString artist;
	CString strIntro;
	CString strmixOut;
	double intro;
	double mixOut;

	//get all file info for the fileID:
	mysqlpp::StoreQueryResult res = m_data->fnGetSongInfoForFileID(fileID);
	
	//find sweepers that can be used for this file:
	mysqlpp::StoreQueryResult resSweepers = m_data->fnGetSweepersForSongID(fileID, g_intShowID);

	//ensure a valid file info results set has been returned:
	if(res.size() != 1)
	{
		return false;
	}

	//get the file title if not null:
	if(res[0]["File_Title"] != mysqlpp::null)
	{
		title = (CString)res[0]["File_Title"];
	}

	//get file artist if not null:
	if(res[0]["File_Artist"] != mysqlpp::null)
	{
		artist = (CString)res[0]["File_Artist"];
	}

	if(res[0]["File_Intro"] != mysqlpp::null)
	{
		intro = (double)res[0]["File_Intro"];
	}
	else
	{
		intro = 0;
	}

	if(res[0]["File_Fadeout"] != mysqlpp::null)
	{
		mixOut = (double)res[0]["File_Fadeout"];
	}
	else
	{
		mixOut = (double)res[0]["File_Duration"];
	}

	if(mixOut == 0)
	{
		mixOut = (double)res[0]["File_Duration"];
	}

	m_duration = ((int)mixOut - (int)intro);

	//get MS:
	int s = (int)ceil(intro);    // convert to seconds - round to nearest second

	int Hours   = s / 3600 % 24;
	int Minutes = s % 3600 / 60;
	//also add hours to mins:
	Minutes += (Hours * 60);

	int Seconds = s % 60;

	strIntro.Format("%02d:%02d",Minutes, Seconds);
	
	//get MS:
	s = (int)ceil(mixOut);    // convert to seconds - round to nearest second

	Hours   = s / 3600 % 24;
	Minutes = s % 3600 / 60;
	//also add hours to mins:
	Minutes += (Hours * 60);

	Seconds = s % 60;

	strmixOut.Format("%02d:%02d",Minutes, Seconds);
	
	//also clear out the sweeper list:
	m_ctlSweep->ResetContent();
	
	//setup the conrtols:
	m_ctlTitle->SetWindowText(title);
	m_ctlTitle->fnSetTextColour(255,255,255);
	m_ctlTitle->RedrawWindow();

	m_ctlArtist->SetWindowTextA(artist);
	m_ctlArtist->fnSetTextColour(255,255,255);
	m_ctlArtist->RedrawWindow();

	m_ctlIntro->SetWindowTextA(strIntro);
	m_ctlIntro->fnSetTextColour(255,255,255);
	m_ctlIntro->RedrawWindow();

	m_ctlMixout->SetWindowTextA(strmixOut);
	m_ctlMixout->fnSetTextColour(255,255,255);
	m_ctlMixout->RedrawWindow();

	//add the 'no sweeper' option.
	int indexNoSweeper = m_ctlSweep->AddString("<No Sweeper>");
	m_mapFileIDFromIdex[indexNoSweeper] = -1;

	//check if the currently cued track is a jingle:
	if(jingle == true)
	{
		//store that fact:
		m_jingle = true;

		//set a visual cue that a sweeper has been cued:
		m_ctlArtist->SetWindowTextA("[Jingle]");

		//store the file ID:
		m_fileID = fileID;

		//set the isEmpty flag to false:
		isEmpty = false;

		m_ctlSweep->SetCurSel(indexNoSweeper);

		//exit here, we don't need to do sweeper checking on Jingles:
		return TRUE;
	}



	//iterate through all sweepers that can be used with this file:
	for(size_t i = 0; i < resSweepers.size(); i ++)
	{
		//ensure valid results:
		if(resSweepers[i]["File_Title"] != mysqlpp::null ||
			resSweepers[i]["File_ID"] != mysqlpp::null)
		{
			int nIndex = 0;

			//add the sweeper to the list box:
			nIndex = m_ctlSweep->AddString((CString)resSweepers[i]["File_Title"]);

			//store the index posistion in the combobox with the fileID:
			m_mapFileIDFromIdex[nIndex] = (int)resSweepers[i]["File_ID"];
		}
	}

	//if the function has been called with a sweeperIndex flag, then
	//we need to pre-select the index in the combo box:
	if(sweepIndex != -1)
	{
		m_ctlSweep->SetCurSel(sweepIndex);
	}
	else
	{
		m_ctlSweep->SetCurSel(indexNoSweeper);
	}

	//store the file ID:
	m_fileID = fileID;

	//set the isEmpty flag to false:
	isEmpty = false;

	return TRUE;
}

//fnCueSongWithSweeper - cue a song with a sweeper assigned to it:
BOOL CMainPlaylistController::fnCueSongWithSweeper(int fileID, int sweeperID)
{
	CString title;
	CString artist;
	double mixOut;
	double intro;
	CString strmixOut;
	CString strIntro;

	//get all file info for the fileID:
	mysqlpp::StoreQueryResult res = m_data->fnGetSongInfoForFileID(fileID);

	//find sweepers that can be used for this file:
	mysqlpp::StoreQueryResult resSweepers = m_data->fnGetSweepersForSongID(fileID, g_intShowID);

	//ensure a valid file info results set has been returned:
	if(res.size() != 1)
	{
		return false;
	}

	//get the file title if not null:
	if(res[0]["File_Title"] != mysqlpp::null)
	{
		title = (CString)res[0]["File_Title"];
	}

	//get the file artist if not null:
	if(res[0]["File_Artist"] != mysqlpp::null)
	{
		artist = (CString)res[0]["File_Artist"];
	}

	if(res[0]["File_Intro"] != mysqlpp::null)
	{
		intro = (double)res[0]["File_Intro"];
	}
	else
	{
		intro = 0;
	}


	if(res[0]["File_Fadeout"] != mysqlpp::null)
	{
		mixOut = (double)res[0]["File_Fadeout"];
	}
	else
	{
		mixOut = 0;
	}

	if(mixOut == 0)
	{
		mixOut = (double)res[0]["File_Duration"];
	}

	//get MS:
	int s = (int)ceil(intro);    // convert to seconds - round to nearest second

	int Hours   = s / 3600 % 24;
	int Minutes = s % 3600 / 60;
	//also add hours to mins:
	Minutes += (Hours * 60);

	int Seconds = s % 60;

	strIntro.Format("%02d:%02d",Minutes, Seconds);
	
	//get MS:
	s = (int)ceil(mixOut);    // convert to seconds - round to nearest second

	Hours   = s / 3600 % 24;
	Minutes = s % 3600 / 60;
	//also add hours to mins:
	Minutes += (Hours * 60);

	Seconds = s % 60;

	strmixOut.Format("%02d:%02d",Minutes, Seconds);

	//setup the conrtols:
	m_ctlTitle->SetWindowText(title);
	m_ctlTitle->fnSetTextColour(255,255,255);
	m_ctlTitle->RedrawWindow();

	m_ctlArtist->SetWindowTextA(artist);
	m_ctlArtist->fnSetTextColour(255,255,255);
	m_ctlArtist->RedrawWindow();

	m_ctlIntro->SetWindowTextA(strIntro);
	m_ctlIntro->fnSetTextColour(255,255,255);
	m_ctlIntro->RedrawWindow();

	m_ctlMixout->SetWindowTextA(strmixOut);
	m_ctlMixout->fnSetTextColour(255,255,255);
	m_ctlMixout->RedrawWindow();


	

	//iterate through all sweepers that can be used with this file:
	for(size_t i = 0; i < resSweepers.size(); i ++)
	{
		//ensure valid results:
		if(resSweepers[i]["File_Title"] != mysqlpp::null ||
			resSweepers[i]["File_ID"] != mysqlpp::null)
		{
			int nIndex = 0;

			//add the sweeper to the list box:
			nIndex = m_ctlSweep->AddString((CString)resSweepers[i]["File_Title"]);

			//store the index posistion in the combobox with the fileID:
			m_mapFileIDFromIdex[nIndex] = (int)resSweepers[i]["File_ID"];
		}
	}

	//iterate through the nIndex and File_ID sweeper map:
	for(std::map<int, int>::iterator j = m_mapFileIDFromIdex.begin();
		j !=  m_mapFileIDFromIdex.end(); j++)
	{
		//check if the stored fileID matches the passed sweeper's file ID:
		if(j->second == sweeperID)
		{
			//cue the sweeper:
			m_ctlSweep->SetCurSel(j->first);
			break;
		}
	}

	//store the file ID:
	m_fileID = fileID;

	//set the isEmpty flag to false:
	isEmpty = false;

	return TRUE;
}

//fnSetHalt - sets the halt flag for the current element:
void CMainPlaylistController::fnSetHalt()
{
	m_halt = true;
}

//fnNeedToHalt - returns weather the halt flag is set for this element:
BOOL CMainPlaylistController::fnNeedToHalt()
{
	return m_halt;
}

//fnResetHalt - resets the halt flag for this element:
void CMainPlaylistController::fnResetHalt()
{
	m_halt = false;
}

//fnGetFileID - return the file ID for the current song that is being cued by this playlist element
int CMainPlaylistController::fnGetFileID(void)
{
	return m_fileID;
}

//fnClear - clear the currently cued song (if any):
void CMainPlaylistController::fnClear(void)
{
	//reset all controls:
	m_ctlArtist->fnSetTextColour(120,120,120);
	m_ctlArtist->SetWindowTextA("Empty");
	m_ctlTitle->fnSetTextColour(120,120,120);
	m_ctlMixout->SetWindowTextA("00:00");
	m_ctlMixout->fnSetTextColour(120,120,120);
	m_ctlMixout->RedrawWindow();
	m_ctlIntro->SetWindowTextA("00:00");
	m_ctlIntro->fnSetTextColour(120,120,120);
	m_ctlIntro->RedrawWindow();
	m_ctlTitle->SetWindowTextA("Empty");
	m_ctlSweep->ResetContent();
	m_ctlHalt->fnSetBackColour(0,0,0,true);

	//reset all flags:
	m_halt = false;
	isEmpty = true;
	m_jingle = false;
	m_fileID = -1;
}

//fnGetSweeperFileID - return the FileID for the sweeper that is selected:
int CMainPlaylistController::fnGetSweeperFileID(void)
{
	if(m_ctlSweep->GetCurSel() == LB_ERR)
	{
		return -1;
	}

	return m_mapFileIDFromIdex[m_ctlSweep->GetCurSel()];
}

//fnIsJingle - return weather the currently cued track is a jingle: 
bool CMainPlaylistController::fnIsJingle()
{
	return m_jingle;
}

//fnGetState - will return a structure contatining state information for the current 
//playlist element:
s_playlistState CMainPlaylistController::fnGetState(bool erase)
{
	CString title;
	CString artist;
	CString intro;
	CString mixout;
	//create a new structure to return:
	s_playlistState plState;

	//can only be called if we arn't empty:
	if(isEmpty == true)
	{
		//set an empty flag on the struct:
		plState.empty = true;

		//return the strcut:
		return plState;
	}

	//populate the structure:
	
	//get the title and artist:
	m_ctlArtist->GetWindowText(artist);
	m_ctlTitle->GetWindowText(title);
	m_ctlIntro->GetWindowTextA(intro);
	m_ctlMixout->GetWindowTextA(mixout);
	
	//set them in the structure:
	plState.artist = artist;
	plState.title = title;
	plState.intro = intro;
	plState.mixout = mixout;

	//set current sweeper index:
	if(m_ctlSweep->GetCurSel() != LB_ERR)
	{
		plState.sweeperIndex = m_ctlSweep->GetCurSel();
	}
	else
	{
		plState.sweeperIndex = -1;
	}
	
	//set other flags:
	plState.fileID = m_fileID;
	plState.empty = false;
	plState.jingle = m_jingle;
	plState.halt = m_halt;

	//erase is neccessery:
	if(erase)
	{
		fnClear();
	}
	
	//return the struct:
	return plState;
}

//fnSetState - will set the state of the current playlist element as defined by the
//s_playlistState structure:
void CMainPlaylistController::fnSetState(s_playlistState plState)
{
	mysqlpp::StoreQueryResult resSweepers;
	//ensure that the structure isn't empty:
	if(plState.empty == true)
	{
		fnClear();

		//exit:
		return;
	}
	
	//set the controls:
	if(plState.halt == true)
	{
		//set the flag:
		m_halt = true;

		//set the button to red:
		m_ctlHalt->fnSetBackColour(255,0,0,true);
	}

	//set the text:
	m_ctlArtist->SetWindowText(plState.artist);
	m_ctlArtist->fnSetTextColour(255,255,255);
	m_ctlTitle->SetWindowText(plState.title);
	m_ctlTitle->fnSetTextColour(255,255,255);
	m_ctlIntro->SetWindowText(plState.intro);
	m_ctlIntro->fnSetTextColour(255,255,255);
	m_ctlMixout->SetWindowTextA(plState.mixout);
	m_ctlMixout->fnSetTextColour(255,255,255);
	m_ctlArtist->RedrawWindow();
	m_ctlTitle->RedrawWindow();
	m_ctlMixout->RedrawWindow();
	m_ctlIntro->RedrawWindow();

	//we are no longer empty:
	isEmpty = false;

	//set the fileID:
	m_fileID = plState.fileID;

	//set the jingle state:
	if(plState.jingle == true)
	{
		m_jingle = true;

		m_ctlArtist->SetWindowText("[Jingle]");

		//we don't need sweeper checking for Jingles, so exit here:
		return;
	}

	//populate the sweeper combo box:
	resSweepers = m_data->fnGetSweepersForSongID(plState.fileID, g_intShowID);

	//add the No Sweeper option:
	int noSweeperID = m_ctlSweep->AddString("<No Sweeper>");
	m_mapFileIDFromIdex[noSweeperID] = -1;

	//iterate through all sweepers that can be used with this file:
	for(size_t i = 0; i < resSweepers.size(); i ++)
	{
		//ensure valid results:
		if(resSweepers[i]["File_Title"] != mysqlpp::null ||
			resSweepers[i]["File_ID"] != mysqlpp::null)
		{
			int nIndex = 0;

			//add the sweeper to the list box:
			nIndex = m_ctlSweep->AddString((CString)resSweepers[i]["File_Title"]);

			//store the index posistion in the combobox with the fileID:
			m_mapFileIDFromIdex[nIndex] = (int)resSweepers[i]["File_ID"];
		}
	}

	//set the sweeper index:
	if(plState.sweeperIndex != -1)
	{
		m_ctlSweep->SetCurSel(plState.sweeperIndex);
	}
	else
	{
		m_ctlSweep->SetCurSel(noSweeperID);
	}
}