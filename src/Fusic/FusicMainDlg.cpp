// FusicMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fusic.h"
#include "FusicMainDlg.h"

//get global vars:
extern int g_intMusicDev;
extern int g_intShowID;
extern playoutSettings g_settings;

//define names for playlist selection types:
#define PLAYLIST_SEL_STATION_SONGS		-1
#define PLAYLIST_SEL_USERS_SONGS		-2
#define PLAYLIST_SEL_STATION_JINGLES	-3
#define PLAYLIST_SEL_USER_JINGLES		-4

// CFusicMainDlg dialog

IMPLEMENT_DYNAMIC(CFusicMainDlg, CDialog)

//standard constructor:
CFusicMainDlg::CFusicMainDlg(CWnd* pParent, CFusicLoginDlg* dlg)
:CDialog(CFusicMainDlg::IDD, pParent), 
//initalise the playlist halt and load buttons:
m_ctlColourButtonHalt1(this),
m_ctlColourButtonHalt2(this),
m_ctlColourButtonHalt3(this),
m_ctlColourButtonHalt4(this),
m_ctlColourButtonHalt5(this),
m_ctlColourButtonLoad1(this),
m_ctlColourButtonLoad2(this),
m_ctlColourButtonLoad3(this),
m_ctlColourButtonLoad4(this),
m_ctlColourButtonLoad5(this)
{
	//create a new auto-dj class:
	m_dj = new CAutoDJ(&m_data, this);
	m_AutoDJActive = false;
	m_needProgressBarUpdate = false;

	//get settings:
	m_data.fnGetSettings();

	//setup the accelerator:
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDR_ACCELERATOR_CART_DEBUG));

	m_pThread = NULL;
	m_loginDlg = dlg;

	//create the bass object:
	m_bass = new CMainBassControllerr(this);
}

//destructor:
CFusicMainDlg::~CFusicMainDlg()
{
	//stop memory leaks:
	if(m_bass)
		delete m_bass;

	//we need to clean up playlist controllers:
	delete m_plController1;
	delete m_plController2;
	delete m_plController3;
	delete m_plController4;
	delete m_plController5;

	//remove the auto-dj:
	delete m_dj;

	//also delete the carts dialog:
	delete m_dlgCarts;

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
	DDX_Control(pDX, IDC_BTN_AUTODJ, m_ctlColourButtonAutoDj);
	DDX_Control(pDX, IDC_BTN_CLEAR, m_ctlColoutButtonClear);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_INTROTIME, m_ctlColourStaticDisplayPlaylistIntro1);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_INTROTIME2, m_ctlColourStaticDisplayPlaylistIntro2);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_INTROTIME3, m_ctlColourStaticDisplayPlaylistIntro3);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_INTROTIME4, m_ctlColourStaticDisplayPlaylistIntro4);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_INTROTIME5, m_ctlColourStaticDisplayPlaylistIntro5);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_INTRO1, m_ctlColourStaticPlaylistIntro1);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_INTRO2, m_ctlColourStaticPlaylistIntro2);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_INTRO3, m_ctlColourStaticPlaylistIntro3);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_INTRO4, m_ctlColourStaticPlaylistIntro4);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_INTRO5, m_ctlColourStaticPlaylistIntro5);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_MIXOUT1, m_ctlColourStaticDisplayMixOut1);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_MIXOUT2, m_ctlColourStaticDisplayMixOut2);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_MIXOUT3, m_ctlColourStaticDisplayMixOut3);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_MIXOUT4, m_ctlColourStaticDisplayMixOut4);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_MIXOUT5, m_ctlColourStaticDisplayMixOut5);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT1, m_ctlColourStatixPlaylistMixOut1);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT2, m_ctlColourStatixPlaylistMixOut2);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT3, m_ctlColourStatixPlaylistMixOut3);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT4, m_ctlColourStatixPlaylistMixOut4);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT5, m_ctlColourStatixPlaylistMixOut5);
	DDX_Control(pDX, IDC_STATIC_MAINLISTLOADING, m_ctlColourStaticLoading);
	DDX_Control(pDX, IDC_BTN_UNLOAD_PLAYLIST1, m_playlistButtonUnload1);
	DDX_Control(pDX, IDC_BTN_UNLOAD_PLAYLIST2, m_playlistButtonUnload2);
	DDX_Control(pDX, IDC_BTN_UNLOAD_PLAYLIST3, m_playlistButtonUnload3);
	DDX_Control(pDX, IDC_BTN_UNLOAD_PLAYLIST4, m_playlistButtonUnload4);
	DDX_Control(pDX, IDC_BTN_UNLOAD_PLAYLIST5, m_playlistButtonUnload5);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_AIRTIME, m_PlaylistStaticAirTime1);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_AIRTIME2, m_PlaylistStaticAirTime2);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_AIRTIME3, m_PlaylistStaticAirTime3);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_AIRTIME4, m_PlaylistStaticAirTime4);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_NEXT_AIRTIME5, m_PlaylistStaticAirTime5);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT6, m_playlistAirTime1);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT7, m_playlistAirTime2);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT8, m_playlistAirTime3);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT9, m_playlistAirTime4);
	DDX_Control(pDX, IDC_STATIC_PLAYLIST_MIXOUT10, m_playlistAirTime5);
	DDX_Control(pDX, IDC_BTN_ONOFFAIR, m_ctlOnOffAir);
}


BEGIN_MESSAGE_MAP(CFusicMainDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_NEXT, &CFusicMainDlg::OnBnClickedBtnNext)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_SEARCH_TEXT, &CFusicMainDlg::OnEnChangeEditSearchText)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCHTYPE, &CFusicMainDlg::OnCbnSelchangeComboSearchtype)
	ON_CBN_SELCHANGE(IDC_COMBO_PLAYLISTSELECT, &CFusicMainDlg::OnCbnSelchangeComboPlaylistselect)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CFusicMainDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CFusicMainDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_AUTODJ, &CFusicMainDlg::OnBnClickedAutoDJ)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CFusicMainDlg::OnBnClickedBtnClear)
	ON_NOTIFY(NM_DBLCLK, IDC_MAINLIST, &CFusicMainDlg::OnNMDblclkMainlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MAINLIST, &CFusicMainDlg::OnLvnItemchangedMainlist)
	ON_MESSAGE(WM_USER_THREAD_FINISHED, &CFusicMainDlg::onThreadFinished)
	ON_BN_CLICKED(IDC_BTN_ONOFFAIR, &CFusicMainDlg::OnBnClickedBtnOnoffair)
END_MESSAGE_MAP()


// CFusicMainDlg message handlers

