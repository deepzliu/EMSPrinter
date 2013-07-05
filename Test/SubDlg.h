#pragma once
#include "afxwin.h"


// SubDlg 对话框

class SubDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SubDlg)

public:
	SubDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SubDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CImage img;

public:
	CEdit m_Text;
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnCstmMsg(WPARAM wParam, LPARAM lParam);
};
