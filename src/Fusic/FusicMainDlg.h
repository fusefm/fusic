#pragma once

#include "FusicCartsDlg.h"		//needed for access the carts dialog class.
#include "ColourStatic.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "colourbutton.h"
#include "ColourComboBox.h"
#include "colourlistbox.h"
#include "MainPlaylistController.h"
#include <map>
#include "FusicMainModel.h"
#include "ColourStaticTimer.h"
#include "FusicProgress.h"

class CFusicCartsDlg;

//forward declatation of the bass class:
class CMainBassControllerr;

#include "MainBassControllerr.h"
#include "backgroundcontroller.h"
// CFusicMainDlg dialog

class CFusicMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CFusicMainDlg)

public:
	CFusicMainDlg(CWnd* pParent = NULL, CFusicLoginDlg* dlg = NULL);   // standard constructor
	virtual ~CFusicMainDlg();

// Dialog Data
	enum { IDD = IDD_FUSIC_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	//public functions:

	// fnSyncCallbackEnd - called when the end of the current
	// stream is reached:
	static void CALLBACK fnSyncCallbackEnd(HSYNC handle, DWORD channel, 
													DWORD data, void* user);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// fnPlaylistPop - move all the songs on the playlist to the next one ready for cueing:
	void fnPlaylistPop(void);

	//button press events:
	afx_msg void OnBnClickedBtnNext();

	//called before the form is shown to set up the dialog:
	virtual BOOL OnInitDialog();

	//onCtlColor - called before the DrawItem() function is called
	//so we can use a differnt brush to paint the form black:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void doOk();
	//public members:
	//form controls:
	CColourStatic m_ctlColourStaticNowPlaying;
	CColourStatic m_ctlColourStaticTitle;
	CColourStatic m_ctlColourStaticArtist;
	CColourStatic m_ctlColourStaticDisplayTitle;
	CColourStatic m_ctlColourStaticDisplayArtist;
	CColourStatic m_ctlColourStaticTotalTime;
	CColourStatic m_ctlColourStaticRemainingTime;
	CColourStatic m_ctlColourStaticRemainingIntroTime;
	CColourStaticTimer m_ctlColourStaticDisplayTotalTime;
	CColourStaticTimer m_ctlColourStaticDisplayRemainingTime;
	CColourStaticTimer m_ctlColourStaticDisplayRemainingIntroTime;
	CColourStaticTimer m_ctlColourStaticTimeBeforeSweeper;
	CColourStaticTimer m_ctlColourStaticDisplayTimeBeforeSweeper;
	CFusicProgress m_ctlProgressSongProgress;
	CColourStatic m_ctlColourStaticDisplayPlaylists;
	CColourButton m_ctlColourButtonNext;
	CColourButton m_ctlColourButtonPause;
	CColourButton m_ctlColourButtonStop;

	//playlist next:
	CColourStatic m_ctlColourStaticDisplayPlaylistNext;
	CColourStatic m_ctlColourStaticDisplayPlaylistNextTitle;
	CColourStatic m_ctlColourStaticPlaylistNextTitle;
	CColourStatic m_ctlColourStaticDisplayPlaylistNextArtist;
	CColourStatic m_ctlColourStaticPlaylistNextArtist;
	CColourStatic m_ctlColourStaticDisplayPlaylistNextSweeper;
	CColourComboBox m_ctlComboBoxPlaylistNextSweeper;

	//playlist 2:
	CColourStatic m_ctlColourStaticDisplayPlaylist2;
	CColourStatic m_ctlColourStaticDisplayPlaylist2Title;
	CColourStatic m_ctlColourStaticPlaylist2Title;
	CColourStatic m_ctlColourStaticDisplayPlaylist2Artist;
	CColourStatic m_ctlColourStaticPlaylist2Artist;
	CColourStatic m_ctlColourStaticDisplayPlaylist2Sweeper;
	CColourComboBox m_ctlComboBoxPlaylist2Sweeper;

	//playlist 3:
	CColourStatic m_ctlColourStaticDisplayPlaylist3;
	CColourStatic m_ctlColourStaticDisplayPlaylist3Title;
	CColourStatic m_ctlColourStaticPlaylist3Title;
	CColourStatic m_ctlColourStaticDisplayPlaylist3Artist;
	CColourStatic m_ctlColourStaticPlaylist3Artist;
	CColourStatic m_ctlColourStaticDisplayPlaylist3Sweeper;
	CColourComboBox m_ctlComboBoxPlaylist3Sweeper;

	//playlist 4:
	CColourStatic m_ctlColourStaticDisplayPlaylist4;
	CColourStatic m_ctlColourStaticDisplayPlaylist4Title;
	CColourStatic m_ctlColourStaticPlaylist4Title;
	CColourStatic m_ctlColourStaticDisplayPlaylist4Artist;
	CColourStatic m_ctlColourStaticPlaylist4Artist;
	CColourStatic m_ctlColourStaticDisplayPlaylist4Sweeper;
	CColourComboBox m_ctlComboBoxPlaylist4Sweeper;

	//playlist 5:
	CColourStatic m_ctlColourStaticDisplayPlaylist5;
	CColourStatic m_ctlColourStaticDisplayPlaylist5Title;
	CColourStatic m_ctlColourStaticPlaylist5Title;
	CColourStatic m_ctlColourStaticDisplayPlaylist5Artist;
	CColourStatic m_ctlColourStaticPlaylist5Artist;
	CColourStatic m_ctlColourStaticDisplayPlaylist5Sweeper;
	CColourComboBox m_ctlComboBoxPlaylist5Sweeper;
	CColourListBox m_ctlColourListBoxSongSelect;

	//playlist controllers:
	CMainPlaylistController* m_plController1;
	CMainPlaylistController* m_plController2;
	CMainPlaylistController* m_plController3;
	CMainPlaylistController* m_plController4;
	CMainPlaylistController* m_plController5;

	//other public members:

	//playlistControllerMap - will map the playlist posistion to the
	//actual controller class:
	std::map<int, CMainPlaylistController*> m_playlistControllerMap;

	//indexFileIDMap - maps the File_ID for a file to it's index
	//in the listbox:
	std::map<int, int> m_indexFileIDMap;
	std::map<int, int> m_indexPlaylistIDMap;

		//main bass controller:
	CMainBassControllerr* m_bass;

private:

	//private member functions:

	// fnGetNexAvaliblePlaylistCart - get the next empty playlist cart that doesn't have a song cued up in it:
	CMainPlaylistController* fnGetNexAvaliblePlaylistCart(void);
	void fnSetupSongsListJingle(mysqlpp::StoreQueryResult res);

	//private member functions:
	void fnSetupSongsList(mysqlpp::StoreQueryResult res);

	//private member variables:

	//pointer to the carts window, this is needed to create (and destroy)
	//the carts window:
	CFusicCartsDlg* m_dlgCarts;
	HACCEL  m_hAccelTable;				//store accelerateor debug info.
	//fonts used on the form:
	CFont m_playlistFont;
	CFont m_songSelectFont;
	CFont m_playlistButtonFont;

	//the intro time for the currently playing song:
	double m_currentIntroTime;

	bool m_needProgressBarUpdate;

	//main data model class:
	CFusicMainModel m_data;

	CBrush m_dlgBackBrush;	//the brush that is used to paint the back of the dialog.
public:
	afx_msg void OnEnChangeEditSearchText();
	CEdit m_ctlColourEditSearchString;
	CColourComboBox m_ctlColourComboSearchType;
	afx_msg void OnCbnSelchangeComboSearchtype();
	CColourComboBox m_ctlColourComboBoxPlaylistSelection;
	afx_msg void OnCbnSelchangeComboPlaylistselect();
	afx_msg void OnLbnDblclkMainlist();
	afx_msg void OnBnClickedBtnPause();
	CColourStatic m_ctlColourStaticDisplayCurrentlyDisplaying;
	CColourStatic m_ctlColourStaticDisplaySearch;
	afx_msg void OnBnClickedBtnStop();
	CColourButton m_ctlColourButtonLoad1;
	CColourButton m_ctlColourButtonHalt1;
	CColourButton m_ctlColourButtonLoad2;
	CColourButton m_ctlColourButtonLoad3;
	CColourButton m_ctlColourButtonLoad4;
	CColourButton m_ctlColourButtonLoad5;
	CColourButton m_ctlColourButtonHalt2;
	CColourButton m_ctlColourButtonHalt3;
	CColourButton m_ctlColourButtonHalt4;
	CColourButton m_ctlColourButtonHalt5;
	afx_msg void OnBnClickedBtnLoadPlaylist1();
	afx_msg void OnBnClickedBtnLoadPlaylist2();
	afx_msg void OnBnClickedBtnLoadPlaylist3();
	afx_msg void OnBnClickedBtnLoadPlaylist4();
	afx_msg void OnBnClickedBtnLoadPlaylist5();
	afx_msg void OnBnClickedBtnHaltPlaylist1();
	afx_msg void OnBnClickedBtnHaltPlaylist2();
	afx_msg void OnBnClickedBtnHaltPlaylist3();
	afx_msg void OnBnClickedBtnHaltPlaylist4();
	afx_msg void OnBnClickedBtnHaltPlaylist5();
	CColourButton m_ctlColourButtonAutoDj;
	afx_msg void OnBnClickedButton1();
	CColourButton m_ctlColoutButtonClear;
	afx_msg void OnBnClickedBtnClear();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