//OnInitDialog - called before the form is shown to set up the dialog:
BOOL CFusicMainDlg::OnInitDialog()
{
	//call the super class function:
	CDialog::OnInitDialog();

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
	m_ctlColourStaticDisplayPlaylistIntro1.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylistIntro1.fnSetBackgroundColour(0,0,0);
	m_ctlColourStaticDisplayPlaylistIntro2.fnSetBackgroundColour(0,0,0);
	m_ctlColourStaticDisplayPlaylistIntro2.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylistIntro2.SetFont(&m_playlistButtonFont);
	m_ctlColourStaticDisplayPlaylistIntro3.fnSetBackgroundColour(0,0,0);
	m_ctlColourStaticDisplayPlaylistIntro3.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylistIntro3.SetFont(&m_playlistButtonFont);
	m_ctlColourStaticDisplayPlaylistIntro4.fnSetBackgroundColour(0,0,0);
	m_ctlColourStaticDisplayPlaylistIntro4.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayPlaylistIntro4.SetFont(&m_playlistButtonFont);
	m_ctlColourStaticDisplayPlaylistIntro5.fnSetBackgroundColour(0,0,0);
	m_ctlColourStaticDisplayPlaylistIntro5.fnSetTextColour(255,255,0);
	m_ctlColourStaticPlaylistIntro1.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro2.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro3.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro4.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro5.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylistIntro5.SetFont(&m_playlistButtonFont);

	m_ctlColourStaticDisplayPlaylistIntro1.SetFont(&m_playlistButtonFont);
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
	m_ctlColourStaticLoading.fnSetBackgroundColour(0,0,0);
	m_ctlColourStaticLoading.fnSetTextColour(255,255,255);
	m_ctlColourStaticLoading.ShowWindow(SW_HIDE);


	//create the song select font:
	m_songSelectFont.CreateFont(18, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Arial");

	//the main list box:
	m_ctlColourListBoxSongSelect.SetFont(&m_songSelectFont);
	m_ctlColourListBoxSongSelect.InsertColumn(0, "Artist", LVCFMT_LEFT);
	m_ctlColourListBoxSongSelect.InsertColumn(1, "Title", LVCFMT_LEFT);
	m_ctlColourListBoxSongSelect.InsertColumn(2, "Duration", LVCFMT_RIGHT);

	//now resize each item in the list box:
//	m_ctlColourListBoxSongSelect.SetItemHeight(0,20);

	//setup controllers:
	m_plController1 = new CMainPlaylistController(1,
		&m_ctlColourStaticPlaylistNextTitle,
		&m_ctlColourStaticPlaylistNextArtist,
		&m_ctlColourButtonHalt1,
		&m_ctlComboBoxPlaylistNextSweeper, &m_data, 
		&m_ctlColourStaticPlaylistIntro1 , &m_ctlColourStatixPlaylistMixOut1);

	m_plController2 = new CMainPlaylistController(2,
		&m_ctlColourStaticPlaylist2Title,
		&m_ctlColourStaticPlaylist2Artist,
		&m_ctlColourButtonHalt2,
		&m_ctlComboBoxPlaylist2Sweeper, &m_data,
		&m_ctlColourStaticPlaylistIntro2 , &m_ctlColourStatixPlaylistMixOut2);


	m_plController3 = new CMainPlaylistController(3,
		&m_ctlColourStaticPlaylist3Title,
		&m_ctlColourStaticPlaylist3Artist,
		&m_ctlColourButtonHalt3,
		&m_ctlComboBoxPlaylist3Sweeper, &m_data,
		&m_ctlColourStaticPlaylistIntro3 , &m_ctlColourStatixPlaylistMixOut3);

	m_plController4 = new CMainPlaylistController(4,
		&m_ctlColourStaticPlaylist4Title,
		&m_ctlColourStaticPlaylist4Artist,
		&m_ctlColourButtonHalt4,
		&m_ctlComboBoxPlaylist4Sweeper, &m_data,
		&m_ctlColourStaticPlaylistIntro4 , &m_ctlColourStatixPlaylistMixOut4);

	m_plController5 = new CMainPlaylistController(5,
		&m_ctlColourStaticPlaylist5Title,
		&m_ctlColourStaticPlaylist5Artist,
		&m_ctlColourButtonHalt5,
		&m_ctlComboBoxPlaylist5Sweeper, &m_data,
		&m_ctlColourStaticPlaylistIntro5 , &m_ctlColourStatixPlaylistMixOut5);

	//set the playlist controllers to the respective buttons:
	m_ctlColourButtonHalt1.setPlaylistController(m_plController1);
	m_ctlColourButtonHalt2.setPlaylistController(m_plController2);
	m_ctlColourButtonHalt3.setPlaylistController(m_plController3);
	m_ctlColourButtonHalt4.setPlaylistController(m_plController4);
	m_ctlColourButtonHalt5.setPlaylistController(m_plController5);
	m_ctlColourButtonLoad1.fnSetPlaylistController(m_plController1);
	m_ctlColourButtonLoad2.fnSetPlaylistController(m_plController2);
	m_ctlColourButtonLoad3.fnSetPlaylistController(m_plController3);
	m_ctlColourButtonLoad4.fnSetPlaylistController(m_plController4);
	m_ctlColourButtonLoad5.fnSetPlaylistController(m_plController5);


	//add to the map:
	m_playlistControllerMap[1] = m_plController1;
	m_playlistControllerMap[2] = m_plController2;
	m_playlistControllerMap[3] = m_plController3;
	m_playlistControllerMap[4] = m_plController4;
	m_playlistControllerMap[5] = m_plController5;

	//other crap:
	m_playlistButtonUnload1.fnSetBackColour(0,0,0);
	m_playlistButtonUnload1.fnSetFontColour(255,255,255);
	m_playlistButtonUnload1.fnSetEdgeColour(255,255,255);
	m_playlistButtonUnload1.SetFont(&m_playlistButtonFont);
	m_playlistButtonUnload1.setupSettings(1, m_playlistControllerMap, this);

	m_playlistButtonUnload2.fnSetBackColour(0,0,0);
	m_playlistButtonUnload2.fnSetFontColour(255,255,255);
	m_playlistButtonUnload2.fnSetEdgeColour(255,255,255);
	m_playlistButtonUnload2.SetFont(&m_playlistButtonFont);
	m_playlistButtonUnload2.setupSettings(2, m_playlistControllerMap, this);

	m_playlistButtonUnload3.fnSetBackColour(0,0,0);
	m_playlistButtonUnload3.fnSetFontColour(255,255,255);
	m_playlistButtonUnload3.fnSetEdgeColour(255,255,255);
	m_playlistButtonUnload3.SetFont(&m_playlistButtonFont);
	m_playlistButtonUnload3.setupSettings(3, m_playlistControllerMap, this);

	m_playlistButtonUnload4.fnSetBackColour(0,0,0);
	m_playlistButtonUnload4.fnSetFontColour(255,255,255);
	m_playlistButtonUnload4.fnSetEdgeColour(255,255,255);
	m_playlistButtonUnload4.SetFont(&m_playlistButtonFont);
	m_playlistButtonUnload4.setupSettings(4, m_playlistControllerMap, this);

	m_playlistButtonUnload5.fnSetBackColour(0,0,0);
	m_playlistButtonUnload5.fnSetFontColour(255,255,255);
	m_playlistButtonUnload5.fnSetEdgeColour(255,255,255);
	m_playlistButtonUnload5.SetFont(&m_playlistButtonFont);
	m_playlistButtonUnload5.setupSettings(5, m_playlistControllerMap, this);
	m_PlaylistStaticAirTime1.fnSetBackgroundColour(0,0,0);
	m_PlaylistStaticAirTime1.fnSetTextColour(255,255,0);
	m_PlaylistStaticAirTime1.SetFont(&m_playlistFont);
	m_PlaylistStaticAirTime2.fnSetBackgroundColour(0,0,0);
	m_PlaylistStaticAirTime2.fnSetTextColour(255,255,0);
	m_PlaylistStaticAirTime2.SetFont(&m_playlistFont);
	m_PlaylistStaticAirTime3.fnSetBackgroundColour(0,0,0);
	m_PlaylistStaticAirTime3.fnSetTextColour(255,255,0);
	m_PlaylistStaticAirTime3.SetFont(&m_playlistFont);
	m_PlaylistStaticAirTime4.fnSetBackgroundColour(0,0,0);
	m_PlaylistStaticAirTime4.fnSetTextColour(255,255,0);
	m_PlaylistStaticAirTime4.SetFont(&m_playlistFont);
	m_PlaylistStaticAirTime5.fnSetBackgroundColour(0,0,0);
	m_PlaylistStaticAirTime5.fnSetTextColour(255,255,0);
	m_PlaylistStaticAirTime5.SetFont(&m_playlistFont);
	m_playlistAirTime1.fnSetTextColour(255,255,255);
	m_playlistAirTime1.SetFont(&m_playlistFont);
	m_playlistAirTime2.fnSetTextColour(255,255,255);
	m_playlistAirTime2.SetFont(&m_playlistFont);
	m_playlistAirTime3.fnSetTextColour(255,255,255);
	m_playlistAirTime3.SetFont(&m_playlistFont);
	m_playlistAirTime4.fnSetTextColour(255,255,255);
	m_playlistAirTime4.SetFont(&m_playlistFont);
	m_playlistAirTime5.fnSetTextColour(255,255,255);
	m_playlistAirTime5.SetFont(&m_playlistFont);
	m_ctlColourComboBoxPlaylistSelection.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplaySearch.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayCurrentlyDisplaying.fnSetTextColour(255,255,0);
	m_ctlColourListBoxSongSelect.SetTextBkColor(RGB(0,0,0));
	m_ctlColourListBoxSongSelect.SetTextColor(RGB(255,255,255));
	m_ctlColourListBoxSongSelect.SetBkColor(RGB(0,0,0));
	m_ctlColourListBoxSongSelect.SetExtendedStyle( 
	m_ctlColourListBoxSongSelect.GetExtendedStyle() |	LVS_EX_FULLROWSELECT );
	m_ctlColourStaticDisplayMixOut1.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayMixOut2.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayMixOut3.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayMixOut4.fnSetTextColour(255,255,0);
	m_ctlColourStaticDisplayMixOut5.fnSetTextColour(255,255,0);
	m_ctlColourStatixPlaylistMixOut1.SetFont(&m_playlistFont);
	m_ctlColourStatixPlaylistMixOut2.SetFont(&m_playlistFont);
	m_ctlColourStatixPlaylistMixOut3.SetFont(&m_playlistFont);
	m_ctlColourStatixPlaylistMixOut4.SetFont(&m_playlistFont);
	m_ctlColourStatixPlaylistMixOut5.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayMixOut1.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayMixOut2.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayMixOut3.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayMixOut4.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayMixOut5.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro1.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro2.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro3.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro4.SetFont(&m_playlistFont);
	m_ctlColourStaticPlaylistIntro5.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylistIntro1.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylistIntro2.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylistIntro3.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylistIntro4.SetFont(&m_playlistFont);
	m_ctlColourStaticDisplayPlaylistIntro5.SetFont(&m_playlistFont);


	//MySQL error handling:
	try
	{
		//setup the playlist selection box:
		//firstly the station's own stuff:
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

			//add user jingles:
			m_indexPlaylistIDMap[
				m_ctlColourComboBoxPlaylistSelection.AddString("User: Jingles") ]
			= PLAYLIST_SEL_USER_JINGLES;
		}//if

		//we are going to being with FUSE:
		m_ctlColourComboBoxPlaylistSelection.SetCurSel(0);

		//need to add song data. Start with the fuse show:
		fnSetupSongsList(m_data.fnGetSongsFowShowId(0));
	}
	catch(const mysqlpp::ConnectionFailed &e)
	{
		CString error;
		error = "ERROR: The connection to the MySQL Server has been dropped. ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::OnInitDialog)");
		MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the program:
		OnOK();
	}
	//catch everything else:
	catch(const mysqlpp::Exception &e)
	{
		CString error;
		error = "ERROR: Got an unknown error from MySQL: ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::OnInitDialog)");
		MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the program:
		OnOK();
	}

	//see if this system is 'On-Air'.
	if(! m_data.fnCheckMachineOnAir())
	{
		MessageBox("This machine is off air. Logging will be disabled. Press the 'Off Air' button "
			"to go on-air.", "Fusic", MB_ICONWARNING);
		m_ctlOnOffAir.fnSetBackColour(0,0,0);
		m_ctlOnOffAir.fnSetEdgeColour(255,255,255);
		m_ctlOnOffAir.fnSetFontColour(255,255,255);
		m_ctlOnOffAir.SetWindowTextA("Off Air");
	}
	else
	{
		m_ctlOnOffAir.fnSetBackColour(255,0,0);
		m_ctlOnOffAir.fnSetEdgeColour(255,255,255);
		m_ctlOnOffAir.fnSetFontColour(0,0,0);
		m_ctlOnOffAir.SetWindowTextA("On Air");
	}

	//need to set up the carts window:
	m_dlgCarts = new CFusicCartsDlg(this, m_loginDlg, this);

	//create the new window:
	m_dlgCarts->Create(CFusicCartsDlg::IDD);

	//show the window:
	m_dlgCarts->ShowWindow(SW_SHOW);

	//ensure that it is in the correct posistion:
	m_dlgCarts->MoveWindow(1280, 0, 1280,1024);

	//ensure we are in the right posistion:
	MoveWindow(0,0,1280,1024);

	SetTimer(WM_USER + 5, 1000, 0);

	m_ctlColourListBoxSongSelect.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
}

