// D:\Sources\uni-diblook\source\src\ValueDialog.cpp : implementation file
//

#include "stdafx.h"
#include "diblook.h"
#include "D:\Sources\uni-diblook\source\include\ValueDialog.h"
#include "afxdialogex.h"


// ValueDialog dialog

IMPLEMENT_DYNAMIC(ValueDialog, CDialog)

ValueDialog::ValueDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ValueDialog::IDD, pParent)
{
	value = 0.8;
}

ValueDialog::~ValueDialog()
{
}

void ValueDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, value);
}


BEGIN_MESSAGE_MAP(ValueDialog, CDialog)
END_MESSAGE_MAP()


// ValueDialog message handlers
