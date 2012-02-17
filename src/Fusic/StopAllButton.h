#pragma once
#include "ColourButton.h"

// CStopAllButton

class CStopAllButton : public CColourButton
{
	DECLARE_DYNAMIC(CStopAllButton)

public:
	CStopAllButton();
	virtual ~CStopAllButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//public member functions:
	//fnSetCartsDialogPointer - set the dialog pointer so that access to
	//the dialog vectors:
	void fnSetCartsDialogPointer(void* pDlg);

	//onLButtonDown - do the stop all button task - stop the carts:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
private:
	void* dlg;
};