// fnSyncCallbackEnd - called when the end of the current stream is reached:
void CALLBACK CFusicMainDlg::fnSyncCallbackEnd(HSYNC handle, DWORD channel, 
											   DWORD data, void* user)
{
	CFusicMainDlg* dlg = (CFusicMainDlg*)user;

	//stop the timer:
	dlg->KillTimer(WM_USER + 10);

	//check if we need to halt playback for the next song:
	BOOL halt = dlg->m_plController1->fnNeedToHalt();

// dlg->m_bass->fnFadeOut(g_settings.next_fade_out_time, true);

	//the stream has stopped, only contine if auto halt isn't set
	//or there isn't another song in the playlist:
	if((dlg->m_plController1->fnIsEmpty() == false)
		&& (dlg->m_plController1->fnNeedToHalt() == false))
	{
		//we need to get the next playlist element:
		dlg->fnDoNextSong();

		//see if auto DJ is running:
		if(dlg->m_AutoDJActive == TRUE)
		{
			//if it is, cue up a new song on playlist element 5:
			dlg->m_dj->fnCueNextSong();
		}
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

void CFusicMainDlg::fnFaderStart()
{
	//if there is nothing cued up then we want to start auto dj:
	if( m_plController1->fnGetFileID() == -1 )
	{
		OnBnClickedAutoDJ();
	}
	else
	{
		//otherwise start the next song playing:
		OnBnClickedBtnNext();
	}
}

//OnTimer - A timer function to refresh coutdown timers and the progress bar:
void CFusicMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == WM_USER + 5)
	{
		//update air times.
		if( m_plController1->fnIsEmpty())
		{
			//clear all the playlist air times:
			m_playlistAirTime1.SetWindowText("");
			m_playlistAirTime2.SetWindowText("");
			m_playlistAirTime3.SetWindowText("");
			m_playlistAirTime4.SetWindowText("");
			m_playlistAirTime5.SetWindowText("");
			return;
		}
		else
		{
			CTime now = CTime::GetCurrentTime();
			if(m_bass->fnPlaying())
			{
				now += CTimeSpan(0,0,0, ((int)m_bass->m_currentMixOutPoint -
					(int)m_bass->fnGetTimeElapsed()));
			}
			if(m_plController1->fnIsEmpty())
			{
				//clear all the playlist air times:
				m_playlistAirTime1.SetWindowText("");
				m_playlistAirTime2.SetWindowText("");
				m_playlistAirTime3.SetWindowText("");
				m_playlistAirTime4.SetWindowText("");
				m_playlistAirTime5.SetWindowText("");
				return;
			}
			//otherwise add on the value and set it:
			m_playlistAirTime1.RedrawWindow();
			m_playlistAirTime1.SetWindowTextA(now.Format("%H:%M:%S"));
			now += CTimeSpan(0,0,0,m_plController1->m_duration);
			if(m_plController2->fnIsEmpty())
			{
				m_playlistAirTime2.SetWindowText("");
				m_playlistAirTime3.SetWindowText("");
				m_playlistAirTime4.SetWindowText("");
				m_playlistAirTime5.SetWindowText("");
				return;
			}
			m_playlistAirTime2.RedrawWindow();
			m_playlistAirTime2.SetWindowTextA(now.Format("%H:%M:%S"));
			now += CTimeSpan(0,0,0, m_plController2->m_duration);
			if(m_plController3->fnIsEmpty())
			{
				m_playlistAirTime3.SetWindowText("");
				m_playlistAirTime4.SetWindowText("");
				m_playlistAirTime5.SetWindowText("");
				return;
			}
			m_playlistAirTime3.RedrawWindow();
			m_playlistAirTime3.SetWindowTextA(now.Format("%H:%M:%S"));
			now += CTimeSpan(0,0,0, m_plController3->m_duration);
			if( m_plController4->fnIsEmpty())
			{
				m_playlistAirTime4.SetWindowText("");
				m_playlistAirTime5.SetWindowText("");
				return;
			}
			m_playlistAirTime4.RedrawWindow();
			m_playlistAirTime4.SetWindowTextA(now.Format("%H:%M:%S"));
			now += CTimeSpan(0,0,0, m_plController4->m_duration);
			if(m_plController5->fnIsEmpty())
			{
				m_playlistAirTime5.SetWindowTextA("");
				return;
			}
			m_playlistAirTime5.RedrawWindow();
			m_playlistAirTime5.SetWindowTextA(now.Format("%H:%M:%S"));
		}
	}
	//ensure that it is the update timer beingalled:
	if(nIDEvent == WM_USER + 10)
	{
		if(m_bass->fnGetTimeElapsed() >= m_currentIntroTime)
		{
			//we have passed intro time:
			if(m_needProgressBarUpdate == true)
			{
				//we have just passed the intro time point, so reset the length of the
				//progress bar to the full song:
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
			//we havn't passwed the intro time, set the time reamining before the intro:
			m_ctlProgressSongProgress.fnSetTimeRemain(
				m_currentIntroTime - m_bass->fnGetTimeElapsed(), false);
		}

		//update remaining time counter:
		m_ctlColourStaticDisplayRemainingTime.fnSetCounterValue(
			m_bass->fnGetTimeRemaining());

		//update the remaining intro time counter:
		m_ctlColourStaticDisplayRemainingIntroTime.fnSetCounterValue(
			m_currentIntroTime - m_bass->fnGetTimeElapsed() );

		//check if a sweeper is going to be played:
		if(m_currentIntroTime != 0 && (m_bass->fnGetSweeperLength() != 0) )
		{
			//if it is, then set the time before the sweeper will be played;
			m_ctlColourStaticDisplayTimeBeforeSweeper.fnSetCounterValue(
				(m_currentIntroTime - m_bass->fnGetSweeperLength() - 0.6) - 
				m_bass->fnGetTimeElapsed());
		}
	}

	//call the super class function for any other timer events:
	CDialog::OnTimer(nIDEvent);
}

// fnPlaylistPop - move all the songs on the playlist to the next one ready for cueing:
void CFusicMainDlg::fnPlaylistPop(void)
{
	//pop 2 - 1:
	m_plController1->fnSetState(m_plController2->fnGetState(true));

	//pop 3 - 2:
	m_plController2->fnSetState(m_plController3->fnGetState(true));

	//pop 4 - 3:
	m_plController3->fnSetState(m_plController4->fnGetState(true));

	//pop 5 - 4:
	m_plController4->fnSetState(m_plController5->fnGetState(true));
}

//button press events:

//OnBnClickedBtnNext - begin playing the next song:
void CFusicMainDlg::OnBnClickedBtnNext()
{
	//ensure auto-dj isn't active:
	if(m_AutoDJActive == true)
	{
		return;
	}
	if(m_bass->isInIntroTime())
	{
		return;
	}
	//play the next song:
	fnDoNextSong();
}

//OnBnClickedBtnPause - pause the current song:
void CFusicMainDlg::OnBnClickedBtnPause()
{
	//ensure auto-dj isn't active:
	if(m_AutoDJActive == true)
	{
		return;
	}
	if(m_bass->isInIntroTime())
	{
		return;
	}
	//pause the song:
	m_bass->fnPause((double)(g_settings.pause_fade_down_time/1000)
		,(double)(g_settings.pause_fade_up_time/1000));
}

//OnBnClickedBtnStop - stop the current song:
void CFusicMainDlg::OnBnClickedBtnStop()
{
	//ensure auto-dj isn't active:
	if(m_AutoDJActive == true)
	{
		return;
	}

	if(m_bass->isInIntroTime())
	{
		return;
	}

	//stop the stream:
	m_bass->fnStop((double)(g_settings.stop_fade_down_time/1000));

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

//OnBnClickedAutoDJ - startup the auto-dj
void CFusicMainDlg::OnBnClickedAutoDJ()
{
	if(!m_AutoDJActive)
	{
		//startup auto DJ:
		//set the auto DJ flag:
		m_AutoDJActive = true;

		//change autoDJ button:
		m_ctlColourButtonAutoDj.SetWindowTextA("Auto DJ: On");
		m_ctlColourButtonAutoDj.fnSetBackColour(0,0,255);

		//clear all auto-halts:
		m_plController1->fnResetHalt();
		m_plController2->fnResetHalt();
		m_plController3->fnResetHalt();
		m_plController4->fnResetHalt();
		m_plController5->fnResetHalt();

		//ensure that all halt buttons are the correct colour:
		m_ctlColourButtonHalt1.fnSetBackColour(RGB(0,0,0));
		m_ctlColourButtonHalt2.fnSetBackColour(RGB(0,0,0));
		m_ctlColourButtonHalt3.fnSetBackColour(RGB(0,0,0));
		m_ctlColourButtonHalt4.fnSetBackColour(RGB(0,0,0));
		m_ctlColourButtonHalt5.fnSetBackColour(RGB(0,0,0));

		//change all other buttons:
		m_ctlColourButtonHalt1.fnSetFontColour(0,0,0);
		m_ctlColourButtonHalt2.fnSetFontColour(0,0,0);
		m_ctlColourButtonHalt3.fnSetFontColour(0,0,0);
		m_ctlColourButtonHalt4.fnSetFontColour(0,0,0);
		m_ctlColourButtonHalt5.fnSetFontColour(0,0,0);
		m_ctlColourButtonLoad1.fnSetFontColour(0,0,0);
		m_ctlColourButtonLoad2.fnSetFontColour(0,0,0);
		m_ctlColourButtonLoad3.fnSetFontColour(0,0,0);
		m_ctlColourButtonLoad4.fnSetFontColour(0,0,0);
		m_ctlColourButtonLoad5.fnSetFontColour(0,0,0);
		m_playlistButtonUnload1.fnSetFontColour(0,0,0);
		m_playlistButtonUnload2.fnSetFontColour(0,0,0);
		m_playlistButtonUnload3.fnSetFontColour(0,0,0);
		m_playlistButtonUnload4.fnSetFontColour(0,0,0);
		m_playlistButtonUnload5.fnSetFontColour(0,0,0);
		m_ctlColoutButtonClear.fnSetFontColour(0,0,0);

		m_ctlColourButtonPause.fnSetBackColour(0,0,0);
		m_ctlColourButtonPause.fnSetFontColour(0,0,0);
		m_ctlColourButtonNext.fnSetBackColour(0,0,0);
		m_ctlColourButtonNext.fnSetFontColour(0,0,0);
		m_ctlColourButtonStop.fnSetBackColour(0,0,0);
		m_ctlColourButtonStop.fnSetFontColour(0,0,0);

		//finish up cueing the playlist:
		while(fnGetNexAvaliblePlaylistCart() != NULL)
		{
			m_dj->fnCueNextSong();
		}

		//start playing if not already:
		if(m_bass->fnPlaying() == FALSE)
		{
			//play the next song:
			fnDoNextSong();

			//finish up cueing the playlist:
			while(fnGetNexAvaliblePlaylistCart() != NULL)
			{
				m_dj->fnCueNextSong();
			}
		}
	}
	else
	{
		m_ctlColourButtonAutoDj.fnSetBackColour(0,0,0);
		m_ctlColourButtonAutoDj.SetWindowText("Auto DJ: Off");

		//change all other buttons:
		m_ctlColourButtonHalt1.fnSetFontColour(255,255,255);
		m_ctlColourButtonHalt2.fnSetFontColour(255,255,255);
		m_ctlColourButtonHalt3.fnSetFontColour(255,255,255);
		m_ctlColourButtonHalt4.fnSetFontColour(255,255,255);
		m_ctlColourButtonHalt5.fnSetFontColour(255,255,255);
		m_ctlColourButtonLoad1.fnSetFontColour(255,255,255);
		m_ctlColourButtonLoad2.fnSetFontColour(255,255,255);
		m_ctlColourButtonLoad3.fnSetFontColour(255,255,255);
		m_ctlColourButtonLoad4.fnSetFontColour(255,255,255);
		m_ctlColourButtonLoad5.fnSetFontColour(255,255,255);
		m_playlistButtonUnload1.fnSetFontColour(255,255,255);
		m_playlistButtonUnload2.fnSetFontColour(255,255,255);
		m_playlistButtonUnload3.fnSetFontColour(255,255,255);
		m_playlistButtonUnload4.fnSetFontColour(255,255,255);
		m_playlistButtonUnload5.fnSetFontColour(255,255,255);

		m_ctlColourButtonPause.fnSetBackColour(255,255,0);
		m_ctlColourButtonPause.fnSetFontColour(0,0,0);
		m_ctlColourButtonNext.fnSetBackColour(0,255,0);
		m_ctlColourButtonNext.fnSetFontColour(0,0,0);
		m_ctlColourButtonStop.fnSetBackColour(255,0,0);
		m_ctlColourButtonStop.fnSetFontColour(0,0,0);
		m_ctlColoutButtonClear.fnSetFontColour(255,255,255);
		m_AutoDJActive = false;
	}

}

//OnBnClickedBtnClear - clear the currnet playlist:
void CFusicMainDlg::OnBnClickedBtnClear()
{
	//ensure that auto-dj isn't active:
	if(m_AutoDJActive == true)
	{
		return;
	}

	//clear all playlist elements:
	m_plController1->fnClear();
	m_plController2->fnClear();
	m_plController3->fnClear();
	m_plController4->fnClear();
	m_plController5->fnClear();

	//ensure halt-buttons are set back to default colours:
	m_ctlColourButtonHalt5.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt4.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt3.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt2.fnSetBackColour(RGB(0,0,0));
	m_ctlColourButtonHalt1.fnSetBackColour(RGB(0,0,0));
}


UINT __cdecl threadUpdateSongList(LPVOID lParam)
{
	searchPlaylistParam* param = (searchPlaylistParam*)lParam;
	

	//clear the list of songs of it's current content:
	param->dlg->m_ctlColourListBoxSongSelect.DeleteAllItems();


	//catch any errors that occur:
	try
	{
		//do the search:
		switch(param->playlistID)
		{
		case PLAYLIST_SEL_STATION_SONGS: // (show station songs):
				//setup songs by searching:
			param->dlg->fnSetupSongsList(param->dlg->m_data.fnGetSongsForShowIDAndSearchString(
					0, param->searchString));
			break;
		case PLAYLIST_SEL_USERS_SONGS:  //show user's songs:
				//setup songs by searchin:
			param->dlg->fnSetupSongsList(param->dlg->m_data.fnGetSongsForShowIDAndSearchString(
					g_intShowID, param->searchString));
			break;

		case PLAYLIST_SEL_STATION_JINGLES:	//shows the station's jingles:

			//setup jingles by searching for jingle title:
			param->dlg->fnSetupSongsList(param->dlg->m_data.fnGetJInglesForShowIDAndSearchString(0,
				param->searchString));
			break;

		case PLAYLIST_SEL_USER_JINGLES: //the User's user jingles
			param->dlg->fnSetupSongsList(param->dlg->m_data.fnGetJInglesForShowIDAndSearchString(g_intShowID,
				param->searchString));
			break;

		default:		//all other playlists that exsist
				//setup songs by searching for song:
			param->dlg->fnSetupSongsList(param->dlg->m_data.fnGetFilesForPlaylistIDAndSearchString(
				param->playlistID, param->searchString));
		}
	}
	//catch any mysql exceptions:
	catch(const mysqlpp::ConnectionFailed &e)
	{
		CString error;
		error = "ERROR: The connection to the MySQL Server has been dropped. ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::OnEnChangeEditSearchText)");
		param->dlg->MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the routinem:
		return 0;
	}
	//catch everything else:
	catch(const mysqlpp::Exception &e)
	{
		CString error;
		error = "ERROR: Got an unknown error from MySQL: ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::OnEnChangeEditSearchText)");
		param->dlg->MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);
	
		//exit the routine:
		return 0;
	}

	param->dlg->m_ctlColourListBoxSongSelect.SetRedraw(TRUE);
	param->dlg->m_ctlColourListBoxSongSelect.RedrawWindow();
	CString* searchPhrase = new CString(param->searchString);
	PostMessage(param->dlg->m_hWnd, WM_USER_THREAD_FINISHED,0,(LPARAM)searchPhrase);
	delete param;
	return 0;
}

