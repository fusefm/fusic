// CartsButton.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "CartsButton.h"
#include "FusicCartsDlg.h"

//get the callback map:
extern T_mapCallback g_mapCartCallbacks;

// CCartsButton

IMPLEMENT_DYNAMIC(CCartsButton, CButton)

CCartsButton::CCartsButton()
{
	//reset members:
	m_streamCart = 0;
	m_strFilePath = "";

}

CCartsButton::~CCartsButton()
{
}


BEGIN_MESSAGE_MAP(CCartsButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CCartsButton::OnBnClicked)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


	
//public member functions implimentation:
	
//fnSetupCartButton - this will setup the button for the given data.
void CCartsButton::fnSetupCartButton(CString strFile, COLOURREF culBackColour, 
									 CString strTitle, double dblDuration,
									 bool blRefreshOnClick, bool blFuseCart)
{
	//see if this is an enabled or disabled button:
	if(culBackColour == RGB(255,255,255) || strFile == "")
	{
		//disable the button:
		fnDisableButton();
		return;
	}
	//otherwise setup the button:
	//setup colours:
	fnSetBackColour(culBackColour, true);
	fnSetEdgeColour(255,255,255);
	fnSetFontColour(255,255,255); 
	fnSetFlashColour(255,0,0);

	//set the file path for the button:
	m_strFilePath = strFile;

	//set the file duration members:
	CString strDuration;
	strDuration.Format("%.1f", dblDuration);
	m_strPrintDuration = strDuration;
	m_strODuration = strDuration;

	//set the title of the button:
	fnSetOrigonalText(strTitle);

	//ensure that the button is enabled:
	EnableWindow(true);
	m_bolState = true;

	//setup the refresh state:
	m_bolRefreshOnClick = blRefreshOnClick;

	//set the cart type (fuse or user):
	m_bolFuseCart = blFuseCart;

	//update button apperance:
	RedrawWindow();
}

void CCartsButton::fnSetDefaultButton()
{
	//default button look:
	fnSetBackColour(0,0,0,true);
	fnSetOrigonalText("Empty");
	fnSetEdgeColour(255,255,255);
	fnSetFontColour(100,100,100);
	m_bolState = false;
	EnableWindow(false);
}

	
//fnDisableButton - this will set the button as disabled setting the appopriate
//back colour and resets all member variables:
void CCartsButton::fnDisableButton()
{
	if(m_bolState == true)
	{
		//reset colours:
		fnSetEdgeColour(255,255,255);
		//fnSetBackColour(236, 233, 216, true);
		fnSetBackColour(0, 0, 0, true);
		fnSetFontColour(100,100,100);

		//disable the button:
		EnableWindow(false);
	
		//reset text:
		SetWindowText("Empty");

		//tell the button it is disabled:
		m_bolState = false;

		//update apperance:
		RedrawWindow();
	}

	//reset members:
	m_strFilePath = "";
}

