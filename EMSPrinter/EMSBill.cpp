// EMSBill.cpp : 实现文件
//

#include "stdafx.h"
#include "EMSPrinter.h"
#include "EMSBill.h"
#include "afxdialogex.h"
#include "common.h"
#include "IndexDlg.h"
#include "..\printcom\printcom.h"
#include <fstream>

// CEMSBill 对话框

IMPLEMENT_DYNAMIC(CEMSBill, CDialogEx)

CEMSBill::CEMSBill(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEMSBill::IDD, pParent)
{

}

CEMSBill::~CEMSBill()
{
}

void CEMSBill::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Sender);
	DDX_Control(pDX, IDC_EDIT2, m_SenderPhone);
	DDX_Control(pDX, IDC_EDIT3, m_SenderCompany);
	DDX_Control(pDX, IDC_EDIT4, m_SenderCustomerCode);
	DDX_Control(pDX, IDC_EDIT5, m_SenderAddr);
	DDX_Control(pDX, IDC_EDIT6, m_SenderCustomerMark);
	DDX_Control(pDX, IDC_EDIT7, m_SenderZipcode);
	DDX_Control(pDX, IDC_EDIT8, m_Receiver);
	DDX_Control(pDX, IDC_EDIT9, m_RecvPhone);
	DDX_Control(pDX, IDC_EDIT10, m_RecvCompany);
	DDX_Control(pDX, IDC_EDIT11, m_RecvCustomerCode);
	DDX_Control(pDX, IDC_EDIT12, m_RecvAddr);
	DDX_Control(pDX, IDC_EDIT13, m_RecvCity);
	DDX_Control(pDX, IDC_EDIT14, m_RecvZipcode);
	DDX_Control(pDX, IDC_EDIT15, m_ListName);
	DDX_Control(pDX, IDC_EDIT16, m_SenderSignature);
	DDX_Control(pDX, IDC_EDIT17, m_SendYear);
	DDX_Control(pDX, IDC_EDIT18, m_SenderMonth);
	DDX_Control(pDX, IDC_EDIT19, m_SendDay);
	DDX_Control(pDX, IDC_EDIT20, m_SendHour);
	DDX_Control(pDX, IDC_CHECK1, m_PrintSendInfo);
	DDX_Control(pDX, IDC_CHECK2, m_PrintBkgPic);
}


BEGIN_MESSAGE_MAP(CEMSBill, CDialogEx)
	ON_MESSAGE(MSG_INFO, CEMSBill::OnMsgInfo)
	ON_MESSAGE(MSG_PRINT, CEMSBill::OnMsgPrint)
	ON_MESSAGE(MSG_SAVE, CEMSBill::OnMsgSave)
	ON_MESSAGE(MSG_SET_INDEX, CEMSBill::OnMsgSetIndex)
	ON_MESSAGE(MSG_CLOSE, CEMSBill::OnMsgClose)
END_MESSAGE_MAP()


// CEMSBill 消息处理程序

int CEMSBill::InitBaseInfo()
{
	m_StartX = 0;
	m_StartY = 0;
	m_ConfigFile = "config.ini";
	ifstream fin(m_ConfigFile, ios::in);
	if(fin.is_open()){
		string line;
		string sub1, sub2;
		CStringA strA;
		int pos = 0;
		while(getline(fin, line)){
			pos = line.find('=', 0);
			sub1 = line.substr(0, pos);
			sub2 = line.substr(pos+1, line.size()-pos-1);
			if(sub1 == "sendflag"){
				if(sub2 == "yes"){
					m_PrintSendInfo.SetCheck(TRUE);
				}else{
					m_PrintSendInfo.SetCheck(FALSE);
				}
			}else if(sub1 == "sender"){
				strA.Format("%s", sub2.c_str());
				m_Sender.SetWindowText(CStrA2CStrW(strA));
			}else if(sub1 == "tel"){
				strA.Format("%s", sub2.c_str());
				m_SenderPhone.SetWindowText(CStrA2CStrW(strA));
			}else if(sub1 == "company"){
				strA.Format("%s", sub2.c_str());
				m_SenderCompany.SetWindowText(CStrA2CStrW(strA));
			}else if(sub1 == "customercode"){
				strA.Format("%s", sub2.c_str());
				m_SenderCustomerCode.SetWindowText(CStrA2CStrW(strA));
			}else if(sub1 == "address"){
				strA.Format("%s", sub2.c_str());
				m_SenderAddr.SetWindowText(CStrA2CStrW(strA));
			}else if(sub1 == "billNo"){
				strA.Format("%s", sub2.c_str());
				m_SenderCustomerMark.SetWindowText(CStrA2CStrW(strA));
			}else if(sub1 == "postalcode"){
				strA.Format("%s", sub2.c_str());
				m_SenderZipcode.SetWindowText(CStrA2CStrW(strA));
			}else if(sub1 == "signature"){
				strA.Format("%s", sub2.c_str());
				m_SenderSignature.SetWindowText(CStrA2CStrW(strA));
			}else if(sub1 == "startx"){
				m_StartX = StrToIntA(sub2.c_str());
			}else if(sub1 == "starty"){
				m_StartY = StrToIntA(sub2.c_str());
			}
		}
		fin.close();
	}else{
		m_Sender.SetWindowText(L"账务");
		m_SenderCompany.SetWindowText(L"同方股份有限公司");
		m_SenderCustomerCode.SetWindowText(L"8537");
		m_SenderAddr.SetWindowText(L"北京市海淀区王庄路1号院清华同方科技大厦D座22层");
		m_SenderZipcode.SetWindowText(L"100083");
		m_SenderSignature.SetWindowText(L"账务");
	}

	CString msg;
	CTime nowtime;
	nowtime = CTime::GetCurrentTime(); 
	msg.Format(L"%04d", nowtime.GetYear());
	m_SendYear.SetWindowText(msg);
	msg.Format(L"%02d", nowtime.GetMonth());
	m_SenderMonth.SetWindowText(msg);
	msg.Format(L"%02d", nowtime.GetDay());
	m_SendDay.SetWindowText(msg);
	msg.Format(L"%02d", nowtime.GetHour());
	m_SendHour.SetWindowText(msg);

	m_PrintBkgPic.ShowWindow(SW_HIDE);

	return 0;
}

