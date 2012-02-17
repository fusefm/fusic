#include "stdafx.h"
#pragma once
#include <map>				//needed for map decloration.
#include "bass.h"			//needed for HSYNC var.
#include "CallbackButton.h" //needed for callbackbutton decloration in map.

//better looking code:
typedef std::map<HSYNC, CCallbackButton*> T_mapCallback;

//globals.h - info for global files:

//global DB Settings, loaded in at application launch time:
struct dbSettings
{
	CString strDBDatabase;
	CString strDBUser;
	CString strDBHost;
	CString strDBPassword;
};