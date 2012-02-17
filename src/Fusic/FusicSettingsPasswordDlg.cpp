// FusicSettingsPasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicSettingsPasswordDlg.h"
#include "FusicSettingsDlg.h"		//needed for settings dialog.

// CFusicSettingsPasswordDlg dialog

IMPLEMENT_DYNAMIC(CFusicSettingsPasswordDlg, CDialog)

CFusicSettingsPasswordDlg::CFusicSettingsPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFusicSettingsPasswordDlg::IDD, pParent)
{

}

CFusicSettingsPasswordDlg::~CFusicSettingsPasswordDlg()
{
}

void CFusicSettingsPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_ctlEditPassword);
}


BEGIN_MESSAGE_MAP(CFusicSettingsPasswordDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_OK, &CFusicSettingsPasswordDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CFusicSettingsPasswordDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CFusicSettingsPasswordDlg message handlers

extern CString g_strSettingsPassword;

void CFusicSettingsPasswordDlg::OnBnClickedBtnOk()
{
	CString strPassword;
	// TODO: Add your control notification handler code here
	//check the password:
	m_ctlEditPassword.GetWindowTextA(strPassword);
	if(strPassword == g_strSettingsPassword)
	{
		//show the settings dialog
		CFusicSettingsDlg dlgSettings(this);
		ShowWindow(SW_HIDE);
		dlgSettings.DoModal();
		OnOK();
	}
	else
	{
		MessageBox("Invalid Password.", "Fusic Settings", MB_OK | MB_ICONERROR);
	}
}

void CFusicSettingsPasswordDlg::OnBnClickedBtnCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CFusicSettingsPasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//set the password text box to focus:
	m_ctlEditPassword.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
