// AutoDJ.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "AutoDJ.h"

extern bool doLog;
extern playoutSettings g_settings;

//CAutoDJ - A class that will control the automatic cueing of songs, along with Jingles and Sweepers after
//a defined period of time.


//public member implimentation:

//constructor - keeps a reference to an instance of the MainModel class
//and the main dialog class:
CAutoDJ::CAutoDJ(CFusicMainModel* data,
				 CFusicMainDlg* mainDlg)
{
	m_JingleCountdown = 1;
	m_SweeperCountdown = 1;

	//set class addresses in the members:
	m_data = data;
	m_mainDlg = mainDlg;

}

//destructor:
CAutoDJ::~CAutoDJ()
{
}

//fnCueNextSong - will get a song ID from the database and cue
//up that song on the playlist:
void CAutoDJ::fnCueNextSong()
{
	if(doLog)
		fnLogOutMsg("CAutoDJ::fnCueNextSong called.");
	//local variables:
	std::vector<int> fileIDsToExclude;
	std::vector<CString> artistsToExclude;
	mysqlpp::StoreQueryResult resPlayableSongs;
	mysqlpp::StoreQueryResult resRecentFiles;
	mysqlpp::StoreQueryResult resArtist;
	mysqlpp::StoreQueryResult resSweeper;

	//check for jingle playback:
	if(m_JingleCountdown > g_settings.autodj_play_jingle)
	{
		//cue up a jingle:
		mysqlpp::StoreQueryResult resJingles = 
			m_data->fnGetJinglesForShowID(0);

		//get the next file ID to play:
		int nextFileID = fnGetRandomNumber(0, (int)resJingles.size() - 1);
		if(doLog)
			fnLogOutMsg("Cueing a jingle.");
		//cue the random file ID:
		m_mainDlg->fnGetNexAvaliblePlaylistCart()->fnCueSong( resJingles[nextFileID
		]["File_ID"], -1, true);

		//reset the counter:
		m_JingleCountdown = 0;
		return;
	}

	//also we need to exclude songs that are already
	//cued up in the playlist:

	//iterate through all the playlist elements:
	for(std::map<int, CMainPlaylistController*>::iterator i = 
		m_mainDlg->m_playlistControllerMap.begin();
		i != m_mainDlg->m_playlistControllerMap.end(); i++)
	{
		//get the file ID that's cued up on each playlist element:
		int fileID = i->second->fnGetFileID();
		
		if(fileID != -1)
		{
			fileIDsToExclude.push_back(fileID);
			
			//find artist info:
			mysqlpp::StoreQueryResult resArtist = 
				m_data->fnGetSongInfoForFileID(fileID);
			if((int)resArtist.size() == 1)
				artistsToExclude.push_back( (CString)resArtist[0]["File_Artist"] );
		}//if
	}//for

	//find file IDs to exclude:
	try
	{
		resRecentFiles = m_data->fnGetFilesPlaiedInTheLast(1);
	}
	//catch a bad connection exception:
	catch (const mysqlpp::ConnectionFailed& e)
	{
		//construct the error message
		CString strMessage;
		strMessage = "ERROR: Lost connection with the MySQL server. ";
		strMessage.Append(e.what());

		//show the message:
		m_mainDlg->MessageBox(strMessage, "Fusic Error", MB_OK | MB_ICONERROR);

		//exit out of the routine:
		return;
	}
	//catch the bad conversation exception:
	catch (const mysqlpp::BadConversion& e)
	{
		std::cerr << "ERROR: Bad conversation with the MySQL server - " <<
			e.what() << " Trying again.";

		//try again:
		try
		{
			resRecentFiles = m_data->fnGetFilesPlaiedInTheLast(g_settings.autodj_repeat_song_hours);
		}
		//catch a bad connection exception:
		catch (const mysqlpp::ConnectionFailed& e)
		{
			//construct the error message
			CString strMessage;
			strMessage = "ERROR: Lost connection with the MySQL server. ";
			strMessage.Append(e.what());

			//show the message:
			m_mainDlg->MessageBox(strMessage, 
				"Fusic Error", MB_OK | MB_ICONERROR);

			//exit out of the routine:
			return;
		}
		//catch the bad conversation exception:
		catch (const mysqlpp::BadConversion& e)
		{
			//error out this time:
			//construct the error message
			CString strMessage;
			strMessage = "ERROR: Bad conversation with MySQL server. ";
			strMessage.Append(e.what());

			//show the message:
			m_mainDlg->MessageBox(strMessage, 
				"Fusic Error", MB_OK | MB_ICONERROR);

			//exit out of the routine:
			return;
		}
	}
	//catch everything else.
	catch(const mysqlpp::Exception &e)
	{
		CString strMessage;
		strMessage = "ERROR: MySQL server error: ";
		strMessage.Append(e.what());

		//show the message:
		m_mainDlg->MessageBox(strMessage, 
			"Fusic Error", MB_OK | MB_ICONERROR);

		//exit out of the routine:
		return;
	}

	//pushback songs:
	for(size_t i = 0; i < resRecentFiles.size(); i++)
	{
		fileIDsToExclude.push_back((int)resRecentFiles[i]["File_ID"]);
	}//for

	//find artists that we need to exclude:
	try
	{
		resArtist = m_data->fnGetFilesPlaiedInTheLast(g_settings.autodj_repeat_artist_hours);
	}
	//catch a bad connection exception:
	catch (const mysqlpp::ConnectionFailed& e)
	{
		//construct the error message
		CString strMessage;
		strMessage = "ERROR: Lost connection with the MySQL server. ";
		strMessage.Append(e.what());

		//show the message:
		m_mainDlg->MessageBox(strMessage, "Fusic Error", MB_OK | MB_ICONERROR);

		//exit out of the routine:
		return;
	}
	//catch the bad conversation exception:
	catch (const mysqlpp::BadConversion& e)
	{				
		std::cerr << "ERROR: Bad conversation with the MySQL server - " <<
			e.what() << " Trying again.";
		//try again:
		try
		{
			resArtist = m_data->fnGetFilesPlaiedInTheLast(2);
		}
		//catch a bad connection exception:
		catch (const mysqlpp::ConnectionFailed& e)
		{
			//construct the error message
			CString strMessage;
			strMessage = "ERROR: Lost connection with the MySQL server. ";
			strMessage.Append(e.what());

			//show the message:
			m_mainDlg->MessageBox(strMessage, 
				"Fusic Error", MB_OK | MB_ICONERROR);

			//exit out of the routine:
			return;
		}
		//catch the bad conversation exception:
		catch (const mysqlpp::BadConversion& e)
		{
			//error out this time:
			//construct the error message
			CString strMessage;
			strMessage = "ERROR: Bad conversation with MySQL server. ";
			strMessage.Append(e.what());

			//show the message:
			m_mainDlg->MessageBox(strMessage, 
				"Fusic Error", MB_OK | MB_ICONERROR);

			//exit out of the routine:
			return;
		}
	}
	//catch everything else.
	catch(const mysqlpp::Exception &e)
	{
		CString strMessage;
		strMessage = "ERROR: MySQL server error: ";
		strMessage.Append(e.what());

		//show the message:
		m_mainDlg->MessageBox(strMessage, 
			"Fusic Error", MB_OK | MB_ICONERROR);

		//exit out of the routine:
		return;
	}

	//add the list of artists to the vector:
	for(size_t i = 0; i < resArtist.size(); i++)
	{
		artistsToExclude.push_back((CString)resArtist[i]["File_Artist"]);
	}//for

	//get the files that are playable::
	try
	{
		resPlayableSongs = 
			m_data->fnGetFilesExcludingFilesAndArtist(fileIDsToExclude,
			artistsToExclude);
	}
	//catch a bad connection exception:
	catch (const mysqlpp::ConnectionFailed& e)
	{
		//construct the error message
		CString strMessage;
		strMessage = "ERROR: Lost connection with the MySQL server. ";
		strMessage.Append(e.what());

		//show the message:
		m_mainDlg->MessageBox(strMessage, "Fusic Error", MB_OK | MB_ICONERROR);

		//exit out of the routine:
		return;
	}
	//catch the bad conversation exception:
	catch (const mysqlpp::BadConversion& e)
	{
		std::cerr << "ERROR: Bad conversation with the MySQL server - " <<
			e.what() << " Trying again.";
		//try again:
		try
		{
			resPlayableSongs = 
				m_data->fnGetFilesExcludingFilesAndArtist(fileIDsToExclude,
				artistsToExclude);
		}
		//catch a bad connection exception:
		catch (const mysqlpp::ConnectionFailed& e)
		{
			//construct the error message
			CString strMessage;
			strMessage = "ERROR: Lost connection with the MySQL server. ";
			strMessage.Append(e.what());

			//show the message:
			m_mainDlg->MessageBox(strMessage, 
				"Fusic Error", MB_OK | MB_ICONERROR);

			//exit out of the routine:
			return;
		}
		//catch the bad conversation exception:
		catch (const mysqlpp::BadConversion& e)
		{
			//error out this time:
			//construct the error message
			CString strMessage;
			strMessage = "ERROR: Bad conversation with MySQL server. ";
			strMessage.Append(e.what());

			//show the message:
			m_mainDlg->MessageBox(strMessage, 
				"Fusic Error", MB_OK | MB_ICONERROR);

			//exit out of the routine:
			return;
		}
	}
	//catch everything else.
	catch(const mysqlpp::Exception &e)
	{
		CString strMessage;
		strMessage = "ERROR: MySQL server error: ";
		strMessage.Append(e.what());

		//show the message:
		m_mainDlg->MessageBox(strMessage, 
			"Fusic Error", MB_OK | MB_ICONERROR);

		//exit out of the routine:
		return;
	}

	//get the next file ID to play:
	int nextFileID = fnGetRandomNumber(0, (int)resPlayableSongs.size() - 1);

	//check weather a sweeeper needs to be added:
	if(m_SweeperCountdown >= g_settings.autodj_play_sweeper)
	{
		//attempt to get a sweeper for the next song:
		try
		{
			resSweeper = m_data->fnGetSweepersForSongID(
				resPlayableSongs[nextFileID]["File_ID"], g_intShowID);
		}
		//catch a bad connection exception:
		catch (const mysqlpp::ConnectionFailed& e)
		{
			//construct the error message
			CString strMessage;
			strMessage = "ERROR: Lost connection with the MySQL server. ";
			strMessage.Append(e.what());

			//show the message:
			m_mainDlg->MessageBox(strMessage, "Fusic Error", MB_OK | MB_ICONERROR);

			//exit out of the routine:
			return;
		}
		//catch the bad conversation exception:
		catch (const mysqlpp::BadConversion& e)
		{
			std::cerr << "ERROR: Bad conversation with the MySQL server - " <<
				e.what() << " Trying again.";
			//try again:
			try
			{
				resSweeper = m_data->fnGetSweepersForSongID(
					resPlayableSongs[nextFileID]["File_ID"], g_intShowID);
			}
			//catch a bad connection exception:
			catch (const mysqlpp::ConnectionFailed& e)
			{
				//construct the error message
				CString strMessage;
				strMessage = "ERROR: Lost connection with the MySQL server. ";
				strMessage.Append(e.what());

				//show the message:
				m_mainDlg->MessageBox(strMessage, 
					"Fusic Error", MB_OK | MB_ICONERROR);

				//exit out of the routine:
				return;
			}
			//catch the bad conversation exception:
			catch (const mysqlpp::BadConversion& e)
			{
				//error out this time:
				//construct the error message
				CString strMessage;
				strMessage = "ERROR: Bad conversation with MySQL server. ";
				strMessage.Append(e.what());

				//show the message:
				m_mainDlg->MessageBox(strMessage, 
					"Fusic Error", MB_OK | MB_ICONERROR);

				//exit out of the routine:
				return;
			}
		}
		//catch everything else.
		catch(const mysqlpp::Exception &e)
		{
			CString strMessage;
			strMessage = "ERROR: MySQL server error: ";
			strMessage.Append(e.what());

			//show the message:
			m_mainDlg->MessageBox(strMessage, 
				"Fusic Error", MB_OK | MB_ICONERROR);

			//exit out of the routine:
			return;
		}


		//check if a sweeper needs to be added:
		if(resSweeper.size() > 0)
		{
			//select a random sweeper:
			int nextSweeper = fnGetRandomNumber(0, (int)resSweeper.size() - 1);

			//get the next playlist element:
			CMainPlaylistController* pl = m_mainDlg->fnGetNexAvaliblePlaylistCart();

			//ensure we have a valid cart:
			if(pl != NULL)
			{
				if(doLog)
					fnLogOutMsg("Cueing song with a sweeper.");
				//cue in the playlsit:
				pl->fnCueSongWithSweeper
					(resPlayableSongs[nextFileID]["File_ID"], resSweeper[nextSweeper]
				["File_ID"]);
			}
			else
			{
				if(doLog)
					fnLogOutMsg("WARNING: Got bad pointer.");
				//return if we have a bad pointer:
				return;
			}

			//we have cued a song, increase the counters:
			m_JingleCountdown += 1;
			m_SweeperCountdown = 0;

			//return after adding the sweeper:
			return;
		}
	}

	//get the next avalible cart:
	CMainPlaylistController* pl;
	pl = m_mainDlg->fnGetNexAvaliblePlaylistCart();

	if(pl != NULL)
	{
		if(doLog)
			fnLogOutMsg("Succesfully Cued a song.");
		//cue up the song:
		pl->fnCueSong( resPlayableSongs[nextFileID
		]["File_ID"]);
	}
	else
	{
		if(doLog)
			fnLogOutMsg("WARNING: Got bad pointer.");
		//return if we have a bad pointer:
		return;
	}

	//we have cued a song, increase the counters:
	m_JingleCountdown += 1;
	m_SweeperCountdown += 1;

	//max retry of 3 songs:
	if(m_SweeperCountdown == 8)
		m_SweeperCountdown = 0;
}

//fnGetRandomNumber - generate a random number between a range:
int CAutoDJ::fnGetRandomNumber(int low, int high)
{
	time_t SeedTime;
	struct tm SeedDate;
	SeedTime = time(0);
	SeedDate = *localtime(&SeedTime);
	int FinalSeed = (int)SeedTime + SeedDate.tm_mday + (SeedDate.tm_mon+1) + (SeedDate.tm_year+1900);
	srand((unsigned int) FinalSeed);

	int Interval = high - low + 1;
	int RandomOffset = rand() % Interval;
	int RandomNumber = low + RandomOffset;
	return RandomNumber;

}