LRESULT CFusicMainDlg::onThreadFinished(WPARAM wParam, LPARAM lParam)
{
	//the thread has finished setting up the window:
	CString* threadSearchPhrase = (CString*)lParam;
	CString currentSearchPhrase;
	m_ctlColourEditSearchString.GetWindowTextA(currentSearchPhrase);

	if(*threadSearchPhrase != currentSearchPhrase)
	{
		//need to fire off another thread:
		searchPlaylistParam* params = new searchPlaylistParam();

		params->dlg = this;
		params->playlistID = m_indexPlaylistIDMap[
		m_ctlColourComboBoxPlaylistSelection.GetCurSel()];
		params->searchString = currentSearchPhrase;
		m_pThread = AfxBeginThread(threadUpdateSongList,
		(LPVOID)params, THREAD_PRIORITY_NORMAL,0,0);
	}
	else
	{
		m_pThread = NULL;
		m_ctlColourStaticLoading.ShowWindow(SW_HIDE);
		m_ctlColourListBoxSongSelect.ShowWindow(SW_SHOW);
	}
	delete threadSearchPhrase;
	return 0;
}
//OnEnChangeEditSearchText - update the songs list when the text has changed:
void CFusicMainDlg::OnEnChangeEditSearchText()
{

	//if there is a thread running, don't do anything, it will check once the thread is 
	//complete weather it needs to create anohter one to do anohter search:
	if(m_pThread != NULL)
	{
		return;
	}
	//hide the main search window:
	m_ctlColourListBoxSongSelect.ShowWindow(SW_HIDE);
	m_ctlColourStaticLoading.ShowWindow(SW_SHOW);

	CString searchString;

	//get the playlist ID that is selected:
	int playlistID = m_indexPlaylistIDMap[
		m_ctlColourComboBoxPlaylistSelection.GetCurSel()];

	//get the text:
	m_ctlColourEditSearchString.GetWindowTextA(searchString);

	searchPlaylistParam* param = new searchPlaylistParam();
	param->playlistID = playlistID;
	param->searchString = searchString;
	param->dlg = this;

	//start off a thread to handle the search:
	CWinThread* pThread = AfxBeginThread(threadUpdateSongList,
		(LPVOID)param, THREAD_PRIORITY_NORMAL,0,0);
	m_pThread = pThread;
}

