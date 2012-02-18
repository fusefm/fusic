#pragma once

#include "FusicCartsDlg.h"		//needed for access the carts dialog class.
#include "ColourStatic.h"		//needed for colour static controls.
#include "colourbutton.h"		//needed for colour button controls.
#include "ColourComboBox.h"		//needed for colour combo box controls.
#include "colourlistbox.h"		//needed for colour list box controls.
#include "MainPlaylistController.h"	//needed for playlist controller refernces.
#include <map>					//needed for std::map.
#include "FusicMainModel.h"		//needed for the main model class
#include "AutoDJ.h"				//needed for the auto-dj class.
#include "ColourStaticTimer.h"	//needed for the static timer control.
#include "FusicProgress.h"		//needed for access to the colour progress control.

//forward declatation of classes:
class CAutoDJ;
class CFusicCartsDlg;
class CMainBassControllerr;
class CPlaylistButtonHalt;
class CPlaylistButtonLoad;
class CFusicLoginDlg;
#include "PlaylistButtonHalt.h"	//needed for the playlist button halt control.
#include "PlaylistButtonLoad.h"	//needed for the playlist button load control.
#include "MainBassControllerr.h"//needed for the main bass controller class.
#include "playlistbuttonunload.h"
#define WM_USER_THREAD_FINISHED (WM_USER+0x101)


struct searchPlaylistParam
{
	int playlistID;
	CString searchString;
	CFusicMainDlg* dlg;
};

// CFusicMainDlg dialog - impliments the main music dialog of Fusic.

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
	CWinThread* m_pThread;

	DECLARE_MESSAGE_MAP()

public:

	//public member functions:

	//OnInitDialog - called before the form is shown to set up the dialog:
	virtual BOOL OnInitDialog();

	LRESULT onThreadFinished(WPARAM wParam,LPARAM lParam);

	//fnSyncCallbackEnd - called when the end of the current
	//stream is reached:
	static void CALLBACK fnSyncCallbackEnd(HSYNC handle, DWORD channel, 
													DWORD data, void* user);

	// fnGetNexAvaliblePlaylistCart - get the next empty playlist cart that doesn't have a song cued up in it:
	CMainPlaylistController* fnGetNexAvaliblePlaylistCart(void);

	//OnTimer - A timer function to refresh coutdown timers and the progress bar:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//fnPlaylistPop - move all the songs on the playlist to the next one ready for cueing:
	void fnPlaylistPop(void);

	//fnFaderStart - handles a fader start.
	void fnFaderStart(void);

	//button press events:
	//OnBnClickedBtnNext - begin playing the next song:
	afx_msg void OnBnClickedBtnNext();

	//OnBnClickedBtnPause - pause the current song:
	afx_msg void OnBnClickedBtnPause();

	//OnBnClickedBtnStop - stop the current song:
	afx_msg void OnBnClickedBtnStop();

	//OnBnClickedAutoDJ - startup the auto-dj
	afx_msg void OnBnClickedAutoDJ();

	//OnBnClickedBtnClear - clear the currnet playlist:
	afx_msg void OnBnClickedBtnClear();

	//OnEnChangeEditSearchText - update the songs list when the text has changed:
	afx_msg void OnEnChangeEditSearchText();

	//OnCbnSelchangeComboSearchtype - change the search type from artist to song:
	afx_msg void OnCbnSelchangeComboSearchtype();

	//OnCbnSelchangeComboPlaylistselect - indicates that the user has selected
	//a differnt playlist or show to view:
	afx_msg void OnCbnSelchangeComboPlaylistselect();

	//OnLbnDblclkMainlist - The user has double clicked on an element on the Main song list box,
	//add it to the playlist:
	afx_msg void OnNMDblclkMainlist(NMHDR *pNMHDR, LRESULT *pResult);

	//PreTranslateMessage - used to catch accelerator keys:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	//onCtlColor - called before the DrawItem() function is called
	//so we can use a differnt brush to paint the form black:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//doOk - a public exporting of the OnOK() function for use by the
	//CartsDlg class:
	void doOk();

	//fnDoNextSong - will fade out the currently playing song (if any) and
	//begin to play the next song in the playlist (if any):
	void fnDoNextSong();

	//fnGetSettings - get the settings.
	void fnGetSettings();

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
	CEdit m_ctlColourEditSearchString;
	CColourComboBox m_ctlColourComboBoxPlaylistSelection;
	CColourStatic m_ctlColourStaticDisplayCurrentlyDisplaying;
	CColourStatic m_ctlColourStaticDisplaySearch;
	CColourButton m_ctlColourButtonAutoDj;
	CColourButton m_ctlColoutButtonClear;