#define MAX_LIST 16
typedef struct _Info_Item{
	char item[6][256];
}Info_Item, *pInfo_Item;

LRESULT CEMSBill::OnMsgInfo(WPARAM wParam, LPARAM lParam)
{
	CStringA str;

	m_RecvCity.SetWindowText(L"");
	m_RecvCustomerCode.SetWindowText(L"");
	m_ListName.SetWindowText(L"");

	pInfo_Item p = (pInfo_Item)lParam;
	str.Format("%s", p->item[0]);
	m_RecvCompany.SetWindowText(CStrA2CStrW(str));
	str.Format("%s", p->item[1]);
	m_Receiver.SetWindowText(CStrA2CStrW(str));
	str.Format("%s", p->item[2]);
	m_RecvPhone.SetWindowText(CStrA2CStrW(str));
	str.Format("%s", p->item[3]);
	m_RecvAddr.SetWindowText(CStrA2CStrW(str));
	str.Format("%s", p->item[4]);
	m_RecvZipcode.SetWindowText(CStrA2CStrW(str));
	str.Format("%s", p->item[5]);
	m_RecvCity.SetWindowText(CStrA2CStrW(str));

	return 0;
}
LRESULT CEMSBill::OnMsgPrint(WPARAM wParam, LPARAM lParam)
{
	CPrinter *pPrinter = (CPrinter*)lParam;	
	if(pPrinter == NULL){
		MessageBox(L"程序有错误", L"警告", MB_ICONWARNING);
		return 1;
	}

	int x, y;
	int maxLen = 80 * pPrinter->GetRate();
	CString font = L"宋体", text;
	CString str[5];
	int fontsize = 27;
	pPrinter->SetPageStart(m_StartX, m_StartY);

	if(m_PrintSendInfo.GetState() == BST_CHECKED){
		x = 8, y = 19;
		m_Sender.GetWindowText(text);
		pPrinter->InsertItem(x,y,fontsize, font, text);
		
		x = 51, y = 18;
		m_SenderPhone.GetWindowText(text);
		pPrinter->InsertItem(x,y,fontsize, font, text);

		x = 11, y = 25;
		m_SenderCompany.GetWindowText(text);
		pPrinter->InsertItem(x,y,fontsize, font, text);

		x = 70, y = 24;
		m_SenderCustomerCode.GetWindowText(text);
		pPrinter->InsertItem(x,y,fontsize, font, text);

		x = 6, y = 31;
		m_SenderAddr.GetWindowText(text);
		if(text.GetLength() * fontsize > maxLen){
			str[0] = text.Left(maxLen / fontsize);
			str[1] = text.Right(text.GetLength() - maxLen / fontsize);
			pPrinter->InsertItem(x,y,fontsize, font, str[0]);
			y += fontsize / pPrinter->GetRate();
			pPrinter->InsertItem(x,y,fontsize, font, str[1]);
		}else{
			pPrinter->InsertItem(x,y,fontsize, font, text);
		}

		x = 65, y = 39;
		m_SenderZipcode.GetWindowText(text);
		str[0] = L"";
		for(int i = 0; i < text.GetLength(); i++){
			str[0] += text.GetAt(i);
			str[0] += L" ";
		}
		pPrinter->InsertItem(x,y,fontsize, font, str[0]);
		
		//时间
		x = 147, y = 52;	
		m_SendYear.GetWindowText(str[0]);
		m_SenderMonth.GetWindowText(str[1]);
		m_SendDay.GetWindowText(str[2]);
		m_SendHour.GetWindowText(str[3]);
		text.Format(L"%s  %s  %s  %s", str[0], str[1], str[2], str[3]);
		pPrinter->InsertItem(x,y,fontsize, font, text);
	}else{
		//时间
		x = 159, y = 52;	
		m_SendYear.GetWindowText(str[0]);
		m_SenderMonth.GetWindowText(str[1]);
		m_SendDay.GetWindowText(str[2]);
		m_SendHour.GetWindowText(str[3]);
		text.Format(L"%s  %s  %s", str[1], str[2], str[3]);
		pPrinter->InsertItem(x,y,fontsize, font, text);
	}
	

	//签名
	x = 155, y = 44;
	m_SenderSignature.GetWindowText(text);
	pPrinter->InsertItem(x,y,fontsize, font, text);

	//收件人信息
	x = 8, y = 50;
	m_Receiver.GetWindowText(text);
	pPrinter->InsertItem(x,y,fontsize, font, text);

	x = 51, y = 49;
	m_RecvPhone.GetWindowText(text);
	pPrinter->InsertItem(x,y,fontsize, font, text);

	x = 11, y = 56;
	m_RecvCompany.GetWindowText(text);
	pPrinter->InsertItem(x,y,fontsize, font, text);

	x = 6, y = 61;
	m_RecvAddr.GetWindowText(text);
	if(text.GetLength() * fontsize > maxLen){
		str[0] = text.Left(maxLen / fontsize);
		str[1] = text.Right(text.GetLength() - maxLen / fontsize);
		pPrinter->InsertItem(x,y,fontsize, font, str[0]);
		y += fontsize / pPrinter->GetRate();
		pPrinter->InsertItem(x,y,fontsize, font, str[1]);
	}else{
		pPrinter->InsertItem(x,y,fontsize, font, text);
	}

	x = 11, y = 77;
	m_RecvCity.GetWindowText(text);
	pPrinter->InsertItem(x,y,fontsize, font, text);

	x = 65, y = 76;
	m_RecvZipcode.GetWindowText(text);
	str[0] = L"";
	for(int i = 0; i < text.GetLength(); i++){
		str[0] += text.GetAt(i);
		str[0] += L" ";
	}
	pPrinter->InsertItem(x,y,fontsize, font, str[0]);

	x = 11, y = 95;
	m_ListName.GetWindowText(text);
	pPrinter->InsertItem(x,y,fontsize, font, text);

	if(m_PrintBkgPic.GetState()){
		CFileDialog fdlg(TRUE, 
			L"图片文件 (*jpg)", 
			NULL, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
			_T("图片文件 (*.jpg)|*.jpg;)||"),
			NULL);
		fdlg.m_ofn.lpstrTitle = L"打开票据样式的图片";
		int ret = fdlg.DoModal();
		if(ret == IDOK){
			CString file = fdlg.GetPathName();
			pPrinter->SetBackground(file);
		}
	}else{
		pPrinter->SetBackground(L"");
	}

	int ret = pPrinter->DoPrint();

	return 0;
}


