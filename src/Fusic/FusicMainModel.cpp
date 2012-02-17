#include "StdAfx.h"
#include "FusicMainModel.h"

CFusicMainModel::CFusicMainModel(void)
{
	//construct a new connection object:
	m_pMySQLConn = NULL;
}

CFusicMainModel::~CFusicMainModel(void)
{
	//ensure that the object is deleted:
	if(m_pMySQLConn != NULL)
	{
		delete m_pMySQLConn;
	}
}

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

	//disconnect from the database:
	disconnect();

	//return the result:
	return res;
}

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
		sqlQuery.Append(";");
		res1 = doQuery(sqlQuery);
	}


	//disconnect from the database:
	disconnect();


	//return the result:
	return res1;
}


mysqlpp::StoreQueryResult CFusicMainModel::fnGetFilesForPlaylistIDAndSearchTitle
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
		sqlQuery.Append(") AND File_Title LIKE '%");
		mysqlpp::Query query = m_pMySQLConn->query();
		query << sqlQuery << mysqlpp::escape << searchString.GetBuffer()
			<< "%';";
		searchString.ReleaseBuffer();
		res1 = query.store();
	}


	//disconnect from the database:
	disconnect();


	//return the result:
	return res1;
}

void CFusicMainModel::fnLogFile(int fileID)
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

	//need the date and time:
	CTime tme = CTime::GetCurrentTime();
	//get the date:
	std::stringstream ss;
	ss << tme.GetYear() << "-" << tme.GetMonth() << "-" << tme.GetDay() <<" "
		<< tme.GetHour() << ":" << tme.GetMinute() << ":" << tme.GetSecond();

	//construct the query:
	sqlQuery = "UPDATE tbl_files SET File_PlayedLast = '";
	sqlQuery.Append(ss.str().c_str());
	sqlQuery.Append("' WHERE File_ID = ");
	sqlQuery.Append(temp);
	sqlQuery.Append(";");

	//do the query:
	doQuery(sqlQuery);

	//disconnect from the database:
	disconnect();
}

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

	//disconnect from the database:
	disconnect();

	//return the result:
	return res;
}
mysqlpp::StoreQueryResult CFusicMainModel::fnGetFilesForPlaylistIDAndSearchArtist
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
		sqlQuery.Append(") AND File_Artist LIKE '%");
		mysqlpp::Query query = m_pMySQLConn->query();
		query << sqlQuery << mysqlpp::escape << searchString.GetBuffer()
			<< "%';";
		searchString.ReleaseBuffer();
		res1 = query.store();
	}


	//disconnect from the database:
	disconnect();


	//return the result:
	return res1;
}

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

	//disconnect from the database:
	disconnect();

	//return the result:
	return res;
}


double CFusicMainModel::fnGetIntroTimeForFileID(int FileID, bool disconnectAfteryQuery)
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

	if(disconnectAfteryQuery == true)
	{
		disconnect();
	}

	if(res[0]["File_Intro"] != mysqlpp::null)
	{
		introTime = (double)res[0]["File_Intro"];
	}

	return introTime;
}

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

	disconnect();

	if(res[0]["File_Duration"] != mysqlpp::null)
	{
		duration = (double)res[0]["File_Duration"];
	}

	return duration;
}


mysqlpp::StoreQueryResult CFusicMainModel::fnGetSweepersForSongID(int fileID)
{
	//the results set:
	mysqlpp::StoreQueryResult res;
	CString sqlQuery;
	CString temp;
	double introTime = 0;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	//first get how long the file is:
	introTime = fnGetIntroTimeForFileID(fileID, false);

	//minimum of 3 secs padding:
	introTime -= 3;

	//convert int to string:
	temp.Format("%.0f", introTime);

	//construct the query:
	sqlQuery = "SELECT * FROM `tbl_files` WHERE File_Type = 'W' AND"
		" Show_ID = 0 AND File_Duration < ";
	sqlQuery.Append(temp);
	sqlQuery.Append(" ORDER BY File_Title");
	sqlQuery.Append(";");

	//do the query:
	res = doQuery(sqlQuery);
	

	//disconnect from the database:
	disconnect();

	//return the result:
	return res;
}

mysqlpp::StoreQueryResult CFusicMainModel::fnGetSongsForShowIDAndSearchStringArtist
	(int showID, CString searchString)
{
		//the results set:
	mysqlpp::StoreQueryResult res;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	
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

	mysqlpp::Query q = m_pMySQLConn->query();

	 q << "SELECT * FROM `tbl_files` WHERE File_Type = 'S' AND"
		" Show_ID = " << showID << " AND File_Artist LIKE '%" 
		<< mysqlpp::escape << searchString.GetBuffer() << "%' ORDER BY File_Artist, File_Title";

	res = q.store();
	searchString.ReleaseBuffer();
	disconnect();

	//return the result:
	return res;
}

mysqlpp::StoreQueryResult CFusicMainModel::fnGetSongsForShowIDAndSearchStringSong
	(int showID, CString searchString)
{
		//the results set:
	mysqlpp::StoreQueryResult res;

	//connect to the database:
	if(!connect())
	{
		return res;
	}

	
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

	mysqlpp::Query q = m_pMySQLConn->query();


	 q << "SELECT * FROM `tbl_files` WHERE File_Type = 'S' AND"
		" Show_ID = " << showID << " AND File_Title LIKE '%" 
		<< mysqlpp::escape << searchString.GetBuffer() << "%' ORDER BY File_Artist, File_Title";

	res = q.store();
	searchString.ReleaseBuffer();
	disconnect();

	//return the result:
	return res;
}
//private methods:

bool CFusicMainModel::connect()
{
	//construct the object:
	m_pMySQLConn = new mysqlpp::Connection;

	//connect to the database:
	return m_pMySQLConn->connect(g_sctDBSettings.strDBDatabase,
		g_sctDBSettings.strDBHost,
		g_sctDBSettings.strDBUser,
		g_sctDBSettings.strDBPassword);
}

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
}

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

	//disconnect from the database:
	disconnect();

	//return the result:
	return res;
}
