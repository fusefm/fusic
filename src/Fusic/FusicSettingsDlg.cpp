// FusicSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicSettingsDlg.h"
#include "FusicSettingsAccessGroupsDlg.h"
#include "Globals.h"		//needed for accessing global db settings
#include "mysql++.h"		//needed for performing a SQL connection test.
#include "bass.h"			//needed to get card data.
#include "md5Capi.h"		//needed for MD5 functions.
#include <vector>			//needed for vector functonality.

//defince global options:
extern CString g_strSettingsPassword;
extern dbSettings g_sctDBSettings;
extern int g_intCartsDev;
extern int g_intMusicDev;
extern CString g_strLDAPHost;
extern CString g_strLDAPSearchAttri;
extern CString g_StrLDAPSearchBase;
extern std::vector<CString> g_vecAG;
extern CString g_strFaderStartEXE;
extern int faderStartAddress;
extern bool faderStart;

// CFusicSettingsDlg dialog

IMPLEMENT_DYNAMIC(CFusicSettingsDlg, CDialog)

CFusicSettingsDlg::CFusicSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFusicSettingsDlg::IDD, pParent)
{

}

CFusicSettingsDlg::~CFusicSettingsDlg()
{
}

void CFusicSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_ctlSettingsPassword);
	DDX_Control(pDX, IDC_EDIT_DB_HOST, m_ctlDBHost);
	DDX_Control(pDX, IDC_EDIT_DB_DATABASE, m_ctlDBDatabase);
	DDX_Control(pDX, IDC_EDIT_DB_USERNAME, m_ctlDBUserName);
	DDX_Control(pDX, IDC_EDIT_DB_PASSWORD, m_ctlDBPassword);
	DDX_Control(pDX, IDC_COMBO_SOUND_CART, m_ctlSndCmboCarts);
	DDX_Control(pDX, IDC_COMBO_SOUND_MUSIC, m_ctlSndCmboMusic);
	DDX_Control(pDX, IDC_EDIT_LDAP_HOST, m_ctlLDAPHost);
	DDX_Control(pDX, IDC_EDIT_LDAP_BINDATTRIBUTE, m_ctlLDAPBindAttribute);
	DDX_Control(pDX, IDC_EDIT_LDAP_BASE, m_ctlLDAPBindBase);
	DDX_Control(pDX, IDC_STATIC_AGCOUNT, m_ctlLDAPAccessGroupsCount);
	DDX_Control(pDX, IDC_EDIT_FADERSTART_ADDRESS, m_ctlFaderStartAddress);
	DDX_Control(pDX, IDC_CHECK_ENABLE_FADERSTART, m_ctlCheckFaderStart);
}


BEGIN_MESSAGE_MAP(CFusicSettingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFusicSettingsDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_SOUND_CART, &CFusicSettingsDlg::OnCbnSelchangeComboSoundCart)
	ON_BN_CLICKED(IDC_BTN_EDITACCESSGROUPS, &CFusicSettingsDlg::OnBnClickedEditaccessgroups)
	ON_BN_CLICKED(IDC_BTN_FADERSTART_BROWSE, &CFusicSettingsDlg::OnBnClickedBtnFaderstartBrowse)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_FADERSTART, &CFusicSettingsDlg::OnBnClickedCheckEnableFaderstart)
END_MESSAGE_MAP()


// CFusicSettingsDlg message handlers

