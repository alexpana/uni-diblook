// ThresholdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diblook.h"
#include "ThresholdDlg.h"


// CThresholdDlg dialog

IMPLEMENT_DYNAMIC(CThresholdDlg, CDialog)

CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThresholdDlg::IDD, pParent)
	, m_Value(0)
{

}

CThresholdDlg::~CThresholdDlg()
{
}

void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Value);
}


BEGIN_MESSAGE_MAP(CThresholdDlg, CDialog)
END_MESSAGE_MAP()


// CThresholdDlg message handlers
