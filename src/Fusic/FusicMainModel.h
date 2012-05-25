#pragma once
#include "mysql++.h"
#include "Globals.h"

//get the DB struct:
extern dbSettings g_sctDBSettings;

//CFusicMainModel - this class will pass data back and forth
//to the database in a nice way :-).

class CFusicMainModel
{
public:                                          
	//constructor:
	CFusicMainModel(void);

	//destructor:
	~CFusicMainModel(void);

	//fnGetSongsForShowID - return a results set for all songs
	//that belong to a show:
	mysqlpp::StoreQueryResult fnGetSongsFowShowId(int ShowID);

	// fnGetSongInfoForFileID - return the results set for one song that is identified by the fileID:
	mysqlpp::StoreQueryResult fnGetSongInfoForFileID(int fileID);

	//fnGetIntroTimeForFileID - get the intro time for a certain FileID:
	double fnGetIntroTimeForFileID(int FileID);

	//fnGetDurationForFileID - get the duration for a certain FileID:
	double fnGetDurationForFileID(int FileID);

	//fnGetFilesForPlaylistIDAndSearchTitle - return the files
	//that are in a playlist and fit a search string :
	mysqlpp::StoreQueryResult fnGetFilesForPlaylistIDAndSearchString
	(int playlistID, CString searchString);

	//fnGetFilesForPlaylistID - return all the files that are contained
	//within a certain playlist:
	mysqlpp::StoreQueryResult fnGetFilesForPlaylistID(int playlistID);

	//fnGetSweepersForSongID - return all sweepers that can be played
	//for a certain File:
	mysqlpp::StoreQueryResult fnGetSweepersForSongID(int fileID, int showID);

	//fnGetSongsForShowIDAndSearchStringArtist - return all files for a certain
	//show that corrispond to the search string:
	mysqlpp::StoreQueryResult fnGetSongsForShowIDAndSearchString(int showID,
		CString searchString);

	//fnGetPlaylistInfroForShowID - return a list of playlists for a certain
	//show:
	mysqlpp::StoreQueryResult fnGetPlaylistInfroForShowID(int showID);

	//fnGetJinglesForShowID - retrun a list of jingles for a certain show:
	mysqlpp::StoreQueryResult fnGetJinglesForShowID(int showID);

	//fnGetJInglesForShowIDAndSearchString - get a list of jingles for a certain
	//show and that match a search string for title:
	mysqlpp::StoreQueryResult fnGetJInglesForShowIDAndSearchString(int showID,
		CString searchString);

	//fnGetFilesExcludingFilesAndArtist - Return a list of files for show ID 0,
	//that exclude an array for file IDs and file Artists:
	mysqlpp::StoreQueryResult fnGetFilesExcludingFilesAndArtist
		(std::vector<int> files,  std::vector<CString> artists);

	//fnGetFilesPlaiedInTheLast - returns a list of files that have been played within
	//the last number of hours:
	mysqlpp::StoreQueryResult fnGetFilesPlaiedInTheLast(int hour);

	//fnLogFile - Log that a file has been played in the database:
	void fnLogFile(int fileID, int showID);

	bool fnCheckMachineOnAir();
	void fnSetMachineOnAir();

		//fnGetSettings - get the settings.
	bool fnGetSettings();

	mysqlpp::StoreQueryResult fnGetFilesToExclude(std::vector<int>& files, 
								  std::vector<CString>& artists);


private:
	bool isConnected;

	//private methods:

	//connect() - create a new MySQL object and connect it to the
	//database:
	bool connect();

	//disconnect to the database:
	void disconnect();

	//internal function to do all queries:
	mysqlpp::StoreQueryResult doQuery(CString SQLQuery);

	//private members:

	//m_pMySQLConn - a pointer to a mysqlpp::connection object.
	mysqlpp::Connection* m_pMySQLConn;
};
