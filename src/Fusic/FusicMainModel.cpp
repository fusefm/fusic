#include "StdAfx.h"
#include "FusicMainModel.h"

//CFusicMainModel - this class will pass data back and forth
//to the database.
extern CString g_systemID;
//constructor:
CFusicMainModel::CFusicMainModel(void)
{
	//construct a new connection object:
	m_pMySQLConn = NULL;
	isConnected = false;
	connect();
}

//destructor:
CFusicMainModel::~CFusicMainModel(void)
{
	//ensure that the object is deleted:
	disconnect();
}

extern playoutSettings g_settings;
extern bool doLog;
bool CFusicMainModel::fnGetSettings()
{
	mysqlpp::StoreQueryResult res;
	if(!connect())
	{
		return false;
	}

	res = doQuery("SELECT * FROM tbl_settings;");

	for(size_t i = 0; i < res.size(); i++ )
	{
		int value;
		CString setting;
		setting = res[i]["Setting_Name"];
		try
		{
			value = (int)res[i]["Setting_Value"];
		}
		catch(const mysqlpp::Exception &e)
		{
			//skip over any casting errors.
			continue;
		}
		if(doLog)
		{
			fnLogOutMsg("Got Setting: " + setting + " " + (CString)res[i]["Setting_Value"]);
		}
		if(setting == "next_fade_out_time")
		{
			g_settings.next_fade_out_time = value;
			g_settings.mix_out_fade_out_time = value;
		}
		else if(setting == "sweeper_fade_down_time")
		{
			g_settings.sweeper_fade_down_time = value;
		}
		else if(setting == "sweeper_fade_down_volume")
		{
			g_settings.sweeper_fade_down_volume = value;
		}
		else if(setting == "sweeper_fade_up_time")
		{
			g_settings.sweeper_fade_up_time = value;
		}
		else if(setting == "pause_fade_down_time")
		{
			g_settings.pause_fade_down_time = value;
		}
		else if(setting == "pause_fade_up_time")
		{
			g_settings.pause_fade_up_time = value;
		}
		else if(setting == "stop_fade_out_time")
		{
			g_settings.stop_fade_down_time = value;
		}
		else if(setting == "autodj_play_sweeper")
		{
			g_settings.autodj_play_sweeper = value;
		}
		else if(setting == "autodj_play_jingle")
		{
			g_settings.autodj_play_jingle = value;
		}
		else if(setting == "autodj_repeat_artist_hours")
		{
			g_settings.autodj_repeat_artist_hours = value;
		}
		else if(setting == "autodj_repeat_song_hours")
		{
			g_settings.autodj_repeat_song_hours = value;
		}
		else if(setting == "cart_fade_out_time")
		{
			g_settings.cart_fade_out_time = value;
		}
	}

	return true;
}

//fnGetSongsForShowID - return a results set for all songs
//that belong to a show:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetSongsFowShowId(int ShowID)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//convert int to string:
	temp.Format("%d", ShowID);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_files` WHERE File_Type = 'S' AND"
		" Show_ID = ";
	sqlQuery.Append(temp);
	sqlQuery.Append(" ORDER BY File_Artist, File_Title");
	sqlQuery.Append(";");

	//do the query:
	res = doQuery(sqlQuery);

	//return the result:
	return res;
}

// fnGetSongInfoForFileID - return the results set for one song that is identified by the fileID:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetSongInfoForFileID(int fileID)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//convert int to string:
	temp.Format("%d", fileID);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_files` WHERE File_ID = ";
	sqlQuery.Append(temp);
	sqlQuery.Append(";");

	//do the query:
	res = doQuery(sqlQuery);

	//return the result:
	return res;
}

//fnGetIntroTimeForFileID - get the intro time for a certain FileID:
double CFusicMainModel::fnGetIntroTimeForFileID(int FileID)
{
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;
	double introTime = 0;

	if(!connect())
	{
		return 0;
	}

	temp.Format("%d", FileID);

	sqlQuery = "SELECT File_Intro FROM `tbl_files` WHERE File_ID = ";
	sqlQuery.Append(temp);
	sqlQuery.Append(";");

	res = doQuery(sqlQuery);

	if(res[0]["File_Intro"] != mysqlpp::null)
	{
		introTime = (double)res[0]["File_Intro"];
	}

	return introTime;
}

//fnGetDurationForFileID - get the duration for a certain FileID:
double CFusicMainModel::fnGetDurationForFileID(int FileID)
{
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;
	double duration = 0;

	if(!connect())
	{
		return 0;
	}

	temp.Format("%d", FileID);

	sqlQuery = "SELECT File_Duration FROM `tbl_files` WHERE File_ID = ";
	sqlQuery.Append(temp);
	sqlQuery.Append(";");

	res = doQuery(sqlQuery);

	if(res[0]["File_Duration"] != mysqlpp::null)
	{
		duration = (double)res[0]["File_Duration"];
	}

	return duration;
}

