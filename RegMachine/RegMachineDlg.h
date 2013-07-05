
// RegMachineDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CRegMachineDlg 对话框
class CRegMachineDlg : public CDialogEx
{
// 构造
public:
	CRegMachineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REGMACHINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGenerateKey();
	CEdit m_Key;
	CEdit m_MAC;
};
