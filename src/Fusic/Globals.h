#include "stdafx.h"
#pragma once
#include <map>				//needed for map decloration.
#include "bass.h"			//needed for HSYNC var.
#include "CallbackButton.h" //needed for callbackbutton decloration in map.

//better looking code:
typedef std::map<HSYNC, CCallbackButton*> T_mapCallback;

//Logging functions:
void fnLogOutMsg( const CString& strText );

//globals.h - info for global files:

//global DB Settings, loaded in at application launch time:
struct dbSettings
{
	CString strDBDatabase;
	CString strDBUser;
	CString strDBHost;
	CString strDBPassword;
};

struct playoutSettings
{
	int next_fade_out_time;
	int sweeper_fade_down_time;
	int sweeper_fade_down_volume;
	int sweeper_fade_up_time;
	int pause_fade_down_time;
	int pause_fade_up_time;
	int stop_fade_down_time;
	int mix_out_fade_out_time;
	int autodj_play_sweeper;
	int autodj_play_jingle;
	int autodj_repeat_song_hours;
	int autodj_repeat_artist_hours;
	int cart_fade_out_time;
};