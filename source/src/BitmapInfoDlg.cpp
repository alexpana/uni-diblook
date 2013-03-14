// BitmapInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diblook.h"
#include "BitmapInfoDlg.h"


// CBitmapInfoDlg dialog

IMPLEMENT_DYNAMIC(CBitmapInfoDlg, CDialog)

CBitmapInfoDlg::CBitmapInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBitmapInfoDlg::IDD, pParent)
	, m_HeaderSize(_T(""))
	, m_BitmapVersion(_T(""))
	, m_BPP(_T(""))
	, m_Width(_T(""))
	, m_Height(_T(""))
	, m_LUTSize(_T(""))
	, m_LUT(_T(""))
{

}

CBitmapInfoDlg::~CBitmapInfoDlg()
{
}

void CBitmapInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_Version);
	DDX_Text(pDX, IDC_STATIC2, m_HeaderSize);
	DDX_Text(pDX, IDC_STATIC1, m_BitmapVersion);
	DDX_Text(pDX, IDC_STATIC4, m_BPP);
	DDX_Text(pDX, IDC_STATIC9, m_Width);
	DDX_Text(pDX, IDC_STATIC5, m_Height);
	DDX_Text(pDX, IDC_STATIC6, m_LUTSize);
	DDX_Text(pDX, IDC_EDIT1, m_LUT);
}


BEGIN_MESSAGE_MAP(CBitmapInfoDlg, CDialog)
END_MESSAGE_MAP()


// CBitmapInfoDlg message handlers
