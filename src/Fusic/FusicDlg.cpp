// FusicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFusicDlg dialog

CFusicDlg::CFusicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFusicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFusicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFusicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFusicDlg)
	DDX_Control(pDX, IDC_USERPASSWORD, m_txtUserPassword);
	DDX_Control(pDX, IDC_USERID, m_txtUserID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFusicDlg, CDialog)
	//{{AFX_MSG_MAP(CFusicDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNLOGIN, OnBtnlogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFusicDlg message handlers

BOOL CFusicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFusicDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFusicDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFusicDlg::OnBtnlogin() 
{
	//need to access theApp object to get options:
	CFusicApp* objApp = (CFusicApp*)AfxGetApp();
	//get the user name and password from the edit boxes:
	CString strUserID;
	CString strUserPassword;
	m_txtUserID.GetWindowText(strUserID);
	m_txtUserPassword.GetWindowText(strUserPassword);
	
	//do LDAP stuff:
	LDAP* svr; 

	//get the LDAP object:
	svr = ldap_init((char*)(LPCTSTR)objApp->m_sctOptions.m_strOptLDAPServer,
		atoi(objApp->m_sctOptions.m_strOptLDAPPort));
	int retval = LdapGetLastError();


	int LDAPVersion = LDAP_VERSION3;
	ldap_set_option(svr, LDAP_OPT_PROTOCOL_VERSION, &LDAPVersion);

	//now do a bind to the server anonymusly:
	struct berval cred;
	cred.bv_val = NULL;
    cred.bv_len = 0;
	retval = ldap_simple_bind_s(svr, NULL, NULL);
	if (retval != LDAP_SUCCESS)
	{
		MessageBox("Error");
		CString errormsg = ldap_err2string(retval);
	}

	
}