CFusicLoginDlg* m_loginDlg;

	//playlist next:
	CColourStatic m_ctlColourStaticDisplayPlaylistNext;
	CColourStatic m_ctlColourStaticDisplayPlaylistNextTitle;
	CColourStatic m_ctlColourStaticPlaylistNextTitle;
	CColourStatic m_ctlColourStaticDisplayPlaylistNextArtist;
	CColourStatic m_ctlColourStaticPlaylistNextArtist;
	CColourStatic m_ctlColourStaticDisplayPlaylistNextSweeper;
	CColourComboBox m_ctlComboBoxPlaylistNextSweeper;
	CPlaylistButtonLoad m_ctlColourButtonLoad1;
	CPlaylistButtonHalt m_ctlColourButtonHalt1;

	//playlist 2:
	CColourStatic m_ctlColourStaticDisplayPlaylist2;
	CColourStatic m_ctlColourStaticDisplayPlaylist2Title;
	CColourStatic m_ctlColourStaticPlaylist2Title;
	CColourStatic m_ctlColourStaticDisplayPlaylist2Artist;
	CColourStatic m_ctlColourStaticPlaylist2Artist;
	CColourStatic m_ctlColourStaticDisplayPlaylist2Sweeper;
	CColourComboBox m_ctlComboBoxPlaylist2Sweeper;
	CPlaylistButtonLoad m_ctlColourButtonLoad2;
	CPlaylistButtonHalt m_ctlColourButtonHalt2;

	//playlist 3:
	CColourStatic m_ctlColourStaticDisplayPlaylist3;
	CColourStatic m_ctlColourStaticDisplayPlaylist3Title;
	CColourStatic m_ctlColourStaticPlaylist3Title;
	CColourStatic m_ctlColourStaticDisplayPlaylist3Artist;
	CColourStatic m_ctlColourStaticPlaylist3Artist;
	CColourStatic m_ctlColourStaticDisplayPlaylist3Sweeper;
	CColourComboBox m_ctlComboBoxPlaylist3Sweeper;
	CPlaylistButtonLoad m_ctlColourButtonLoad3;
	CPlaylistButtonHalt m_ctlColourButtonHalt3;


	//playlist 4:
	CColourStatic m_ctlColourStaticDisplayPlaylist4;
	CColourStatic m_ctlColourStaticDisplayPlaylist4Title;
	CColourStatic m_ctlColourStaticPlaylist4Title;
	CColourStatic m_ctlColourStaticDisplayPlaylist4Artist;
	CColourStatic m_ctlColourStaticPlaylist4Artist;
	CColourStatic m_ctlColourStaticDisplayPlaylist4Sweeper;
	CColourComboBox m_ctlComboBoxPlaylist4Sweeper;
	CPlaylistButtonLoad m_ctlColourButtonLoad4;
	CPlaylistButtonHalt m_ctlColourButtonHalt4;

	//playlist 5:
	CColourStatic m_ctlColourStaticDisplayPlaylist5;
	CColourStatic m_ctlColourStaticDisplayPlaylist5Title;
	CColourStatic m_ctlColourStaticPlaylist5Title;
	CColourStatic m_ctlColourStaticDisplayPlaylist5Artist;
	CColourStatic m_ctlColourStaticPlaylist5Artist;
	CColourStatic m_ctlColourStaticDisplayPlaylist5Sweeper;
	CColourComboBox m_ctlComboBoxPlaylist5Sweeper;
	CColourListBox m_ctlColourListBoxSongSelect;
	CPlaylistButtonLoad m_ctlColourButtonLoad5;
	CPlaylistButtonHalt m_ctlColourButtonHalt5;

	//playlist controllers:
	CMainPlaylistController* m_plController1;
	CMainPlaylistController* m_plController2;
	CMainPlaylistController* m_plController3;
	CMainPlaylistController* m_plController4;
	CMainPlaylistController* m_plController5;

	//other public members:

	//m_playlistControllerMap - will map the playlist posistion to the
	//actual controller class:
	std::map<int, CMainPlaylistController*> m_playlistControllerMap;

	//m_indexFileIDMap - maps the File_ID for a file to it's index
	//in the listbox:
	std::map<int, int> m_indexFileIDMap;

	std::map<int,int>m_FileIDIndexMap;

	//m_indexPlaylistIDMap - maps the Playlist_ID of a playlist to it's index
	//in the playlist selection listbox:
	std::map<int, int> m_indexPlaylistIDMap;

	//main bass controller:
	CMainBassControllerr* m_bass;
	
	//Indicate weather the Auto-DJ is active:
	bool m_AutoDJActive;