//onBnClicked - function called when the button is clicked, this will allow us to
//play the jingle and stop the jingle if there is one playing. Also we will refresh the button
//state from the database if that is required (eg, news).
void CCartsButton::OnBnClicked()
{
	//see if a jingle is playing:
	if(m_streamCart != 0)
	{
		//stop cart playing.
		fnStop(true);
		return;
	}

	//refresh the cart if we need too:
	if(m_bolRefreshOnClick == true)
	{
		//get the parent dialog with access to the
		//button maps so we can get out ID:
		CFusicCartsDlg* dlg = (CFusicCartsDlg*)GetParent();
		CString strShowID;
		CString strButtonID;
		CString strPageNumber;
		CString strQuery;
		
		//setup the showID and button ID:
		if(m_bolFuseCart == true)
		{
			//Show ID will be 0 (fuse show):
			strShowID = "0";

			//get the page number for the top:
			strPageNumber.Format("%d", dlg->m_intCurrnetDialogPageTop);

			//attempt to get the button ID:
			for(mapCart::iterator i = dlg->m_mapBtnCartTop.begin(); 
				i != dlg->m_mapBtnCartTop.end(); i++)
			{
				if(i->second == (CCartsButton*) this)
				{
					strButtonID = i->first;
					break;
				}//if
			}//for
		}//if
		else
		{
			//get the Show ID:
			strShowID.Format("%d", g_intShowID);

			//get the bottom page number:
			strPageNumber.Format("%d", dlg->m_intCurrnetDialogPageBottom);

			//get the button ID:
			for(mapCart::iterator i = dlg->m_mapBtnCartBottom.begin(); 
				i != dlg->m_mapBtnCartBottom.end(); i++)
			{
				if(i->second == (CCartsButton*) this)
				{
					strButtonID = i->first;
					break;
				}//if
			}//for
		}//else

		//build the SQL query:
		strQuery = "SELECT * FROM tbl_carts WHERE Show_ID = ";
		strQuery += strShowID;
		strQuery += " AND Cart_ID = ";
		strQuery += strPageNumber;
		strQuery += strButtonID;
		
		//do the query:
		mysqlpp::StoreQueryResult res = dlg->fnGetResultSetForQuery(strQuery);
		if(m_bolFuseCart == true)
		{
			dlg->fnSetupButtonForPageData(res, FUSECARTS);
		}
		else
		{
			dlg->fnSetupButtonForPageData(res, USERCARTS);
		}

	}//if

	//need to start the cart playing:

	//ensure we have the correct device for bass:
	BASS_SetDevice(g_intCartsDev);

	//create the channel:
	m_streamCart = BASS_StreamCreateFile(false, m_strFilePath, 0,0,0);
	if(m_streamCart == 0)
	{
		//an error occured:
		CString strError;
		char a[10];
		_itoa_s(BASS_ErrorGetCode(), a,10 ,10);
		strError = "ERROR: could not create stream: " + m_strFilePath;
		strError += ". Error Code: ";
		strError += a;
		MessageBox(strError, "Carts Pane", MB_OK | MB_ICONERROR);
		return;
	}

	//get the length of the file:
	m_dblStreamLength = BASS_ChannelBytes2Seconds(m_streamCart, 
		BASS_ChannelGetLength(m_streamCart, BASS_POS_BYTE));

	//begin playing:
	if(!BASS_ChannelPlay(m_streamCart, FALSE))
	{
		//an error occured:
		CString strError;
		char a[10];
		_itoa_s(BASS_ErrorGetCode(), a,10 , 10);
		strError = "ERROR: could not play jingle: " + m_strFilePath;
		strError += ". Error Code: ";
		strError += a;
		MessageBox(strError, "Carts Pane", MB_OK | MB_ICONERROR);
		return;
	}

	//set the sync point to when the stream ends:
	m_syncHandle = BASS_ChannelSetSync(m_streamCart, 
		BASS_SYNC_END, 0, &fnEndCallback, 0);

	//setup the timers for counting on the button and the flash colour:
	SetTimer(IDT_CARTFLASH, 500, 0);
	SetTimer(IDT_CARTTIME, 100, 0);

	//add to the callback map:
	g_mapCartCallbacks[m_syncHandle] = (CCallbackButton*)this;
}

//OnTimer - this will hook all timer events, allowing us to setup timing events
//for countdown and flash changes:
void CCartsButton::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == IDT_CARTFLASH)
	{
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
	else if(nIDEvent == IDT_CARTTIME)
	{
		//we have a countdown timer, find out where we are through the stream:
		CString strTime;
		double dblTimeLeft;

		//get the time left
		dblTimeLeft = m_dblStreamLength - BASS_ChannelBytes2Seconds(m_streamCart,
			BASS_ChannelGetPosition(m_streamCart, BASS_POS_BYTE));

		//format the string:
		strTime.Format("%.1f", dblTimeLeft);

		//set the button duration:
		m_strPrintDuration = strTime;

		//update apperance:
		RedrawWindow();
	}


	CCallbackButton::OnTimer(nIDEvent);
}

//fnDoButtonCallback - called when the button is called back.
void CCartsButton::fnDoButtonCallack(HSTREAM channel)
{

	//need to stop bass:
	//ensure that the jingle has stopped playing:
	BASS_ChannelStop(channel);

	//free up all stream resources:
	BASS_StreamFree(channel);

	//reset button:
	fnStop();
}

