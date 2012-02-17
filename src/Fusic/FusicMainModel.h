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
	double fnGetIntroTimeForFileID(int FileID, bool disconnectAfteryQuery = true);

	//fnGetDurationForFileID - get the duration for a certain FileID:
	double fnGetDurationForFileID(int FileID);

	mysqlpp::StoreQueryResult fnGetFilesForPlaylistIDAndSearchTitle
	(int playlistID, CString searchString);
	mysqlpp::StoreQueryResult fnGetFilesForPlaylistIDAndSearchArtist
	(int playlistID, CString searchString);
	//
	mysqlpp::StoreQueryResult fnGetFilesForPlaylistID(int playlistID);

	mysqlpp::StoreQueryResult fnGetSweepersForSongID(int fileID);

	mysqlpp::StoreQueryResult fnGetSongsForShowIDAndSearchStringArtist(int showID,
		CString searchString);

	mysqlpp::StoreQueryResult fnGetSongsForShowIDAndSearchStringSong(int showID,
		CString searchString);

	mysqlpp::StoreQueryResult fnGetPlaylistInfroForShowID(int showID);

	mysqlpp::StoreQueryResult fnGetJinglesForShowID(int showID);

	void fnLogFile(int fileID);


private:
	//private methods:

	//connect - connect to the MySQL database:
	bool connect();

	//disconnect to the database:
	void disconnect();

	//internal function to do all queries:
	mysqlpp::StoreQueryResult doQuery(CString SQLQuery);

	//private members:

	//m_pMySQLConn - a pointer to a mysqlpp::connection object.
	mysqlpp::Connection* m_pMySQLConn;
};