private:

	//private member functions:

	//fnSetupSongsListJingle - setup the song selection list box
	//for a list of jingles:
	void fnSetupSongsListJingle(mysqlpp::StoreQueryResult res);


	void fnCheckStrikeOut();

	//private member variables:

	CAutoDJ* m_dj;	//the autoDJ class.

	//pointer to the carts window, this is needed to create (and destroy)
	//the carts window:
	CFusicCartsDlg* m_dlgCarts;

	//accelerator table:
	HACCEL  m_hAccelTable;	

	//fonts used on the form:
	CFont m_playlistFont;
	CFont m_songSelectFont;
	CFont m_playlistButtonFont;

	//the intro time for the currently playing song:
	double m_currentIntroTime;

	//determins weather the progress bar needs erasing on next refresh:
	bool m_needProgressBarUpdate;



	CBrush m_dlgBackBrush;	//the brush that is used to paint the back of the dialog.
public:
	//main data model class:
	CFusicMainModel m_data;
	//fnSetupSongsList - setup the song selection list box
	//for a list of songs:
	void fnSetupSongsList(mysqlpp::StoreQueryResult res);

	CColourStatic m_ctlColourStaticDisplayPlaylistIntro1;
	CColourStatic m_ctlColourStaticDisplayPlaylistIntro2;
	CColourStatic m_ctlColourStaticDisplayPlaylistIntro3;
	afx_msg void OnLvnItemchangedMainlist(NMHDR *pNMHDR, LRESULT *pResult);
	CColourStatic m_ctlColourStaticDisplayPlaylistIntro4;
	CColourStatic m_ctlColourStaticDisplayPlaylistIntro5;
	CColourStatic m_ctlColourStaticPlaylistIntro1;
	CColourStatic m_ctlColourStaticPlaylistIntro2;
	CColourStatic m_ctlColourStaticPlaylistIntro3;
	CColourStatic m_ctlColourStaticPlaylistIntro4;
	CColourStatic m_ctlColourStaticPlaylistIntro5;
	CColourStatic m_ctlColourStaticDisplayMixOut1;
	CColourStatic m_ctlColourStaticDisplayMixOut2;
	CColourStatic m_ctlColourStaticDisplayMixOut3;
	CColourStatic m_ctlColourStaticDisplayMixOut4;
	CColourStatic m_ctlColourStaticDisplayMixOut5;
	CColourStatic m_ctlColourStatixPlaylistMixOut1;
	CColourStatic m_ctlColourStatixPlaylistMixOut2;
	CColourStatic m_ctlColourStatixPlaylistMixOut3;
	CColourStatic m_ctlColourStatixPlaylistMixOut4;
	CColourStatic m_ctlColourStatixPlaylistMixOut5;
	CColourStatic m_ctlColourStaticLoading;
	CPlaylistButtonUnload m_playlistButtonUnload1;
	CPlaylistButtonUnload m_playlistButtonUnload2;
	CPlaylistButtonUnload m_playlistButtonUnload3;
	CPlaylistButtonUnload m_playlistButtonUnload4;
	CPlaylistButtonUnload m_playlistButtonUnload5;
	CColourStatic m_PlaylistStaticAirTime1;
	CColourStatic m_PlaylistStaticAirTime2;
	CColourStatic m_PlaylistStaticAirTime3;
	CColourStatic m_PlaylistStaticAirTime4;
	CColourStatic m_PlaylistStaticAirTime5;
	CColourStatic m_playlistAirTime1;
	CColourStatic m_playlistAirTime2;
	CColourStatic m_playlistAirTime3;
	CColourStatic m_playlistAirTime4;
	CColourStatic m_playlistAirTime5;
	CColourButton m_ctlOnOffAir;
	afx_msg void OnBnClickedBtnOnoffair();
};
