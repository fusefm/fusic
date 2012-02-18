// FusicCartsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicCartsDlg.h"
//a quick enum to help distringuish between fuse and user carts:
#define FUSECARTS 1
#define USERCARTS 2

//get access to callback maps:


//get access to fader start exr:
extern CString g_strFaderStartEXE;
extern int faderStartAddress;
extern bool faderStart;

// CFusicCartsDlg dialog

IMPLEMENT_DYNAMIC(CFusicCartsDlg, CDialog)

CFusicCartsDlg::CFusicCartsDlg(CWnd* pParent,CFusicLoginDlg* pLoginDialog, CFusicMainDlg* 
							   pMainDlg)
: CDialog(CFusicCartsDlg::IDD, pParent)
{
	//create the back brush to pain the dialog with:
	m_dlgBackBrush.CreateSolidBrush(RGB(0,0,0));

	m_pMainDlg = pMainDlg;

	//get the mysqlpp connection object:
	m_PMYSQLConn = new mysqlpp::Connection(false);

	//setup the accelerator:
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDR_ACCELERATOR_CART_DEBUG));

	//reset all members:
	m_intCurrnetDialogPageTop = 1;		//we are going start on page 1.
	m_intCurrnetDialogPageBottom = 1;	//we are going start on page 1.

	//set the login dialog variable:
	m_pLoginDlg = pLoginDialog;
}

CFusicCartsDlg::~CFusicCartsDlg()
{

	//remove the connection:
	m_PMYSQLConn->disconnect();
	delete m_PMYSQLConn;

}

void CFusicCartsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CART_1_1, m_ctlBtnCart11);
	DDX_Control(pDX, IDC_BTN_CART_1_2, m_ctlBtnCart12);
	DDX_Control(pDX, IDC_BTN_CART_1_3, m_ctlBtnCart13);
	DDX_Control(pDX, IDC_BTN_CART_1_4, m_ctlBtnCart14);
	DDX_Control(pDX, IDC_BTN_CART_1_5, m_ctlBtnCart15);
	DDX_Control(pDX, IDC_BTN_CART_2_1, m_ctlBtnCart21);
	DDX_Control(pDX, IDC_BTN_CART_2_2, m_ctlBtnCart22);
	DDX_Control(pDX, IDC_BTN_CART_2_3, m_ctlBtnCart23);
	DDX_Control(pDX, IDC_BTN_CART_2_4, m_ctlBtnCart24);
	DDX_Control(pDX, IDC_BTN_CART_2_5, m_ctlBtnCart25);
	DDX_Control(pDX, IDC_BTN_CART_3_1, m_ctlBtnCart31);
	DDX_Control(pDX, IDC_BTN_CART_3_2, m_ctlBtnCart32);
	DDX_Control(pDX, IDC_BTN_CART_3_3, m_ctlBtnCart33);
	DDX_Control(pDX, IDC_BTN_CART_3_4, m_ctlBtnCart34);
	DDX_Control(pDX, IDC_BTN_CART_3_5, m_ctlBtnCart35);
	DDX_Control(pDX, IDC_BTN_CART_4_1, m_ctlBtnCart41);
	DDX_Control(pDX, IDC_BTN_CART_4_2, m_ctlBtnCart42);
	DDX_Control(pDX, IDC_BTN_CART_4_3, m_ctlBtnCart43);
	DDX_Control(pDX, IDC_BTN_CART_4_4, m_ctlBtnCart44);
	DDX_Control(pDX, IDC_BTN_CART_4_5, m_ctlBtnCart45);
	DDX_Control(pDX, IDC_BTN_CART_5_1, m_ctlBtnCart51);
	DDX_Control(pDX, IDC_BTN_CART_5_2, m_ctlBtnCart52);
	DDX_Control(pDX, IDC_BTN_CART_5_3, m_ctlBtnCart53);
	DDX_Control(pDX, IDC_BTN_CART_5_4, m_ctlBtnCart54);
	DDX_Control(pDX, IDC_BTN_CART_5_5, m_ctlBtnCart55);
	DDX_Control(pDX, IDC_BTN_CART_6_1, m_ctlBtnCart61);
	DDX_Control(pDX, IDC_BTN_CART_6_2, m_ctlBtnCart62);
	DDX_Control(pDX, IDC_BTN_CART_6_3, m_ctlBtnCart63);
	DDX_Control(pDX, IDC_BTN_CART_6_4, m_ctlBtnCart64);
	DDX_Control(pDX, IDC_BTN_CART_6_5, m_ctlBtnCart65);
	DDX_Control(pDX, IDC_BTN_CART_7_1, m_ctlBtnCart71);
	DDX_Control(pDX, IDC_BTN_CART_7_2, m_ctlBtnCart72);
	DDX_Control(pDX, IDC_BTN_CART_7_3, m_ctlBtnCart73);
	DDX_Control(pDX, IDC_BTN_CART_7_4, m_ctlBtnCart74);
	DDX_Control(pDX, IDC_BTN_CART_7_5, m_ctlBtnCart75);
	DDX_Control(pDX, IDC_BTN_CART_8_1, m_ctlBtnCart81);
	DDX_Control(pDX, IDC_BTN_CART_8_2, m_ctlBtnCart82);
	DDX_Control(pDX, IDC_BTN_CART_8_3, m_ctlBtnCart83);
	DDX_Control(pDX, IDC_BTN_CART_8_4, m_ctlBtnCart84);
	DDX_Control(pDX, IDC_BTN_CART_8_5, m_ctlBtnCart85);
	DDX_Control(pDX, IDC_BTN_NAV_TOP_1, m_ctlBtnNavTop1);
	DDX_Control(pDX, IDC_BTN_NAV_TOP_2, m_ctlBtnNavTop2);
	DDX_Control(pDX, IDC_BTN_NAV_TOP_3, m_ctlBtnNavTop3);
	DDX_Control(pDX, IDC_BTN_NAV_TOP_4, m_ctlBtnNavTop4);
	DDX_Control(pDX, IDC_BTN_NAV_TOP_5, m_ctlBtnNavTop5);
	DDX_Control(pDX, IDC_BTN_NAV_TOP_6, m_ctlBtnNavTop6);
	DDX_Control(pDX, IDC_BTN_NAV_TOP_7, m_ctlBtnNavTop7);
	DDX_Control(pDX, IDC_BTN_NAV_TOP_8, m_ctlBtnNavTop8);
	DDX_Control(pDX, IDC_BTN_NAV_BOTTOM_1, m_ctlBtnNavBottom1);
	DDX_Control(pDX, IDC_BTN_NAV_BOTTOM_2, m_ctlBtnNavBottom2);
	DDX_Control(pDX, IDC_BTN_NAV_BOTTOM_3, m_ctlBtnNavBottom3);
	DDX_Control(pDX, IDC_BTN_NAV_BOTTOM_4, m_ctlBtnNavBottom4);
	DDX_Control(pDX, IDC_BTN_NAV_BOTTOM_5, m_ctlBtnNavBottom5);
	DDX_Control(pDX, IDC_BTN_NAV_BOTTOM_6, m_ctlBtnNavBottom6);
	DDX_Control(pDX, IDC_BTN_NAV_BOTTOM_7, m_ctlBtnNavBottom7);
	DDX_Control(pDX, IDC_BTN_NAV_BOTTOM_8, m_ctlBtnNavBottom8);
	DDX_Control(pDX, IDC_BTN_STOPALL, m_ctlBtnStopall);
	DDX_Control(pDX, IDC_BTN_RECONNECT, m_ctlBtnReconnect);
	DDX_Control(pDX, IDC_BTN_LOGOUT, m_ctlBtnLogout);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_ctlBtnRefresh);
}