LRESULT CEMSBill::OnMsgSave(WPARAM wParam, LPARAM lParam)
{
	CInfoDB *db = (CInfoDB*)lParam;
	CString text;
	CStringA strA;
	int ret = 0;
	string findkey, company, receiver, tel, address, postalcode, city;

	m_Receiver.GetWindowText(text);
	receiver = CStrW2CStrA(text);

	m_RecvPhone.GetWindowText(text);
	tel = CStrW2CStrA(text);

	m_RecvCompany.GetWindowText(text);
	company = CStrW2CStrA(text);
	if(text == L""){
		MessageBox(L"公司名不能为空！", L"警告", MB_ICONWARNING);
		return 1;
	}

	m_RecvAddr.GetWindowText(text);
	address = CStrW2CStrA(text);

	m_RecvZipcode.GetWindowText(text);
	postalcode = CStrW2CStrA(text);

	m_RecvCity.GetWindowText(text);
	city = CStrW2CStrA(text);


	strA.Format("确认更新“%s”的信息吗？", company.c_str());
	ret = MessageBox(CStrA2CStrW(strA), L"提示", MB_YESNO | MB_ICONWARNING);
	if(ret != 6){
		return 1;
	}

	if(db->QueryByCompany(company, db->QueryCompanyCallback, db) == 0){
		//数据库中有该项，则修改
		ret = db->Modify(findkey, company, receiver, tel, address, postalcode, city);
	}else{
		//数据库中无该项，则新建
		ret = db->Insert(findkey, company, receiver, tel, address, postalcode, city);
	}

	if(ret == 0){
		MessageBox(L"数据更新成功！", L"提示", MB_OK);
	}else{
		MessageBox(L"数据更新失败！", L"警告", MB_ICONWARNING);
	}
	
	return 0;
}

