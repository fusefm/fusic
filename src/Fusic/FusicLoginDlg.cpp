// FusicLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicLoginDlg.h"
#include "FusicMainDlg.h"
#include "FusicShowSelectionDlg.h"
#include "FusicSettingsPasswordDlg.h"	//needed for settings password dialog.
#include <vector>

#define new DEBUG_NEW


// CFusicLoginDlg dialog


//get global vars:
extern CString g_strLDAPHost;
extern CString g_strLDAPSearchAttri;
extern CString g_StrLDAPSearchBase;
extern std::vector<CString> g_vecAG;

CFusicLoginDlg::CFusicLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFusicLoginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFusicLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERID, m_ctlLDAPUser);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_ctlLDAPPassword);
	DDX_Control(pDX, IDC_STATIC_CARTSLIST, m_ctlStaticCartsList);
	DDX_Control(pDX, IDC_STATIC_CARTS_LIST_TIME, m_ctlStaticCartsListTimes);
}

BEGIN_MESSAGE_MAP(CFusicLoginDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CFusicLoginDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_SETTINGS, &CFusicLoginDlg::OnBnClickedBtnSettings)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CFusicLoginDlg::OnBnClickedBtnQuit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFusicLoginDlg message handlers

BOOL CFusicLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctlLDAPUser.SetFocus();

	return FALSE;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFusicLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFusicLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CFusicLoginDlg::fnDoLDAPAuthentication(CString strUserID, 
											CString strPassword)
{
	int intTimeout = 2;
	ULONG lngRetValue = 0;
	int lngLDAPVersion = LDAP_VERSION3;

	//begin LDAP authentication:
	LDAP* ldapHandle;
	ldapHandle = ldap_init(g_strLDAPHost.GetBuffer(), LDAP_PORT);
	if(ldapHandle == NULL)
	{
		//error creating LDAP object:
		CString strError;
		CString strTemp;
		strTemp = ldap_err2string(LdapGetLastError());
		strError = "Error: Could not create LDAP Object.\n"
			"Call: ldap_init Params: ";
		strError += g_strLDAPHost;
		strError += ", LDAP_PORT.\nError String: ";
		strError += strTemp;
		MessageBox(strError, "Fusic Login", MB_OK | MB_ICONERROR);
		return false;
	}

	//set a smaller timeout:
	lngRetValue = ldap_set_option(ldapHandle, LDAP_OPT_TIMELIMIT, &intTimeout);

	if(lngRetValue != LDAP_SUCCESS)
	{
		//error setting ldap options:
		CString strError;
		CString strTemp;
		strTemp = ldap_err2string(lngRetValue);
		strError = "Error: Could not set LDAP options.\n"
			"Call: ldap_set_option Params: ldapHandle, "
			"LDAP_OPT_TIMELIMIT, 5\n";
		strError += "Error String: ";
		strError += strTemp;
		MessageBox(strError, "Fusic Login", MB_OK | MB_ICONERROR);
	}

	//ensure ldap version 3:
	lngRetValue = ldap_set_option(ldapHandle, LDAP_OPT_VERSION, &lngLDAPVersion);

	if(lngRetValue != LDAP_SUCCESS)
	{
		//error setting ldap options:
		CString strError;
		CString strTemp;
		strTemp = ldap_err2string(lngRetValue);
		strError = "Error: Could not set LDAP options.\n"
			"Call: ldap_set_option Params: ldapHandle, "
			"LDAP_OPT_VERSION, LDAP_VERSION3\n";
		strError += "Error String: ";
		strError += strTemp;
		MessageBox(strError, "Fusic Login", MB_OK | MB_ICONERROR);
	}

	//build up the DN:
	CString strBindDN;
	strBindDN = g_strLDAPSearchAttri;
	strBindDN += "=";
	strBindDN += strUserID;
	strBindDN += ",";
	strBindDN += g_StrLDAPSearchBase;

	//attempt the Bind:
	lngRetValue = ldap_simple_bind_s(ldapHandle, strBindDN.GetBuffer(),
		strPassword.GetBuffer());

	//if they entered a invalid username or password show a less
	//gruesome dialog box:
	if(lngRetValue == LDAP_INVALID_CREDENTIALS)
	{
		MessageBox("Invalid user ID or password.", "Fusic Login", MB_OK | MB_ICONERROR);
		ldap_unbind(ldapHandle);
		g_strLDAPHost.ReleaseBuffer();
		strPassword.ReleaseBuffer();
		strUserID.ReleaseBuffer();
		return false;
	}

	if(lngRetValue != LDAP_SUCCESS)
	{
		CString strError;
		CString strTemp;
		strTemp = ldap_err2string(lngRetValue);
		strError = "Error: Could Not login.\nCall: ldap_simple_bind_s."
			" Params: ldapHandle ";
		strError += strBindDN;
		strError += ", strPassword\nError String: ";
		strError += strTemp;
		MessageBox(strError, "Fusic Login", MB_OK | MB_ICONERROR);
		ldap_unbind(ldapHandle);
		g_strLDAPHost.ReleaseBuffer();
		strPassword.ReleaseBuffer();
		strUserID.ReleaseBuffer();
		return false;
	}


	bool bolFoundUserInAccessGroup = false;
	for(std::vector<CString>::iterator i = g_vecAG.begin(); 
		i != g_vecAG.end(); i++)
	{
		LDAPMessage* resChain = NULL;	//to get LDAP results.
		LDAPMessage* res = NULL;		//the individual result.
		struct berval **val;			//for storing an array of users for an LDAP group.

		//create the search string:
		CString strSearchString;
		strSearchString = "(cn=";
		strSearchString += *i;
		strSearchString += ")";

		//make sure that the user is in the access groups:
		lngRetValue = ldap_search_ext_s(ldapHandle, g_StrLDAPSearchBase.GetBuffer(),
			LDAP_SCOPE_SUBTREE, strSearchString.GetBuffer(), NULL, 0, NULL, NULL, NULL,
			0, &resChain);

		//print out error message if it failed:
		if(lngRetValue != LDAP_SUCCESS)
		{
			CString strError;
			CString strTemp;
			strTemp = ldap_err2string(lngRetValue);
			strError = "Error: Could Not do search.\nCall: ldap_search_ext_s.";
			strError += "\nError String: ";
			strError += strTemp;
			MessageBox(strError, "Fusic Login", MB_OK | MB_ICONERROR);
			ldap_unbind(ldapHandle);
			g_strLDAPHost.ReleaseBuffer();
			strPassword.ReleaseBuffer();
			strUserID.ReleaseBuffer();
			g_StrLDAPSearchBase.ReleaseBuffer();
			strSearchString.ReleaseBuffer();
			return false;
		}
		
		//make sure we got a group:
		if(resChain == NULL)
		{
			MessageBox("Error: resChain is NULL pointer.", "Fusic Login",
				MB_OK | MB_ICONERROR);
						ldap_unbind(ldapHandle);
			g_strLDAPHost.ReleaseBuffer();
			strPassword.ReleaseBuffer();
			strUserID.ReleaseBuffer();
			g_StrLDAPSearchBase.ReleaseBuffer();
			strSearchString.ReleaseBuffer();
			return false;
		}
		
		//get the first LDAP result (we are only expecting to get one):
		res = ldap_first_entry(ldapHandle, resChain);
		
		//make sure we got a result:
		if(res == NULL)
		{
			MessageBox("Error: res is NULL pointer.", "Fusic Login",
				MB_OK | MB_ICONERROR);
						ldap_unbind(ldapHandle);
			g_strLDAPHost.ReleaseBuffer();
			strPassword.ReleaseBuffer();
			strUserID.ReleaseBuffer();
			g_StrLDAPSearchBase.ReleaseBuffer();
			strSearchString.ReleaseBuffer();
			return false;
		}

		//get all "memberUid" attributes (an array of user ids 
		//that are in the group):
		val = ldap_get_values_len(ldapHandle, res, "memberUid");

		if(val == NULL)
		{
			MessageBox("Error: val is NULL pointer.", "Fusic Login",
				MB_OK | MB_ICONERROR);
						ldap_unbind(ldapHandle);
			g_strLDAPHost.ReleaseBuffer();
			strPassword.ReleaseBuffer();
			strUserID.ReleaseBuffer();
			g_StrLDAPSearchBase.ReleaseBuffer();
			strSearchString.ReleaseBuffer();
			return false;
		}

		int nNumberOfResults;
		nNumberOfResults =	ldap_count_values_len(val);
		for(int i = 0; i < nNumberOfResults; i++)
		{
			char* strTemp;
			strTemp = strUserID.GetBuffer();
			if(strcmp(strTemp, val[i]->bv_val) == 0)
			{
				//we have found the user in one of the access groups:
				bolFoundUserInAccessGroup = true;

				//free RAM:
				ldap_value_free_len(val);
				ldap_msgfree(resChain);
				g_StrLDAPSearchBase.ReleaseBuffer();
				strSearchString.ReleaseBuffer();
				break;
			}
		}

		//break out of this loop if we found the user:
		if(bolFoundUserInAccessGroup == true)
			break;

		//the user was not found for this group, free memory and set
		//pointers back to NULL.
		ldap_value_free_len(val);
		ldap_msgfree(resChain);
		g_StrLDAPSearchBase.ReleaseBuffer();
		strSearchString.ReleaseBuffer();
		res = NULL;
		resChain = NULL;
	}


	//if we got to this point then LDAP has authenticated the user.
	//close down LDAP:
	//unbind:
	lngRetValue = ldap_unbind_s(ldapHandle);

	if(lngRetValue != LDAP_SUCCESS)
	{
		CString strError;
		CString strTemp;
		strTemp = ldap_err2string(lngRetValue);
		strError = "Error: Could not unbind from LDAP.\nCall:"
			"ldap_unbind. Params: ldapHandle.\nErrorString: ";
		strError += strTemp;
		MessageBox(strError, "Fusic Login", MB_OK | MB_ICONERROR);
		return false;
	}

	//release all buffers:
	g_strLDAPHost.ReleaseBuffer();
	strUserID.ReleaseBuffer();
	strPassword.ReleaseBuffer();

	//successfully authenintaced user:
	return bolFoundUserInAccessGroup;
}

