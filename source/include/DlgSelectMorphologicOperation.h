#pragma once


// DlgSelectMorphologicOperation dialog

class DlgSelectMorphologicOperation : public CDialog
{
	DECLARE_DYNAMIC(DlgSelectMorphologicOperation)

public:
	DlgSelectMorphologicOperation(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgSelectMorphologicOperation();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Count;
	int m_OpType;
};