BEGIN_MESSAGE_MAP(CFusicCartsDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_DEBUG, &CFusicCartsDlg::OnBnClickedBtnDebug)
	ON_BN_CLICKED(IDC_BTN_RECONNECT, &CFusicCartsDlg::OnBnClickedBtnReconnect)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, &CFusicCartsDlg::OnBnClickedBtnLogout)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CFusicCartsDlg::OnBnClickedBtnRefresh)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//onCtlColor - this will allow us to pass the black brush to paint the dialog a black
//colour.
HBRUSH CFusicCartsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//see if its the form we need to paint:
	if(nCtlColor == CTLCOLOR_DLG)
	{
		//return our black brush.
		return m_dlgBackBrush;
	}

	return hbr;
}

BOOL CFusicCartsDlg::OnInitDialog()
{
	mysqlpp::StoreQueryResult resFuse;
	mysqlpp::StoreQueryResult resUser;
	CDialog::OnInitDialog();

	//set the stop all button dialog pointer:
	m_ctlBtnStopall.fnSetCartsDialogPointer((void*)this);

	//set up button maps:
	fnSetupButtonMaps();

	//set up all nav buttons:
	for(mapNav::iterator i = m_mapBtnNavBottom.begin(); i != m_mapBtnNavBottom.end();
		i++)
	{
		i->second->fnSetupNavButtonForPage(i->first, USERCARTS, (void*)this);
	}

	for(mapNav::iterator i = m_mapBtnNavTop.begin(); i != m_mapBtnNavTop.end();
		i++)
	{
		i->second->fnSetupNavButtonForPage(i->first, FUSECARTS, (void*)this);
	}

	//setup all carts buttons:
	for(mapCart::iterator i = m_mapBtnCartBottom.begin(); i != m_mapBtnCartBottom.end();
		i++)
	{
		i->second->fnSetDefaultButton();
	}

	for(mapCart::iterator i = m_mapBtnCartTop.begin(); i != m_mapBtnCartTop.end();
		i++)
	{
		i->second->fnSetDefaultButton();
	}



	//setup stopall button:
	m_ctlBtnStopall.fnSetBackColour(255,0,0, true);
	m_ctlBtnStopall.fnSetFontColour(255,255,0);
	m_ctlBtnStopall.fnSetEdgeColour(255,255,0);
	m_ctlBtnStopall.fnSetClickColourChange(true);

	//setup logout button:
	m_ctlBtnLogout.fnSetBackColour(255,0,0,true);
	m_ctlBtnLogout.fnSetFontColour(255,255,0);
	m_ctlBtnLogout.fnSetEdgeColour(255,255,0);
	m_ctlBtnLogout.fnSetClickColourChange(true);

	//setup reconnect button:
	m_ctlBtnReconnect.fnSetBackColour(255,0,0,true);
	m_ctlBtnReconnect.fnSetFontColour(255,255,0);
	m_ctlBtnReconnect.fnSetEdgeColour(255,255,0);
	m_ctlBtnReconnect.fnSetClickColourChange(true);

	//setup refresh button:
	m_ctlBtnRefresh.fnSetBackColour(255,0,0,true);
	m_ctlBtnRefresh.fnSetFontColour(255,255,0);
	m_ctlBtnRefresh.fnSetEdgeColour(255,255,0);
	m_ctlBtnRefresh.fnSetClickColourChange(true);


	//create the connection to mysql:
	if(!m_PMYSQLConn->connect(g_sctDBSettings.strDBDatabase,
		g_sctDBSettings.strDBHost,
		g_sctDBSettings.strDBUser,
		g_sctDBSettings.strDBPassword))
	{
		//we didnt connect:
		CString strError;
		strError = "Error: could not connecto the mysql database: ";
		strError += m_PMYSQLConn->error();
		strError += ".";
		MessageBox(strError, "Carts Pane", MB_OK | MB_ICONERROR);
	}

	//need to get all fuse carts:
	resFuse = fnGetResultSetForQuery("SELECT * FROM tbl_carts where Show_ID = 0 and Cart_ID like '1%'");

	//setup fuse carts:
	fnSetupButtonForPageData(resFuse, FUSECARTS);

	//do the query for user carts:
	CString strQuery;
	CString strShowID;
	strShowID.Format("%d", g_intShowID);
	strQuery = "SELECT * FROM tbl_carts where Show_ID = ";
	strQuery += strShowID;
	strQuery += " and Cart_ID like '1%'";

	//do the query:
	resUser = fnGetResultSetForQuery(strQuery);

	//setup the user buttons:
	fnSetupButtonForPageData(resUser, USERCARTS);

	//we need now to get carts that where left playing on logout:
	for(std::vector<SCartsList>::iterator i = m_pLoginDlg->m_vecCartsList.begin();
		i != m_pLoginDlg->m_vecCartsList.end(); i++)
	{
		//we want to set all current first buttons up (as these will be displaied first):
		//get the button we firstly need to change:
		CCartsButton* curBtn = NULL;
		if(i->intPageNumber == 1)
		{
			curBtn = m_mapBtnCartTop[i->strCartPosistion];
			if(curBtn == NULL)
				break;

			for(T_mapCallback::iterator j = g_mapCartCallbacks.begin(); j != g_mapCartCallbacks.end();
				j++)
			{
				if(j->first == i->hsyCartsSync)
				{
					g_mapCartCallbacks.erase(j);
					break;
				}
			}


			//set all settings back;
			curBtn->fnPageMoveOn(i->hstCartsStream, i->hsyCartsSync);

			//also reset the callback:
			BASS_ChannelRemoveSync(i->hstCartsStream, i->hsyCartsSync);

			//set the new callback for the button:
			HSYNC callback = BASS_ChannelSetSync(i->hstCartsStream, BASS_SYNC_END, 0, 
				&CCallbackButton::fnEndCallback, 0);

			//add to the callback map:
			g_mapCartCallbacks[callback] = (CCallbackButton*)curBtn;
		}
		else
		{
			//the other buttons arn't on the first page.
			//so we need to put them back into the Navigation stream vector:
			//fist find the nav button that this cart will corrispond to:
			CNavigationButton* navBtn = m_mapBtnNavTop[i->intPageNumber];

			//remove the callback before we change it from the map:
			for(T_mapCallback::iterator j = g_mapCartCallbacks.begin();
				j != g_mapCartCallbacks.end(); j++)
			{
				if(j->first == i->hsyCartsSync)
				{
					g_mapCartCallbacks.erase(j);
					break;
				}
			}

			BASS_ChannelRemoveSync(i->hstCartsStream, i->hsyCartsSync);
			HSYNC synx = BASS_ChannelSetSync(i->hstCartsStream, BASS_SYNC_END,0,
				&CCallbackButton::fnEndCallback, 0);

			SNavigationStreams sNav;
			sNav.intPageNumber = i->intPageNumber;
			sNav.stream = i->hstCartsStream;
			sNav.syncHandle = i->hsyCartsSync;
			sNav.syncHandle = synx;
			//sNav.

			//now the tricky part, we need to create a std::mapCart::iterator for the
			//button that we can store in the struct; the only way I can see of doing this
			//is to iterate over the buttons untill we find one with the appropriate ID:
			for(mapCart::iterator ii = m_mapBtnCartTop.begin(); ii != m_mapBtnCartTop.end();
				ii++)
			{
				if(ii->first == i->strCartPosistion)
				{
					sNav.btnIterator = ii;
					break;
				}
			}

			//reset the callback:
			g_mapCartCallbacks[i->hsyCartsSync] = navBtn;

			//incrase the reference count on the nav button (to start it flashing):
			navBtn->fnIncreasePlayCount();

			//also add to the nav stream map:
			m_vecNavStreamsTop.push_back(sNav);
		}

	}

	//allow the window to be changed.
	AllowSetForegroundWindow(ASFW_ANY);

	//clear out the vector on the login dialog for the next logout population:
	m_pLoginDlg->m_vecCartsList.clear();

	//begin the fader start timer:
	SetTimer(WM_USER + 10, 10, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
}




//private member functions implimentation:

//fnSetupButtonMaps - sets up button maps for each button.
void CFusicCartsDlg::fnSetupButtonMaps()
{
	//top cart buttons:
	m_mapBtnCartTop["11"] = &m_ctlBtnCart11;
	m_mapBtnCartTop["12"] = &m_ctlBtnCart12;
	m_mapBtnCartTop["13"] = &m_ctlBtnCart13;
	m_mapBtnCartTop["14"] = &m_ctlBtnCart14;
	m_mapBtnCartTop["15"] = &m_ctlBtnCart15;
	m_mapBtnCartTop["21"] = &m_ctlBtnCart21;
	m_mapBtnCartTop["22"] = &m_ctlBtnCart22;
	m_mapBtnCartTop["23"] = &m_ctlBtnCart23;
	m_mapBtnCartTop["24"] = &m_ctlBtnCart24;
	m_mapBtnCartTop["25"] = &m_ctlBtnCart25;
	m_mapBtnCartTop["31"] = &m_ctlBtnCart31;
	m_mapBtnCartTop["32"] = &m_ctlBtnCart32;
	m_mapBtnCartTop["33"] = &m_ctlBtnCart33;
	m_mapBtnCartTop["34"] = &m_ctlBtnCart34;
	m_mapBtnCartTop["35"] = &m_ctlBtnCart35;
	m_mapBtnCartTop["41"] = &m_ctlBtnCart41;
	m_mapBtnCartTop["42"] = &m_ctlBtnCart42;
	m_mapBtnCartTop["43"] = &m_ctlBtnCart43;
	m_mapBtnCartTop["44"] = &m_ctlBtnCart44;
	m_mapBtnCartTop["45"] = &m_ctlBtnCart45;

	//bottom cart buttons:
	m_mapBtnCartBottom["11"] = &m_ctlBtnCart51;
	m_mapBtnCartBottom["12"] = &m_ctlBtnCart52;
	m_mapBtnCartBottom["13"] = &m_ctlBtnCart53;
	m_mapBtnCartBottom["14"] = &m_ctlBtnCart54;
	m_mapBtnCartBottom["15"] = &m_ctlBtnCart55;
	m_mapBtnCartBottom["21"] = &m_ctlBtnCart61;
	m_mapBtnCartBottom["22"] = &m_ctlBtnCart62;
	m_mapBtnCartBottom["23"] = &m_ctlBtnCart63;
	m_mapBtnCartBottom["24"] = &m_ctlBtnCart64;
	m_mapBtnCartBottom["25"] = &m_ctlBtnCart65;
	m_mapBtnCartBottom["31"] = &m_ctlBtnCart71;
	m_mapBtnCartBottom["32"] = &m_ctlBtnCart72;
	m_mapBtnCartBottom["33"] = &m_ctlBtnCart73;
	m_mapBtnCartBottom["34"] = &m_ctlBtnCart74;
	m_mapBtnCartBottom["35"] = &m_ctlBtnCart75;
	m_mapBtnCartBottom["41"] = &m_ctlBtnCart81;
	m_mapBtnCartBottom["42"] = &m_ctlBtnCart82;
	m_mapBtnCartBottom["43"] = &m_ctlBtnCart83;
	m_mapBtnCartBottom["44"] = &m_ctlBtnCart84;
	m_mapBtnCartBottom["45"] = &m_ctlBtnCart85;

	//top nav buttons:
	m_mapBtnNavTop[1] = &m_ctlBtnNavTop1;
	m_mapBtnNavTop[2] = &m_ctlBtnNavTop2;
	m_mapBtnNavTop[3] = &m_ctlBtnNavTop3;
	m_mapBtnNavTop[4] = &m_ctlBtnNavTop4;
	m_mapBtnNavTop[5] = &m_ctlBtnNavTop5;
	m_mapBtnNavTop[6] = &m_ctlBtnNavTop6;
	m_mapBtnNavTop[7] = &m_ctlBtnNavTop7;
	m_mapBtnNavTop[8] = &m_ctlBtnNavTop8;

	//bottom nav buttons:
	m_mapBtnNavBottom[1] = &m_ctlBtnNavBottom1;
	m_mapBtnNavBottom[2] = &m_ctlBtnNavBottom2;
	m_mapBtnNavBottom[3] = &m_ctlBtnNavBottom3;
	m_mapBtnNavBottom[4] = &m_ctlBtnNavBottom4;
	m_mapBtnNavBottom[5] = &m_ctlBtnNavBottom5;
	m_mapBtnNavBottom[6] = &m_ctlBtnNavBottom6;
	m_mapBtnNavBottom[7] = &m_ctlBtnNavBottom7;
	m_mapBtnNavBottom[8] = &m_ctlBtnNavBottom8;

}

//fnSetupButtonForPageData - this will setup the cart buttons for a given set of page
//data for both fuse and user carts:
void CFusicCartsDlg::fnSetupButtonForPageData(mysqlpp::StoreQueryResult &buttonData, int intCartPosistion)
{
	for(size_t i = 0; i < buttonData.num_rows(); i++)
	{
		//firstly get the button attributes:
		CString strID = buttonData[i]["Cart_ID"];
		CString strColourCode = buttonData[i]["Cart_Colour"];
		CString strCartBtnTitle = buttonData[i]["Cart_Title"];
		bool blRefreshOnClick = (bool)buttonData[i]["Cart_Refresh"];
		CString strFilePath;
		int intRed = 0;
		int intGreen = 0;
		int intBlue = 0;
		double dblDuration = 0;
		bool blValueSet = false;

		//ensure that we have a valid ID:
		if(strID.GetLength() != 3)
		{
			CString strError;
			strError = "Warning: Invalid cart ID: ";
			strError += strID;
			strError += ".";
			MessageBox(strError, "Carts Pane", MB_OK | MB_ICONWARNING);
			break;
		}

		//decode the colours:
		if(!fnSplitColours(strColourCode, intRed, intGreen, intBlue))
		{
			//coulndt split:
			CString strError;
			strError = "Warning: Could not decode colour sequence: ";
			strError += strColourCode;
			strError += ", for button: ";
			strError += strID;
			strError += ".";
			MessageBox(strError, "Carts Pane", MB_OK | MB_ICONWARNING);
			break;
		}

		//get rid of the first number as this denotes the page number:
		strID.Delete(0,1);

		//get the file path:
		strFilePath = fnGetFilePathForFileID((int)buttonData[i]["File_ID"]);

		//get the file duration:
		dblDuration = fnGetFileDurationForFileID((int)buttonData[i]["File_ID"]);

		//see which carts we are chaning:
		if(intCartPosistion == FUSECARTS)
		{
			//change fuse carts:
			m_mapBtnCartTop[strID]->fnSetupCartButton(strFilePath,
				RGB(intRed, intGreen, intBlue), strCartBtnTitle, dblDuration, 
				blRefreshOnClick, true);

		}
		else if(intCartPosistion == USERCARTS)
		{
			//change user carts:
			m_mapBtnCartBottom[strID]->fnSetupCartButton(strFilePath,
				RGB(intRed, intGreen, intBlue), strCartBtnTitle, dblDuration, 
				blRefreshOnClick, false);
		}
		else
		{
			//we got an unknonn number:
			return;
		}
	}
}

// fnSplitColours - this will take a CString encoded colour code (RGB), split and convert to integers:
bool CFusicCartsDlg::fnSplitColours(CString strColourCode, int& intRed, int& intGreen, int& intBlue)
{
	CString strTemp;
	//make sure that the string is 9 chars long:
	if(strColourCode.GetLength() != 9)
	{
		return false;
	}
	//get the red portion of the colour code:
	strTemp = strColourCode.Left(3);

	//convert to an int:
	intRed = atoi(strTemp.GetBuffer());
	//clear memory:
	strTemp.ReleaseBuffer();

	//erase the first 3 number as we have used them:
	strColourCode.Delete(0,3);
	//erase the temp string:
	strTemp.Empty();

	//get the green portion of the colour code:
	strTemp = strColourCode.Left(3);

	//convert to int:
	intGreen = atoi(strTemp.GetBuffer());

	//clear memory:
	strTemp.ReleaseBuffer();

	//erase the next 3 number again, just leaving us with the blue colour:
	strColourCode.Delete(0, 3);
	//erase the temp string:
	strTemp.Empty();

	//get the blue bit:
	strTemp = strColourCode.Left(3);

	//convert to int:
	intBlue = atoi(strTemp.GetBuffer());
	//clear RAM:
	strTemp.ReleaseBuffer();


	return true;
}

// fnGetFilePathForFileID - this will return a file location given a file ID from the database.
CString CFusicCartsDlg::fnGetFilePathForFileID(int intFileID)
{
	char temp[10];
	//need to do some conversion here.
	_itoa_s(intFileID, temp,10 ,10);

	//we assume that we have a working connection to the server. We firstly need to build the query:
	CString strQuery = "SELECT File_Location FROM tbl_files WHERE File_ID = ";
	strQuery += temp;

	//do the query and store the result:
	mysqlpp::StoreQueryResult res = fnGetResultSetForQuery(strQuery);

	//get the path name and see if it's there:
	if(res.num_rows() == 1)
	{
		return (CString)res[0]["File_Location"];
	}
	else
	{
		return "";
	}
}

// fnGetFileDurationForFileID - this will return a file duration given a file ID from the database.
double CFusicCartsDlg::fnGetFileDurationForFileID(int intFileID)
{
	char temp[10];
	//need to do some conversion here.
	_itoa_s(intFileID, temp,10 ,10);

	//we assume that we have a working connection to the server. We firstly need to build the query:
	CString strQuery = "SELECT File_Duration FROM tbl_files WHERE File_ID = ";
	strQuery += temp;

	//do the query and store the result:
	mysqlpp::StoreQueryResult res = fnGetResultSetForQuery(strQuery);

	//get the path name and see if it's there:
	if(res.num_rows() == 1)
	{
		return (double)res[0]["File_Duration"];
	}
	else
	{
		return 0;
	}
}


//global vars needed for the function below:
static int intLoopCounter = 0;
static bool bolskip = false;
// fnGetResultSetForQuery - this will perform a MYSQL query on the database and return the result.
// WARNING - Ensure that the m_PMYSQLConn has been connected first!
mysqlpp::StoreQueryResult CFusicCartsDlg::fnGetResultSetForQuery(CString strSQLQuery)
{
	//do the query:
	mysqlpp::Query query = m_PMYSQLConn->query(strSQLQuery);

	//store the result;
	mysqlpp::StoreQueryResult res = query.store();

	//check the result:
	if((bool)res == false && bolskip == false)
	{
		//here we want to try to re-connect to MySQL (silently) three times and if that
		//fails warn the user, but then display a 'reconnect' button so the user doesn't
		//need to log out and back in to re-establish a connection.
		if(intLoopCounter > 2)
		{
			int msgRet = 0;
			msgRet = MessageBox("Error: Got bad results set. This is usually caused when the server is shut down"
				" or has been reset. Do you wish to attempt to reconnect to the server?", "Fusic Carts Pane", MB_YESNO |
				MB_ICONERROR);

			//see what the response was:
			switch (msgRet)
			{
			case IDYES:
				//old object is invalid:
				m_PMYSQLConn->disconnect();
				delete m_PMYSQLConn;

				//get a new object:
				m_PMYSQLConn = new mysqlpp::Connection(false);

				//attempt to reconnect:
				if(!m_PMYSQLConn->connect(g_sctDBSettings.strDBDatabase,
					g_sctDBSettings.strDBHost,
					g_sctDBSettings.strDBUser,
					g_sctDBSettings.strDBPassword))
				{
					//we didnt connect:
					CString strError;
					strError = "Error: could not connecto the mysql database: ";
					strError += m_PMYSQLConn->error();
					strError += ".";
					MessageBox(strError, "Carts Pane", MB_OK | MB_ICONERROR);
				}
				//redo the query:
				return fnGetResultSetForQuery(strSQLQuery);
				break;

			case IDNO:
				//exit:
				bolskip = true;
				//show the reconnect button
				m_ctlBtnReconnect.EnableWindow(true);
				m_ctlBtnReconnect.ShowWindow(SW_SHOW);
				break;
			}
		}
		else
		{
			//increment the loop counter:
			intLoopCounter ++;

			//old object is invalid:
			m_PMYSQLConn->disconnect();
			delete m_PMYSQLConn;

			//get a new object:
			m_PMYSQLConn = new mysqlpp::Connection(false);

			//attempt to reconnect:
			m_PMYSQLConn->connect(g_sctDBSettings.strDBDatabase,
				g_sctDBSettings.strDBHost,
				g_sctDBSettings.strDBUser,
				g_sctDBSettings.strDBPassword);
			//redo the query:
			return fnGetResultSetForQuery(strSQLQuery);
		}
	}

	//reset the loop counter for next time:
	intLoopCounter = 0;

	//give back the result:
	return res;
}


void CFusicCartsDlg::PostNcDestroy()
{
	//because this dialog is on the heap, we need to ensure that the relevant
	//function to destroy the window is called.
	CDialog::PostNcDestroy();

	//NOTE: This will need to be changed when the dialog is back on the heap with the 
	//main dialog::
	//delete this;
}

void CFusicCartsDlg::OnBnClickedBtnDebug()
{
	// prints out debugging information:
	CString strMessage;
	int intCounter = 0;
	char* buf = (char*)malloc(15);
	strMessage = "BEGIN DEBUG.\n";
	//top stream vector:
	strMessage += "Current top stream vectors:\n";
	for(std::vector<SNavigationStreams>::iterator i  = m_vecNavStreamsTop.begin();
		i != m_vecNavStreamsTop.end(); i++)
	{
		//add the iterator number:
		intCounter ++;
		itoa(intCounter, buf, 10);
		strMessage += "\t";
		strMessage += buf;
		strMessage += ":";

		//add the data:
		strMessage += "\tButton ID: ";
		strMessage += i->btnIterator->first;
		strMessage += "\n\t\tButton Address: 0x";
		itoa((int)i->btnIterator->second, buf, 16);
		strMessage += buf;
		strMessage += "\n\t\tPage Number: ";
		itoa(i->intPageNumber, buf, 10);
		strMessage += buf;
		strMessage += "\n\t\tStream Handle: 0x";
		itoa((int)i->stream, buf, 16);
		strMessage += buf;
		strMessage += "\n\t\tSync Handle: 0x";
		itoa((int)i->syncHandle, buf, 16);
		strMessage += buf;
		strMessage += "\n";
	}

	//reset the counter:
	intCounter = 0;

	//bottom stream vectors:
	strMessage += "Current bottom stream vectors:\n";
	for(std::vector<SNavigationStreams>::iterator i  = m_vecNavStreamsBottom.begin();
		i != m_vecNavStreamsBottom.end(); i++)
	{
		//add the iterator number:
		intCounter ++;
		itoa(intCounter, buf, 10);
		strMessage += "\t";
		strMessage += buf;
		strMessage += ":";

		//add the data:
		strMessage += "\tButton ID: ";
		strMessage += i->btnIterator->first;
		strMessage += "\n\t\tButton Address: 0x";
		itoa((int)i->btnIterator->second, buf, 16);
		strMessage += buf;
		strMessage += "\n\t\tPage Number: ";
		itoa(i->intPageNumber, buf, 10);
		strMessage += buf;
		strMessage += "\n\t\tStream Handle: 0x";
		itoa((int)i->stream, buf, 16);
		strMessage += buf;
		strMessage += "\n\t\tSync Handle: 0x";
		itoa((int)i->syncHandle, buf, 16);
		strMessage += buf;
		strMessage += "\n";
	}

	//reset the counter:
	intCounter = 0;

	//print out the callback map:
	strMessage += "Global callback map:\n";
	for(T_mapCallback::iterator i = g_mapCartCallbacks.begin();
		i != g_mapCartCallbacks.end(); i++)
	{
		intCounter ++;
		itoa(intCounter, buf, 10);
		strMessage += "\t";
		strMessage += buf;
		strMessage += ":";
		strMessage += "\tCallback Handle: 0x";
		itoa((int)i->first, buf, 16);
		strMessage += buf;
		strMessage += "\n\t\tCallback Button Handle: 0x";
		itoa((int)i->second, buf, 16);
		strMessage += buf;
		strMessage += "\n";
	}
	strMessage += "END DEBUG.";
	MessageBox(strMessage, "Fusic Carts Debug", MB_OK | MB_ICONINFORMATION);
}

//fnGetCurrentStream - Return the current stream that the button is playing:
HSTREAM CCartsButton::fnGetCurrentStream()
{
	return m_streamCart;
}

BOOL CFusicCartsDlg::PreTranslateMessage(MSG* pMsg)
{
	//handle the messages for the accelerator keys:
	if(m_hAccelTable)
	{
		if(::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return (TRUE);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CFusicCartsDlg::OnBnClickedBtnStopall()
{


}

void CFusicCartsDlg::OnBnClickedBtnReconnect()
{
	//attempt to reconnect at next SQL query:
	m_ctlBtnReconnect.EnableWindow(false);
	m_ctlBtnReconnect.ShowWindow(SW_HIDE);
	bolskip = false;
}

void CFusicCartsDlg::OnBnClickedBtnLogout()
{
	//ensure we want to logout:
	int res = MessageBox("Are you sure you want to log out?\nNOTE: All user carts will be "
		"halted when you logout.",
		"Fusic", MB_YESNO | MB_ICONQUESTION);

	if(res == IDNO)
	{
		//don't logout:
		return;
	}


	//stop all user carts:
	//(stop all nav buttons):
	for(std::vector<SNavigationStreams>::iterator i = m_vecNavStreamsBottom.begin();
		i != m_vecNavStreamsBottom.end(); i++)
	{
		//fade out the cart (500 ms):
		BASS_ChannelSlideAttribute(i->stream, BASS_ATTRIB_VOL, 0, 500);
		//also remove from the map:
		for(T_mapCallback::iterator j = g_mapCartCallbacks.end(); j != g_mapCartCallbacks.end();
			j++)
		{
			if(j->first == i->syncHandle)
			{
				g_mapCartCallbacks.erase(j);
				break;
			}
		}

		//remove old end sync:
		BASS_ChannelRemoveSync(i->stream, i->syncHandle);

		//create a new sync at the end of the slide:
		//NOTE: We should be safe to call this callback function, it is a part of a class
		//that will be destroyed when the window is, however, it is a static function and 
		//therefore
		//does not need a class to be created in order to use the function:
		BASS_ChannelSetSync(i->stream, BASS_SYNC_SLIDE, 0, &CCallbackButton::fnSlideCallback,
			NULL);
	}

	//(stop page buttons):
	for(mapCart::iterator i = m_mapBtnCartBottom.begin(); i != m_mapBtnCartBottom.end();
		i++)
	{
		i->second->fnStop(true);
	}

	//add all the currently playing fuse carts:
	//(current wall):
	for(mapCart::iterator i = m_mapBtnCartTop.begin(); i != m_mapBtnCartTop.end(); i++)
	{
		if(i->second->fnGetCurrentStream() != 0)
		{
			CString strTitle;
			SCartsList lst;
			HSYNC callbackSync;

			//remove the callback from the global map:
			for(T_mapCallback::iterator j = g_mapCartCallbacks.begin(); j != g_mapCartCallbacks.end();
				j++)
			{
				if(j->first == i->second->fnGetCurrentSync())
				{
					g_mapCartCallbacks.erase(j);
					break;
				}
			}

			//reset the callback:
			BASS_ChannelRemoveSync(i->second->fnGetCurrentStream(), 
				i->second->fnGetCurrentSync());
			callbackSync = BASS_ChannelSetSync(i->second->fnGetCurrentStream(), 
				BASS_SYNC_END, 0, CFusicLoginDlg::fnLoginDialogCartCallback, m_pLoginDlg);
			//create a new cart list element:

			i->second->GetWindowTextA(strTitle);
			lst.hstCartsStream = i->second->fnGetCurrentStream();
			lst.hsyCartsSync = 0;
			lst.strTitle = strTitle;
			lst.hsyCartsSync = callbackSync;
			lst.intPageNumber = m_intCurrnetDialogPageTop;
			lst.strCartPosistion = i->first;
			m_pLoginDlg->fnAddCartToList(lst);
		}//if
	}//for

	//(nav walls):
	for(std::vector<SNavigationStreams>::iterator ii = m_vecNavStreamsTop.begin();
		ii != m_vecNavStreamsTop.end(); ii++)
	{
		CString strTitle;
		SCartsList lst;
		HSYNC callbackSync;
		lst.hstCartsStream = ii->stream;

		lst.intPageNumber = ii->intPageNumber;
		lst.strCartPosistion = ii->btnIterator->first;

		//remove the callback from the global map:
		for(T_mapCallback::iterator j = g_mapCartCallbacks.begin(); j != g_mapCartCallbacks.end();
			j++)
		{
			if(j->first == ii->syncHandle)
			{
				g_mapCartCallbacks.erase(j);
				break;
			}
		}

		//change the callbacks:
		BASS_ChannelRemoveSync(ii->stream, 
			ii->syncHandle);
		callbackSync = BASS_ChannelSetSync(ii->stream, 
			BASS_SYNC_END, 0, CFusicLoginDlg::fnLoginDialogCartCallback, m_pLoginDlg);
		lst.hsyCartsSync = callbackSync;

		//to get the title we will need to do an SQL query :-(
		CString strQuery;
		CString strPageNumber;
		strPageNumber.Format("%d", ii->intPageNumber);
		strQuery = "SELECT Cart_Title FROM tbl_carts WHERE Show_ID = 0 AND Cart_ID = ";
		strQuery += strPageNumber;
		strQuery += ii->btnIterator->first;

		//do the query:
		mysqlpp::StoreQueryResult res = fnGetResultSetForQuery(strQuery);
		lst.strTitle = res[0]["Cart_Title"];
		m_pLoginDlg->fnAddCartToList(lst);
	}

	//kill the main dialog:
	m_pMainDlg->doOk();

	//logout of the fusic carts screen:
	OnOK();
}

void CFusicCartsDlg::OnBnClickedBtnRefresh()
{
	//need to refresh both fuse and user carts:
	CString strShowID;
	CString strPageNumber;

	//refresh fuse stuff first (show_id 0):
	CString strQuery = "SELECT * FROM tbl_carts WHERE Show_ID = 0 AND Cart_ID LIKE '";
	strPageNumber.Format("%d", m_intCurrnetDialogPageTop);
	strQuery += strPageNumber;
	strQuery += "%'";
	mysqlpp::StoreQueryResult res = fnGetResultSetForQuery(strQuery);
	fnSetupButtonForPageData(res, FUSECARTS);

	//refresh the user carts:
	strQuery.Empty();
	strPageNumber.Empty();

	strPageNumber.Format("%d", m_intCurrnetDialogPageBottom);
	strShowID.Format("%d", g_intShowID);
	strQuery = "SELECT * FROM tbl_carts WHERE Show_ID = ";
	strQuery += strShowID;
	strQuery += " AND Cart_ID LIKE '";
	strQuery += strPageNumber;
	strQuery += "%'";
	res = fnGetResultSetForQuery(strQuery);
	fnSetupButtonForPageData(res, USERCARTS);
}
bool bolRan = false;
void CFusicCartsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	/*if(nIDEvent == (WM_USER +10))
	{
		if(faderStart)
		{
			int intState = DlPortReadPortUlong((ULONG)faderStartAddress);
			if(intState == -32 && bolRan == false)
			{
				m_pMainDlg->fnFaderStart();
				bolRan = true;
			}
			if(intState == -16 && bolRan == true)
			{
				bolRan = false;
			}
		}
	}*/

	CDialog::OnTimer(nIDEvent);
}