//fnGetFilesForPlaylistIDAndSearchTitle - return the files
//that are in a playlist and fit a search string for the artist:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetFilesForPlaylistIDAndSearchString
(int playlistID, CString searchString)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	mysqlpp::StoreQueryResult res1;
	CString sqlQuery;
	std::vector<int> fileIDs;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//convert int to string:
	temp.Format("%d", playlistID);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_playlist_files` WHERE Playlist_ID  = ";
	sqlQuery.Append(temp);
	sqlQuery.Append(";");

	//do the query:
	res = doQuery(sqlQuery);

	//create the vector:
	for(size_t i = 0; i < res.size(); i++)
	{
		fileIDs.push_back((int)res[i]["File_ID"]);	
	}

	//now get all the file IDs:
	sqlQuery.Empty();

	if(fileIDs.size() > 0)
	{
		sqlQuery = "SELECT * FROM tbl_files WHERE (";
		for(std::vector<int>::iterator i = fileIDs.begin(); i != fileIDs.end();
			i++)
		{
			//get the file ID as a string:
			temp.Format("%d", *i);
			sqlQuery.Append("File_ID = ");
			sqlQuery.Append(temp);
			sqlQuery.Append(" OR ");
		}

		//remove the last OR:
		sqlQuery.Delete(sqlQuery.GetLength() - 3, 3);
		sqlQuery.Append(") AND (File_Artist LIKE '%");
		mysqlpp::Query query = m_pMySQLConn->query();
		query << sqlQuery << mysqlpp::escape << searchString.GetBuffer()
			<< "%' OR File_Title LIKE '%";
		query << mysqlpp::escape << searchString.GetBuffer()
			<< "%') ORDER BY File_Artist, File_Title;";
		searchString.ReleaseBuffer();
		res1 = query.store();
	}

	//return the result:
	return res1;
}

//fnGetFilesForPlaylistID - return all the files that are contained
//within a certain playlist:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetFilesForPlaylistID
(int playlistID)
{


	//the results set:
	mysqlpp::StoreQueryResult res;
	mysqlpp::StoreQueryResult res1;
	CString sqlQuery;
	std::vector<int> fileIDs;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//convert int to string:
	temp.Format("%d", playlistID);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_playlist_files` WHERE Playlist_ID  = ";
	sqlQuery.Append(temp);
	sqlQuery.Append(";");

	//do the query:
	res = doQuery(sqlQuery);

	//create the vector:
	for(size_t i = 0; i < res.size(); i++)
	{
		fileIDs.push_back((int)res[i]["File_ID"]);	
	}

	//now get all the file IDs:
	sqlQuery.Empty();

	if(fileIDs.size() > 0)
	{
		sqlQuery = "SELECT * FROM tbl_files WHERE ";
		for(std::vector<int>::iterator i = fileIDs.begin(); i != fileIDs.end();
			i++)
		{
			//get the file ID as a string:
			temp.Format("%d", *i);
			sqlQuery.Append("File_ID = ");
			sqlQuery.Append(temp);
			sqlQuery.Append(" OR ");
		}

		//remove the last OR:
		sqlQuery.Delete(sqlQuery.GetLength() - 3, 3);
		sqlQuery.Append(" ORDER BY File_Artist, File_Title");
		sqlQuery.Append(";");
		res1 = doQuery(sqlQuery);
	}

	//return the result:
	return res1;
}

