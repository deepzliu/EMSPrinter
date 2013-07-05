
// EMSPrinterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EMSPrinter.h"
#include "EMSPrinterDlg.h"
#include "afxdialogex.h"
#include "common.h"
#include <fstream>
#include<string>
#include <stdio.h>
#include<iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma comment(lib, "../Debug/printcom.lib")
#else
#pragma comment(lib, "../Release/printcom.lib")
#endif


#include < windows.h > 
#include < wincon.h > 
#include < stdlib.h > 
#include < stdio.h > 
#include < time.h >

#pragma comment(lib,"Iphlpapi.lib")

bool Getlocalmac(BYTE mac[8]) 
{
	BYTE szSystemInfo[4096]; // 在程序执行完毕后，此处存储取得的系统特征码
    UINT uSystemInfoLen = 0; // 在程序执行完毕后，此处存储取得的系统特征码的长度
    // 网卡 MAC 地址，注意: MAC 地址是可以在注册表中修改的

	UINT uErrorCode=0;
	IP_ADAPTER_INFO iai;
	ULONG uSize = 0;
	DWORD dwResult = GetAdaptersInfo( &iai, &uSize );

	if( dwResult == ERROR_BUFFER_OVERFLOW )
	{
		IP_ADAPTER_INFO* piai=( IP_ADAPTER_INFO* )HeapAlloc( GetProcessHeap( ), 0, uSize );
		if( piai != NULL ){
			dwResult = GetAdaptersInfo(piai, &uSize );
			if( ERROR_SUCCESS == dwResult )	{
				IP_ADAPTER_INFO* piai2 = piai;
				while(piai2!=NULL &&(uSystemInfoLen+piai2->AddressLength )< 4096U)
				{
					CopyMemory(szSystemInfo+uSystemInfoLen,piai2->Address, piai2->AddressLength );
					uSystemInfoLen += piai2->AddressLength;
					memcpy(mac, piai2->Address, 8);					
					piai2 = piai2->Next;
					break;
				}
			}else{
				uErrorCode = 0xF0000000U + dwResult;
			}
			VERIFY( HeapFree( GetProcessHeap( ), 0, piai ) );
		}else{
			return false;//FALSE;
		}
	}else{
		uErrorCode = 0xE0000000U + dwResult;
	}

	if( uErrorCode != 0U )
	{
		return false;//FALSE;
	}
	return false;
}




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEMSPrinterDlg 对话框


CEMSPrinterDlg::CEMSPrinterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEMSPrinterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CEMSPrinterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Type);
	DDX_Control(pDX, IDC_LIST2, m_InfoList);
	DDX_Control(pDX, IDC_EDIT1, m_SearchStr);
}

BEGIN_MESSAGE_MAP(CEMSPrinterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CEMSPrinterDlg::OnSearchRealtime)
	ON_BN_CLICKED(IDC_BUTTON2, &CEMSPrinterDlg::OnSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CEMSPrinterDlg::OnDelete)
	ON_BN_CLICKED(IDC_BUTTON5, &CEMSPrinterDlg::OnPrint)
	ON_COMMAND(ID_32774, &CEMSPrinterDlg::OnImportInfo)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CEMSPrinterDlg::OnNMClickList2)
	ON_COMMAND(ID_32775, &CEMSPrinterDlg::On32775)
	ON_COMMAND(ID_32773, &CEMSPrinterDlg::OnExportInfo)
	ON_BN_CLICKED(IDC_BUTTON1, &CEMSPrinterDlg::OnSetIndex)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CEMSPrinterDlg 消息处理程序

