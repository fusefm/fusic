#pragma once
class CFusicMainDlg;
#include "ColourButton.h"
#include "MainPlaylistController.h"

// CPlaylistButtonLoad - a class that handles the loading of songs
// into the playlist.

class CPlaylistButtonLoad : public CColourButton
{
	DECLARE_DYNAMIC(CPlaylistButtonLoad)

public:
	CPlaylistButtonLoad(CFusicMainDlg* dlg = NULL);
	virtual ~CPlaylistButtonLoad();

	void fnSetPlaylistController(CMainPlaylistController* pl);

	//OnBnClicked - handle the pressing of the load button:
	afx_msg void OnBnClicked();
protected:
	DECLARE_MESSAGE_MAP()

private:
	CMainPlaylistController* m_pl;	//a refernce to the playlist contoller class for which this button is assigned.
	CFusicMainDlg* m_dlg;			//a refernce to the main dialog.

};