void CFusicLoginDlg::OnBnClickedBtnLogin()
{
	int intTimeout = 5;
	ULONG lngRetValue = 0;
	int lngLDAPVersion = LDAP_VERSION3;
	CString strUserID;
	CString strPassword;

	//get login details:
	m_ctlLDAPPassword.GetWindowTextA(strPassword);
	m_ctlLDAPUser.GetWindowTextA(strUserID);

	//make sure we have some details:
	if(strUserID == "")
	{
		MessageBox("Please Enter a User ID.", "Fusic Login", MB_OK | MB_ICONERROR);
		m_ctlLDAPUser.SetFocus();
		return;
	}
	if(strPassword == "")
	{
		MessageBox("Please enter a password.", "Fusic Login", MB_OK | MB_ICONERROR);
		m_ctlLDAPPassword.SetFocus();
		return;
	}

	//do LDAP auth:	
	if(fnDoLDAPAuthentication(strUserID, strPassword) == false)
	{
		//if authentication falied dont show anything else:
		MessageBox("Access Denied.", "Fusic Login", MB_OK | MB_ICONERROR);
		m_ctlLDAPUser.SetFocus();
		return;
	}

	//show the show selection dialog:
	CFusicShowSelectionDlg dlgShowSelect(this, strUserID);
	if(dlgShowSelect.DoModal() == IDCANCEL)
	{
		//clear the username and password text fields for next login:
		m_ctlLDAPPassword.SetWindowTextA("");
		m_ctlLDAPUser.SetWindowTextA("");
		m_ctlLDAPUser.SetFocus();
		return;
	}

	//clear the username and password text fields for next login:
	m_ctlLDAPPassword.SetWindowTextA("");
	m_ctlLDAPUser.SetWindowTextA("");
	m_ctlLDAPUser.SetFocus();

	//hide the login window:  
	ShowWindow(SW_HIDE);

	//create the main dialog:
	CFusicMainDlg dlgFusicMain(this, this);
	dlgFusicMain.DoModal();
	
	//show after the user has logged out of the dialog:
	ShowWindow(SW_SHOW);
}