BOOL CEMSPrinterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitMainDlg();

	/*
	if(Authenticate() == 1){
		OnCancel();
	}
	*/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEMSPrinterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEMSPrinterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEMSPrinterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CEMSPrinterDlg::InitMainDlg()
{
	SetWindowText(L"寄票打印软件 V1.3");

	CreateSubDlg(1);
	m_Type.ShowWindow(SW_HIDE);

	//m_Type.InsertString(0,L"客户");
	//m_Type.InsertString(1,L"其它");
	//m_Type.SetCurSel(0);

	LONG lStyle;
	lStyle = GetWindowLong(m_InfoList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL; //设置style
	SetWindowLong(m_InfoList.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD dwStyle = m_InfoList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_InfoList.SetExtendedStyle(dwStyle); //设置扩展风格

	m_InfoList.InsertColumn(0,L"No", LVCFMT_LEFT, 25);  
	m_InfoList.InsertColumn(1,L"公司名称", LVCFMT_LEFT, 250); 
	CString str;
	for(int i = 0; i < MAX_LIST; i++){
		m_InfoList.InsertItem(i, L"");
	}
	m_InfoListRows = 0;
	
	pInfo_Item p;
	for(int i = 0; i < MAX_LIST + 1; i++){
		p = new Info_Item;
		m_ListItems.push_back(p);
	}

	m_InfoDB.InitDB();
	m_Printer.SetPageSize(L"A4");
	m_Printer.SetPageStart(0, 0);
	OnSearchRealtime();
	return 0;
}

int CEMSPrinterDlg::CreateSubDlg(int flag)
{

	if(flag == 1){
		m_EmsBill = new CEMSBill(this);
		m_EmsBill->Create(IDD_DIALOG_EMS_BILL);
		m_EmsBill->SetParent(this);
		m_EmsBill->ShowWindow(SW_SHOW);
		CRect rect;
		m_EmsBill->GetWindowRect(&rect);
		m_EmsBill->MoveWindow(180, 80, rect.right - rect.left, rect.bottom - rect.top);
		m_EmsBill->InitBaseInfo();
	}

	return 0;
}

void CEMSPrinterDlg::OnSearchRealtime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码		
	m_SearchStr.GetWindowText(m_CondW);
	m_CondA = CStrW2CStrA(m_CondW);
	m_CondA.MakeLower();
	m_Cond = m_CondA;

	for(int i = 0; i < MAX_LIST + 1; i++){
		for(int j = 0; j < 6; j++){
			memset(m_ListItems[i]->item[j], 0, 2);
		}
	}

	Search(m_Cond, this);
	
	HWND hwnd = m_EmsBill->m_hWnd;
	m_InfoList.SetItemState(0,  LVIS_SELECTED,  LVIS_SELECTED);
	::SendMessage(hwnd,MSG_INFO, 0, (LPARAM)m_ListItems[0]);
}


void CEMSPrinterDlg::OnSave()
{
	// TODO: 在此添加控件通知处理程序代码

	HWND hwnd = m_EmsBill->m_hWnd;
	::SendMessage(hwnd,MSG_SAVE, 0, (LPARAM)&m_InfoDB);
	OnSearchRealtime();

}

void CEMSPrinterDlg::OnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CStringA strA;
	int ret = 0;
	HWND hwnd = m_EmsBill->m_hWnd;
    for(int i=0; i<m_InfoList.GetItemCount(); i++)
    {
        if( m_InfoList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
        {			
			//::SendMessage(hwnd,MSG_INFO, 0, (LPARAM)&str);
			m_ListItems[i];
			strA.Format("  删除“%s”？",m_ListItems[i]->item[0]);
			ret = MessageBox(CStrA2CStrW(strA), L"提示", MB_YESNO | MB_ICONWARNING);
			if(ret != 6){
				break;
			}
			if(m_InfoDB.Delete(m_ListItems[i]->item[0]) == 0){
				MessageBox(L"删除成功！", L"提示", MB_OK);
				OnSearchRealtime();
			}else{
				MessageBox(L"删除失败！", L"提示", MB_OK | MB_ICONWARNING);
			}
        }
    }
}


void CEMSPrinterDlg::OnPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hwnd = m_EmsBill->m_hWnd;
	::SendMessage(hwnd,MSG_PRINT, 0, (LPARAM)&m_Printer);
}

void CEMSPrinterDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//MessageBox(L"退出");

	pInfo_Item p;
	for(int i = 0; i < MAX_LIST + 1; i++){
		p = m_ListItems[i];
		delete[] p;
	}
	m_ListItems.clear();

	CDialogEx::OnCancel();
}


