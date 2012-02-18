#pragma once
// CPlaylistButtonUnload
#include <map>
#include "ColourButton.h"
#include "MainPlaylistController.h"

class CFusicMainDlg;

class CPlaylistButtonUnload : public CColourButton
{
	DECLARE_DYNAMIC(CPlaylistButtonUnload)

public:
	CPlaylistButtonUnload();
	virtual ~CPlaylistButtonUnload();

	void setupSettings(int buttonPosistion, 
		std::map<int, CMainPlaylistController*>& map,
		CFusicMainDlg* dlg);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();

private:
	int myID;
	std::map<int, CMainPlaylistController*> m_mapMap;
	CFusicMainDlg* m_dlg;
};