void CFusicLoginDlg::OnBnClickedBtnSettings()
{
	//need to show password dialog
	ShowWindow(SW_HIDE);
	CFusicSettingsPasswordDlg passDlg;
	passDlg.DoModal();
	ShowWindow(SW_SHOW);
}

void CFusicLoginDlg::OnBnClickedBtnQuit()
{
	//ensure that the user wishes to quit:
	int response = MessageBox("Are you sure that you wish to quit?", "Fusic Login", 
		MB_YESNO | MB_ICONINFORMATION);

	if(response == IDYES)
	{
		//go ahead and quit:
		OnCancel();
	}
}


//next comes all the stuff to do with the logging out but keeping carts
//paying on the login screen:

void CFusicLoginDlg::fnAddCartToList(SCartsList CartListElementToAdd)
{
	if((int)m_vecCartsList.size() == 0)
	{
		//if we are about to add a first elemet we need to start off the timer:
		SetTimer(IDT_CART_LOGIN_LIST, 50, NULL);
		m_vecCartsList.push_back(CartListElementToAdd);
		//realign thetext on the static control:
		//m_ctlStaticCartsList.a
	}
	else
	{
		//otherwise we need to resize the window and also remove the element:
		RECT dlgRect;
		RECT editRect;
		RECT editTimeRect;
		GetWindowRect(&dlgRect);
		m_ctlStaticCartsList.GetWindowRect(&editRect);
		m_ctlStaticCartsListTimes.GetWindowRect(&editTimeRect);
		

		//increase the size of the dialog:
		dlgRect.bottom = dlgRect.bottom + 15;
		SetWindowPos(NULL,0,0,(dlgRect.right - dlgRect.left), 
			(dlgRect.bottom - dlgRect.top), SWP_NOMOVE);

		//increase the size of the edit box:
		editRect.bottom = editRect.bottom + 15;
		m_ctlStaticCartsList.SetWindowPos(NULL, 0,0,(editRect.right - editRect.left),
			(editRect.bottom - editRect.top), SWP_NOMOVE);
		
		//increase the size of the time edit box:
		editTimeRect.bottom = editTimeRect.bottom + 15;
		m_ctlStaticCartsListTimes.SetWindowPos(NULL, 0,0,(editTimeRect.right - 
			editTimeRect.left), (editTimeRect.bottom - editTimeRect.top), SWP_NOMOVE);


		//add the element:
		m_vecCartsList.push_back(CartListElementToAdd);
	}
}
void CFusicLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == IDT_CART_LOGIN_LIST)
	{
		CString strCartsList;
		CString strTimeCartsList;
		//update the display with the time let for each cart:
		for(std::vector<SCartsList>::iterator i = m_vecCartsList.begin();
			i != m_vecCartsList.end(); i++)
		{
			//calculate the time left:
			//we have a countdown timer, find out where we are through the stream:
			CString strTime;
			double dblTimeLeft;
			double dblStreamLength;

			//calculate the length:
			dblStreamLength = BASS_ChannelBytes2Seconds(i->hstCartsStream,
				BASS_ChannelGetLength(i->hstCartsStream, BASS_POS_BYTE));

			//get the time left
			dblTimeLeft = dblStreamLength - BASS_ChannelBytes2Seconds(i->hstCartsStream,
				BASS_ChannelGetPosition(i->hstCartsStream, BASS_POS_BYTE));

			//format the string:
			strTime.Format("%.1f", dblTimeLeft);

			//format the string:
			strCartsList += i->strTitle;
			strCartsList += "\n";

			strTimeCartsList += strTime;
			strTimeCartsList += "\n";
		}
		//set the string:
		m_ctlStaticCartsList.SetWindowTextA(strCartsList);
		m_ctlStaticCartsListTimes.SetWindowTextA(strTimeCartsList);
	}

	CDialog::OnTimer(nIDEvent);
}

