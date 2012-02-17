#pragma once

#include "CallbackButton.h"			//needed for callback button subclass
// CJingleButton

class CJingleButton : public CCallbackButton
{
	DECLARE_DYNAMIC(CJingleButton)

public:
	CJingleButton();
	virtual ~CJingleButton();

protected:
	DECLARE_MESSAGE_MAP()
};


