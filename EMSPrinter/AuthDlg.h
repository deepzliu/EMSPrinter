#pragma once
#include "afxwin.h"


// CAuthDlg 对话框

class CAuthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAuthDlg)

public:
	CAuthDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAuthDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Key;
	CString key;
	virtual void OnCancel();
	virtual void OnOK();
};
