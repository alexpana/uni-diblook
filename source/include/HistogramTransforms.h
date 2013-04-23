#pragma once


// HistogramTransforms dialog

class HistogramTransforms : public CDialog
{
	DECLARE_DYNAMIC(HistogramTransforms)

public:
	HistogramTransforms(CWnd* pParent = NULL);   // standard constructor
	virtual ~HistogramTransforms();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double GMin;
	double GMax;
	double Gamma;
	double Lum;
	int Operation;
};
