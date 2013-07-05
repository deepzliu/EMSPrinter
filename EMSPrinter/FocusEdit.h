#pragma once


// CFocusEdit

class CFocusEdit : public CEdit
{
	DECLARE_DYNAMIC(CFocusEdit)

public:
	CFocusEdit();
	virtual ~CFocusEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


