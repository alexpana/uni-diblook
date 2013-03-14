#pragma once

#include "Histogram.h"


// CHistogramDlg dialog

class CHistogramDlg : public CDialog
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:

	CHistogramDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistogramDlg();

// Dialog Data
	enum { IDD = IDD_HISTOGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CHistogram* m_Histogram;
	afx_msg void OnPaint();
};
