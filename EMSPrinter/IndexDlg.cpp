// IndexDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EMSPrinter.h"
#include "IndexDlg.h"
#include "afxdialogex.h"


// CIndexDlg 对话框

IMPLEMENT_DYNAMIC(CIndexDlg, CDialogEx)

CIndexDlg::CIndexDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIndexDlg::IDD, pParent)
{

}

CIndexDlg::~CIndexDlg()
{
}

void CIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPANY_INFO, m_CompanyInfo);
	DDX_Control(pDX, IDC_EDIT1, m_FindKey);
}


BEGIN_MESSAGE_MAP(CIndexDlg, CDialogEx)
END_MESSAGE_MAP()


// CIndexDlg 消息处理程序


void CIndexDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString str;
	CStringA strA;

	m_CompanyInfo.GetWindowText(str);
	m_Company = CStrW2CStrA(str);
	m_FindKey.GetWindowText(str);
	m_Index = CStrW2CStrA(str);
	CDialogEx::OnOK();
}


void CIndexDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类	

	CDialogEx::OnCancel();
}


BOOL CIndexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	CString str;
	CStringA strA;

	SetWindowText(L"设置索引");

	CFont editfont;
	editfont.CreateFont(20, 0 ,0 , 0,
				FW_NORMAL,	FALSE, FALSE, FALSE, GB2312_CHARSET/*ANSI_CHARSET*/,
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
				DEFAULT_PITCH | FF_MODERN , L"宋体");
	m_FindKey.SetFont(&editfont);       

	strA.Format("%s", m_Company.c_str());
	str = L"公司名称：";
	str += CStrA2CStrW(strA);
	m_CompanyInfo.SetWindowText(str);
	strA.Format("%s", m_Index.c_str());
	m_FindKey.SetWindowText(CStrA2CStrW(strA));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
