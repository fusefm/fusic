#pragma once
#include "afxwin.h"


// CFusicSettingsPasswordDlg dialog

class CFusicSettingsPasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(CFusicSettingsPasswordDlg)

public:
	CFusicSettingsPasswordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFusicSettingsPasswordDlg();

// Dialog Data
	enum { IDD = IDD_FUSIC_SETTINGS_PASSWORD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	// Password edit control for entering the settings password:
	CEdit m_ctlEditPassword;
public:
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
	virtual BOOL OnInitDialog();
};