BOOL CFusicSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//populate the dialog with current settings (if there are any).
	if(g_strSettingsPassword != "")
	{
		//populate with settings:
		m_ctlSettingsPassword.SetWindowTextA(g_strSettingsPassword);
		m_ctlDBDatabase.SetWindowTextA(g_sctDBSettings.strDBDatabase);
		m_ctlDBHost.SetWindowTextA(g_sctDBSettings.strDBHost);
		m_ctlDBUserName.SetWindowTextA(g_sctDBSettings.strDBUser);
		m_ctlDBPassword.SetWindowTextA(g_sctDBSettings.strDBPassword);
		m_ctlLDAPBindAttribute.SetWindowTextA(g_strLDAPSearchAttri);
		m_ctlLDAPBindBase.SetWindowTextA(g_StrLDAPSearchBase);
		m_ctlLDAPHost.SetWindowTextA(g_strLDAPHost);
		CString address;
		address.Format("%x", faderStartAddress);
		m_ctlFaderStartAddress.SetWindowTextA(address);
		if(faderStart)
		{
			m_ctlCheckFaderStart.SetCheck(BST_CHECKED);
			m_ctlFaderStartAddress.EnableWindow(true);
		}
		else
		{
			m_ctlCheckFaderStart.SetCheck(BST_UNCHECKED);
			m_ctlFaderStartAddress.EnableWindow(false);

		}
	}
	//populate the sound comobo boxes:
	BASS_DEVICEINFO devInfo;
	for(int i = 1; BASS_GetDeviceInfo(i, &devInfo); i++)
	{
		m_ctlSndCmboCarts.InsertString((i - 1), devInfo.name);
		m_ctlSndCmboMusic.InsertString((i - 1), devInfo.name);
	}

	//set combo boxes to current values:
	if(g_intCartsDev != 0)
	{
		m_ctlSndCmboCarts.SetCurSel(g_intCartsDev - 1);
	}

	if(g_intMusicDev != 0)
	{
		m_ctlSndCmboMusic.SetCurSel(g_intMusicDev - 1);
	}

	//set the number of access groups:
	int nSize;
	CString strSize;
	nSize = g_vecAG.size();

	strSize.Format("%i", nSize);

	//set the size:
	m_ctlLDAPAccessGroupsCount.SetWindowTextA(strSize);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFusicSettingsDlg::OnBnClickedOk()
{
	CString strSettingsPassword;
	CString strDBUsername;
	CString strDBPassword;
	CString strDBHost;
	CString strDBDatbase;
	CString strLDAPHost;
	CString strLDAPBindAttribute;
	CString strLDAPBindBase;
	CString strFaderStartAddress;
	bool doFaderStart;

	//get the options:
	m_ctlSettingsPassword.GetWindowTextA(strSettingsPassword);
	m_ctlDBDatabase.GetWindowTextA(strDBDatbase);
	m_ctlDBHost.GetWindowTextA(strDBHost);
	m_ctlDBPassword.GetWindowTextA(strDBPassword);
	m_ctlDBUserName.GetWindowTextA(strDBUsername);
	m_ctlLDAPHost.GetWindowTextA(strLDAPHost);
	m_ctlLDAPBindAttribute.GetWindowTextA(strLDAPBindAttribute);
	m_ctlLDAPBindBase.GetWindowTextA(strLDAPBindBase);

	//ensure that there are sound cards selected:
	if(m_ctlSndCmboCarts.GetCurSel() == CB_ERR)
	{
		MessageBox("Please select a sound card for Carts sound output.",
			"Fusic Settings", MB_OK | MB_ICONERROR);
		return;
	}

	if(m_ctlSndCmboMusic.GetCurSel() == CB_ERR)
	{
		MessageBox("Please select a sound card for Music output.",
			"Fusic Settings", MB_OK | MB_ICONERROR);
		return;
	}

	//get the sound options:
	int intCartsOutputDev;
	int intMusicOutputDev;
	CString strHash, strDevString;
	BASS_DEVICEINFO devInfo;
	Cmd5Capi md5;

	intCartsOutputDev = m_ctlSndCmboCarts.GetCurSel() + 1;	//(+1 since of 0 no audio dev).
	intMusicOutputDev = m_ctlSndCmboMusic.GetCurSel() + 1;

	//create the dev string:
	for(int devi = 0; BASS_GetDeviceInfo(devi, &devInfo); devi++)
	{
		strDevString += devInfo.name;
	}

	//do the hash:
	strHash = md5.Digest(strDevString);

	//make sure that all options are set:
	if(strSettingsPassword == "")
	{
		MessageBox("Please enter a settings password.", "Fusic Settings",
			MB_OK | MB_ICONERROR);
		m_ctlSettingsPassword.SetFocus();
		return;
	}
	if(strDBDatbase == "")
	{
		MessageBox("Please enter a Fusic database.", "Fusic Settings",
			MB_OK | MB_ICONERROR);
		m_ctlDBDatabase.SetFocus();
		return;
	}
	if(strDBHost == "")
	{
		MessageBox("Please enter a database host.", "Fusic Settings",
			MB_OK | MB_ICONERROR);
		m_ctlDBHost.SetFocus();
		return;
	}
	if(strDBPassword == "")
	{
		MessageBox("Please enter a database password.", "Fusic Settings",
			MB_OK | MB_ICONERROR);
		m_ctlDBPassword.SetFocus();
		return;
	}
	if(strDBUsername == "")
	{
		MessageBox("Please enter a database username.", "Fusic Settings",
			MB_OK | MB_ICONERROR);
		m_ctlDBUserName.SetFocus();
		return;
	}

	if(strLDAPBindBase == "")
	{
		MessageBox("Please enter a LDAP Bind Base DN.", "Fusic Settings",
			MB_OK | MB_ICONERROR);
		m_ctlLDAPBindBase.SetFocus();
		return;
	}

	if(strLDAPBindAttribute == "")
	{
		MessageBox("Please Enter a LDAP Bind Prefix.", "Fusic Settings",
			MB_OK | MB_ICONERROR);
		m_ctlLDAPBindAttribute.SetFocus();
		return;
	}

	if(strLDAPHost == "")
	{
		MessageBox("Please enter a LDAP Host.", "Fusic Settings",
			MB_OK | MB_ICONERROR);
		m_ctlLDAPHost.SetFocus();
		return;
	}

	int theIntAddress;
	CString theAddress;
	if(m_ctlCheckFaderStart.GetCheck() == BST_UNCHECKED)
	{
		doFaderStart = false;
	}
	else
	{
		doFaderStart = true;
		m_ctlFaderStartAddress.GetWindowTextA(theAddress);
		if(theAddress.IsEmpty())
		{
			MessageBox("Please enter a port base address if you want to "
				"use fader start.", "Fusic Settings", MB_OK | MB_ICONERROR);
			return;
		}
		//we got an address, convert it to an int value;
		sscanf_s(theAddress.GetBuffer(), "%x",	&theIntAddress);
		theAddress.ReleaseBuffer();

	}

	//see if we can connect with the options specified:
	mysqlpp::Connection conn(false);

	if(!conn.connect(strDBDatbase, strDBHost, strDBUsername,
		strDBPassword))
	{
		//we didn't connect:
		CString strError;
		strError = "Could not connect to the database: ";
		strError += conn.error();
		strError += ". Please check your connection settings.";
		MessageBox(strError, "Fusic Settings", MB_OK | MB_ICONERROR);
		m_ctlDBDatabase.SetFocus();
		return;
	}



	//we connected:
	conn.disconnect();
	MessageBox("Datbase connection successful, saving settings.",
		"Fusic Settings", MB_OK | MB_ICONINFORMATION);

	//store registry settings:
	CFusicApp* app = (CFusicApp*)AfxGetApp();

	//store settings password:
	app->WriteProfileStringA("", "SettingsPassword", strSettingsPassword);

	//store the fader start settings:

	//store db settings:
	app->WriteProfileStringA("Database Settings", "Host", strDBHost);
	app->WriteProfileStringA("Database Settings", "Database", strDBDatbase);
	app->WriteProfileStringA("Database Settings", "Username", strDBUsername);
	app->WriteProfileStringA("Database Settings", "Password", strDBPassword);

	//store LDAP Settings:
	app->WriteProfileStringA("LDAP Settings", "Host", strLDAPHost);
	app->WriteProfileStringA("LDAP Settings", "BindAttribute", strLDAPBindAttribute);
	app->WriteProfileStringA("LDAP Settings", "BindBase", strLDAPBindBase);

	//store sound settings:
	app->WriteProfileStringA("Sound Settings", "SoundCardHash", strHash);
	app->WriteProfileInt("Sound Settings", "CartsOutputDev", intCartsOutputDev);
	app->WriteProfileInt("Sound Settings", "MusicOutputDev", intMusicOutputDev);

	//write out fader start settings:
	if(doFaderStart == false)
	{
		app->WriteProfileInt("Fader Start Settings", "FaderStartEnabled", 0);
		app->WriteProfileInt("Fader Start Settings", "FaderStartAdderss", 0);
	}
	else
	{
		app->WriteProfileInt("Fader Start Settings", "FaderStartEnabled", 1);
		app->WriteProfileInt("Fader Start Settings", "FaderStartAddress", theIntAddress);
	}

	//set the global settings:
	g_strSettingsPassword = strSettingsPassword;
	g_sctDBSettings.strDBDatabase = strDBDatbase;
	g_sctDBSettings.strDBHost = strDBHost;
	g_sctDBSettings.strDBPassword = strDBPassword;
	g_sctDBSettings.strDBUser = strDBUsername;
	g_intCartsDev = intCartsOutputDev;
	g_intMusicDev = intMusicOutputDev;
	g_strLDAPHost = strLDAPHost;
	g_strLDAPSearchAttri = strLDAPBindAttribute;
	g_StrLDAPSearchBase = strLDAPBindBase;
	faderStart = true;
	faderStartAddress = theIntAddress;
	
	//successful:
	OnOK();
}

