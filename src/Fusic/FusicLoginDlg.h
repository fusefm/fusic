// FusicLoginDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "bass.h"
#include <vector>
// CFusicLoginDlg dialog

//struct for storing stream data:
struct SCartsList
{
	HSTREAM hstCartsStream;
	HSYNC	hsyCartsSync;
	CString	strTitle;
	int intPageNumber;
	CString strCartPosistion;
};

//time identifier:
#define IDT_CART_LOGIN_LIST WM_USER + 20

class CFusicLoginDlg : public CDialog
{
// Construction
public:
	CFusicLoginDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FUSIC_LOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	bool fnDoLDAPAuthentication(CString strUserID, CString strPassword);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//public member functions:

	//OnBnClickedBtnLogin - attempt to do LDAP authentication to
	//login the user:
	afx_msg void OnBnClickedBtnLogin();

	//OnBnClickedBtnSettings - show the settings dialog (after getting past
	//the password protection):
	afx_msg void OnBnClickedBtnSettings();

	//OnBnClickedBtnQuit - Exit the app:
	afx_msg void OnBnClickedBtnQuit();

	//fnAddCartToList - add a cart to the list of carts
	//should only be called when the carts screen is logging out
	//and the carts need to be shown.
	void fnAddCartToList(SCartsList CartListElementToAdd);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static void CALLBACK fnLoginDialogCartCallback(HSYNC handle, DWORD channel, DWORD data, void* user);

	//all form controls:
	CEdit m_ctlLDAPUser;
	CEdit m_ctlLDAPPassword;
	CStatic m_ctlStaticCartsList;
	CStatic m_ctlStaticCartsListTimes;
public:
	//private member variables:
	std::vector<SCartsList> m_vecCartsList;		//m_vecCartsList - a list of all carts currently playing.
};
