// EdgeDetectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "diblook.h"
#include "EdgeDetectDialog.h"
#include "afxdialogex.h"


// EdgeDetectDialog dialog

IMPLEMENT_DYNAMIC(EdgeDetectDialog, CDialog)

EdgeDetectDialog::EdgeDetectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EdgeDetectDialog::IDD, pParent)
	, type(0)
	, value(0)
{

}

EdgeDetectDialog::~EdgeDetectDialog()
{
}

void EdgeDetectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO1, type);
	DDX_CBIndex(pDX, IDC_COMBO2, value);
}


BEGIN_MESSAGE_MAP(EdgeDetectDialog, CDialog)
END_MESSAGE_MAP()


// EdgeDetectDialog message handlers