void CFusicSettingsDlg::OnCbnSelchangeComboSoundCart()
{
	// TODO: Add your control notification handler code here
}

void CFusicSettingsDlg::OnBnClickedEditaccessgroups()
{
	//create and show the edit dialog:
	CFusicSettingsAccessGroupsDlg dlg(this);
	dlg.DoModal();

	//when it has finished we need to update the counter:
	int nSize;
	CString strSize;
	nSize = g_vecAG.size();

	strSize.Format("%i", nSize);

	//set the size:
	m_ctlLDAPAccessGroupsCount.SetWindowTextA(strSize);

}

void CFusicSettingsDlg::OnBnClickedBtnFaderstartBrowse()
{
	//create a file chooser:
	CFileDialog dlgFilePicker(TRUE,NULL,NULL,NULL,
		"Executable Files (*.exe)|*.exe|Batch Files(*.bat)|*.bat|"
		"All Files (*.*)|*.*||");

	//show the dialog:
	int intResult = dlgFilePicker.DoModal();

	if(intResult == IDOK)
	{
		m_ctlFaderStartEXE.SetWindowTextA(dlgFilePicker.GetPathName());
	}

}

void CFusicSettingsDlg::OnBnClickedCheckEnableFaderstart()
{
	if(m_ctlCheckFaderStart.GetCheck() == BST_CHECKED)
	{	
		m_ctlFaderStartAddress.EnableWindow(true);
	}
	else
	{
		m_ctlFaderStartAddress.EnableWindow(false);
	}
}