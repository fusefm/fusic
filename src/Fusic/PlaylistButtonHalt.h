
#pragma once
class CFusicMainDlg;
#include "ColourButton.h"
#include "MainPlaylistController.h"
// CPlaylistButtonHalt

class CPlaylistButtonHalt : public CColourButton
{
	DECLARE_DYNAMIC(CPlaylistButtonHalt)

public:
	CPlaylistButtonHalt(CFusicMainDlg* dlg = NULL);
	virtual ~CPlaylistButtonHalt();

	afx_msg void OnBnClicked();
	void setPlaylistController(CMainPlaylistController* pl);

protected:
	DECLARE_MESSAGE_MAP()

private:
	CMainPlaylistController* m_pl;
	CFusicMainDlg* m_dlg;
};


