#pragma once
#include "afxwin.h"

#pragma once


// CFindKeyDlg 对话框

class CFindKeyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindKeyDlg)

public:
	CFindKeyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindKeyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FIND_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Tip;
	CEdit m_Key;
};
