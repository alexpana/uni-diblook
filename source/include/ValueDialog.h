#pragma once


// ValueDialog dialog

class ValueDialog : public CDialog
{
	DECLARE_DYNAMIC(ValueDialog)

public:
	ValueDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ValueDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };
	double value;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
