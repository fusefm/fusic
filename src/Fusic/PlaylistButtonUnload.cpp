// PlaylistButtonUnload.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "PlaylistButtonUnload.h"
#include "FusicMainDlg.h"


// CPlaylistButtonUnload

IMPLEMENT_DYNAMIC(CPlaylistButtonUnload, CButton)

CPlaylistButtonUnload::CPlaylistButtonUnload()
{

}

void CPlaylistButtonUnload::setupSettings(int buttonPosistion, 
		std::map<int, CMainPlaylistController*>& map,
		CFusicMainDlg* dlg)
{
	myID = buttonPosistion;
	m_mapMap = map;
	m_dlg = dlg;
}

CPlaylistButtonUnload::~CPlaylistButtonUnload()
{
}


BEGIN_MESSAGE_MAP(CPlaylistButtonUnload, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CPlaylistButtonUnload::OnBnClicked)
END_MESSAGE_MAP()



// CPlaylistButtonUnload message handlers



void CPlaylistButtonUnload::OnBnClicked()
{
	// TODO: Add your control notification handler code here
	//find our controller...

	if(m_dlg->m_AutoDJActive == true)
	{
		return;
	}

	CMainPlaylistController* myPlaylistController = m_mapMap[myID];

	if(myPlaylistController->fnIsEmpty())
	{
		//the playlist is empty so, do nothing:
		return;
	}
	else
	{
		myPlaylistController->fnClear();
	}


	//if this is the last element then, we quit here:
	if(myID == 5)
	{
		return;
	}

	if(m_mapMap[myID+1]->fnIsEmpty())
	{
		//return as there is nothing else cued after this element.
		return;
	}

	//loop trhough each elment after:
	for(int i = myID; i < 5; i++)
	{
		m_mapMap[i]->fnSetState(m_mapMap[i+1]->fnGetState(true));
	}

}
