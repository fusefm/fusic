#pragma once
#include "afxwin.h"
#include "CartsButton.h"				//needed for access to cart button functions.
#include "navigationbutton.h"			//needed for access to nav button functions.
#include "StopAllButton.h"				//needed for stop a;l; button access.
#include <map>							//needed for access to std::map functions.
#include "mysql++.h"					//needed for access to mysqlpp functions.
#include "globals.h"					//needed for acccess to global variables.
#include "FusicLoginDlg.h"				//needed for access to login dialog.
#include "colourbutton.h"
class CFusicMainDlg;
#include "FusicMainDlg.h"


//this will hold info about the navigation button streams:
struct SNavigationStreams
{
	std::map<CString, CCartsButton*>::iterator btnIterator;
	HSTREAM stream;
	int intPageNumber;
	HSYNC syncHandle;
};

//get the DB struct:
extern dbSettings g_sctDBSettings;

//get show details:
extern int g_intShowID;

//get sound options:
extern int g_intCartsDev;

//get callback maps:
extern T_mapCallback g_mapCartCallbacks;

//a quick enum to help distringuish between fuse and user carts:
#define FUSECARTS 1
#define USERCARTS 2

//to save typing:
typedef std::map<CString, CCartsButton*> mapCart;
typedef std::map<int, CNavigationButton*> mapNav;

// CFusicCartsDlg dialog class:



class CFusicCartsDlg : public CDialog
{
	DECLARE_DYNAMIC(CFusicCartsDlg)

public:
	CFusicCartsDlg(CWnd* pParent = NULL, CFusicLoginDlg* pLoginDialog = NULL,
		CFusicMainDlg* pMainDlg = NULL);// standard constructor
	virtual ~CFusicCartsDlg();

	//allow CNavigationButton to access protected members:
	friend class CNavigationButton;

// Dialog Data
	enum { IDD = IDD_FUSIC_CARTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

								//public member variables:

	//declare all Cart buttons:
	CCartsButton m_ctlBtnCart11;
	CCartsButton m_ctlBtnCart12;
	CCartsButton m_ctlBtnCart13;
	CCartsButton m_ctlBtnCart14;
	CCartsButton m_ctlBtnCart15;
	CCartsButton m_ctlBtnCart21;
	CCartsButton m_ctlBtnCart22;
	CCartsButton m_ctlBtnCart23;
	CCartsButton m_ctlBtnCart24;
	CCartsButton m_ctlBtnCart25;
	CCartsButton m_ctlBtnCart31;
	CCartsButton m_ctlBtnCart32;
	CCartsButton m_ctlBtnCart33;
	CCartsButton m_ctlBtnCart34;
	CCartsButton m_ctlBtnCart35;
	CCartsButton m_ctlBtnCart41;
	CCartsButton m_ctlBtnCart42;
	CCartsButton m_ctlBtnCart43;
	CCartsButton m_ctlBtnCart44;
	CCartsButton m_ctlBtnCart45;
	CCartsButton m_ctlBtnCart51;
	CCartsButton m_ctlBtnCart52;
	CCartsButton m_ctlBtnCart53;
	CCartsButton m_ctlBtnCart54;
	CCartsButton m_ctlBtnCart55;
	CCartsButton m_ctlBtnCart61;
	CCartsButton m_ctlBtnCart62;
	CCartsButton m_ctlBtnCart63;
	CCartsButton m_ctlBtnCart64;
	CCartsButton m_ctlBtnCart65;
	CCartsButton m_ctlBtnCart71;
	CCartsButton m_ctlBtnCart72;
	CCartsButton m_ctlBtnCart73;
	CCartsButton m_ctlBtnCart74;
	CCartsButton m_ctlBtnCart75;
	CCartsButton m_ctlBtnCart81;
	CCartsButton m_ctlBtnCart82;
	CCartsButton m_ctlBtnCart83;
	CCartsButton m_ctlBtnCart84;
	CCartsButton m_ctlBtnCart85;

