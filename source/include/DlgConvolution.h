#pragma once


// DlgConvolution dialog

class DlgConvolution : public CDialog
{
	DECLARE_DYNAMIC(DlgConvolution)

public:
	DlgConvolution(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgConvolution();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m11;
	double m12;
	double m13;
	double m21;
	double m22;
	double m23;
	double m31;
	double m32;
	double m33;
	int mIndex;
};
