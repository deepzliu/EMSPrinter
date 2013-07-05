
// EMSPrinterDlg.h : 头文件
//

#pragma once
#include "SubDlg.h"

#include "../printcom/printcom.h"

// CEMSPrinterDlg 对话框
class CEMSPrinterDlg : public CDialogEx
{
// 构造
public:
	CEMSPrinterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EMSPRINTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	SubDlg *m_dlg;

	CPrinter m_Printer;

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
	afx_msg void OnBnClickedButton1();

	BOOL PrintPic(CString strFileName,int iCopies,CString strPrintName);

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
