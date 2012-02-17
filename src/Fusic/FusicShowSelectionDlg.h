#pragma once
#include "afxcmn.h"
#include <vector>
#include "afxwin.h"

// CFusicShowSelectionDlg dialog

struct showStruct
{
	CString strShowID;
	CString strShowName;
	int nIndex;
};

class CFusicShowSelectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CFusicShowSelectionDlg)

public:
	CFusicShowSelectionDlg(CWnd* pParent = NULL, CString strShowID = "");   // standard constructor
	virtual ~CFusicShowSelectionDlg();

// Dialog Data
	enum { IDD = IDD_FUSIC_SHOWSELECTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strUserID;
	std::vector<showStruct> m_vecShows;
	CListBox m_ctlListShows;
	afx_msg void OnBnClickedOk();
};