void CEMSPrinterDlg::OnImportInfo()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog fdlg(TRUE, 
		L"csv Files (*csv)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
		_T("csv Files (*.csv)|*.csv;)||"),
		NULL);

	int ret = fdlg.DoModal();
	if(ret == IDCANCEL) return;

	CStringW file = fdlg.GetPathName();
	m_Filepath = CStrW2CStrA(file.GetBuffer());

	CWinThread *hThread = AfxBeginThread((AFX_THREADPROC)ImportInfoProc, this);
	m_ThreadFlag = -1;
	if(hThread == NULL){
		ret = IDCANCEL;
	}else{
		m_Progress.TheWaitThread(hThread, &m_ThreadFlag);
		ret = m_Progress.DoModal();
	}
	if(ret == IDCANCEL){
		m_ThreadFlag = -2;
	}

	OnSearchRealtime();
}

int CEMSPrinterDlg::ImportInfoProc(void *pUser)
{
	CEMSPrinterDlg *dlg = (CEMSPrinterDlg*)pUser;
	string sfile = dlg->m_Filepath;;
	ifstream fin(sfile);
	if(!fin.is_open()){
		dlg->MessageBox(L"文件打开失败", L"提示", MB_ICONWARNING);
		return 1;
	}

	string line, substr;
	int count = 0, n = 0, tmp = 0;
	char str[7][256];
	int cur_pos, find_pos, nSize = 0;;
	while(getline(fin,line)){
		cur_pos = 0, find_pos = 0;
		if(dlg->m_ThreadFlag == -2) break;//强制退出
		for(int i = 0; i < 7; i++){
			memset(str[i], 0, 256);
		}
		n = 0;
		nSize = line.size();
		while(1){			
			find_pos = line.find(',', cur_pos);

			if(find_pos != string::npos){
				if(cur_pos != find_pos){					
					substr = line.substr(cur_pos, find_pos - cur_pos);					
					memcpy(str[n], substr.c_str(), substr.size()+1);
				}
				cur_pos = find_pos+1;
			}else{
				if(line.size() > cur_pos+1){
					substr = line.substr(cur_pos, nSize - cur_pos);					
					memcpy(str[n], substr.c_str(), substr.size()+1);
				}
				break;
			}
			if(++n > 6) break;
		}
		line.clear();
		if(dlg->m_InfoDB.Insert(str[0], str[1], str[2], str[3], str[4], str[5], str[6]) == 0){
			count ++;
		}
		tmp++;
	}
	dlg->m_ThreadFlag = count;
	fin.close();

	return 0;
}

int CEMSPrinterDlg::InsertListItem(int argc, char **argv, char **azColName)
{
	CStringA strA;
	CString str;
	string text;
	str.Format(L"%d", m_InfoListRows+1);
	text = argv[1];
	text = UTF_82ASCII(text);
	strA.Format("%s", text.c_str());
	m_InfoList.SetItemText(m_InfoListRows, 0, str);	
	m_InfoList.SetItemText(m_InfoListRows, 1, CStrA2CStrW(strA));

	for(int i = 1; i < 8; i++){
		text = argv[i];
		text = UTF_82ASCII(text);
		strcpy(m_ListItems[m_InfoListRows]->item[i-1], text.c_str());
	}

	m_InfoListRows++;
	if(m_InfoListRows > MAX_LIST){
		return 1;//列表中只展示有限条数据，不需要展示所有
	}
	return 0;
}

int CEMSPrinterDlg::QueryCallback(void *pUser, int argc, char **argv, char **azColName)
{
	CEMSPrinterDlg *pDlg = (CEMSPrinterDlg*)pUser;
	return pDlg->InsertListItem(argc, argv, azColName);
}

int CEMSPrinterDlg::Search(string condition, void *pUser)
{
	m_InfoListRows = 0;
	for(int i = 0; i < MAX_LIST; i++){
		m_InfoList.SetItemText(i, 0, L"");
		m_InfoList.SetItemText(i, 1, L"");
	}
	m_InfoDB.Query(condition, QueryCallback, this);
	return 0;
}


void CEMSPrinterDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString str;
	HWND hwnd = m_EmsBill->m_hWnd;
    for(int i=0; i<m_InfoList.GetItemCount(); i++)
    {
        if( m_InfoList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
        {		
			::SendMessage(hwnd,MSG_INFO, 0, (LPARAM)m_ListItems[i]);
        }
    }
}


