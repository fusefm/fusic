// FusicMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicMainDlg.h"
#include "MainPlaylistController.h"
#include <map>

//get global vars:
extern int g_intMusicDev;
extern int g_intShowID;

// CFusicMainDlg dialog

IMPLEMENT_DYNAMIC(CFusicMainDlg, CDialog)

CFusicMainDlg::CFusicMainDlg(CWnd* pParent, CFusicLoginDlg* dlg)
	: CDialog(CFusicMainDlg::IDD, pParent)
{
	//need to set up the carts window:
	m_dlgCarts = new CFusicCartsDlg(this, dlg, this);
	m_dlgCarts->Create(CFusicCartsDlg::IDD);
	m_dlgCarts->ShowWindow(SW_SHOW);
	m_dlgCarts->MoveWindow(1280, 0, 1280,1024);
	m_needProgressBarUpdate = false;

	//setup the accelerator:
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDR_ACCELERATOR_CART_DEBUG));
	
	//create the bass object:
	m_bass = new CMainBassControllerr(this);
}

CFusicMainDlg::~CFusicMainDlg()
{
	//stop memory leaks:
	if(m_bass)
		delete m_bass;
}
void CFusicMainDlg::doOk()
{
	OnOK();
}

void CFusicMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NOWPLAYING, m_ctlColourStaticNowPlaying);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_ctlColourStaticTitle);
	DDX_Control(pDX, IDC_STATC_ARTIST, m_ctlColourStaticArtist);
	DDX_Control(pDX, IDC_STATIC_DISPLAY_TITLE, m_ctlColourStaticDisplayTitle);
	DDX_Control(pDX, IDC_STATIC_DISPLAY_ARTIST, m_ctlColourStaticDisplayArtist);
	DDX_Control(pDX, IDC_STATIC_TOTALTIME, m_ctlColourStaticTotalTime);
	DDX_Control(pDX, IDC_STATIC_REMAININGTIME, m_ctlColourStaticRemainingTime);
	DDX_Control(pDX, IDC_STATIC_REMAININGINTROTIME, m_ctlColourStaticRemainingIntroTime);
	DDX_Control(pDX, IDC_STATC_DISPLAY_TOTALTIME, m_ctlColourStaticDisplayTotalTime);
	DDX_Control(pDX, IDC_STATC_DISPLAY_REMAININGTIME, m_ctlColourStaticDisplayRemainingTime);
	DDX_Control(pDX, IDC_STATC_DISPLAY_REMAININGINTROTIME, m_ctlColourStaticDisplayRemainingIntroTime);
	DDX_Control(pDX, IDC_STATIC_TIMEBEFORESWEEPER, m_ctlColourStaticTimeBeforeSweeper);
	DDX_Control(pDX, IDC_STATC_DISPLAY_TIMEBEFORESWEEPER, m_ctlColourStaticDisplayTimeBeforeSweeper);
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgressSongProgress);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST, m_ctlColourStaticDisplayPlaylists);
	DDX_Control(pDX, IDC_BTN_NEXT, m_ctlColourButtonNext);
	DDX_Control(pDX, IDC_BTN_PAUSE, m_ctlColourButtonPause);
	DDX_Control(pDX, IDC_BTN_STOP, m_ctlColourButtonStop);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT, m_ctlColourStaticDisplayPlaylistNext);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_TITLE, m_ctlColourStaticDisplayPlaylistNextTitle);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_TITLE_OUT, m_ctlColourStaticPlaylistNextTitle);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_ARTIST, m_ctlColourStaticDisplayPlaylistNextArtist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_ARTIST_OUT, m_ctlColourStaticPlaylistNextArtist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_SWEEPER, m_ctlColourStaticDisplayPlaylistNextSweeper);
	DDX_Control(pDX, IDC_COMBO_PLAYLIST_NEXT_SWEEPER_IN, m_ctlComboBoxPlaylistNextSweeper);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_2, m_ctlColourStaticDisplayPlaylist2);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_2_TITLE, m_ctlColourStaticDisplayPlaylist2Title);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_2_TITLE_OUT, m_ctlColourStaticPlaylist2Title);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_2_ARTIST, m_ctlColourStaticDisplayPlaylist2Artist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_2_ARTIST_OUT, m_ctlColourStaticPlaylist2Artist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_2_SWEEPER, m_ctlColourStaticDisplayPlaylist2Sweeper);
	DDX_Control(pDX, IDC_COMBO_PLAYLIST_2_SWEEPER_IN, m_ctlComboBoxPlaylist2Sweeper);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_3, m_ctlColourStaticDisplayPlaylist3);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_3_TITLE, m_ctlColourStaticDisplayPlaylist3Title);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_3_TITLE_OUT, m_ctlColourStaticPlaylist3Title);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_3_ARTIST, m_ctlColourStaticDisplayPlaylist3Artist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_3_ARTIST_OUT, m_ctlColourStaticPlaylist3Artist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_3_SWEEPER, m_ctlColourStaticDisplayPlaylist3Sweeper);
	DDX_Control(pDX, IDC_COMBO_PLAYLIST_3_SWEEPER_IN, m_ctlComboBoxPlaylist3Sweeper);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_4, m_ctlColourStaticDisplayPlaylist4);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_4_TITLE, m_ctlColourStaticDisplayPlaylist4Title);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_4_TITLE_OUT, m_ctlColourStaticPlaylist4Title);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_4_ARTIST, m_ctlColourStaticDisplayPlaylist4Artist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_4_ARTIST_OUT, m_ctlColourStaticPlaylist4Artist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_4_SWEEPER, m_ctlColourStaticDisplayPlaylist4Sweeper);
	DDX_Control(pDX, IDC_COMBO_PLAYLIST_4_SWEEPER_IN, m_ctlComboBoxPlaylist4Sweeper);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_5, m_ctlColourStaticDisplayPlaylist5);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_5_TITLE, m_ctlColourStaticDisplayPlaylist5Title);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_5_TITLE_OUT, m_ctlColourStaticPlaylist5Title);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_5_ARTIST, m_ctlColourStaticDisplayPlaylist5Artist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_5_ARTIST_OUT, m_ctlColourStaticPlaylist5Artist);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_5_SWEEPER, m_ctlColourStaticDisplayPlaylist5Sweeper);
	DDX_Control(pDX, IDC_COMBO_PLAYLIST_5_SWEEPER_IN, m_ctlComboBoxPlaylist5Sweeper);
	DDX_Control(pDX, IDC_MAINLIST, m_ctlColourListBoxSongSelect);
	DDX_Control(pDX, IDC_EDIT_SEARCH_TEXT, m_ctlColourEditSearchString);
	DDX_Control(pDX, IDC_COMBO_SEARCHTYPE, m_ctlColourComboSearchType);
	DDX_Control(pDX, IDC_COMBO_PLAYLISTSELECT, m_ctlColourComboBoxPlaylistSelection);
	DDX_Control(pDX, IDC_STATIC_CURRENTLYDISPLAYING, m_ctlColourStaticDisplayCurrentlyDisplaying);
	DDX_Control(pDX, IDC_STATIC_SEARCH, m_ctlColourStaticDisplaySearch);

	DDX_Control(pDX, IDC_BTN_LOAD_PLAYLIST1, m_ctlColourButtonLoad1);
	DDX_Control(pDX, IDC_BTN_HALT_PLAYLIST1, m_ctlColourButtonHalt1);
	DDX_Control(pDX, IDC_BTN_LOAD_PLAYLIST2, m_ctlColourButtonLoad2);
	DDX_Control(pDX, IDC_BTN_LOAD_PLAYLIST3, m_ctlColourButtonLoad3);
	DDX_Control(pDX, IDC_BTN_LOAD_PLAYLIST4, m_ctlColourButtonLoad4);
	DDX_Control(pDX, IDC_BTN_LOAD_PLAYLIST5, m_ctlColourButtonLoad5);
	DDX_Control(pDX, IDC_BTN_HALT_PLAYLIST2, m_ctlColourButtonHalt2);
	DDX_Control(pDX, IDC_BTN_HALT_PLAYLIST3, m_ctlColourButtonHalt3);
	DDX_Control(pDX, IDC_BTN_HALT_PLAYLIST4, m_ctlColourButtonHalt4);
	DDX_Control(pDX, IDC_BTN_HALT_PLAYLIST5, m_ctlColourButtonHalt5);
	DDX_Control(pDX, IDC_BUTTON1, m_ctlColourButtonAutoDj);
	DDX_Control(pDX, IDC_BTN_CLEAR, m_ctlColoutButtonClear);
}