//fnGetSweepersForSongID - return all sweepers that can be played
//for a certain File:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetSweepersForSongID(int fileID, int showID)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	mysqlpp::StoreQueryResult resFile;
	CString sqlQuery;
	CString temp;
	double introTime = 0;
	double mixInTime = 0;


	//first get how long the file is:
	resFile = fnGetSongInfoForFileID(fileID);

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//ensure we got a valid result:
	if(resFile.size() == 0)
	{
		return res;
	}

	//ensure that the song has an intro:
	if(resFile[0]["File_Intro"] == mysqlpp::null)
	{
		return res;
	}
	else
	{

		//get the intro time:
		introTime = (double)resFile[0]["File_Intro"];
	}
	if(resFile[0]["File_Fadein"] == mysqlpp::null)
	{
		mixInTime = 0;
	}
	else
	{
		mixInTime = (double)resFile[0]["File_Fadein"];
	}

	//remove the mix in time:
	introTime -= mixInTime;

	//calculate from the fade out and fade in times:
	introTime -= (double)((g_settings.sweeper_fade_down_time +
		g_settings.sweeper_fade_up_time)/1000);

	//convert int to string:
	temp.Format("%.0f", introTime);
	CString temp2;
	temp2.Format("%d", showID);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_files` WHERE File_Type = 'W' AND"
		" (Show_ID = ";
	sqlQuery.Append(temp2);
	sqlQuery.Append(" OR Show_ID = 0)");
	sqlQuery.Append(" AND File_Duration < ");
	sqlQuery.Append(temp);
	sqlQuery.Append(" ORDER BY File_Title");
	sqlQuery.Append(";");

	//do the query:
	res = doQuery(sqlQuery);

	//return the result:
	return res;
}

//fnGetSongsForShowIDAndSearchStringArtist - return all files for a certain
//show that corrispond to the search string for artist:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetSongsForShowIDAndSearchString
(int showID, CString searchString)
{
	//the results set:
	mysqlpp::StoreQueryResult res;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	mysqlpp::Query q = m_pMySQLConn->query();

	q << "SELECT * FROM `tbl_files` WHERE File_Type = 'S' AND"
		" Show_ID = " << showID << " AND (File_Artist LIKE '%" 
		<< mysqlpp::escape << searchString.GetBuffer() << "%' OR File_Title LIKE '%"
		<< mysqlpp::escape << searchString.GetBuffer() << "%') ORDER BY File_Artist, File_Title";

	res = q.store();
	searchString.ReleaseBuffer();

	//return the result:
	return res;
}

//fnGetPlaylistInfroForShowID - return a list of playlists for a certain
//show:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetPlaylistInfroForShowID
(int showID)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//convert int to string:
	temp.Format("%d", showID);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_playlist` WHERE"
		" Show_ID = ";
	sqlQuery.Append(temp);
	sqlQuery.Append(";");

	//do the query:
	res = doQuery(sqlQuery);

	//return the result:
	return res;
}

//fnGetJinglesForShowID - retrun a list of jingles for a certain show:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetJinglesForShowID(int showID)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//convert int to string:
	temp.Format("%d", showID);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_files` WHERE File_Type = 'J' AND"
		" Show_ID = ";
	sqlQuery.Append(temp);
	sqlQuery.Append(" ORDER BY File_Artist, File_Title");
	sqlQuery.Append(";");

	//do the query:
	res = doQuery(sqlQuery);

	//return the result:
	return res;
}

//fnGetJInglesForShowIDAndSearchString - get a list of jingles for a certain
//show and that match a search string for title:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetJInglesForShowIDAndSearchString(
	int showID, CString searchString)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	std::vector<int> fileIDs;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	mysqlpp::Query query = m_pMySQLConn->query();
	query << "SELECT * FROM tbl_files WHERE Show_ID = " << showID <<
		" AND File_Type = 'J' AND (File_Title LIKE '%"<< mysqlpp::escape << 
		searchString.GetBuffer() << "%' OR File_Artist LIKE '%"<< mysqlpp::escape << 
		searchString.GetBuffer() << "%') ORDER BY File_Artist, File_Title;";
	searchString.ReleaseBuffer();
	res = query.store();

	//return the result:
	return res;
}

//fnGetFilesExcludingFilesAndArtist - Return a list of files for show ID 0,
//that exclude an array for file IDs and file Artists:
mysqlpp::StoreQueryResult CFusicMainModel::
fnGetFilesExcludingFilesAndArtist(std::vector<int> files, 
								  std::vector<CString> artists)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	mysqlpp::Query q = m_pMySQLConn->query();

	//construct the query:
	q << "SELECT * FROM `tbl_files` WHERE Show_ID = 0 AND "
		"File_Type = 'S'";

	//only append the first "AND" if we have files:
	if(files.size() > 0)
	{
		q << " AND ";
	}

	//add files ID's not to include in the query:
	for(std::vector<int>::iterator i = files.begin();
		i!= files.end(); i++)
	{
		q << "File_ID != ";
		q << *i;
		if( (i + 1) != files.end() )
		{
			q << " AND ";
		}
	}

	//only add the second "AND" if we have artists:
	if(artists.size() > 0)
	{
		q << " AND ";
	}

	//now the artists:
	int tmp = 0;
	for(std::vector<CString>::iterator j = artists.begin();
		j != artists.end(); j++)
	{
		q << "File_Artist != '";
		q << mysqlpp::escape << j->GetBuffer();
		j->ReleaseBuffer();
		if( (j + 1) != artists.end() )
		{
			q << "' AND ";
		}
		else
		{
			q << "'";
		}
	}

	q << ";";

	res = q.store();
	
	//return the result:
	return res;
}

mysqlpp::StoreQueryResult CFusicMainModel::fnGetFilesToExclude(std::vector<int>& files, 
								  std::vector<CString>& artists)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	mysqlpp::Query q = m_pMySQLConn->query();

	//construct the query:
	q << "SELECT * FROM `tbl_files` WHERE Show_ID = 0 AND "
		"File_Type = 'S'";

	//only append the first "AND" if we have files:
	if(files.size() > 0)
	{
		q << " AND (";
	}

	//add files ID's not to include in the query:
	for(std::vector<int>::iterator i = files.begin();
		i!= files.end(); i++)
	{
		q << "File_ID = ";
		q << *i;
		if( (i + 1) != files.end() )
		{
			q << " OR ";
		}
	}

	if(files.size() > 0)
	{
		q << ")";
	}

	//only add the second "AND" if we have artists:
	if(artists.size() > 0)
	{
		q << " OR (";
	}

	//now the artists:
	int tmp = 0;
	for(std::vector<CString>::iterator j = artists.begin();
		j != artists.end(); j++)
	{
		q << "File_Artist = '";
		q << mysqlpp::escape << j->GetBuffer();
		j->ReleaseBuffer();
		if( (j + 1) != artists.end() )
		{
			q << "' OR ";
		}
		else
		{
			q << "'";
		}
	}
	if(artists.size() > 0)
	{
		q << ")";
	}

	q << ";";

	res = q.store();
	
	//return the result:
	return res;
}

//fnGetFilesPlaiedInTheLast - returns a list of files that have been played within
//the last number of hours:
mysqlpp::StoreQueryResult CFusicMainModel::fnGetFilesPlaiedInTheLast
(int hour)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//convert int to string:
	temp.Format("%d", hour);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_files` WHERE File_PlayedLast > "
		"SUBTIME(NOW( ) , '";
	sqlQuery.Append(temp);
	sqlQuery.Append(":0:0');");

	//do the query:
	res = doQuery(sqlQuery);

	//return the result:
	return res;
}
//fnLogFile - Log that a file has been played in the database:
void CFusicMainModel::fnLogFile(int fileID, int showID)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;

	//connect to the database:
	if(!connect())
	{
		return;
	}

	//convert int to string:
	temp.Format("%d", fileID);
	CString temp2;
	temp2.Format("%d", showID);
	
	//create the query:
	sqlQuery = "CALL p_logFile(";
	sqlQuery.Append(temp);
	sqlQuery.Append(", ");
	sqlQuery.Append(temp2 + ");");

	//do the query:
	doQuery(sqlQuery);
}

