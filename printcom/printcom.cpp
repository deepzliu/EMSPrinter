// printcom.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "printcom.h"

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

CPrinter::CPrinter()
{
 	m_StartX = 0;
	m_StartY = 0;
	m_DPI = 150;
	m_PrinterRate = (double)m_DPI / 25.4;
	//m_PageSize = "A4";
	m_PageWidth = 210;
	m_PageHeight = 297;
	m_Bkginfo.img = new CImage;
	m_PrintDlg = new CPrintDialog(FALSE);
	m_DCPrinter = new CDC;
}

CPrinter::~CPrinter()
{
	if(!m_Bkginfo.img->IsNull()){
		m_Bkginfo.img->Destroy();
	}
	delete m_Bkginfo.img;
	delete m_PrintDlg;
	delete m_DCPrinter;
}

int CPrinter::ReadConfig()
{
	return 0; 
}

int CPrinter::SetPageSize(CString An)
{
	if(An == L"A4"){
		m_PageWidth = 210;
		m_PageHeight = 297;
	}else if(An == L"A3"){
		m_PageWidth = 297;
		m_PageHeight = 420;
	}else if(An == L"A2"){
		m_PageWidth = 420;
		m_PageHeight = 597;
	}else if(An == L"A5"){
		m_PageWidth = 148;
		m_PageHeight = 210;
	}else if(An == L"A6"){
		m_PageWidth = 105;
		m_PageHeight = 148;
	}else if(An == L"A1"){
		m_PageWidth = 597;
		m_PageHeight = 840;
	}else if(An == L"B1"){
		m_PageWidth = 520;
		m_PageHeight = 740;
	}else if(An == L"B2"){
		m_PageWidth = 370;
		m_PageHeight = 520;
	}else if(An == L"B3"){
		m_PageWidth = 260;
		m_PageHeight = 370;
	}else if(An == L"B4"){
		m_PageWidth = 185;
		m_PageHeight = 260;
	}else if(An == L"B5"){
		m_PageWidth = 130;
		m_PageHeight = 185;
	}else{
		return UNKNOWN_PAGE_SIZE;
	}
	return SUCCESS; 
}

int CPrinter::SetPageStart(int x, int y)
{
	m_StartX = x;
	m_StartY = y;
	return SUCCESS;
}

int CPrinter::InsertItem(int x, int y, int fontsize, CString font, CString text)
{
	TextItem item;
	item.x = x + m_StartX;
	item.y = y + m_StartY;
	item.fontsize = fontsize;
	item.font = font;
	item.text = text;

	m_Items.push_back(item);

	return SUCCESS;
}

int CPrinter::DeleteItem(int x, int y)
{
	unsigned int nSize  = m_Items.size();
	for(unsigned int i = 0; i < nSize; i++){
		if(m_Items[i].x == x && m_Items[i].y == y){
			m_Items.erase(m_Items.begin() + i);
			return SUCCESS;
		}
	}
	return ERR;
}

int CPrinter::DeleteAllItems()
{
	m_Items.erase(m_Items.begin(), m_Items.end());
	return SUCCESS;
}

int CPrinter::SetBackground(int x, int y, int width, int height, CString path)
{
	m_Bkginfo.x = x;
	m_Bkginfo.y = y;
	m_Bkginfo.width = width;
	m_Bkginfo.height = height;
	m_Bkginfo.path = path;
	if(m_Bkginfo.img->IsNull() == false){
		m_Bkginfo.img->Destroy();
	}
	if(m_Bkginfo.img->Load(path) != 0){
		return LOAD_IMG_FAILED;
	}

	return SUCCESS;
}

int CPrinter::SetBackground(CString path)
{
	m_Bkginfo.x = 0;
	m_Bkginfo.y = 0;
	m_Bkginfo.path = path;
	if(m_Bkginfo.img->IsNull() == false){
		m_Bkginfo.img->Destroy();
	}
	if(m_Bkginfo.img->Load(path) != 0){
		return LOAD_IMG_FAILED;
	}

	m_Bkginfo.width = m_Bkginfo.img->GetWidth();
	m_Bkginfo.height = m_Bkginfo.img->GetHeight();

	return SUCCESS;
}

int CPrinter::TextLayout(HDC hdc)
{
	if(hdc == NULL){
		return NULL_POINTER;
	}
	
	m_PrinterRate = (double)m_DPI / 25.4;

	//set background
	if(!m_Bkginfo.path.IsEmpty()){
		//have not consider adjusting different page size
		double width = m_PageWidth * m_PrinterRate;
		double height = (double)width * m_Bkginfo.height / ((double)m_Bkginfo.width);
		m_Bkginfo.img->StretchBlt(*m_DCPrinter, m_Bkginfo.x, m_Bkginfo.y, (int)width, (int)height);
	}

	//set text
	double x, y;
	CFont pNewFont;
	int nSize = m_Items.size();
	 CGdiObject* pOldFont = m_DCPrinter->SelectStockObject(SYSTEM_FONT);
	
	 
	for(int i = 0; i < nSize; i++){
		//font
		if(i == 0 || ( i > 0 && m_Items[i].font != m_Items[i - 1].font)){
			if(pNewFont.GetObjectType() != 0){
				pNewFont.DeleteObject();
			}
			pNewFont.CreateFont(m_Items[i].fontsize, 0 ,0 , 0,
				FW_NORMAL,	FALSE, FALSE, FALSE, GB2312_CHARSET/*ANSI_CHARSET*/,
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
				DEFAULT_PITCH | FF_MODERN , m_Items[i].font);
			 m_DCPrinter->SelectObject(pNewFont);
		}

		//text
		x = m_PrinterRate * (double)m_Items[i].x;
		y = m_PrinterRate * (double)m_Items[i].y;
		//multi-line to TextOut if the text is too long
		m_DCPrinter->TextOut((int)x, (int)y, m_Items[i].text);
	}

	if(pNewFont.GetObjectType() != 0){
		pNewFont.DeleteObject();
	}
	m_DCPrinter->SelectObject(pOldFont);

	m_Items.clear();

	return SUCCESS;
}

int CPrinter::DoPrint()
{
	int ret = 0;

	m_PrintDlg->DoModal();
	HDC hdcPrinter = m_PrintDlg->GetPrinterDC();
	m_DPI = GetDeviceCaps(hdcPrinter, LOGPIXELSX);

	if (hdcPrinter == NULL)
	{
		return NO_PRINTER;
	}
	else
	{	   
	   m_DCPrinter->Attach(hdcPrinter);
	   m_DCPrinter->SetBkMode(TRANSPARENT); 
	   // call StartDoc() to begin printing
	   DOCINFO docinfo;
	   memset(&docinfo, 0, sizeof(docinfo));
	   docinfo.cbSize = sizeof(docinfo);
	   docinfo.lpszDocName = _T("EMS国内标准快递");

	   
	   // if it fails, complain and exit gracefully
	   if (m_DCPrinter->StartDoc(&docinfo) < 0)
	   {
		   m_DCPrinter->Detach();
		  return UNINITIAL;
	   }
	   else
	   {
		  // start a page
		  if (m_DCPrinter->StartPage() < 0)
		  {
			 m_DCPrinter->AbortDoc();
			 m_DCPrinter->Detach();
			 return START_PAGE_FAILED;
		  }
		  else
		  {
			  ret = TextLayout(hdcPrinter);
			  
			  m_DCPrinter->EndPage();
			  m_DCPrinter->EndDoc();
			  if(ret != 0){
				  m_DCPrinter->Detach();
			  	  return ret;
			  }
		  }
	   }
	}
	m_DCPrinter->Detach();
	return SUCCESS;
}

