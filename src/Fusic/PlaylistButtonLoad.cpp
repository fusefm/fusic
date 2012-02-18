// PlaylistButtonLoad.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "PlaylistButtonLoad.h"
#include "FusicMainDlg.h"

// CPlaylistButtonLoad

IMPLEMENT_DYNAMIC(CPlaylistButtonLoad, CButton)

CPlaylistButtonLoad::CPlaylistButtonLoad(CFusicMainDlg* dlg)
{
	//set the reference to the main dialog:
	m_dlg = dlg;

	//set the playlist pointer to null:
	m_pl = NULL;
}

CPlaylistButtonLoad::~CPlaylistButtonLoad()
{
}


BEGIN_MESSAGE_MAP(CPlaylistButtonLoad, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CPlaylistButtonLoad::OnBnClicked)
END_MESSAGE_MAP()



// CPlaylistButtonLoad message handlers

void CPlaylistButtonLoad::fnSetPlaylistController(CMainPlaylistController* pl)
{
	m_pl = pl;
}

void CPlaylistButtonLoad::OnBnClicked()
	{
	//ensure that the Auto-DJ isn't active:
	if(m_dlg->m_AutoDJActive == true)
	{
		return;
	}

	//ensure a playlist controller has been set:
	if(m_pl == NULL)
	{
		return;
	}

	//get the type of files that are curretnly displayed,
	//either (jingles = -3) or (songs != -3)
	int playlistSelectionType = m_dlg->m_indexPlaylistIDMap[
		m_dlg->m_ctlColourComboBoxPlaylistSelection.GetCurSel()];

		//if this controllers is empty, we want to cue up in the next avalible
		//playlist posistion:
		if(m_pl->fnIsEmpty())
		{
			CMainPlaylistController* plEmpty;
			int selectionIndex = 0;

			//find the next avalible slot:
			plEmpty = m_dlg->fnGetNexAvaliblePlaylistCart();

			//get the index that is selected on the songs listbox:
			selectionIndex = m_dlg->m_ctlColourListBoxSongSelect.GetSelectionMark();

			if(selectionIndex == LB_ERR)
			{
				//nothing selected:
				return;
			}

			if(plEmpty == NULL)
			{
				//there isn't an avlible slot:
				return;
			}

			//cue up the song:
			if(playlistSelectionType  == -3)
			{
				//cue up a jingle:
				plEmpty->fnCueSong(m_dlg->m_indexFileIDMap[selectionIndex], -1, true);
			}
			else
			{
				//cue up a song:
				plEmpty->fnCueSong(m_dlg->m_indexFileIDMap[selectionIndex], -1, false);
			}
		}
		else
		{
			//there is a song already cued in this playlist element, so overwrite
			//what is currently selected:

			int selectionIndex = 0;

			//get the currently selected index from the songs list:
			selectionIndex = m_dlg->m_ctlColourListBoxSongSelect.GetSelectionMark();

			if(selectionIndex == LB_ERR)
			{
				//nothing selected:
				return;
			}

			if(playlistSelectionType == -3)
			{
				//cue up a jingle:
				m_pl->fnCueSong(m_dlg->m_indexFileIDMap[selectionIndex], -1, true);
			}
			else
			{
				//cue up a song:
				m_pl->fnCueSong(m_dlg->m_indexFileIDMap[selectionIndex], -1, false);
			}
		}
}
