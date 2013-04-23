// D:\sources\uni-diblook\source\src\DlgSelectMorphologicOperation.cpp : implementation file
//

#include "stdafx.h"
#include "diblook.h"
#include "HistogramTransforms.h"
#include "afxdialogex.h"


// DlgSelectMorphologicOperation dialog

IMPLEMENT_DYNAMIC(HistogramTransforms, CDialogEx)

	HistogramTransforms::HistogramTransforms(CWnd* pParent /*=NULL*/)
	: CDialog(HistogramTransforms::IDD, pParent)
	, GMax(0)
	, GMin(0)
	, Gamma(0)
	, Lum(0)
	, Operation(0)
{

}

HistogramTransforms::~HistogramTransforms()
{
}

void HistogramTransforms::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, GMax);
	DDX_Text(pDX, IDC_EDIT1, GMin);
	DDX_Text(pDX, IDC_EDIT3, Gamma);
	DDX_Text(pDX, IDC_EDIT4, Lum);
	DDX_CBIndex(pDX, IDC_COMBO1, Operation);
}

BEGIN_MESSAGE_MAP(HistogramTransforms, CDialog)
END_MESSAGE_MAP()


// DlgSelectMorphologicOperation message handlers
