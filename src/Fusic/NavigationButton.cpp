// NavigationButton.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "NavigationButton.h"
#include "FusicCartsDlg.h"
#include "globals.h"	
#include "mysql++.h"

//get access to the callback map:
extern T_mapCallback g_mapCartCallbacks;

// CNavigationButton

IMPLEMENT_DYNAMIC(CNavigationButton, CButton)

CNavigationButton::CNavigationButton()
{
	//reset members:
	m_intCartsOnPagePlaying = 0;
}

CNavigationButton::~CNavigationButton()
{
}


BEGIN_MESSAGE_MAP(CNavigationButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CNavigationButton::OnBnClicked)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//resets the reference counter and stops the button from flashing if it is doing so
//used when the stop all button is pressed and the jingles have been stopped withiout
//notifying the navbutton:
void CNavigationButton::fnResetButton()
{
	if(m_intCartsOnPagePlaying != 0)
	{
		//need to stop flashing:
		KillTimer(IDT_FLASNAV);

		//tell it not to display flash colours:
		fnSetFlashOnNextRedraw(false);

		//reset colours:
		fnSetOrigonalBackColour();

		//set reference counter to 0:
		m_intCartsOnPagePlaying = 0;
	}
}


//public member functions implimentation:
//fnSetupNavButtonForPage - this will set up the nav button for a specific page:
void CNavigationButton::fnSetupNavButtonForPage(int intPageNumber, int intNavButtonPosistion,
												void* dlg)
{
	//set the page number:
	m_intNavPage = intPageNumber;

	//see which page we are displaying:
	if(m_intNavPage == 1)
	{
		//start off with a shaded back colour as this page will be selected first.
		fnSetBackColour(0,100,0, true);

		//prevent the user from pressing the button that they are already on.
		EnableWindow(false);
	}
	else
	{
		//otherwise black background:
		fnSetBackColour(0,0,0, true);
	}

	//set other colours:
	fnSetFontColour(0,255,0);
	fnSetEdgeColour(0,255,0);
	fnSetFlashColour(255,0,0);

	//make colours change:
	fnSetClickColourChange(true);

	//set the button text:
	CString strCaption;
	strCaption.Format("%d", m_intNavPage);
	SetWindowText(strCaption);

	//set the dialog carts class:
	m_pCartsDlg = dlg;

	//set the button posistion:
	m_intNavPosistion = intNavButtonPosistion;
}

