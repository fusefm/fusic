// PlaylistButtonHalt.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "PlaylistButtonHalt.h"
#include "FusicMainDlg.h"

// CPlaylistButtonHalt

IMPLEMENT_DYNAMIC(CPlaylistButtonHalt, CButton)

CPlaylistButtonHalt::CPlaylistButtonHalt(CFusicMainDlg* dlg)
{
	//add a refernce to this buttons playlist controller:
	m_pl = NULL;
	m_dlg = dlg;
}

CPlaylistButtonHalt::~CPlaylistButtonHalt()
{
}


BEGIN_MESSAGE_MAP(CPlaylistButtonHalt, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CPlaylistButtonHalt::OnBnClicked)
END_MESSAGE_MAP()



// CPlaylistButtonHalt message handlers

void CPlaylistButtonHalt::setPlaylistController(CMainPlaylistController *pl)
{
	//set the playlist controller:
	if(pl != NULL)
	{
		m_pl = pl;
	}
}

void CPlaylistButtonHalt::OnBnClicked()
{
	//the playlist controller hasn't been set.
	if(m_pl == NULL)
	{
		return;
	}

	//ensure that the AutoDJ is active:
	if(m_dlg->m_AutoDJActive == true)
	{
		//if it is, we don't want to be able to activate it:
		return;
	}
	
	//ensure that a song is cued before setting auto-halt:
	if(m_pl->fnIsEmpty() == true)
	{
		return;
	}

	if(m_pl->fnNeedToHalt() == FALSE)
	{
		//set auto-halt:
		m_pl->fnSetHalt();
		
		//set the back colour to red:
		fnSetBackColour(255,0,0,true);

	}
	else
	{
		m_pl->fnResetHalt();

		//set the back colour back to black:
		fnSetBackColour(0,0,0,true);
	}
}