//OnCbnSelchangeComboSearchtype - change the search type from artist to song:
void CFusicMainDlg::OnCbnSelchangeComboSearchtype()
{
	//just call the searchText changed handler, as this will handle,
	//the chaning of the search type:
	OnEnChangeEditSearchText();
}

//OnCbnSelchangeComboPlaylistselect - indicates that the user has selected
//a differnt playlist or show to view:
void CFusicMainDlg::OnCbnSelchangeComboPlaylistselect()
{
	/*int nIndex;
	int plID;

	//get the currently selected playlist index:
	nIndex = m_ctlColourComboBoxPlaylistSelection.GetCurSel();

	//clear out the main song list:
	m_ctlColourListBoxSongSelect.SetRedraw(false);
	m_ctlColourListBoxSongSelect.DeleteAllItems();

	//get the playlist ID:
	plID = m_indexPlaylistIDMap[nIndex];


	//Catch any MySQL errors:
	try
	{
		switch(plID)
		{
		case PLAYLIST_SEL_STATION_SONGS:
		
			//show station files:
			fnSetupSongsList(m_data.fnGetSongsFowShowId(0));
			break;
		
		case PLAYLIST_SEL_USERS_SONGS:
	
			//show user files:
			fnSetupSongsList(m_data.fnGetSongsFowShowId(g_intShowID));
			break;
	
		case PLAYLIST_SEL_STATION_JINGLES:

			//station's jingles:
			fnSetupSongsListJingle(m_data.fnGetJinglesForShowID(0));
			break;

		default:
			int i = 0;
			//setup an actual playlist:
			fnSetupSongsList(m_data.fnGetFilesForPlaylistID(plID));
		}
	}
	catch(const mysqlpp::ConnectionFailed &e)
	{
		CString error;
		error = "ERROR: The connection to the MySQL Server has been dropped. ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::OnCbnSelchangeComboPlaylistselect)");
		MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the routinem:
		return;
	}
	//catch everything else:
	catch(const mysqlpp::Exception &e)
	{
		CString error;
		error = "ERROR: Got an unknown error from MySQL: ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::OnCbnSelchangeComboPlaylistselect)");
		MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the routine:
		return;
	}
	m_ctlColourListBoxSongSelect.SetRedraw(true);*/
	OnEnChangeEditSearchText();
}