//OnBnClicked - called when the nav button is clicked, here we start
//to set up all the buttons for the next page as well as moving callbacks
//and increaseing the reference count on the old nav button.
void CNavigationButton::OnBnClicked()
{
	//firstly get the current page being displaied:
	CFusicCartsDlg* dlgCart = (CFusicCartsDlg*)GetParent();

	if(m_intNavPosistion == USERCARTS)
	{
		//get the button we are moving off from:
		CNavigationButton* oldNavButton;
		oldNavButton = dlgCart->m_mapBtnNavBottom[
			dlgCart->m_intCurrnetDialogPageBottom];

			//set the old button back to not current page colours:
			oldNavButton->fnSetBackColour(0,0,0,true);
			oldNavButton->EnableWindow(true);

			//firstly tell all buttons we are going to change:
			for(mapCart::iterator i = dlgCart->m_mapBtnCartBottom.begin();
				i != dlgCart->m_mapBtnCartBottom.end(); i++)
			{
				HSTREAM stream;
				HSYNC sync;
				stream = i->second->fnPageMoveOff(sync);
				//see if the button is playing:
				if(stream != 0)
				{
					//it is playing:
					SNavigationStreams navstruct;
					navstruct.btnIterator = i;
					navstruct.intPageNumber = dlgCart->m_intCurrnetDialogPageBottom;
					navstruct.stream = stream;
					navstruct.syncHandle = sync;

					//pushback the info:
					dlgCart->m_vecNavStreamsBottom.push_back(navstruct);

					//add a refernce count to the old button:
					oldNavButton->fnIncreasePlayCount();

					//change the callback info to the old button:
					g_mapCartCallbacks[sync] = (CCallbackButton*)oldNavButton;
				}
			}

			//do the query for the next page:
			CString strTemp;
			CString strQuery = "SELECT * FROM tbl_carts where Show_ID = ";
			strTemp.Format("%d", g_intShowID);
			strQuery += strTemp;
			strQuery += " and Cart_ID like '";
			strTemp.Format("%d", m_intNavPage);
			strQuery += strTemp;
			strQuery +="%'";

			mysqlpp::StoreQueryResult res = dlgCart->fnGetResultSetForQuery(strQuery);

			//setup the buttons for that page:
			dlgCart->fnSetupButtonForPageData(res, USERCARTS);

			//set the colours for this button as the currnet one.
			fnSetBackColour(0,100,0, true);

			//disable this button from clicking:
			EnableWindow(false);

			//move the current page number on:
			dlgCart->m_intCurrnetDialogPageBottom = m_intNavPage;

			//see if there are any buttons on this page that are currently playing:
			bool bolErased = true;
			while(bolErased == true)
			{
				bolErased = false;
				for(std::vector<SNavigationStreams>::iterator i = dlgCart->m_vecNavStreamsBottom.begin();
					i != dlgCart->m_vecNavStreamsBottom.end(); i++)
				{
					//see if the cart is for the current page we are going to:
					if(i->intPageNumber == m_intNavPage)
					{
						//set the button back:
						i->btnIterator->second->fnPageMoveOn(i->stream, i->syncHandle);

						//change the callback back to the button:
						g_mapCartCallbacks[i->syncHandle] = (CCallbackButton*)
							i->btnIterator->second;

						//remove the entry from the vector:
						dlgCart->m_vecNavStreamsBottom.erase(i);

						//since we have come accross a jingle playing and we are now
						//returning it back to the button we need to decrease the play
						//count on the nav button:
						fnDecreasePlayCount();
						bolErased = true;
						break;
					}
				}
			}

	}
	else
	{
		//we are chaning fuse carts arround:
		//get the button we are moving off from:
		CNavigationButton* oldNavButton;
		oldNavButton = dlgCart->m_mapBtnNavTop[
			dlgCart->m_intCurrnetDialogPageTop];

			//set the old button back to not current page colours:
			oldNavButton->fnSetBackColour(0,0,0,true);
			oldNavButton->EnableWindow(true);

			//firstly tell all buttons we are going to change:
			for(mapCart::iterator i = dlgCart->m_mapBtnCartTop.begin();
				i != dlgCart->m_mapBtnCartTop.end(); i++)
			{
				HSTREAM stream;
				HSYNC sync;
				stream = i->second->fnPageMoveOff(sync);
				//see if the button is playing:
				if(stream != 0)
				{
					//it is playing:
					SNavigationStreams navstruct;
					navstruct.btnIterator = i;
					navstruct.intPageNumber = dlgCart->m_intCurrnetDialogPageTop;
					navstruct.stream = stream;
					navstruct.syncHandle = sync;

					//pushback the info:
					dlgCart->m_vecNavStreamsTop.push_back(navstruct);

					//add a refernce count to the old button:
					oldNavButton->fnIncreasePlayCount();

					//change the callback info to the old button:
					g_mapCartCallbacks[sync] = (CCallbackButton*)oldNavButton;
				}
			}

			//do the query for the next page:
			CString strTemp;
			CString strQuery = "SELECT * FROM tbl_carts where Show_ID = 0 "
				"and Cart_ID like '";
			strTemp.Format("%d", m_intNavPage);
			strQuery += strTemp;
			strQuery +="%'";

			mysqlpp::StoreQueryResult res = dlgCart->fnGetResultSetForQuery(strQuery);

			//setup the buttons for that page:
			dlgCart->fnSetupButtonForPageData(res, FUSECARTS);

			//set the colours for this button as the currnet one.
			fnSetBackColour(0,100,0, true);

			//disable this button from clicking:
			EnableWindow(false);

			//move the current page number on:
			dlgCart->m_intCurrnetDialogPageTop = m_intNavPage;

			//see if there are any buttons on this page that are currently playing:
			bool bolErased = true;
			while(bolErased == true)
			{
				bolErased = false;
				for(std::vector<SNavigationStreams>::iterator i = dlgCart->m_vecNavStreamsTop.begin();
					i != dlgCart->m_vecNavStreamsTop.end(); i++)
				{
					//see if the cart is for the current page we are going to:
					if(i->intPageNumber == m_intNavPage)
					{
						//set the button back:
						i->btnIterator->second->fnPageMoveOn(i->stream, i->syncHandle);

						//change the callback back to the button:
						g_mapCartCallbacks[i->syncHandle] = (CCallbackButton*)
							i->btnIterator->second;

						//remove the entry from the vector:
						dlgCart->m_vecNavStreamsTop.erase(i);

						//since we have come accross a jingle playing and we are now
						//returning it back to the button we need to decrease the play
						//count on the nav button:
						fnDecreasePlayCount();
						bolErased = true;
						break;
					}
				}
			}
	}
}

//fnIncrasePlayCount - increase the number of carts that are playing on that
//page.
void CNavigationButton::fnIncreasePlayCount()
{
	m_intCartsOnPagePlaying += 1;

	if(m_intCartsOnPagePlaying == 1)
	{
		//need to make the button flash.
		SetTimer(IDT_FLASNAV, 500, 0);
	}
}

//fnDecreasePlayCount - Decrease the number of carts that are playing on that
//page.
void CNavigationButton::fnDecreasePlayCount()
{
	m_intCartsOnPagePlaying -= 1;
	if(m_intCartsOnPagePlaying == 0)
	{
		//need to stop flashing:
		KillTimer(IDT_FLASNAV);

		//tell it not to display flash colours:
		fnSetFlashOnNextRedraw(false);

		//reset colours:
		fnSetOrigonalBackColour();
	}
}

//OnTimer - Hook into the timer function and make the button flash:
void CNavigationButton::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == IDT_FLASNAV)
	{
		//make the button flash:
		if(fnGetFlashState())
		{
			//dont display flash colours:
			fnSetFlashOnNextRedraw(false);
		}
		else
		{
			fnSetFlashOnNextRedraw(true);
		}
	}

	CCallbackButton::OnTimer(nIDEvent);
}

//fnDoButtonCallback - impliment the callback for Navigation buttons:
void CNavigationButton::fnDoButtonCallack(HSTREAM channel)
{
	//all we need to do is de-incremnt the refernce count:
	fnDecreasePlayCount();

	//also remove the stream from the vector:
	CFusicCartsDlg* dlg = (CFusicCartsDlg*)m_pCartsDlg;

	for(std::vector<SNavigationStreams>::iterator i = dlg->m_vecNavStreamsBottom.begin();
		i != dlg->m_vecNavStreamsBottom.end(); i++)
	{
		if(i->stream == channel)
		{
			dlg->m_vecNavStreamsBottom.erase(i);
			break;
		}
	}
	for(std::vector<SNavigationStreams>::iterator i = dlg->m_vecNavStreamsTop.begin();
		i != dlg->m_vecNavStreamsTop.end(); i++)
	{
		if(i->stream == channel)
		{
			dlg->m_vecNavStreamsTop.erase(i);
			break;
		}
	}

}