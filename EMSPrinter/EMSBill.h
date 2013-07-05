#pragma once
#include "afxwin.h"
#include "FocusEdit.h"
#include <iostream>
#include <string>
using namespace std;
// CEMSBill 对话框

class CEMSBill : public CDialogEx
{
	DECLARE_DYNAMIC(CEMSBill)

public:
	CEMSBill(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEMSBill();

// 对话框数据
	enum { IDD = IDD_DIALOG_EMS_BILL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_StartX;
	int m_StartY;

	CFocusEdit m_Sender;
	CFocusEdit m_SenderPhone;
	CFocusEdit m_SenderCompany;
	CFocusEdit m_SenderCustomerCode;
	CFocusEdit m_SenderAddr;
	CFocusEdit m_SenderCustomerMark;
	CFocusEdit m_SenderZipcode;		

	CFocusEdit m_SenderSignature;
	CFocusEdit m_SendYear;
	CFocusEdit m_SenderMonth;
	CFocusEdit m_SendDay;
	CFocusEdit m_SendHour;

	CFocusEdit m_Receiver;
	CFocusEdit m_RecvPhone;
	CFocusEdit m_RecvCompany;
	CFocusEdit m_RecvCustomerCode;
	CFocusEdit m_RecvAddr;
	CFocusEdit m_RecvCity;
	CFocusEdit m_RecvZipcode;

	CFocusEdit m_ListName;	
	CButton m_PrintSendInfo;
	CButton m_PrintBkgPic;

	string m_ConfigFile;
	
	afx_msg LRESULT OnMsgInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgPrint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgSave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgSetIndex(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgClose(WPARAM wParam, LPARAM lParam);

	static int GetFindKeyCallback(void *pUser, int argc, char **argv, char **azColName);



public:
	int InitBaseInfo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