//OnLbnDblclkMainlist - The user has double clicked on an element on the Main song list box,
//add it to the playlist:
void CFusicMainDlg::OnNMDblclkMainlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		//ensure that auto dj isn't active:
	if(m_AutoDJActive == true)
	{
		return;
	}

	CMainPlaylistController* pl;
	int selectionIndex = pNMItemActivate->iItem;

	//get the next avalible playlist slot:	
	pl = fnGetNexAvaliblePlaylistCart();

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
	try
	{
		//check if we are cueing up a jingle or a song:
		int sel = m_indexPlaylistIDMap[
			m_ctlColourComboBoxPlaylistSelection.GetCurSel()];

			if(sel == PLAYLIST_SEL_STATION_JINGLES)
			{
				//cue up a jingle:
				pl->fnCueSong(m_indexFileIDMap[selectionIndex], -1, true);
			}
			else
			{
				//cue up a song:
				pl->fnCueSong(m_indexFileIDMap[selectionIndex], -1, false);
			}
	}
	//catch any MySQL exceptions:
	catch(const mysqlpp::ConnectionFailed &e)
	{
		CString error;
		error = "ERROR: The connection to the MySQL Server has been dropped. ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::fnDoNextSong)");
		MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the routinem:
		return;
	}
	//catch everything else:
	catch(const mysqlpp::Exception &e)
	{
		CString error;
		error = "ERROR: Got an unknown error from MySQL: ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::fnDoNextSong)");
		MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the routine:
		return;
	}
	*pResult = 0;
}