//private methods:

//connect() - create a new MySQL object and connect it to the
//database:
bool CFusicMainModel::connect()
{
	if (!isConnected) {
		//construct the object:
		m_pMySQLConn = new mysqlpp::Connection;

		//connect to the database:
		isConnected = m_pMySQLConn->connect(g_sctDBSettings.strDBDatabase,
			g_sctDBSettings.strDBHost,
			g_sctDBSettings.strDBUser,
			g_sctDBSettings.strDBPassword);

	}
	return isConnected;
}

//disconnect - disconnect from the database:
void CFusicMainModel::disconnect()
{
	if(m_pMySQLConn != NULL)
	{
		//close the connection:
		m_pMySQLConn->disconnect();

		//delete the object:
		delete m_pMySQLConn;

		//reset the pointer:
		m_pMySQLConn = NULL;
	}
	isConnected = false;
}

//StoreQueryResult - internal function to do all queries:
mysqlpp::StoreQueryResult CFusicMainModel::doQuery(CString SQLQuery)
{
	//variables:
	mysqlpp::StoreQueryResult res;

	if(m_pMySQLConn == NULL)
	{
		//hanvn't connected, return empty results set:
		return res;
	}

	if(!m_pMySQLConn->connected())
	{
		//not connected:
		return res;
	}

	//do the query on the server:
	mysqlpp::Query query = m_pMySQLConn->query(SQLQuery);

	//store the results set:
	res = query.store();

	//return the results set:
	return res;
}

bool CFusicMainModel::fnCheckMachineOnAir()
{
	if(!connect())
	{
		return false;
	}
	CString sqlQuery;
	sqlQuery = "SELECT * from tbl_settings WHERE Setting_Name = 'onair_system_id';";
	mysqlpp::StoreQueryResult res = doQuery(sqlQuery);
	if(res[0]["Setting_Value"] == mysqlpp::null)
	{
		return false;
	}
	else
	{
		if((CString)res[0]["Setting_Value"] == g_systemID)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void CFusicMainModel::fnSetMachineOnAir()
{

	if(!connect())
	{
		return;
	}

	mysqlpp::Query q = m_pMySQLConn->query();

	q << "UPDATE tbl_settings SET Setting_Value = '";
	q << mysqlpp::escape << g_systemID.GetBuffer();
	q << "' WHERE Setting_Name = 'onair_system_id';";
	g_systemID.ReleaseBuffer();
	
	q.exec();
}
