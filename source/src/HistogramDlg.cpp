// HistogramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diblook.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"


// CHistogramDlg dialog

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent )
	: CDialog(CHistogramDlg::IDD, pParent)
{
}

CHistogramDlg::~CHistogramDlg()
{
	delete m_Histogram;
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CHistogramDlg::OnPaint()
{
	static int LEFT = 10;
	static int HEIGHT = 190;
	static int BOTTOM = 200;

	CPaintDC dc(this);
	POINT pct;
	for( int i = 0; i < m_Histogram->length; ++i ){
		pct.x = LEFT + i;
		int N = (int)(((float)m_Histogram->normalizedValue(i) / (float)m_Histogram->normalizedMax() ) * HEIGHT);
		for( int j = 0; j < N; ++j ){
			pct.y = BOTTOM - j;
			dc.SetPixel(pct, RGB( 0, 0, 0 ) );
		}
	}
}
