// DlgConvolution.cpp : implementation file
//

#include "stdafx.h"
#include "diblook.h"
#include "DlgConvolution.h"
#include "afxdialogex.h"


// DlgConvolution dialog

IMPLEMENT_DYNAMIC(DlgConvolution, CDialogEx)

DlgConvolution::DlgConvolution(CWnd* pParent /*=NULL*/)
	: CDialog(DlgConvolution::IDD, pParent)
	, m11(0)
	, m12(0)
	, m13(0)
	, m21(0)
	, m22(0)
	, m23(0)
	, m31(0)
	, m32(0)
	, m33(0)
	, mIndex(0)
{
}

DlgConvolution::~DlgConvolution()
{
}

void DlgConvolution::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m11);
	DDX_Text(pDX, IDC_EDIT7, m12);
	DDX_Text(pDX, IDC_EDIT10, m13);
	DDX_Text(pDX, IDC_EDIT5, m21);
	DDX_Text(pDX, IDC_EDIT8, m22);
	DDX_Text(pDX, IDC_EDIT11, m23);
	DDX_Text(pDX, IDC_EDIT6, m31);
	DDX_Text(pDX, IDC_EDIT9, m32);
	DDX_Text(pDX, IDC_EDIT12, m33);
	DDX_CBIndex(pDX, IDC_COMBO1, mIndex);
}


BEGIN_MESSAGE_MAP(DlgConvolution, CDialog)
END_MESSAGE_MAP()