void CALLBACK CFusicLoginDlg::fnLoginDialogCartCallback(HSYNC handle, 
	DWORD channel, DWORD data, void* user)
{
	RECT rctDialog;
	RECT rctStaticNames;
	RECT rctStaticTimes;

	CFusicLoginDlg* pDlg = (CFusicLoginDlg*)user;
	//stop the cart and free the resources:
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);

	//remove the stream from the vector:
	for(std::vector<SCartsList>::iterator i = pDlg->m_vecCartsList.begin();
		i != pDlg->m_vecCartsList.end(); i++)
	{
		if(i->hstCartsStream == channel)
		{
			//delete the element from the array:
			pDlg->m_vecCartsList.erase(i);
			break;
		}
	}
	if(pDlg->m_vecCartsList.size() != 0)
	{
		//get rects:
		pDlg->GetWindowRect(&rctDialog);
		pDlg->m_ctlStaticCartsList.GetWindowRect(&rctStaticNames);
		pDlg->m_ctlStaticCartsListTimes.GetWindowRect(&rctStaticTimes);

		//resize the rects:
		rctDialog.bottom -= 15;
		rctStaticNames.bottom -= 15;
		rctStaticTimes.bottom -= 15;

			
		//reposostion the window:
		pDlg->SetWindowPos(NULL, 0, 0, (rctDialog.right - rctDialog.left),
		(rctDialog.bottom - rctDialog.top), SWP_NOMOVE);
	
		pDlg->m_ctlStaticCartsList.SetWindowPos(NULL, 0,0, (rctStaticNames.right - 
		rctStaticNames.left), (rctStaticNames.bottom - rctStaticNames.top), SWP_NOMOVE);

		pDlg->m_ctlStaticCartsListTimes.SetWindowPos(NULL, 0,0,(rctStaticTimes.right - 
		rctStaticTimes.left), (rctStaticTimes.bottom - rctStaticTimes.top), SWP_NOMOVE);
	}
	else
	{
		//set the window text back:
		pDlg->m_ctlStaticCartsList.SetWindowTextA("No Carts Currently Playing.");

		//reset the timer list:
		pDlg->m_ctlStaticCartsListTimes.SetWindowTextA("");

		//stop the timer:
		pDlg->KillTimer(IDT_CART_LOGIN_LIST);
	}
}

