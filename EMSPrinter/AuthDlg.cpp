// AuthDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EMSPrinter.h"
#include "AuthDlg.h"
#include "afxdialogex.h"


// CAuthDlg 对话框

IMPLEMENT_DYNAMIC(CAuthDlg, CDialogEx)

CAuthDlg::CAuthDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAuthDlg::IDD, pParent)
{

}

CAuthDlg::~CAuthDlg()
{
}

void CAuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Key);
}


BEGIN_MESSAGE_MAP(CAuthDlg, CDialogEx)
END_MESSAGE_MAP()


// CAuthDlg 消息处理程序


void CAuthDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CAuthDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_Key.GetWindowText(key);
	CDialogEx::OnOK();
}