LRESULT CEMSBill::OnMsgClose(WPARAM wParam, LPARAM lParam){

	// TODO: 在此处添加消息处理程序代码
	CString str;
	CStringA strA;
	int ret = 0;
	str.Format(L"是否保存寄件人信息？\n如有更新将被覆盖。 ");
	ret = MessageBox(str, L"提示", MB_YESNO | MB_ICONWARNING);
	if(ret != 6){
		return 1;
	}
	
	ofstream fout(m_ConfigFile, ios::trunc|ios::out);
	if(fout.is_open()){
		string line;

		strA.Format("startx=%d", m_StartX);
		line = strA;
		fout << line << endl;

		strA.Format("starty=%d", m_StartY);
		line = strA;
		fout << line << endl;

		if(m_PrintSendInfo.GetState() == BST_CHECKED){
			strA = "yes";
		}else{
			strA = "no";
		}
		line = "sendflag=";
		line += strA;
		fout << line << endl;

		m_Sender.GetWindowText(str);
		line = "sender=";
		strA = CStrW2CStrA(str);
		line += strA;
		fout << line << endl;
		
		m_SenderPhone.GetWindowText(str);
		line = "tel=";
		strA = CStrW2CStrA(str);
		line += strA;
		fout << line << endl;

		m_SenderCompany.GetWindowText(str);
		line = "company=";
		strA = CStrW2CStrA(str);
		line += strA;
		fout << line << endl;

		m_SenderCustomerCode.GetWindowText(str);
		line = "customercode=";
		strA = CStrW2CStrA(str);
		line += strA;
		fout << line << endl;

		m_SenderAddr.GetWindowText(str);
		line = "address=";
		strA = CStrW2CStrA(str);
		line += strA;
		fout << line << endl;

		m_SenderZipcode.GetWindowText(str);
		line = "postalcode=";
		strA = CStrW2CStrA(str);
		line += strA;
		fout << line << endl;

		m_SenderSignature.GetWindowText(str);
		line = "signature=";
		strA = CStrW2CStrA(str);
		line += strA;
		fout << line << endl;
		
		fout.close();
	}
	return 0;
}

int CEMSBill::GetFindKeyCallback(void *pUser, int argc, char **argv, char **azColName)
{
	string *index = (string*)pUser;
	if(argc > 0){
		*index = argv[0];
	}
	return 0;

}

LRESULT CEMSBill::OnMsgSetIndex(WPARAM wParam, LPARAM lParam)
{
	CInfoDB *db = (CInfoDB*)lParam;
	CIndexDlg dlg;
	CString text;
	string company, index;

	m_RecvCompany.GetWindowText(text);
	company = CStrW2CStrA(text);
	db->GetIndex(company, GetFindKeyCallback, &index);

	dlg.SetCompanyAndIndex(company, index);
	if(dlg.DoModal() == IDOK){
		index = dlg.GetIndex();

		CString text;
		CStringA strA;
		int ret = 0;
		string findkey, company, receiver, tel, address, postalcode, city;

		m_Receiver.GetWindowText(text);
		receiver = CStrW2CStrA(text);

		m_RecvPhone.GetWindowText(text);
		tel = CStrW2CStrA(text);

		m_RecvCompany.GetWindowText(text);
		company = CStrW2CStrA(text);
		if(text == L""){
			MessageBox(L"公司名不能为空！", L"警告", MB_ICONWARNING);
			return 1;
		}

		m_RecvAddr.GetWindowText(text);
		address = CStrW2CStrA(text);

		m_RecvZipcode.GetWindowText(text);
		postalcode = CStrW2CStrA(text);

		m_RecvCity.GetWindowText(text);
		city = CStrW2CStrA(text);

		if(db->UpdateIndex(index, company, receiver, tel, address, postalcode, city) == 0){
			MessageBox(L"修改索引成功", L"提示", MB_OK);
		}else{
			MessageBox(L"修改索引失败", L"警告", MB_ICONWARNING);
		}
	}

	return 0;
}


BOOL CEMSBill::PreTranslateMessage(MSG* pMsg)
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
