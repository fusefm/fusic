#pragma once
#include "afxwin.h"
#include <vector>

// CFusicSettingsAccessGroupsDlg dialog

class CFusicSettingsAccessGroupsDlg : public CDialog
{
	DECLARE_DYNAMIC(CFusicSettingsAccessGroupsDlg)

public:
	CFusicSettingsAccessGroupsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFusicSettingsAccessGroupsDlg();

// Dialog Data
	enum { IDD = IDD_FUSIC_SETTINGS_ACCESSGROUPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctlEditAccessGroup;
	CListBox m_ctlListAccessGroups;
	CButton m_ctlBtnDelete;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListLdapAccessgroups();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedOk();
	std::vector<CString> m_vecO;		//store the old vector incase the cancel button is pressed.
	afx_msg void OnBnClickedCancel();
};
