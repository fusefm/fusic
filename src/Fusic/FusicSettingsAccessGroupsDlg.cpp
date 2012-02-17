// FusicSettingsAccessGroupsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicSettingsAccessGroupsDlg.h"
#include <vector>	//needed for vector access.
//get access to the global vecror:
extern std::vector<CString> g_vecAG;

// CFusicSettingsAccessGroupsDlg dialog

IMPLEMENT_DYNAMIC(CFusicSettingsAccessGroupsDlg, CDialog)

CFusicSettingsAccessGroupsDlg::CFusicSettingsAccessGroupsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFusicSettingsAccessGroupsDlg::IDD, pParent)
{

}

CFusicSettingsAccessGroupsDlg::~CFusicSettingsAccessGroupsDlg()
{
}

void CFusicSettingsAccessGroupsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LDAP_ACCESSGROUP, m_ctlEditAccessGroup);
	DDX_Control(pDX, IDC_LIST_LDAP_ACCESSGROUPS, m_ctlListAccessGroups);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_ctlBtnDelete);
}


BEGIN_MESSAGE_MAP(CFusicSettingsAccessGroupsDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_LDAP_ACCESSGROUPS, &CFusicSettingsAccessGroupsDlg::OnLbnSelchangeListLdapAccessgroups)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CFusicSettingsAccessGroupsDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CFusicSettingsAccessGroupsDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDOK, &CFusicSettingsAccessGroupsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFusicSettingsAccessGroupsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CFusicSettingsAccessGroupsDlg message handlers

BOOL CFusicSettingsAccessGroupsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//change focus to edit box:
	m_ctlEditAccessGroup.SetFocus();

	//disable the delete button untill an item is highlighted:
	m_ctlBtnDelete.EnableWindow(false);

	//see if we have anything in the vector:
	if(g_vecAG.size() == 0)
	{
		//no more initaliseation to be done if nothing is set up.
		return FALSE;
	}

	//add each element of the vector to the list control:
	for(std::vector<CString>::iterator i = g_vecAG.begin(); 
		i != g_vecAG.end(); i++)
	{
		m_ctlListAccessGroups.AddString(*i);
	}


	//backup old vector:
	m_vecO = g_vecAG;

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFusicSettingsAccessGroupsDlg::OnLbnSelchangeListLdapAccessgroups()
{
	m_ctlBtnDelete.EnableWindow(true);
}

void CFusicSettingsAccessGroupsDlg::OnBnClickedButtonDelete()
{
	//get the text of the current slection:
	CString strSelection;
	m_ctlListAccessGroups.GetText(m_ctlListAccessGroups.GetCurSel(),
		strSelection);

	//remove from the vector:
	for(std::vector<CString>::iterator i = g_vecAG.begin();
		i != g_vecAG.end(); i++)
	{
		if((*i) == strSelection)
		{
			g_vecAG.erase(i);
			break;
		}
	}

	//remove from the list:
	m_ctlListAccessGroups.DeleteString(m_ctlListAccessGroups.GetCurSel());

	//set the old text back to the edit box:
	m_ctlEditAccessGroup.SetWindowTextA(strSelection);
	m_ctlEditAccessGroup.SetFocus();

	//disable this button:
	m_ctlBtnDelete.EnableWindow(false);
}

void CFusicSettingsAccessGroupsDlg::OnBnClickedButtonAdd()
{
	CString strAccessGroup;

	//get the access group:
	m_ctlEditAccessGroup.GetWindowTextA(strAccessGroup);

	//ensure the user typed something in:
	if(strAccessGroup == "")
	{
		return;
	}

	//add the string to the list:
	m_ctlListAccessGroups.SetCurSel(
		m_ctlListAccessGroups.AddString(strAccessGroup));

	//add the string to the vector:
	g_vecAG.push_back(strAccessGroup);

	//clear the edit box:
	m_ctlEditAccessGroup.SetWindowTextA("");

	//enable the delete button:
	m_ctlBtnDelete.EnableWindow();
}

void CFusicSettingsAccessGroupsDlg::OnBnClickedOk()
{
	//Ensure the user has added some access groups:
	if(g_vecAG.size() == 0)
	{
		MessageBox("Please enter some LDAP access groups.", "Fusic Settings",
			MB_OK | MB_ICONINFORMATION);
		return;
	}

	//get the app:
	CFusicApp* theApp = (CFusicApp*)AfxGetApp();

	//concaternate the accessgroups string:
	CString strAccessGroups;

	for(std::vector<CString>::iterator i = g_vecAG.begin();
		i != g_vecAG.end(); i++)
	{
		strAccessGroups += (*i);
		strAccessGroups += ";";
	}

	//write out the string:
	theApp->WriteProfileStringA("LDAP Settings", "AccessGroups", strAccessGroups);

	//all done.
	OnOK();
}

void CFusicSettingsAccessGroupsDlg::OnBnClickedCancel()
{
	//set back the old vector:

	g_vecAG = m_vecO;

	OnCancel();
}
