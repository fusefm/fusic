#include "stdafx.h"
#include "Globals.h"
#include <fstream>
#include <map>			//needed for map functions
#include <vector>		//needed for vector functions.

CString g_strSettingsPassword;	//settings password for accessing settigs

//Logging functions
void fnLogOutMsg( const CString& msg ) {
	std::fstream logFile( "log.txt",
		std::fstream::out | std::fstream::app );
	logFile << msg << std::endl;
	logFile.flush();
	logFile.close();
}

dbSettings g_sctDBSettings;		//settings for DB access, populated at
								//app startup.
playoutSettings g_settings;

CString g_strLDAPHost;			//LDAP Host option.
CString g_strLDAPSearchAttri;	//LDAP Search Attribute.
CString g_StrLDAPSearchBase;	//LDAP Search Base.
std::vector<CString> g_vecAG;	//LDAP Access Groups Vector.
bool faderStart;
CString g_systemID;

int g_intUserID = 0;				//the user id that is currently logged on.
int g_intShowID = 0;				//the show ID that is currently in use.
int g_intCartsDev = 0;				//device to use for playback of Carts.
int g_intMusicDev = 0;				//device to use for playback of Music.
T_mapCallback g_mapCartCallbacks;	//the callback map for the carts wall.
bool doLog = false;