BEGIN_MESSAGE_MAP(CFusicMainDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_NEXT, &CFusicMainDlg::OnBnClickedBtnNext)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_SEARCH_TEXT, &CFusicMainDlg::OnEnChangeEditSearchText)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCHTYPE, &CFusicMainDlg::OnCbnSelchangeComboSearchtype)
	ON_CBN_SELCHANGE(IDC_COMBO_PLAYLISTSELECT, &CFusicMainDlg::OnCbnSelchangeComboPlaylistselect)
	ON_LBN_DBLCLK(IDC_MAINLIST, &CFusicMainDlg::OnLbnDblclkMainlist)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CFusicMainDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CFusicMainDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_LOAD_PLAYLIST1, &CFusicMainDlg::OnBnClickedBtnLoadPlaylist1)
	ON_BN_CLICKED(IDC_BTN_LOAD_PLAYLIST2, &CFusicMainDlg::OnBnClickedBtnLoadPlaylist2)
	ON_BN_CLICKED(IDC_BTN_LOAD_PLAYLIST3, &CFusicMainDlg::OnBnClickedBtnLoadPlaylist3)
	ON_BN_CLICKED(IDC_BTN_LOAD_PLAYLIST4, &CFusicMainDlg::OnBnClickedBtnLoadPlaylist4)
	ON_BN_CLICKED(IDC_BTN_LOAD_PLAYLIST5, &CFusicMainDlg::OnBnClickedBtnLoadPlaylist5)
	ON_BN_CLICKED(IDC_BTN_HALT_PLAYLIST1, &CFusicMainDlg::OnBnClickedBtnHaltPlaylist1)
	ON_BN_CLICKED(IDC_BTN_HALT_PLAYLIST2, &CFusicMainDlg::OnBnClickedBtnHaltPlaylist2)
	ON_BN_CLICKED(IDC_BTN_HALT_PLAYLIST3, &CFusicMainDlg::OnBnClickedBtnHaltPlaylist3)
	ON_BN_CLICKED(IDC_BTN_HALT_PLAYLIST4, &CFusicMainDlg::OnBnClickedBtnHaltPlaylist4)
	ON_BN_CLICKED(IDC_BTN_HALT_PLAYLIST5, &CFusicMainDlg::OnBnClickedBtnHaltPlaylist5)
	ON_BN_CLICKED(IDC_BUTTON1, &CFusicMainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CFusicMainDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CFusicMainDlg message handlers

BOOL CFusicMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//setup BASS:
	BASS_Init(g_intMusicDev, 44100,0 ,NULL ,NULL);

	//form Looks:::

	//create a new solid brush to paint the back of the 
	//dialog:
	m_dlgBackBrush.CreateSolidBrush(RGB(0,0,0));

	//setup the dialog controls colours:
	//nowplaying (static):
	m_ctlColourStaticNowPlaying.fnSetTextColour(255,255,0);
	
	//nowplaying title and artist (static):
	m_ctlColourStaticTitle.fnSetTextColour(255,255,0);
	m_ctlColourStaticArtist.fnSetTextColour(255,255,0);

	//nowplaying title and artist (display):
	m_ctlColourStaticDisplayArtist.fnSetTextColour(255,255,255);
	m_ctlColourStaticDisplayTitle.fnSetTextColour(255,255,255);

	//the timers:
	m_ctlColourStaticTotalTime.fnSetTextColour(255,255,0);
	m_ctlColourStaticRemainingTime.fnSetTextColour(255,255,0);
	m_ctlColourStaticRemainingIntroTime.fnSetTextColour(255,255,0);
	m_ctlColourStaticTimeBeforeSweeper.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayTotalTime.fnSetTextColour(120,120,120);
	m_ctlColourStaticDisplayRemainingTime.fnSetTextColour(120,120,120);
	m_ctlColourStaticDisplayRemainingIntroTime.fnSetTextColour(120,120,120);
	m_ctlColourStaticDisplayTimeBeforeSweeper.fnSetTextColour(120,120,120);

	//the progress bar:
	m_ctlProgressSongProgress.SetBkColor(RGB(0,0,0));
	m_ctlProgressSongProgress.SetBarColor(RGB(0,255,0));

	//create the playlist font:
	m_playlistFont.CreateFont(15, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Arial");

	//and some others:
	m_playlistButtonFont.CreateFont(17, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Arial");

	

	//playlist things (next):
	m_ctlColourStaticDisplayPlaylistNext.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylistNextArtist.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylistNextArtist.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistNextArtist.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylistNextTitle.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylistNextTitle.fnSetTextColour(255,255,0);
	m_ctlColourStaticPlaylistNextTitle.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylists.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylistNextSweeper.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylistNextSweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylistNextSweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylistNextSweeper.fnSetBackColour(RGB(0,0,0));
	m_ctlComboBoxPlaylistNextSweeper.fnSetTextColour(RGB(255,255,255));

	//playlist things (2):
	m_ctlColourStaticDisplayPlaylist2.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist2Artist.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist2Artist.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylist2Artist.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylist2Title.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylist2Title.fnSetTextColour(255,255,0);
	m_ctlColourStaticPlaylist2Title.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylists.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist2Sweeper.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist2Sweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylist2Sweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylist2Sweeper.fnSetBackColour(RGB(0,0,0));
	m_ctlComboBoxPlaylist2Sweeper.fnSetTextColour(RGB(255,255,255));

	//playlist things (3):
	m_ctlColourStaticDisplayPlaylist3.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist3Artist.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist3Artist.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylist3Artist.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylist3Title.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylist3Title.fnSetTextColour(255,255,0);
	m_ctlColourStaticPlaylist3Title.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylists.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist3Sweeper.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist3Sweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylist3Sweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylist3Sweeper.fnSetBackColour(RGB(0,0,0));
	m_ctlComboBoxPlaylist3Sweeper.fnSetTextColour(RGB(255,255,255));

	//playlist things (4):
	m_ctlColourStaticDisplayPlaylist4.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist4Artist.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist4Artist.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylist4Artist.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylist4Title.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylist4Title.fnSetTextColour(255,255,0);
	m_ctlColourStaticPlaylist4Title.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylists.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist4Sweeper.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist4Sweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylist4Sweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylist4Sweeper.fnSetBackColour(RGB(0,0,0));
	m_ctlComboBoxPlaylist4Sweeper.fnSetTextColour(RGB(255,255,255));

	//playlist things (5):
	m_ctlColourStaticDisplayPlaylist5.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist5Artist.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist5Artist.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylist5Artist.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylist5Title.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylist5Title.fnSetTextColour(255,255,0);
	m_ctlColourStaticPlaylist5Title.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylists.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist5Sweeper.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylist5Sweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylist5Sweeper.SetFont(&m_playlistFont);
	m_ctlComboBoxPlaylist5Sweeper.fnSetBackColour(RGB(0,0,0));
	m_ctlComboBoxPlaylist5Sweeper.fnSetTextColour(RGB(255,255,255));

	m_ctlColourEditSearchString.SetFont(&m_playlistFont);
	m_ctlColourEditSearchString.SetWindowPos(this, 0,0,221, 8, SWP_NOMOVE);

	//buttons:
	m_ctlColourButtonNext.fnSetBackColour(0,255,0,true);
	m_ctlColourButtonNext.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonNext.fnSetFontColour(0,0,0);
	m_ctlColourButtonPause.fnSetBackColour(255,255,0,true);
	m_ctlColourButtonPause.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonPause.fnSetFontColour(0,0,0);
	m_ctlColourButtonStop.fnSetBackColour(255,0,0,true);
	m_ctlColourButtonStop.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonStop.fnSetFontColour(0,0,0);
	m_ctlColoutButtonClear.fnSetFontColour(255,255,255);
	m_ctlColoutButtonClear.fnSetBackColour(0,0,0,true);
	m_ctlColoutButtonClear.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonAutoDj.fnSetBackColour(0,0,0,true);
	m_ctlColourButtonAutoDj.fnSetFontColour(255,255,255);
	m_ctlColourButtonAutoDj.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonLoad1.fnSetBackColour(0,0,0 ,true);
	m_ctlColourButtonLoad1.fnSetFontColour(255,255,255);
	m_ctlColourButtonLoad1.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonHalt1.fnSetBackColour(0,0,0,true);
	m_ctlColourButtonHalt1.fnSetFontColour(255,255,255);
	m_ctlColourButtonHalt1.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonLoad2.fnSetBackColour(0,0,0 ,true);
	m_ctlColourButtonLoad2.fnSetFontColour(255,255,255);
	m_ctlColourButtonLoad2.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonHalt2.fnSetBackColour(0,0,0,true);
	m_ctlColourButtonHalt2.fnSetFontColour(255,255,255);
	m_ctlColourButtonHalt2.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonLoad3.fnSetBackColour(0,0,0 ,true);
	m_ctlColourButtonLoad3.fnSetFontColour(255,255,255);
	m_ctlColourButtonLoad3.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonHalt3.fnSetBackColour(0,0,0,true);
	m_ctlColourButtonHalt3.fnSetFontColour(255,255,255);
	m_ctlColourButtonHalt3.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonLoad4.fnSetBackColour(0,0,0 ,true);
	m_ctlColourButtonLoad4.fnSetFontColour(255,255,255);
	m_ctlColourButtonLoad4.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonHalt4.fnSetBackColour(0,0,0,true);
	m_ctlColourButtonHalt4.fnSetFontColour(255,255,255);
	m_ctlColourButtonHalt4.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonLoad5.fnSetBackColour(0,0,0 ,true);
	m_ctlColourButtonLoad5.fnSetFontColour(255,255,255);
	m_ctlColourButtonLoad5.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonHalt5.fnSetBackColour(0,0,0,true);
	m_ctlColourButtonHalt5.fnSetFontColour(255,255,255);
	m_ctlColourButtonHalt5.fnSetEdgeColour(255,255,255);
	m_ctlColourButtonHalt1.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonHalt2.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonHalt3.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonHalt4.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonHalt5.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonLoad1.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonLoad2.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonLoad3.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonLoad4.SetFont(&m_playlistButtonFont);
	m_ctlColourButtonLoad5.SetFont(&m_playlistButtonFont);


	//create the song select font:
	m_songSelectFont.CreateFont(18, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Arial");
	//the main list box:
	//m_ctlColourListBoxSongSelect.fnSetBackColour(0,0,0);
	//m_ctlColourListBoxSongSelect.fnSetTextColour(255,255,255);
	m_ctlColourListBoxSongSelect.SetFont(&m_songSelectFont);

	//now resize each item:
	m_ctlColourListBoxSongSelect.SetItemHeight(0,20);
	
	//setup controllers:
	m_plController1 = new CMainPlaylistController(1,
		&m_ctlColourStaticPlaylistNextTitle,
		&m_ctlColourStaticPlaylistNextArtist,
		&m_ctlComboBoxPlaylistNextSweeper, &m_data);

	m_plController2 = new CMainPlaylistController(2,
		&m_ctlColourStaticPlaylist2Title,
		&m_ctlColourStaticPlaylist2Artist,
		&m_ctlComboBoxPlaylist2Sweeper, &m_data);


	m_plController3 = new CMainPlaylistController(3,
		&m_ctlColourStaticPlaylist3Title,
		&m_ctlColourStaticPlaylist3Artist,
		&m_ctlComboBoxPlaylist3Sweeper, &m_data);

	m_plController4 = new CMainPlaylistController(4,
		&m_ctlColourStaticPlaylist4Title,
		&m_ctlColourStaticPlaylist4Artist,
		&m_ctlComboBoxPlaylist4Sweeper, &m_data);

	m_plController5 = new CMainPlaylistController(5,
		&m_ctlColourStaticPlaylist5Title,
		&m_ctlColourStaticPlaylist5Artist,
		&m_ctlComboBoxPlaylist5Sweeper, &m_data);


	//add to the map:
	m_playlistControllerMap[1] = m_plController1;
	m_playlistControllerMap[2] = m_plController2;
	m_playlistControllerMap[3] = m_plController3;
	m_playlistControllerMap[4] = m_plController4;
	m_playlistControllerMap[5] = m_plController5;

	//other crap:
	m_ctlColourComboSearchType.fnSetBackColour(0,0,0);
	m_ctlColourComboSearchType.fnSetTextColour(255,255,255);
	m_ctlColourComboSearchType.SetFont(&m_playlistFont);
	m_ctlColourComboSearchType.AddString("By Artist");
	m_ctlColourComboSearchType.AddString("By Title");
	m_ctlColourComboSearchType.SetCurSel(0);
	m_ctlColourComboBoxPlaylistSelection.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplaySearch.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayCurrentlyDisplaying.fnSetTextColour(255,255,0);

	//setup the playlist selection box:
	//firstly fuse stuff:
	m_indexPlaylistIDMap[
		m_ctlColourComboBoxPlaylistSelection.AddString("Fuse: All Songs")] 
		= -1;
	
	//get fuse playlists:
	mysqlpp::StoreQueryResult res = m_data.fnGetPlaylistInfroForShowID(0);
	for(size_t i = 0; i < res.size(); i++)
	{
		CString temp;
		temp = "Fuse: ";
		temp.Append(res[i]["Playlist_Name"]);
		m_indexPlaylistIDMap[
			m_ctlColourComboBoxPlaylistSelection.AddString(temp)]
			= res[i]["Playlist_ID"];
	}

	//get fuse Jingles:
	m_indexPlaylistIDMap[
		m_ctlColourComboBoxPlaylistSelection.AddString("Fuse: Jingles")] 
		= -3;

	//get the user playlist info:
	res = m_data.fnGetPlaylistInfroForShowID(g_intShowID);
	if(g_intShowID != 0)
	{
		//only run if we arn't the fuse show:
		//add the user's songs to the playlist selection:
		m_indexPlaylistIDMap[
			m_ctlColourComboBoxPlaylistSelection.AddString("User: All Songs")] 
			= -2;
		
		//loop through each playlist and add it to the listox and the map:
		for(size_t i = 0; i < res.size(); i++)
		{
			CString temp;
			temp = "User: ";
			temp.Append(res[i]["Playlist_Name"]);
			m_indexPlaylistIDMap[
				m_ctlColourComboBoxPlaylistSelection.AddString(temp)]
				= res[i]["Playlist_ID"];
		}//for
	}//if

	//we are going to being with FUSE:
	m_ctlColourComboBoxPlaylistSelection.SetCurSel(0);


	//need to add song data. Start with the fuse show:
	fnSetupSongsList(m_data.fnGetSongsFowShowId(0));

	//ensure we are in the right posistion:
	MoveWindow(0,0,1280,1024);

	m_ctlColourListBoxSongSelect.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
}

HBRUSH CFusicMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{


	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	// TODO:  Change any attributes of the DC here
	//see if its the form we need to paint:
	if(nCtlColor == CTLCOLOR_DLG)
	{
		//return our black brush.
		return m_dlgBackBrush;
	}

	//also change the combo box xcolours:

	
	if(nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() ==
		IDC_COMBO_SEARCHTYPE)
	{
		pDC->SetTextColor(RGB(255,255,0));
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}

	if(nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkColor(RGB(0,0,0));
	}
	return (HBRUSH)::GetStockObject(BLACK_BRUSH);
}
	

//private member functions implimentation::

//fnSetupSongsList - will setup the main listbox with all the songs
//for a show ID. Will also populate the File_ID map with the indexes
//for the songs in the listbox:
void CFusicMainDlg::fnSetupSongsList(mysqlpp::StoreQueryResult res)
{
	CString temp;
	for(size_t i = 0; i < (int)res.size(); i++)
	{
		int index;
		temp.Empty();

		temp = (CString)res[i]["File_Artist"];
		temp += " - ";
		temp += (CString)res[i]["File_Title"];
		temp += "\0";

		//add a string to the list:
		index = m_ctlColourListBoxSongSelect.AddString(temp);
		m_indexFileIDMap[index] = (int)res[i]["File_ID"];
	}
}

void CFusicMainDlg::fnSetupSongsListJingle(mysqlpp::StoreQueryResult res)
{
	CString temp;
	for(size_t i = 0; i < (int)res.size(); i++)
	{
		int index;
		temp.Empty();

		temp += (CString)res[i]["File_Title"];
		temp += "\0";

		//add a string to the list:
		index = m_ctlColourListBoxSongSelect.AddString(temp);
		m_indexFileIDMap[index] = (int)res[i]["File_ID"];
	}
}
// fnGetNexAvaliblePlaylistCart - get the next empty playlist cart that doesn't have a song cued up in it:
CMainPlaylistController* CFusicMainDlg::fnGetNexAvaliblePlaylistCart(void)
{
	for(std::map<int, CMainPlaylistController*>::iterator i =
		m_playlistControllerMap.begin(); i != m_playlistControllerMap.end();
		i++)
	{
		if(i->second->fnIsEmpty() == true)
		{
			return i->second;
		}
	}
	return NULL;
}

void CFusicMainDlg::OnBnClickedBtnNext()
{
	int fileID = 0;
	int fileIDSweeper = -1;
	double introTime = 0;
	double length = 0;
	mysqlpp::StoreQueryResult resSweeper;
	//start a song playing:

	//firstly ensure that a song is cued:
	if(m_plController1->fnIsEmpty() == true)
	{
		return;
	}

	fileID = m_plController1->fnGetFileID();
	fileIDSweeper = m_plController1->fnGetSweeperFileID();

	//clear the playlist element:
	m_plController1->fnClear();

	if(fileIDSweeper != -1)
	{
		resSweeper = m_data.fnGetSongInfoForFileID(fileIDSweeper);
	}

	//get all the data we need:
	mysqlpp::StoreQueryResult res = m_data.fnGetSongInfoForFileID(fileID);
	CString title = (CString)res[0]["File_Title"];
	CString artist = (CString)res[0]["File_Artist"];


	//set the main controls:
	m_ctlColourStaticDisplayArtist.SetWindowTextA(artist);
	m_ctlColourStaticDisplayTitle.SetWindowTextA(title);

	if(m_bass->fnPlaying() == TRUE)
	{
		m_bass->fnFadeOut(1000, false);
	}

	//start the song playing:
	if(fileIDSweeper != -1)
	{
		length = m_bass->fnStartWithSweeper((CString)(CString)res[0]["File_Location"],
			(CString)resSweeper[0]["File_Location"],
			m_data.fnGetIntroTimeForFileID(fileID));
	}
	else
	{
		length = m_bass->fnStart((CString)res[0]["File_Location"]);
	}

	//set the length of the stream:
	m_ctlColourStaticDisplayTotalTime.fnSetCounterValue(length);

	//ensure that the progress bar updates it'self:
	m_needProgressBarUpdate = true;

	//also get the intro time of the file:
	introTime = m_data.fnGetIntroTimeForFileID(fileID);
	m_currentIntroTime = introTime;
	m_ctlColourStaticDisplayRemainingIntroTime.fnSetCounterValue(introTime);

	if(introTime == 0)
	{
		m_ctlProgressSongProgress.fnSetTotalTime(length);
	}
	else
	{
		if(fileIDSweeper != -1)
		{
			m_ctlColourStaticDisplayTimeBeforeSweeper.fnSetCounterValue(introTime - 
				m_bass->fnGetSweeperLength() - 0.6);
		}
		m_ctlProgressSongProgress.fnSetTotalTime(introTime);
	}
	
	//log the play:
	m_data.fnLogFile(fileID);

	//start a timer for countdown timers:
	SetTimer(WM_USER + 10, 20, 0);

	//do the pop:
	fnPlaylistPop();
}

// fnSyncCallbackEnd - called when the end of the current stream is reached:
void CALLBACK CFusicMainDlg::fnSyncCallbackEnd(HSYNC handle, DWORD channel, 
													DWORD data, void* user)
{
	CFusicMainDlg* dlg = (CFusicMainDlg*)user;
	
	//stop the timer:
	dlg->KillTimer(WM_USER + 10);

	//cleanup bass:
	dlg->m_bass->fnReset();
	BOOL halt = dlg->m_plController1->fnNeedToHalt();

	//the stream has stopped, only contine if auto halt isn't set
	//or there isn't another song in the playlist:
	if((dlg->m_plController1->fnIsEmpty() == false)
		&& (dlg->m_plController1->fnNeedToHalt() == false))
	{
		//we need to get the next playlist element:
		dlg->OnBnClickedBtnNext();
		return;
	}

	//there isn't another song cued so just:
	
	//reset all controls:
	dlg->m_ctlColourStaticDisplayArtist.SetWindowTextA("Empty");
	dlg->m_ctlColourStaticDisplayTitle.SetWindowTextA("Empty");
	dlg->m_ctlColourStaticDisplayTotalTime.fnSetCounterValue(0);
	dlg->m_ctlColourStaticDisplayRemainingIntroTime.fnSetCounterValue(0);
	dlg->m_ctlColourStaticDisplayRemainingTime.fnSetCounterValue(0);
	dlg->m_ctlProgressSongProgress.fnReset();
}

void CFusicMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == WM_USER + 10)
	{
		if(m_bass->fnGetTimeElapsed() >= m_currentIntroTime)
		{
			if(m_needProgressBarUpdate == true)
			{

				m_ctlProgressSongProgress.fnSetTotalTime(m_bass->fnGetTimeElapsed()
					+ m_bass->fnGetTimeRemaining());
				m_needProgressBarUpdate = false;

			}

				 //update the progress bar:
			m_ctlProgressSongProgress.fnSetTimeRemain(
				m_bass->fnGetTimeRemaining());
		}
		else
		{
			m_ctlProgressSongProgress.fnSetTimeRemain(
				m_currentIntroTime - m_bass->fnGetTimeElapsed(), false);
		}

		//update counters:
		m_ctlColourStaticDisplayRemainingTime.fnSetCounterValue(
			m_bass->fnGetTimeRemaining());
		m_ctlColourStaticDisplayRemainingIntroTime.fnSetCounterValue(
			m_currentIntroTime - m_bass->fnGetTimeElapsed() );
		if(m_currentIntroTime != 0 && (m_bass->fnGetSweeperLength() != 0) )
		{
			m_ctlColourStaticDisplayTimeBeforeSweeper.fnSetCounterValue(
				(m_currentIntroTime - m_bass->fnGetSweeperLength() - 0.6) - 
				m_bass->fnGetTimeElapsed());
		}
	}
	CDialog::OnTimer(nIDEvent);
}

// fnPlaylistPop - move all the songs on the playlist to the next one ready for cueing:
void CFusicMainDlg::fnPlaylistPop(void)
{
	int tempID5;
	int tempID4;
	int tempID3;
	int tempID2;
	int tempSeeep5;
	int tempSeeep4;
	int tempSeeep3;
	int tempSeeep2;
	bool halt5;
	bool halt4;
	bool halt3;
	bool halt2;

	//get all file IDS:
	tempID5 = m_plController5->fnGetFileID();
	tempID4 = m_plController4->fnGetFileID();
	tempID3 = m_plController3->fnGetFileID();
	tempID2 = m_plController2->fnGetFileID();

	tempSeeep5 = m_plController5->fnGetSweepIndex();
	tempSeeep4 = m_plController4->fnGetSweepIndex();
	tempSeeep3 = m_plController3->fnGetSweepIndex();
	tempSeeep2 = m_plController2->fnGetSweepIndex();
	
	halt5 = m_plController5->fnNeedToHalt();
	halt4 = m_plController4->fnNeedToHalt();
	halt3 = m_plController3->fnNeedToHalt();
	halt2 = m_plController2->fnNeedToHalt();

	//clear all playlists:
	m_plController5->fnClear();
	m_plController4->fnClear();
	m_plController3->fnClear();
	m_plController2->fnClear();
	m_plController1->fnClear();

	//also halt buttons:
	m_ctlColourButtonHalt1.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt2.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt3.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt4.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt5.fnSetBackColour(RGB(0,0,0));


	//set the songs:
	if(tempID5 != -1)
	{
		m_plController4->fnCueSong(tempID5, tempSeeep5);
		if(halt5)
		{
			m_plController4->fnSetHalt();
			m_ctlColourButtonHalt4.fnSetBackColour(RGB(255,0,0));
		}
	}

	if(tempID4 != -1)
	{
		m_plController3->fnCueSong(tempID4, tempSeeep4);
		if(halt4)
		{
			m_plController3->fnSetHalt();
			m_ctlColourButtonHalt3.fnSetBackColour(RGB(255,0,0));
		}
	}
	
	if(tempID3 != -1)
	{
		m_plController2->fnCueSong(tempID3, tempSeeep3);
		if(halt3)
		{
			m_plController2->fnSetHalt();
			m_ctlColourButtonHalt2.fnSetBackColour(RGB(255,0,0));
		}
	}
	if(tempID2 != -1)
	{
		m_plController1->fnCueSong(tempID2, tempSeeep2);
		if(halt2)
		{
			m_plController1->fnSetHalt();
			m_ctlColourButtonHalt1.fnSetBackColour(RGB(255,0,0));
		}
	}
}

void CFusicMainDlg::OnEnChangeEditSearchText()
{
	CString searchString;

	//get the text:
	m_ctlColourEditSearchString.GetWindowTextA(searchString);

	//clear the list box of it's current content:
	m_ctlColourListBoxSongSelect.ResetContent();
	
	//also clear out the map:
	m_indexFileIDMap.clear();
	//do the search:
	if(searchString != "")
	{
		if(m_indexPlaylistIDMap[
			m_ctlColourComboBoxPlaylistSelection.GetCurSel()] == -1)
		{
			if(m_ctlColourComboSearchType.GetCurSel() == 0)
			{
				fnSetupSongsList(m_data.fnGetSongsForShowIDAndSearchStringArtist(
					0, searchString));
			}
			else if(m_ctlColourComboSearchType.GetCurSel() == 1)
			{
				fnSetupSongsList(m_data.fnGetSongsForShowIDAndSearchStringSong(
					0, searchString));
			}
		}
		else if(m_indexPlaylistIDMap[
			m_ctlColourComboBoxPlaylistSelection.GetCurSel()] == -2)
		{
			if(m_ctlColourComboSearchType.GetCurSel() == 0)
			{
				fnSetupSongsList(m_data.fnGetSongsForShowIDAndSearchStringArtist(
					g_intShowID, searchString));
			}
			else if(m_ctlColourComboSearchType.GetCurSel() == 1)
			{
				fnSetupSongsList(m_data.fnGetSongsForShowIDAndSearchStringSong(
					g_intShowID, searchString));
			}
		}
		else
		{
			if(m_ctlColourComboSearchType.GetCurSel() == 0)
			{
				fnSetupSongsList(m_data.fnGetFilesForPlaylistIDAndSearchArtist(
					m_indexPlaylistIDMap[
						m_ctlColourComboBoxPlaylistSelection.GetCurSel()], searchString));
			}
			else if(m_ctlColourComboSearchType.GetCurSel() == 1)
			{
				fnSetupSongsList(m_data.fnGetFilesForPlaylistIDAndSearchTitle(
					m_indexPlaylistIDMap[
						m_ctlColourComboBoxPlaylistSelection.GetCurSel()], searchString));
			}
		}
	}
	else
	{
		//if the text field is blank we want all the songs:
		OnCbnSelchangeComboPlaylistselect();
	}
	
}

void CFusicMainDlg::OnCbnSelchangeComboSearchtype()
{
	// TODO: Add your control notification handler code here
	OnEnChangeEditSearchText();
}

void CFusicMainDlg::OnCbnSelchangeComboPlaylistselect()
{

	int nIndex;
	int plID;
	nIndex = m_ctlColourComboBoxPlaylistSelection.GetCurSel();
	m_ctlColourListBoxSongSelect.ResetContent();
	plID = m_indexPlaylistIDMap[nIndex];
	if(plID != -3)
	{
		//enable all things:
		m_ctlColourComboSearchType.EnableWindow(true);
		m_ctlColourEditSearchString.EnableWindow(true);
	}
	if(plID == -1)
	{
		//show fuse files:
		fnSetupSongsList(m_data.fnGetSongsFowShowId(0));
		return;
	}
	else if(plID == -2)
	{
		//show user files:
		fnSetupSongsList(m_data.fnGetSongsFowShowId(g_intShowID));
		return;
	}
	else if(plID == -3)
	{
		//fuse jingles:
		fnSetupSongsListJingle(m_data.fnGetJinglesForShowID(0));
		m_ctlColourComboSearchType.EnableWindow(false);
		m_ctlColourEditSearchString.EnableWindow(false);
	}
	else
	{
		//otherwise we have a playlist:
		fnSetupSongsList(m_data.fnGetFilesForPlaylistID(plID));
		return;
	}
}

void CFusicMainDlg::OnLbnDblclkMainlist()
{
	CMainPlaylistController* pl;
	int selectionIndex = 0;
	//firstly find the next avalible slot:	
	pl = fnGetNexAvaliblePlaylistCart();

	selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
	
	if(selectionIndex == LB_ERR)
	{
		//nothing selected:
		return;
	}

	if(pl == NULL)
	{
		//there isn't an avlible slot:
		return;
	}
	//cue up the song:
	pl->fnCueSong(m_indexFileIDMap[selectionIndex]);
}

void CFusicMainDlg::OnBnClickedBtnPause()
{
	// TODO: Add your control notification handler code here
	m_bass->fnPause(0.5,0.5);
}

void CFusicMainDlg::OnBnClickedBtnStop()
{
	//stop the stream:
	m_bass->fnStop(1.0);

	//stop the timer:
	KillTimer(WM_USER + 10);

	//reset all controls:
	m_ctlColourStaticDisplayArtist.SetWindowTextA("Empty");
	m_ctlColourStaticDisplayTitle.SetWindowTextA("Empty");
	m_ctlColourStaticDisplayTotalTime.fnSetCounterValue(0);
	m_ctlColourStaticDisplayRemainingIntroTime.fnSetCounterValue(0);
	m_ctlColourStaticDisplayRemainingTime.fnSetCounterValue(0);
	m_ctlProgressSongProgress.fnReset();
}

void CFusicMainDlg::OnBnClickedBtnLoadPlaylist1()
{
	// TODO: Add your control notification handler code here
	if(m_plController1->fnIsEmpty())
	{
		CMainPlaylistController* pl;
		int selectionIndex = 0;
		//firstly find the next avalible slot:
		pl = fnGetNexAvaliblePlaylistCart();

		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
	
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}

		if(pl == NULL)
		{
			//there isn't an avlible slot:
			return;
		}
		//cue up the song:
		pl->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
	else
	{
		int selectionIndex = 0;
		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}
		//cue up the song:
		m_plController1->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
}

void CFusicMainDlg::OnBnClickedBtnLoadPlaylist2()
{
		// TODO: Add your control notification handler code here
	if(m_plController2->fnIsEmpty())
	{
		CMainPlaylistController* pl;
		int selectionIndex = 0;
		//firstly find the next avalible slot:
		pl = fnGetNexAvaliblePlaylistCart();

		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
	
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}

		if(pl == NULL)
		{
			//there isn't an avlible slot:
			return;
		}
		//cue up the song:
		pl->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
	else
	{
		int selectionIndex = 0;
		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}
		//cue up the song:
		m_plController2->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
}

void CFusicMainDlg::OnBnClickedBtnLoadPlaylist3()
{
		// TODO: Add your control notification handler code here
	if(m_plController3->fnIsEmpty())
	{
		CMainPlaylistController* pl;
		int selectionIndex = 0;
		//firstly find the next avalible slot:
		pl = fnGetNexAvaliblePlaylistCart();

		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
	
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}

		if(pl == NULL)
		{
			//there isn't an avlible slot:
			return;
		}
		//cue up the song:
		pl->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
	else
	{
		int selectionIndex = 0;
		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}
		//cue up the song:
		m_plController3->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
}

void CFusicMainDlg::OnBnClickedBtnLoadPlaylist4()
{
	// TODO: Add your control notification handler code here
	if(m_plController4->fnIsEmpty())
	{
		CMainPlaylistController* pl;
		int selectionIndex = 0;
		//firstly find the next avalible slot:
		pl = fnGetNexAvaliblePlaylistCart();

		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
	
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}

		if(pl == NULL)
		{
			//there isn't an avlible slot:
			return;
		}
		//cue up the song:
		pl->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
	else
	{
		int selectionIndex = 0;
		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}
		//cue up the song:
		m_plController4->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
}

void CFusicMainDlg::OnBnClickedBtnLoadPlaylist5()
{
		// TODO: Add your control notification handler code here
	if(m_plController5->fnIsEmpty())
	{
		CMainPlaylistController* pl;
		int selectionIndex = 0;
		//firstly find the next avalible slot:
		pl = fnGetNexAvaliblePlaylistCart();

		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
	
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}

		if(pl == NULL)
		{
			//there isn't an avlible slot:
			return;
		}
		//cue up the song:
		pl->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
	else
	{
		int selectionIndex = 0;
		selectionIndex = m_ctlColourListBoxSongSelect.GetCurSel();
		if(selectionIndex == LB_ERR)
		{
			//nothing selected:
			return;
		}
		//cue up the song:
		m_plController5->fnCueSong(m_indexFileIDMap[selectionIndex]);
	}
}

void CFusicMainDlg::OnBnClickedBtnHaltPlaylist1()
{
	// TODO: Add your control notification handler code here
	if(m_plController1->fnNeedToHalt() == FALSE)
	{
		if( m_plController1->fnIsEmpty() == true)
		{
			return;
		}
		//set it to true:
		m_plController1->fnSetHalt();
		m_ctlColourButtonHalt1.fnSetBackColour(RGB(255,0,0));
	}
	else
	{
		m_plController1->fnResetHalt();
		m_ctlColourButtonHalt1.fnSetBackColour(RGB(0,0,0));
	}
}

void CFusicMainDlg::OnBnClickedBtnHaltPlaylist2()
{
	if(m_plController2->fnNeedToHalt() == FALSE)
	{
		if( m_plController2->fnIsEmpty() == true)
		{
			return;
		}
		//set it to true:
		m_plController2->fnSetHalt();
		m_ctlColourButtonHalt2.fnSetBackColour(RGB(255,0,0));
	}
	else
	{
		m_plController2->fnResetHalt();
		m_ctlColourButtonHalt2.fnSetBackColour(RGB(0,0,0));
	}
}

void CFusicMainDlg::OnBnClickedBtnHaltPlaylist3()
{

	if(m_plController3->fnNeedToHalt() == FALSE)
	{
		if( m_plController3->fnIsEmpty() == true)
		{
			return;
		}
		//set it to true:
		m_plController3->fnSetHalt();
		m_ctlColourButtonHalt3.fnSetBackColour(RGB(255,0,0));
	}
	else
	{
		m_plController3->fnResetHalt();
		m_ctlColourButtonHalt3.fnSetBackColour(RGB(0,0,0));
	}
}

void CFusicMainDlg::OnBnClickedBtnHaltPlaylist4()
{
	if(m_plController4->fnNeedToHalt() == FALSE)
	{
		if( m_plController4->fnIsEmpty() == true)
		{
			return;
		}
		//set it to true:
		m_plController4->fnSetHalt();
		m_ctlColourButtonHalt4.fnSetBackColour(RGB(255,0,0));
	}
	else
	{
		m_plController2->fnResetHalt();
		m_ctlColourButtonHalt4.fnSetBackColour(RGB(0,0,0));
	}
}

void CFusicMainDlg::OnBnClickedBtnHaltPlaylist5()
{
	if(m_plController5->fnNeedToHalt() == FALSE)
	{
		if( m_plController5->fnIsEmpty() == true)
		{
			return;
		}
		//set it to true:
		m_plController5->fnSetHalt();
		m_ctlColourButtonHalt5.fnSetBackColour(RGB(255,0,0));
	}
	else
	{
		m_plController5->fnResetHalt();
		m_ctlColourButtonHalt5.fnSetBackColour(RGB(0,0,0));
	}
}

void CFusicMainDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}

void CFusicMainDlg::OnBnClickedBtnClear()
{
	m_plController1->fnClear();
	m_plController2->fnClear();
	m_plController3->fnClear();
	m_plController4->fnClear();
	m_plController5->fnClear();
	m_ctlColourButtonHalt5.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt4.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt3.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt2.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt1.fnSetBackColour(RGB(0,0,0));

}

BOOL CFusicMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//handle the messages for the accelerator keys:
	if(m_hAccelTable)
	{
		if(::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return (TRUE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
