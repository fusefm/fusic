// FusicDlg.h : header file
//

#if !defined(AFX_FUSICDLG_H__ACA9E7DD_0239_47E4_B58B_9B2D1078539D__INCLUDED_)
#define AFX_FUSICDLG_H__ACA9E7DD_0239_47E4_B58B_9B2D1078539D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFusicDlg dialog

class CFusicDlg : public CDialog
{
// Construction
public:
	CFusicDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFusicDlg)
	enum { IDD = IDD_FUSIC_DIALOG };
	CEdit	m_txtUserPassword;
	CEdit	m_txtUserID;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFusicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFusicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnlogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUSICDLG_H__ACA9E7DD_0239_47E4_B58B_9B2D1078539D__INCLUDED_)
