// FusicShowSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "Fusic.h"
#include "FusicShowSelectionDlg.h"
#include <mysql++.h>

//get global settings:
extern dbSettings g_sctDBSettings;
extern int g_intShowID;

// CFusicShowSelectionDlg dialog

IMPLEMENT_DYNAMIC(CFusicShowSelectionDlg, CDialog)

CFusicShowSelectionDlg::CFusicShowSelectionDlg(CWnd* pParent, 
											   CString strUserID)
	: CDialog(CFusicShowSelectionDlg::IDD, pParent)
{
	//set the show ID passed thorough the construction:
	m_strUserID = strUserID;
}

CFusicShowSelectionDlg::~CFusicShowSelectionDlg()
{
}

void CFusicShowSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOWS, m_ctlListShows);
}


BEGIN_MESSAGE_MAP(CFusicShowSelectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFusicShowSelectionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFusicShowSelectionDlg message handlers

BOOL CFusicShowSelectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//connect to MySQL:
	mysqlpp::Connection conn(false);
	if(!conn.connect(g_sctDBSettings.strDBDatabase, g_sctDBSettings.strDBHost,
		g_sctDBSettings.strDBUser, g_sctDBSettings.strDBPassword))
	{
		//we didnt connect:
		CString strError;
		strError = "Error: could not connecto the mysql database: ";
		strError += conn.error();
		strError += ".";
		MessageBox(strError, "Carts Pane", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//do the query for all shows for the given user ID:
	CString strSQL;
	strSQL = "SELECT * FROM tbl_show_users WHERE `User_ID` = '";
	strSQL += m_strUserID;
	strSQL += "';";

	//do the query;
	mysqlpp::Query query = conn.query(strSQL);

	//get the result:
	mysqlpp::StoreQueryResult res = query.store();

	//Only show the Fuse show if a person doesn't have another show.
	if((int)res.size() == 0)
	{
		showStruct fuseShow;
		fuseShow.strShowID = "0";
		fuseShow.strShowName = "Fuse FM (Default)";
		m_vecShows.push_back(fuseShow);
	}

	//loop through the results and store them:
	for(size_t i = 0; i < res.num_rows(); i++)
	{
		showStruct showTemp;

		//set default vars:
		showTemp.nIndex = 0;
		showTemp.strShowName = "";

		//get the show ID:
		showTemp.strShowID = (CString)res[i]["Show_ID"];

		//push back to the vector:
		m_vecShows.push_back(showTemp);
	}

	//get show Names:
	for(std::vector<showStruct>::iterator j = m_vecShows.begin();
		j != m_vecShows.end(); j++)
	{
		//build up query:
		CString strQuery;
		strQuery = "SELECT * FROM tbl_show WHERE `Show_ID`='";
		strQuery += j->strShowID;
		strQuery += "';";
		//do it:
		mysqlpp::Query nameQuery = conn.query(strQuery);
		mysqlpp::StoreQueryResult resName = nameQuery.store();

		//make sure we have some results before attempting to get them:
		if(resName.num_rows() == (size_t)1)
		{
			//get the results:
			j->strShowName = (CString)resName[0]["Show_Name"];
		}
	}
	
	//build up the list control:
	for(std::vector<showStruct>::iterator k = m_vecShows.begin();
		k != m_vecShows.end(); k++)
	{
		int index;
		//add each item to the list:
		index = m_ctlListShows.AddString(k->strShowName);

		//update the vector with the index posistion:
		k->nIndex = index;
	}

	m_ctlListShows.SetCurSel(0);
	m_ctlListShows.SetFocus();

	//disconnect from MySQL;
	conn.disconnect();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFusicShowSelectionDlg::OnBnClickedOk()
{
	//ensure that the user selected a show:
	CString strSelection;
	int selIndex = m_ctlListShows.GetCurSel();

	//make sure something is selected:	
	if(selIndex == LB_ERR)
	{
		MessageBox("Please select a show.", "Fusic Show Selection",
			MB_OK | MB_ICONINFORMATION);
		return;
	}
	
	//get the string:
	m_ctlListShows.GetText(m_ctlListShows.GetCurSel(),
		strSelection);
	
	//find the correct index in the vector:
	for(std::vector<showStruct>::iterator i = m_vecShows.begin();
		i != m_vecShows.end(); i++)
	{
		if(strSelection == i->strShowName)
		{
			g_intShowID = atoi(i->strShowID);
			break;
		}
	}

	OnOK();
}