	//declare all nav buttons:
	CNavigationButton m_ctlBtnNavTop1;
	CNavigationButton m_ctlBtnNavTop2;
	CNavigationButton m_ctlBtnNavTop3;
	CNavigationButton m_ctlBtnNavTop4;
	CNavigationButton m_ctlBtnNavTop5;
	CNavigationButton m_ctlBtnNavTop6;
	CNavigationButton m_ctlBtnNavTop7;
	CNavigationButton m_ctlBtnNavTop8;
	CNavigationButton m_ctlBtnNavBottom1;
	CNavigationButton m_ctlBtnNavBottom2;
	CNavigationButton m_ctlBtnNavBottom3;
	CNavigationButton m_ctlBtnNavBottom4;
	CNavigationButton m_ctlBtnNavBottom5;
	CNavigationButton m_ctlBtnNavBottom6;
	CNavigationButton m_ctlBtnNavBottom7;
	CNavigationButton m_ctlBtnNavBottom8;

	int m_intCurrnetDialogPageTop;		//the page the dialog is currently displaying (top screen).
	int m_intCurrnetDialogPageBottom;	//the page the dialog is currently displaying (bottom screen)


	//button maps:
	mapCart m_mapBtnCartTop;
	mapCart m_mapBtnCartBottom;
	mapNav m_mapBtnNavTop;
	mapNav m_mapBtnNavBottom;

	//navstream vector:
	std::vector<SNavigationStreams> m_vecNavStreamsTop;
	std::vector<SNavigationStreams> m_vecNavStreamsBottom;

	//stopall button:
	CStopAllButton m_ctlBtnStopall;


									//public member functions:

	//onCtlColor - this will allow us to pass the black brush to paint the dialog a black
	//colour.
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//onInitDialog - this will allow us to do some initaliseation of the dialog such as
	//setting button colours and setting up for cart page 1:
	virtual BOOL OnInitDialog();

	
	//fnSetupButtonForPageData - this will setup the cart buttons for a given set of page
	//data for both fuse and user carts:
	void fnSetupButtonForPageData(mysqlpp::StoreQueryResult& buttonData, int intCartPosistion);

	// fnGetResultSetForQuery - this will perform a MYSQL query on the database and return the result.
	// WARNING - Ensure that the m_PMYSQLConn has been connected first!
	mysqlpp::StoreQueryResult fnGetResultSetForQuery(CString strSQLQuery);

private:
									//private member variables:

	CBrush m_dlgBackBrush;				//the brush that is used to paint the back of the carts dialog.
	mysqlpp::Connection* m_PMYSQLConn;	//connection to mysql for carts.
	HACCEL  m_hAccelTable;				//store accelerateor debug info.
	CFusicLoginDlg*		m_pLoginDlg;	//the pointer to the login dialog
	CFusicMainDlg*		m_pMainDlg;
									
									//private member functions:

	//fnSetupButtonMaps - sets up button maps for each button.
	void fnSetupButtonMaps();

	// fnSplitColours - this will take a CString encoded colour code (RGB), split and convert to integers:
	bool fnSplitColours(CString strColourCode, int& intRed, int& intGreen, int& intBlue);

	// fnGetFilePathForFileID - this will return a file location given a file ID from the database.
	CString fnGetFilePathForFileID(int intFileID);

	// fnGetFileDurationForFileID - this will return a file duration given a file ID from the database.
	double fnGetFileDurationForFileID(int intFileID);

protected:
	virtual void PostNcDestroy();

	//protected member functions:

public:
	afx_msg void OnBnClickedBtnDebug();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnStopall();
	CColourButton m_ctlBtnReconnect;
	CColourButton m_ctlBtnLogout;
	afx_msg void OnBnClickedBtnReconnect();
	afx_msg void OnBnClickedBtnLogout();
	CColourButton m_ctlBtnRefresh;
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};