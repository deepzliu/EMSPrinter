#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPregressDlg 对话框

class CPregressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPregressDlg)

public:
	CPregressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPregressDlg();

// 对话框数据
	enum { IDD = IDD_DLG_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	
	
public:
	CWinThread *m_hThread;
	CWinThread *m_hThreadSelf;
	int TheWaitThread(CWinThread *hThread, int *flag);
	static int WaitThreadProc(void *pUser);
	virtual BOOL OnInitDialog();
	CStatic m_Text;
	CProgressCtrl m_Progress;
	int *m_ThreadFlag;
	virtual void OnCancel();
};