//fnStop - this will stop a button playing and put the button back into it's
//origonal state.
void CCartsButton::fnStop(bool bolStopPlay)
{
	//kill the timers:
	KillTimer(IDT_CARTFLASH);
	KillTimer(IDT_CARTTIME);
	
	//do not flash:
	fnSetFlashOnNextRedraw(false);

	//reset Text:
	m_strPrintDuration = m_strODuration;
	
	//set back colours:
	fnSetOrigonalBackColour();

	//update apperance:
	RedrawWindow();

	if(bolStopPlay == true)
	{
		//remove members:
		m_dblStreamLength = 0;

		//slide the volume down so the cart fades out:
		BASS_ChannelSlideAttribute(m_streamCart, BASS_ATTRIB_VOL, 0, 500);

		//reset the callback
		BASS_ChannelRemoveSync(m_streamCart, m_syncHandle);

		//remove from the global map:
		for(T_mapCallback::iterator i = g_mapCartCallbacks.begin();
			i != g_mapCartCallbacks.end(); i++)
		{
			if(i->first == m_syncHandle)
			{
				g_mapCartCallbacks.erase(i);
				break;
			}
		}

		//set the slide callback:
		BASS_ChannelSetSync(m_streamCart, BASS_SYNC_SLIDE, 
			0, &fnSlideCallback, 0);
	}

	//reset all members:
	m_streamCart = 0;
	m_syncHandle = 0;
}


//fnPageMoveOff - this is called when a page change is happening and the button
//needs to report back the cart that is playing (if any) and reset for the next
//button:
HSTREAM CCartsButton::fnPageMoveOff(HSYNC &syncHandle)
{
	HSTREAM stream = m_streamCart;
	if(m_streamCart != 0)
	{
		//set the sync handle:
		syncHandle = m_syncHandle;

		//reset the button:
		fnStop();

		return stream;
	}
	return 0;
}


//DrawItem - Overide the default draw function from CColourButton
//to allow us to write out the cart tile and duration at the 
//top and bottom resepectivly.
void CCartsButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//if the button is disabled then we can just let the
	//normal draw function draw it:
	if(m_bolState == false)
	{
		CColourButton::DrawItem(lpDrawItemStruct);

		//dont need to draw again so exit function:	
		return;
	}

	//draw it:
	CDC dc;
	CRect rt;

	//Get device context object
	dc.Attach(lpDrawItemStruct->hDC);     

	//get the button rect:
	rt = lpDrawItemStruct->rcItem;

	//create the text rect:
	CRect textRt(rt.left, 
		rt.top + 4, rt.right, rt.bottom - 4);	//used for text padding.

	//set the custom back colour wether it be flash or otherwise.
	if(m_bolFlashNextRedraw == true)
	{
		dc.FillSolidRect(rt, m_culFlash);
	}
	else
	{
		dc.FillSolidRect(rt, m_culBack);
	}

	//set the font colour:
	dc.SetTextColor(m_culFont);

	//draw the text before the boarder to prevent overlapping:
	CString strTemp;
	GetWindowText(strTemp);
	dc.DrawText(strTemp, textRt, DT_CENTER | DT_TOP | DT_WORDBREAK);

	//draw the duration at the bottom too:
	dc.DrawText(m_strPrintDuration, textRt, DT_CENTER | 
		DT_BOTTOM | DT_SINGLELINE);

	//get the state of the button:
	UINT state = lpDrawItemStruct->itemState;

	if ( (state & ODS_SELECTED) )            
	{
		// If it is pressed
		if(m_bolChangeColoursClick == true)
		{
			//change some colours round:
			dc.FillSolidRect(rt, m_culFont);
			dc.SetTextColor(m_culBack);
		}
		//draw the edge of the button:
		fnDrawButtonEdge(&dc, m_culEdge, rt);
	}
	else 
	{
		//draw the edge of the button:
		fnDrawButtonEdge(&dc, m_culEdge, rt);
	}

	//free memory:
	dc.Detach();
	
}

//fnPageMoveOn - this is called when a page change is happening and the button
//needs to be set as the cart it is playing for the page has not yet
//finished playing:
void CCartsButton::fnPageMoveOn(HSTREAM stream, HSYNC syncHandle)
{
	//set member variables:
	m_streamCart = stream;
	m_syncHandle = syncHandle;

	//recalculate the stream length:
	m_dblStreamLength = BASS_ChannelBytes2Seconds(m_streamCart, 
		BASS_ChannelGetLength(m_streamCart, BASS_POS_BYTE));
	
	//set the callback back to this cart button:
	g_mapCartCallbacks[syncHandle] = (CCallbackButton*)this;

	//get the timers going again:
	SetTimer(IDT_CARTFLASH, 500, 0);
	SetTimer(IDT_CARTTIME, 100, 0);
}

//OnEraseBkgnd - we overide this in order to stop redrawwindow() from redrawing the
//button if it hasn't changed. This will help stop button flickering.
BOOL CCartsButton::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

HSYNC CCartsButton::fnGetCurrentSync()
{
	return m_syncHandle;
}
void CCartsButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CCartsButton::OnBnClicked();
}
