#pragma once


// EdgeDetectDialog dialog

class EdgeDetectDialog : public CDialog
{
	DECLARE_DYNAMIC(EdgeDetectDialog)

public:
	EdgeDetectDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~EdgeDetectDialog();

// Dialog Data
	enum { IDD = IDD_EDGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int type;
	int value;
};
