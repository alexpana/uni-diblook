// D:\sources\uni-diblook\source\src\DlgSelectMorphologicOperation.cpp : implementation file
//

#include "stdafx.h"
#include "diblook.h"
#include "D:\sources\uni-diblook\source\include\DlgSelectMorphologicOperation.h"
#include "afxdialogex.h"


// DlgSelectMorphologicOperation dialog

IMPLEMENT_DYNAMIC(DlgSelectMorphologicOperation, CDialogEx)

DlgSelectMorphologicOperation::DlgSelectMorphologicOperation(CWnd* pParent /*=NULL*/)
	: CDialog(DlgSelectMorphologicOperation::IDD, pParent)
	, m_Count(_T(""))
	, m_OpType(0)
{

}

DlgSelectMorphologicOperation::~DlgSelectMorphologicOperation()
{
}

void DlgSelectMorphologicOperation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_Count);
	DDX_CBIndex(pDX, IDC_COMBO1, m_OpType);
}

BEGIN_MESSAGE_MAP(DlgSelectMorphologicOperation, CDialog)
END_MESSAGE_MAP()


// DlgSelectMorphologicOperation message handlers
