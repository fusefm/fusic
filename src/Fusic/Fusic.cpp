// Fusic.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicLoginDlg.h"
#include "Globals.h"			//needed for global variable access.
#include "FusicSettingsDlg.h"	//needed for access to settings dialog.
#include "bass.h"				//needed for bass functions.
#include "md5Capi.h"			//needed for md5.
#include "FusicSettingsPasswordDlg.h"	//needed for settings password dialog.
#include <vector>

// CFusicApp

BEGIN_MESSAGE_MAP(CFusicApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFusicApp construction

CFusicApp::CFusicApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance.
}


// The one and only CFusicApp object

CFusicApp theApp;

// CFusicApp initialization

//declare some global stuff:
extern CString g_strSettingsPassword;
extern dbSettings g_sctDBSettings;
extern int g_intCartsDev;
extern int g_intMusicDev;
extern CString g_strLDAPHost;
extern CString g_systemID;
extern CString g_strLDAPSearchAttri;
extern CString g_StrLDAPSearchBase;
extern std::vector<CString> g_vecAG;
extern bool faderStart;

BOOL CFusicApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization:

	CString strAccessGroups;	//access groups temp string.
	//need to get global info:
	SetRegistryKey("Fuse FM Software");

	//get the password;
	g_strSettingsPassword = GetProfileString("", "SettingsPassword");
	if(g_strSettingsPassword == "")
	{
		//tell the user:
		MessageBox(NULL, "It seems as though you are running"
			" Fusic for the first time. Please continue to set up"
			" the settings for Fusic.", "Fusic", MB_OK | MB_ICONINFORMATION);
		//run the settings dialog:
		CFusicSettingsDlg dlgSettings;
		dlgSettings.DoModal();
	}
	else
	{
		//get the rest of the settings:

		g_sctDBSettings.strDBHost = 
			GetProfileString("Database Settings", "Host");

		g_sctDBSettings.strDBDatabase = 
			GetProfileString("Database Settings", "Database");

		g_sctDBSettings.strDBUser = 
			GetProfileString("Database Settings", "Username");

		g_sctDBSettings.strDBPassword = 
			GetProfileString("Database Settings", "Password");


		//LDAP Settings:
		g_strLDAPHost = GetProfileString("LDAP Settings", "Host");
		g_strLDAPSearchAttri = GetProfileString("LDAP Settings", "BindAttribute");
		g_StrLDAPSearchBase = GetProfileString("LDAP Settings", "BindBase");
		strAccessGroups = GetProfileString("LDAP Settings", "AccessGroups");

		//get unique string:
		g_systemID = GetProfileString("", "HostString");
		if(g_systemID == "")
		{
			//generate a system ID:
			UUID* uu = new UUID();
			RPC_CSTR sTemp;
			if(uu != NULL)
			{
				UuidCreateSequential(uu);
				UuidToString(uu, &sTemp);
				g_systemID = sTemp;
				RpcStringFree(&sTemp);
				WriteProfileString("", "HostString", g_systemID);
				delete uu;
				uu = NULL;
			}
			else
			{
				MessageBox(NULL, "ERROR: could not create a host string,"
					"On-Air checking disabled.", "Fusic", MB_ICONERROR);
			}
		}

		//sound options:
		g_intCartsDev = GetProfileInt("Sound Settings", "CartsOutputDev",0);
		g_intMusicDev = GetProfileInt("Sound Settings", "MusicOutputDev",0);

		//fader start opts
		int temp = GetProfileInt("Fader Start Settings", "FaderStartEnabled",  0);
		if(temp == 0)
		{
			faderStart = false;
		}
		else
		{
			faderStart = true;
		}

		//make sure that we got all settings:
		if(g_sctDBSettings.strDBDatabase == "")
		{
			MessageBox(NULL, "There is no database defined in settings.",
				"Fusic", MB_OK | MB_ICONERROR);
			return FALSE;
		}

		if(g_sctDBSettings.strDBHost == "")
		{
			MessageBox(NULL, "There is no host defined in settings.",
				"Fusic", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(g_sctDBSettings.strDBPassword == "")
		{
			MessageBox(NULL, "There is no passowrd defined in settings.",
				"Fusic", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(g_sctDBSettings.strDBUser == "")
		{
			MessageBox(NULL, "There is no username defined in settings.",
				"Fusic", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(g_intCartsDev == 0)
		{
			MessageBox(NULL, "There is no carts output device defined in settings.",
				"Fusic", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(g_intMusicDev == 0)
		{
			MessageBox(NULL, "There is no music output device defined in settings.",
				"Fusic", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(g_strLDAPHost == "")
		{
			MessageBox(NULL, "There is no LDAP Host Defined in Settings.", "Fusic",
				MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(g_strLDAPSearchAttri == "")
		{
			MessageBox(NULL, "There is no LDAP Bind Attribute in settings.", "Fusic",
				MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(g_StrLDAPSearchBase == "")
		{
			MessageBox(NULL, "There is no LDAP Bind Base in settings.", "Fusic",
				MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(strAccessGroups == "")
		{
			MessageBox(NULL, "There are no LDAP Access Groups defined.", "Fusic",
				MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}

	//setup the ldap vector:
	int nIndex = strAccessGroups.Find(";");
	while(nIndex != -1)
	{
		//add the string to the vector:
		g_vecAG.push_back(strAccessGroups.Left(nIndex));

		//erase the pushed back string (+1  for the extra ";"):
		strAccessGroups.Delete(0,nIndex + 1);

		//find the next index for the next iteration:
		nIndex = strAccessGroups.Find(";");
	}

	//do the hash sound card check:
	CString strHash, strDevString, strRegHash;
	BASS_DEVICEINFO devInfo;
	Cmd5Capi md5;

	//create the dev string:
	for(int devi = 0; BASS_GetDeviceInfo(devi, &devInfo); devi++)
	{
		strDevString += devInfo.name;
	}

	//do the hash:
	strHash = md5.Digest(strDevString);

	//get the reg hash:
	strRegHash = GetProfileString("Sound Settings", "SoundCardHash", "none");

	//make sure we got a hash
	if(strRegHash == "none")
	{
		MessageBox(NULL, "Warning: Could not get the sound card hash from the registry."
			" Sound card checking disabled.", "Fusic", MB_OK | MB_ICONWARNING);
	}
	else
	{
		//do the sound card check.
		if(strRegHash != strHash)
		{
			//the hash didnt match, tell the user:
			MessageBox(NULL, "Your sound card configuration has changed. Please re-set "
				"sound settings.", "Fusic", MB_OK | MB_ICONINFORMATION);

			//show the settings dialog:
			CFusicSettingsPasswordDlg passDlg;
			passDlg.DoModal();
		}
	}

	//set up bass:
	if(!BASS_Init(g_intCartsDev, 44100, 0, NULL, NULL))
	{
		char a[10];
		_itoa_s(BASS_ErrorGetCode(), a,10 ,10);
		CString strError;
		strError = "Error: could not init sound library. Error code: ";
		strError += a;
		MessageBox(NULL, strError, "Fusic", MB_OK | MB_ICONERROR);
	}

	//load in other modules:
	BASS_PluginLoad("bass_aac.dll", 0);
	BASS_PluginLoad("bassflac.dll", 0);
	BASS_PluginLoad("basswma.dll", 0);
	BASS_PluginLoad("bass_alac.dll", 0);

	//show the login dialog:
	CFusicLoginDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	//ensure that we free up BASS:
	BASS_Free();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}