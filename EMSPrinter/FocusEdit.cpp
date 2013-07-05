// FocusEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "EMSPrinter.h"
#include "FocusEdit.h"


// CFocusEdit

IMPLEMENT_DYNAMIC(CFocusEdit, CEdit)

CFocusEdit::CFocusEdit()
{

}

CFocusEdit::~CFocusEdit()
{
}


BEGIN_MESSAGE_MAP(CFocusEdit, CEdit)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CFocusEdit 消息处理程序




void CFocusEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	CEdit::PostMessage(EM_SETSEL,0,GetWindowTextLength());

}
