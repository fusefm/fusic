// Fusic.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

//help memory leak detection:
//#include <vld.h>


// CFusicApp:
// See Fusic.cpp for the implementation of this class
//

class CFusicApp : public CWinApp
{
public:
	CFusicApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFusicApp theApp;