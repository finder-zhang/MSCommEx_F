
// MSCommTestDlg.h : header file
//

#pragma once


#include "Src/MSCommEx_F.h"
#include "afxwin.h"

// CMSCommTestDlg dialog
class CMSCommTestDlg : public CDialog
{
protected:

	CMSCommEx_F		_mscomm;
	
// Construction
public:
	CMSCommTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MSCOMMTEST_DIALOG };	

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpenComm();
	afx_msg void OnBnClickedBtnWrite();
protected:
	CEdit _EditWrite;
};