//PreTranslateMessage - used to catch accelerator keys:
BOOL CFusicMainDlg::PreTranslateMessage(MSG* pMsg)
{

	//handle the messages for the accelerator keys:
	if(m_hAccelTable)
	{
		if(::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return (TRUE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//onCtlColor - called before the DrawItem() function is called
//so we can use a differnt brush to paint the form black and other controls:
HBRUSH CFusicMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

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

	//change all other edit boxes to black backgroud and white text:
	if(nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkColor(RGB(0,0,0));
	}
	return (HBRUSH)::GetStockObject(BLACK_BRUSH);
}

//doOk - a public exporting of the OnOK() function for use by the
//CartsDlg class:
void CFusicMainDlg::doOk()
{
	OnOK();
}

//fnDoNextSong - will fade out the currently playing song (if any) and
//begin to play the next song in the playlist (if any):
void CFusicMainDlg::fnDoNextSong()
{
	int fileID = 0;
	int fileIDSweeper = -1;
	double introTime = 0;
	double length = 0;
	double mixIn = 0;
	double mixOut = 0;
	CString artist;
	CString title;
	bool jingle = false;
	mysqlpp::StoreQueryResult resSweeper;
	//start a song playing:

	//ensure that a song is cued in the next playlist element:
	if(m_plController1->fnIsEmpty() == true)
	{
		return;
	}

	//get the file ID from the playlist controller:
	fileID = m_plController1->fnGetFileID();

	//get the sweeper file id from the playlist controller:
	fileIDSweeper = m_plController1->fnGetSweeperFileID();

	//check if the file is a jingle:
	jingle = m_plController1->fnIsJingle();

	//clear the playlist element:
	m_plController1->fnClear();

	//catch all MySQL errors:
	try
	{
		if(fileIDSweeper != -1)
		{
			resSweeper = m_data.fnGetSongInfoForFileID(fileIDSweeper);
		}

		//get all the data we need:
		mysqlpp::StoreQueryResult res = m_data.fnGetSongInfoForFileID(fileID);

		//ensure no null values have been set:
		if(res[0]["File_Title"] != mysqlpp::null)
		{
			//set the title:
			title = (CString)res[0]["File_Title"];
		}

		//ensure that no null values have been set:
		if(res[0]["File_Artist"] != mysqlpp::null)
		{
			//set the artist:
			artist = (CString)res[0]["File_Artist"];
		}
		
		//ensure that no null values have been set:
		if(res[0]["File_Fadein"] != mysqlpp::null)
		{
			//set the mixIn time:
			mixIn = (double)res[0]["File_Fadein"];
		}

		//ensure that no null values have been set:
		if(res[0]["File_Fadeout"] != mysqlpp::null)
		{
			//set the mixOut time:
			mixOut = (double)res[0]["File_Fadeout"];
		}

		//ensure that no null values have been set:
		if(res[0]["File_Intro"] != mysqlpp::null)
		{
			//set the intro time:
			introTime = (double)res[0]["File_Intro"];
		}

		//set the main controls:
		m_ctlColourStaticDisplayArtist.SetWindowTextA(artist);
		m_ctlColourStaticDisplayTitle.SetWindowTextA(title);

		//if a song is currently playing, fade it out:
		if(m_bass->fnPlaying() == TRUE)
		{
			m_bass->fnFadeOut(g_settings.next_fade_out_time, true);
		}

		if(fileIDSweeper != -1)
		{
			//start the song playing with a sweeper:
			length = m_bass->fnStartWithSweeper((CString)(CString)res[0]["File_Location"],
				(CString)resSweeper[0]["File_Location"],
				introTime, mixIn, mixOut);
		}
		else
		{
			//start the song playing without a sweeper:
			length = m_bass->fnStart((CString)res[0]["File_Location"],
				mixIn, mixOut);
		}

		//set the length of the stream:
		m_ctlColourStaticDisplayTotalTime.fnSetCounterValue(length);

		//ensure that the progress bar updates it'self:
		m_needProgressBarUpdate = true;

		//set the intro tim into the member variable:
		m_currentIntroTime = introTime;

		//set the intro time remaining control's time:
		m_ctlColourStaticDisplayRemainingIntroTime.fnSetCounterValue(introTime);

		//clear the progres bar:
		m_ctlProgressSongProgress.fnSetTimeRemain(0);
		m_ctlProgressSongProgress.fnReset();
		m_ctlProgressSongProgress.RedrawWindow();


		if(introTime == 0)
		{
			//if no intro time is set, then just set the total length of the track
			//as the progress bar's length:
			m_ctlProgressSongProgress.fnSetTotalTime(length);
		}
		else
		{
			if(fileIDSweeper != -1)
			{
				//if there is a sweeper playing, set the time before it is played:
				m_ctlColourStaticDisplayTimeBeforeSweeper.fnSetCounterValue(introTime - 
					m_bass->fnGetSweeperLength() - 0.6);
			}

			//also set the time before the intro on the control:
			m_ctlProgressSongProgress.fnSetTotalTime(introTime);
		}

		//log the play only if we're not a jingle and the machine is on air:
		if(m_data.fnCheckMachineOnAir())
		{
			if(!jingle)
			{
				if(m_AutoDJActive)
				{
					m_data.fnLogFile(fileID, 0);
				}
				else
				{
					m_data.fnLogFile(fileID, g_intShowID);
				}
			}
			m_ctlOnOffAir.fnSetBackColour(255,0,0);
			m_ctlOnOffAir.fnSetFontColour(0,0,0);
			m_ctlOnOffAir.SetWindowTextA("On Air");
		}
		else
		{
			m_ctlOnOffAir. fnSetBackColour(0,0,0);
			m_ctlOnOffAir.fnSetFontColour(255,255,255);
			m_ctlOnOffAir.SetWindowTextA("Off Air");
		}
	}
	//catch any MySQL exceptions:
	catch(const mysqlpp::ConnectionFailed &e)
	{
		CString error;
		error = "ERROR: The connection to the MySQL Server has been dropped. ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::fnDoNextSong)");
		MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the routine:
		return;
	}
	//catch everything else:
	catch(const mysqlpp::Exception &e)
	{
		CString error;
		error = "ERROR: Got an unknown error from MySQL: ";
		error.Append(e.what());
		error.Append(" (CFusicMainDlg::fnDoNextSong)");
		MessageBox(error, "Fusic Error.", MB_OK | MB_ICONERROR);

		//exit the routine:
		return;
	}

	//clear the progress bar:


	//start a timer for countdown timers:
	SetTimer(WM_USER + 10, 20, 0);

	//do the pop:
	fnPlaylistPop();

	//highlight the song for none playback:
	if(m_data.fnCheckMachineOnAir())
	{
		if(g_settings.autodj_repeat_artist_hours != 0 ||
			g_settings.autodj_repeat_song_hours != 0)
		{
			for(int i = 0; i < m_ctlColourListBoxSongSelect.GetItemCount(); i ++)
			{
				CString indexTitle = m_ctlColourListBoxSongSelect.GetItemText(i, 1);
				CString indexArtist = m_ctlColourListBoxSongSelect.GetItemText(i, 0);
		
				if(indexArtist == artist)
				{
					if(g_settings.autodj_repeat_artist_hours != 0)
					{
						//highlight this row as it is the same artist:
						m_ctlColourListBoxSongSelect.SetRowBgColor(i, RGB(180,0,0));
						m_ctlColourListBoxSongSelect.SetRowStyle(i, LIS_BGCOLOR | LIS_STROKE);
					}
				}
				if(indexTitle == title)
				{
					if(g_settings.autodj_repeat_song_hours != 0)
					{
						m_ctlColourListBoxSongSelect.SetRowBgColor(i, RGB(180,0,0));
						m_ctlColourListBoxSongSelect.SetRowStyle(i, LIS_BGCOLOR | LIS_STROKE);
					}
				}
			}
		}
	}
}

//private member functions implimentation::

//fnSetupSongsList - will setup the main listbox with all the songs
//for a show ID. Will also populate the File_ID map with the indexes
//for the songs in the listbox:
void CFusicMainDlg::fnSetupSongsList(mysqlpp::StoreQueryResult res)
{
	CString title;
	CString artist;
	CTime now = CTime::GetCurrentTime();
	now -= CTimeSpan(0, g_settings.autodj_repeat_song_hours, 0, 0);
	int index = 0;
	mysqlpp::DateTime fileLastPlayed;
	mysqlpp::DateTime dateNow(now.GetTime());
	std::list<CString> artistToStrilke;
	now = CTime::GetCurrentTime();
	now -= CTimeSpan(0, g_settings.autodj_repeat_artist_hours, 0, 0);
	mysqlpp::DateTime artistCheckTime(now.GetTime());

	//clear the maps:
	m_indexFileIDMap.clear();
	m_FileIDIndexMap.clear();

	m_ctlColourListBoxSongSelect.SetItemCount((int)res.size());
	m_ctlColourListBoxSongSelect.LockWindowUpdate();
	//iterate thorugh all results:
	for(size_t i = 0; i < (int)res.size(); i++)
	{
		title.Empty();
		artist.Empty();
		CString strDuration;

	
		//construct the string for the list box:
		//ensure that no null values have been set:
		if(res[i]["File_Artist"] != mysqlpp::null)
		{
			//add the artist first:
			artist = (CString)res[i]["File_Artist"];
		}

		//ensure that no null values have been set:
		if(res[i]["File_Title"] != mysqlpp::null)
		{
			//then the title:
			title = (CString)res[i]["File_Title"];
		}
		if(res[i]["File_Duration"] != mysqlpp::null)
		{
			double duration = (double)res[i]["File_Duration"];
			//get MS:
			int s = (int)ceil(duration);    // convert to seconds - round to nearest second

			int Hours   = s / 3600 % 24;
			int Minutes = s % 3600 / 60;
			//also add hours to mins:
			Minutes += (Hours * 60);

			int Seconds = s % 60;

			strDuration.Format("%02d:%02d",Minutes, Seconds);
		}

		if(res[i]["File_PlayedLast"] != mysqlpp::null)
		{
			fileLastPlayed = res[i]["File_PlayedLast"];
		}
		
		//add a string to the list:
		index = m_ctlColourListBoxSongSelect.InsertItem((index + 1), artist);
		m_ctlColourListBoxSongSelect.SetItemText(index, 1, title);
		m_ctlColourListBoxSongSelect.SetItemText(index, 2, strDuration);

		//set alternating backgroud colour:
		if((index % 2) == 1)
		{
			m_ctlColourListBoxSongSelect.SetRowBgColor(index, RGB(0,0,0));
		}
		else
		{
			m_ctlColourListBoxSongSelect.SetRowBgColor(index, RGB(30,30,30));
		}
		m_ctlColourListBoxSongSelect.SetRowStyle(index, LIS_BGCOLOR);
		//map the insertion index to the file ID:
		m_indexFileIDMap[index] = (int)res[i]["File_ID"];
		//m_FileIDIndexMap[(int)res[i]["File_ID"]] = index;
	}

	mysqlpp::StoreQueryResult resFileExcludeList = 
		m_data.fnGetFilesPlaiedInTheLast(g_settings.autodj_repeat_song_hours);
	mysqlpp::StoreQueryResult resArtistExcludeList = 
		m_data.fnGetFilesPlaiedInTheLast(g_settings.autodj_repeat_artist_hours);
	
	for(int iIndex = 0; iIndex < m_ctlColourListBoxSongSelect.GetItemCount(); iIndex ++)
	{
		CString indexArtist;
		CString indexTitle;
		indexArtist = m_ctlColourListBoxSongSelect.GetItemText(iIndex, 0);
		indexTitle = m_ctlColourListBoxSongSelect.GetItemText(iIndex, 1);
		ASSERT((int)resFileExcludeList.num_rows() <= (int)resArtistExcludeList.num_rows());
		for(int i = 0; i < resArtistExcludeList.size(); i ++)
		{
			if((CString)resFileExcludeList[i]["File_Title"] == indexTitle &&
				(CString)resArtistExcludeList[i]["File_Artist"] == indexArtist)
			{
				//do highlight;
				m_ctlColourListBoxSongSelect.SetRowBgColor(iIndex, RGB(180,0,0));
				m_ctlColourListBoxSongSelect.SetRowStyle(iIndex, LIS_BGCOLOR | LIS_STROKE);
				continue;
			}
			if((CString)resArtistExcludeList[i]["File_Artist"] == indexArtist)
			{
				//do highlihght.
				m_ctlColourListBoxSongSelect.SetRowBgColor(iIndex, RGB(180,0,0));
				m_ctlColourListBoxSongSelect.SetRowStyle(iIndex, LIS_BGCOLOR | LIS_STROKE);
				continue;
			}
		}
	}

	m_ctlColourListBoxSongSelect.UnlockWindowUpdate();

	//RESIZE THE LIST:
	for(int i = 0 ; i < 3; i++)
	{
		m_ctlColourListBoxSongSelect.SetColumnWidth(i,
			LVSCW_AUTOSIZE_USEHEADER);
			
	}

	//also strike out songs that shouldn't be played:
	//fnCheckStrikeOut();
}

//fnSetupSongsListJinglre - will setup the main listbox with all the jingles
//for a show ID. Will also populate the File_ID map with the indexes
//for the songs in the listbox:
void CFusicMainDlg::fnSetupSongsListJingle(mysqlpp::StoreQueryResult res)
{
	CString title;
	CString artist;
	int index = 0;
	//iterate through all results:
	for(size_t i = 0; i < (int)res.size(); i++)
	{
		title.Empty();
		artist.Empty();
		CString strDuration;

	
		//construct the string for the list box:
		//ensure that no null values have been set:
		if(res[i]["File_Artist"] != mysqlpp::null)
		{
			//add the artist first:
			artist = (CString)res[i]["File_Artist"];
		}

		//ensure that no null values have been set:
		if(res[i]["File_Title"] != mysqlpp::null)
		{
			//then the title:
			title = (CString)res[i]["File_Title"];
		}
		if(res[i]["File_Duration"] != mysqlpp::null)
		{
			double duration = (double)res[i]["File_Duration"];
			//get MS:
			int s = (int)ceil(duration);    // convert to seconds - round to nearest second

			int Hours   = s / 3600 % 24;
			int Minutes = s % 3600 / 60;
			//also add hours to mins:
			Minutes += (Hours * 60);

			int Seconds = s % 60;

			strDuration.Format("%02d:%02d",Minutes, Seconds);
		}

		//add a string to the list:
		index = m_ctlColourListBoxSongSelect.InsertItem((index + 1), artist);
		m_ctlColourListBoxSongSelect.SetItemText(index, 1, title);
		m_ctlColourListBoxSongSelect.SetItemText(index, 2, strDuration);

		//set alternating backgroud colour:
		if((index % 2) == 1)
		{
			m_ctlColourListBoxSongSelect.SetRowBgColor(index, RGB(0,0,0));
		}
		else
		{
			m_ctlColourListBoxSongSelect.SetRowBgColor(index, RGB(30,30,30));
		}
		m_ctlColourListBoxSongSelect.SetRowStyle(index, LIS_BGCOLOR);

		//map the insertion index to the file ID:
		m_indexFileIDMap[index] = (int)res[i]["File_ID"];
	}
}


void CFusicMainDlg::OnLvnItemchangedMainlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CFusicMainDlg::fnCheckStrikeOut()
{
	mysqlpp::StoreQueryResult resAritsts;
	mysqlpp::StoreQueryResult resSongs;
	mysqlpp::StoreQueryResult res;
	std::vector<int> fileIDsToStrike;
	std::vector<CString> artistsToStrike;
	resSongs = m_data.fnGetFilesPlaiedInTheLast(1);
	resAritsts = m_data.fnGetFilesPlaiedInTheLast(g_settings.autodj_repeat_artist_hours);
	for(int k = 0; k < (int)resSongs.size(); k ++)
	{
		fileIDsToStrike.push_back((int)resSongs[k]["File_ID"]);
	}
	for(int l = 0; l < (int)resAritsts.size(); l++)
	{
		artistsToStrike.push_back((CString)resAritsts[l]["File_Artist"]);
	}
	res = m_data.fnGetFilesToExclude(fileIDsToStrike, artistsToStrike);
	m_ctlColourListBoxSongSelect.SetRedraw(FALSE);
	for(int j = 0; j < (int)res.size(); j++)
	{
		int fileID = (int)res[j]["File_ID"];
		int index = m_FileIDIndexMap[fileID];
	//	m_ctlColourListBoxSongSelect.SetRowBgColor(index, RGB(150,0,0));
	//	m_ctlColourListBoxSongSelect.SetRowStyle(index, LIS_BGCOLOR | LIS_STROKE);
	}
	m_ctlColourListBoxSongSelect.SetRedraw(TRUE);
	m_ctlColourListBoxSongSelect.RedrawWindow();
}

void CFusicMainDlg::OnBnClickedBtnOnoffair()
{
	// TODO: Add your control notification handler code here
	if(m_data.fnCheckMachineOnAir())
	{
		return;
	}
	else
	{
		m_data.fnSetMachineOnAir();
		m_ctlOnOffAir.fnSetBackColour(255,0,0);
		m_ctlOnOffAir.fnSetFontColour(0,0,0);
		m_ctlOnOffAir.SetWindowTextA("On Air");
	}
}
