// printcom.h : printcom DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "atlimage.h"
#include <vector>
#include "errinfo.h"
#include "sqlite3.h"

using namespace std;

// CprintcomApp
// 有关此类实现的信息，请参阅 printcom.cpp
//
/*

class CprintcomApp : public CWinApp
{
public:
	CprintcomApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
*/

typedef struct _TextItem
{
	int x, y;
	int fontsize;
	CString font;
	CString text;
}TextItem;

typedef struct _Bkg_Info
{
	int x, y;
	int width, height;
	CString path;
	CImage *img;
}Bkg_Info;

class CPrinter
{
public:
	CPrinter();
	~CPrinter();

	//set printer
	int SetPageSize(CString An);	
	int SetPageStart(int x, int y);
	
	int GetDPI(){ return m_DPI;}
	double GetRate(){ return (double)m_DPI / 25.4;}

	//set print content, x and y are in millimeters
	int InsertItem(int x, int y, int fontsize, CString font, CString text);
	int DeleteItem(int x, int y);
	int DeleteAllItems();
	int SetBackground(int x, int y, int width, int height, CString path);
	int SetBackground(CString path);//start from (0, 0) and set the default size

	int DoPrint();

private:
	
	int ReadConfig();
	int TextLayout(HDC hdc);

	CPrintDialog *m_PrintDlg;
	CDC *m_DCPrinter;
	double m_PrinterRate;
	int m_DPI;
	int m_StartX;//dpi
	int m_StartY;//dpi
	int m_PageWidth;//mm
	int m_PageHeight;//mm
	Bkg_Info m_Bkginfo;
	vector<TextItem> m_Items;
};


struct Column_Map{
	string column;
	string real;	
};

class CInfoDB
{
public:
	CInfoDB();
	~CInfoDB();

	//根据公司名作为主键
	int InitDB();
	int ExecSQL(string sql);
	int SetDBFile(char *file);

	//用findkey作查询字段
	int Query(string query, int(*callback)(void *, int, char**, char**),  void *pUser);
	int Insert(string findkey, string company, string receiver, string tel, string address, string postalcode, string city);
	int Delete(string company);//根据公司名删除
	//根据公司名修改
	int Modify(string findkey, string company, string receiver, string tel, string address, string postalcode, string city);
	int QueryByCompany(string query, int(*callback)(void *, int, char**, char**),  void *pUser);
	static int QueryCompanyCallback(void *pUser, int argc, char **argv, char **azColName);
	int UpdateIndex(string findkey, string company, string receiver, string tel, string address, string postalcode, string city);
	int GetIndex(string company, int(*callback)(void *, int, char**, char**),  void *pUser);

	int QueryCompanyFlag;
	string m_TmpFindKey;

private:

	char m_DBFile[256];
	sqlite3 *m_db;
	string m_sql;
	char sqlstr[1024];
	string m_Table;
	Column_Map *m_Column_Map;	
	static int CallBackFun(void *NotUsed, int argc, char **argv, char **azColName);
};



string getLetter(string strText);
string getLetter(string strText, string &acronym);
string findLetter(int nCode);

std::wstring Utf82Unicode(const std::string& utf8string);
std::string WideByte2Acsi(std::wstring& wstrcode);
std::string UTF_82ASCII(std::string& strUtf8Code);
std::wstring Acsi2WideByte(std::string& strascii);
std::string Unicode2Utf8(const std::wstring& widestring);
std::string ASCII2UTF_8(std::string& strAsciiCode);