int CEMSPrinterDlg::Authenticate()
{
	BYTE mac[8];
	char buf[64];
	CString genKey;
	CString appKey;
	CStringA strA;

	Getlocalmac(mac);
	GenerateKey(mac, genKey);

	CFile fp;
	int flag = 0;
	int ret = 0;
	if(fp.Open(L"key.bin", CFile::modeReadWrite)){
		memset(buf, 0, 64);
		ret = fp.Read(buf, 20);
		if(ret > 0){
			strA.Format("%s", buf);
			appKey = CStrA2CStrW(strA);
		}else{
			flag = 1;
			fp.Close();
		}
	}else{
		flag = 1;
	}

	if(flag == 1){
		ret = m_Auth.DoModal();
		if(ret = IDOK){
			fp.Open(L"key.bin", CFile::modeCreate | CFile::modeReadWrite);
			appKey = m_Auth.key;
			strA = CStrW2CStrA(appKey);
			fp.Write(strA.GetBuffer(), strA.GetLength());
		}else{
			return 1;
		}	
	}
	
	if(genKey == appKey){
		return 0;
	}else{
		MessageBox(L"认证失败，请注册后使用，谢谢！！", L"提示", MB_ICONWARNING);
		return 1;
	}
	return 0;	
}


int CEMSPrinterDlg::GenerateKey(BYTE mac[8], CString &key)
{
	// TODO: 在此添加控件通知处理程序代码
	CStringA strA;
	
	int A = 0, B = 0;
	unsigned char nMac[8] = {0}, tMac[8] = {0};
	int m = 0;
	
	memcpy(nMac, mac, 8);
	strA = "";
		for(int i = 0; i < 6; i++){
		A += nMac[i];
	}
	A *= 5;

	for(int i = 0; i < 6; i++){
		tMac[i] = ~nMac[i];
	}

	nMac[0] = tMac[4];
	nMac[1] = tMac[2];
	nMac[2] = tMac[5];
	nMac[3] = tMac[1];
	nMac[4] = tMac[3];
	nMac[5] = tMac[0];

	strA.Format("%02X%02X-%04X%02X%02X", nMac[2],nMac[3], A, nMac[4],nMac[5]);

	key = CStrA2CStrW(strA);

	return 0;
}


BOOL CEMSPrinterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch(pMsg->message){
		case WM_KEYDOWN:
			if(pMsg->wParam == (VK_CONTROL & VK_RETURN)){
				return FALSE;
			}else if(pMsg->wParam == VK_RETURN){
				return FALSE;
			}

			break;
		case WM_KEYUP:

			break;
		case WM_CHAR:

			break;
		case WM_SYSKEYDOWN:

			break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CEMSPrinterDlg::On32775()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}

int CEMSPrinterDlg::SaveCallback(void *pUser, int argc, char **argv, char **azColName)
{
	string str, line;
	FILE *fp = (FILE*)pUser;
	for(int i = 0; i < argc-1; i++){
		str = argv[i];
		if((i == 3 || i == 5) && !str.empty()){
			line += '\'';
		}
		line += UTF_82ASCII(str);
		line += ",";
	}
	line[line.size() - 1] = '\n';
	fwrite(line.c_str(), 1, line.size(), fp);
	return 0;
}

void CEMSPrinterDlg::OnExportInfo()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog fdlg(FALSE, 
		L"csv Files (*csv)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,// |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
		_T("csv Files (*.csv)|*.csv;)||"),
		NULL);

	int ret = fdlg.DoModal();
	if(ret == IDCANCEL) return;

	CStringW file = fdlg.GetPathName();
	m_Filepath = CStrW2CStrA(file.GetBuffer());
	string sfile = m_Filepath;

	FILE *fp = fopen(sfile.c_str(), "w");
	if(fp == NULL){
		MessageBox(L"文件打开失败");
	}else{
		m_InfoDB.Query("", SaveCallback, fp);
		fclose(fp);
	}

}


void CEMSPrinterDlg::OnSetIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hwnd = m_EmsBill->m_hWnd;
	::SendMessage(hwnd,MSG_SET_INDEX, 0, (LPARAM)&m_InfoDB);
	OnSearchRealtime();	
}


void CEMSPrinterDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//MessageBox(L"关闭", L"提示", MB_OK);
	HWND hwnd = m_EmsBill->m_hWnd;
	::SendMessage(hwnd,MSG_CLOSE, 0, (LPARAM)&m_InfoDB);
	CDialogEx::OnClose();
}
