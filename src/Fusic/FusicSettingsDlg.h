#pragma once
#include "afxwin.h"


// CFusicSettingsDlg dialog

class CFusicSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CFusicSettingsDlg)

public:
	CFusicSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFusicSettingsDlg();

// Dialog Data
	enum { IDD = IDD_FUSIC_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_ctlSettingsPassword;
	CEdit m_ctlDBHost;
	CEdit m_ctlDBDatabase;
	CEdit m_ctlDBUserName;
	CEdit m_ctlDBPassword;
	afx_msg void OnBnClickedOk();
	CComboBox m_ctlSndCmboCarts;
	CComboBox m_ctlSndCmboMusic;
	afx_msg void OnCbnSelchangeComboSoundCart();
	CEdit m_ctlLDAPHost;
	CEdit m_ctlLDAPBindAttribute;
	CEdit m_ctlLDAPBindBase;
	CStatic m_ctlLDAPAccessGroupsCount;
	afx_msg void OnBnClickedEditaccessgroups();
	afx_msg void OnBnClickedBtnFaderstartBrowse();
	CEdit m_ctlFaderStartEXE;
	CEdit m_ctlFaderStartAddress;
	CButton m_ctlCheckFaderStart;
	afx_msg void OnBnClickedCheckEnableFaderstart();
};
