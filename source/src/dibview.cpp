// dibview.cpp : implementation of the CDibView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "diblook.h"

#include "dibdoc.h"
#include "dibview.h"
#include "dibapi.h"
#include "mainfrm.h"

#include "HRTimer.h"

#include <math.h>

#include "BitmapInfoDlg.h"
#include "ThresholdDlg.h"

#include "Histogram.h"
#include "HistogramDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define BEGIN_PROCESSING() INCEPUT_PRELUCRARI()

#define END_PROCESSING(Title) SFARSIT_PRELUCRARI(Title)

#define INCEPUT_PRELUCRARI() \
	CDibDoc* pDocSrc=GetDocument();										\
	CDocTemplate* pDocTemplate=pDocSrc->GetDocTemplate();				\
	CDibDoc* pDocDest=(CDibDoc*) pDocTemplate->CreateNewDocument();		\
	BeginWaitCursor();													\
	HDIB hBmpSrc=pDocSrc->GetHDIB();									\
	HDIB hBmpDest = (HDIB)::CopyHandle((HGLOBAL)hBmpSrc);				\
	if ( hBmpDest==0 ) {												\
		pDocTemplate->RemoveDocument(pDocDest);							\
		return;															\
	}																	\
	BYTE* lpD = (BYTE*)::GlobalLock((HGLOBAL)hBmpDest);					\
	BYTE* lpS = (BYTE*)::GlobalLock((HGLOBAL)hBmpSrc);					\
	int iColors = DIBNumColors((char *)&(((LPBITMAPINFO)lpD)->bmiHeader)); \
	RGBQUAD *bmiColorsDst = ((LPBITMAPINFO)lpD)->bmiColors;	\
	RGBQUAD *bmiColorsSrc = ((LPBITMAPINFO)lpS)->bmiColors;	\
	BYTE * lpDst = (BYTE*)::FindDIBBits((LPSTR)lpD);	\
	BYTE * lpSrc = (BYTE*)::FindDIBBits((LPSTR)lpS);	\
	int dwWidth  = ::DIBWidth((LPSTR)lpS);\
	int dwHeight = ::DIBHeight((LPSTR)lpS);\
	int w=WIDTHBYTES(dwWidth*((LPBITMAPINFOHEADER)lpS)->biBitCount);	\
	HRTimer my_timer;	\
	my_timer.StartTimer();	\

#define BEGIN_SOURCE_PROCESSING \
	CDibDoc* pDocSrc=GetDocument();										\
	BeginWaitCursor();													\
	HDIB hBmpSrc=pDocSrc->GetHDIB();									\
	BYTE* lpS = (BYTE*)::GlobalLock((HGLOBAL)hBmpSrc);					\
	int iColors = DIBNumColors((char *)&(((LPBITMAPINFO)lpS)->bmiHeader)); \
	RGBQUAD *bmiColorsSrc = ((LPBITMAPINFO)lpS)->bmiColors;	\
	BYTE * lpSrc = (BYTE*)::FindDIBBits((LPSTR)lpS);	\
	int dwWidth  = ::DIBWidth((LPSTR)lpS);\
	int dwHeight = ::DIBHeight((LPSTR)lpS);\
	int w=WIDTHBYTES(dwWidth*((LPBITMAPINFOHEADER)lpS)->biBitCount);	\
	


#define END_SOURCE_PROCESSING	\
	::GlobalUnlock((HGLOBAL)hBmpSrc);								\
    EndWaitCursor();												\
/////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------
#define SFARSIT_PRELUCRARI(Titlu)	\
	double elapsed_time_ms = my_timer.StopTimer();	\
	CString Title;	\
	Title.Format("%s - Proc. time = %.2f ms", Titlu, elapsed_time_ms);	\
	::GlobalUnlock((HGLOBAL)hBmpDest);								\
	::GlobalUnlock((HGLOBAL)hBmpSrc);								\
    EndWaitCursor();												\
	pDocDest->SetHDIB(hBmpDest);									\
	pDocDest->InitDIBData();										\
	pDocDest->SetTitle((LPCSTR)Title);									\
	CFrameWnd* pFrame=pDocTemplate->CreateNewFrame(pDocDest,NULL);	\
	pDocTemplate->InitialUpdateFrame(pFrame,pDocDest);	\

/////////////////////////////////////////////////////////////////////////////
// CDibView

IMPLEMENT_DYNCREATE(CDibView, CScrollView)

BEGIN_MESSAGE_MAP(CDibView, CScrollView)
	//{{AFX_MSG_MAP(CDibView)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_MESSAGE(WM_DOREALIZE, OnDoRealize)
	ON_COMMAND(ID_PROCESSING_PARCURGERESIMPLA, OnProcessingParcurgereSimpla)
	//}}AFX_MSG_MAP

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_LABORATOR1_CROPCIRCLE, &CDibView::OnLaborator1Cropcircle)
	ON_COMMAND(ID_LABORATOR2_AFISAREINFO, &CDibView::OnLaborator2Afisareinfo)
	ON_COMMAND(ID_LABORATOR2_GRAYSCALE2, &CDibView::OnLaborator2Grayscale2)
	ON_COMMAND(ID_LABORATOR2_BLACKWHITE, &CDibView::OnLaborator2Blackwhite)
	ON_COMMAND(ID_LABORATOR3_HISTOGRAMA, &CDibView::OnLaborator3Histograma)
	ON_COMMAND(ID_LABORATOR3_REDUCE, &CDibView::OnLaborator3Reduce)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDibView construction/destruction

CDibView::CDibView()
{
}

CDibView::~CDibView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDibView drawing

void CDibView::OnDraw(CDC* pDC)
{
	CDibDoc* pDoc = GetDocument();

	HDIB hDIB = pDoc->GetHDIB();
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
		int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
		::GlobalUnlock((HGLOBAL) hDIB);
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		if (pDC->IsPrinting())   // printer DC
		{
			// get size of printer page (in pixels)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			// get printer pixels per inch
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			//
			// Best Fit case -- create a rectangle which preserves
			// the DIB's aspect ratio, and fills the page horizontally.
			//
			// The formula in the "->bottom" field below calculates the Y
			// position of the printed bitmap, based on the size of the
			// bitmap, the width of the page, and the relative size of
			// a printed pixel (cyInch / cxInch).
			//
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
		}
		else   // not printer DC
		{
			rcDest = rcDIB;
		}
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDibView printing

BOOL CDibView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CDibView commands


LRESULT CDibView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);
	CDibDoc* pDoc = GetDocument();
	if (pDoc->GetHDIB() == NULL)
		return 0L;  // must be a new document

	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);

		CClientDC appDC(pAppFrame);
		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);

		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				pDoc->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tSelectPalette failed in CDibView::OnPaletteChanged\n");
		}
	}

	return 0L;
}

void CDibView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	ASSERT(GetDocument() != NULL);

	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}


void CDibView::OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView)
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		ASSERT(pActivateView == this);
		OnDoRealize((WPARAM)m_hWnd, 0);   // same as SendMessage(WM_DOREALIZE);
	}
}

void CDibView::OnEditCopy()
{
	CDibDoc* pDoc = GetDocument();
	// Clean clipboard of contents, and copy the DIB.

	if (OpenClipboard())
	{
		BeginWaitCursor();
		EmptyClipboard();
		SetClipboardData (CF_DIB, CopyHandle((HANDLE) pDoc->GetHDIB()) );
		CloseClipboard();
		EndWaitCursor();
	}
}



void CDibView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
}


void CDibView::OnEditPaste()
{
	HDIB hNewDIB = NULL;

	if (OpenClipboard())
	{
		BeginWaitCursor();

		hNewDIB = (HDIB) CopyHandle(::GetClipboardData(CF_DIB));

		CloseClipboard();

		if (hNewDIB != NULL)
		{
			CDibDoc* pDoc = GetDocument();
			pDoc->ReplaceHDIB(hNewDIB); // and free the old DIB
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
			pDoc->UpdateAllViews(NULL);
		}
		EndWaitCursor();
	}
}


void CDibView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
}

void CDibView::OnProcessingParcurgereSimpla() 
{
	BEGIN_PROCESSING();

	// Makes a grayscale image by equalizing the R, G, B components from the LUT
	for (int k=0;  k < iColors ; k++)
		bmiColorsDst[k].rgbRed=bmiColorsDst[k].rgbGreen=bmiColorsDst[k].rgbBlue=k;


	//  Goes through the bitmap pixels and performs their negative	
	for (int i=0;i<dwHeight;i++)
		for (int j=0;j<dwWidth;j++)
		  {	
			lpDst[i*w+j]= 255 - lpSrc[i*w+j]; //makes image negative
	  }

	END_PROCESSING("Negativ imagine");
}


int insideCircle( int x, int y, int cx, int cy, int r ){
	return ((x-cx) * (x-cx) + (y-cy)*(y-cy)) < r * r;
}

void setPixelColor24( BYTE* src, int w, int x, int y, BYTE red, BYTE green, BYTE blue ){
	src[ x * w + 3 * y ]		= red;
	src[ x * w + 3 * y + 1 ]	= green;
	src[ x * w + 3 * y + 2 ]	= blue;
}

void getPixelColor24( BYTE* src, int w, int x, int y, BYTE& red, BYTE& green, BYTE& blue ){
	red		= src[ x * w + 3 * y ];
	green	= src[ x * w + 3 * y + 1 ];
	blue	= src[ x * w + 3 * y + 2 ];
}

void setPixelColor( BYTE* dst, int w, int x, int y, BYTE color, int bpp ){
	switch( bpp ){
		case 8:
			dst[ x * w + y ] = color;
			break;
		case 4:
			if( y % 2 == 0 ){
				BYTE old = (dst[ x * w + y / 2 ] & 0xf);
				dst[ x * w + y / 2 ] = ((color << 4) & 0xf0) + old;
			} else {
				BYTE old = (dst[ x * w + y / 2 ] & 0xf0);
				dst[ x * w + y / 2 ] = (color & 0xf) + old;
			}
			break;
		default:
			break;
	}
}

BYTE getPixelColor( BYTE* src, int w, int x, int y, int bpp ){
	switch( bpp ){
		case 8:
			return src[ x * w + y ];

		case 4:
			if( y % 2 == 0 ){
				return (src[ x * w + y / 2 ] >> 4);
			} else {
				return src[ x * w + y / 2 ] & 0xf;
			}

		default:
			return 0;
	}
}

void copyPixelColor( BYTE *src, BYTE *dst, int w, int x, int y, int bpp ){
	switch( bpp ){
		case 24:
			BYTE red, green, blue;
			getPixelColor24(src, w, x, y, red, green, blue );
			setPixelColor24(dst, w, x, y, red, green, blue );
			return;
		default:
			BYTE color = getPixelColor(src, w, x, y, bpp );
			setPixelColor(dst, w, x, y, color, bpp );
	}
}

void clearPixelColor( BYTE* dst, int w, int x, int y, int bpp ){
	switch( bpp ){
		case 24:
			setPixelColor24( dst, w, x, y, 0, 0, 0 );
			break;
		default:
			setPixelColor( dst, w, x, y, 0, bpp );
	}
}

int getBppFromColorSpace( int colorSpace ){
	if( colorSpace == 0 ){
		return 24;
	}

	int bpp = 0;
	while( colorSpace > 0 ){
		++bpp;
		colorSpace >>= 1;
	}
	return bpp - 1;
}

void CDibView::OnLaborator1Cropcircle()
{

	BEGIN_PROCESSING();
	int r = 100;

	int bpp = getBppFromColorSpace( iColors );

	for ( int x = 0; x < dwHeight; x++ ){
		for ( int y = 0; y < dwWidth; y++ ){
			if( insideCircle(x, y, dwHeight/2, dwWidth/2, r) ){
				copyPixelColor(lpSrc, lpDst, w, x, y, bpp );
			} else {
				clearPixelColor(lpDst, w, x, y, bpp );
			}
		}
	}

	END_PROCESSING("Crop Circle");
}

void CDibView::OnLaborator2Afisareinfo()
{
	BEGIN_SOURCE_PROCESSING;
	CBitmapInfoDlg dlgBitmap;
	LPBITMAPINFO pBitmapInfoSrc = (LPBITMAPINFO)lpS;

	dlgBitmap.m_HeaderSize.Format("Header Size: %d", 10 );
	dlgBitmap.m_BPP.Format("BPP: %d", pBitmapInfoSrc->bmiHeader.biBitCount);
	dlgBitmap.m_Width.Format("Width: %d", pBitmapInfoSrc->bmiHeader.biWidth);
	dlgBitmap.m_Height.Format("Height: %d", pBitmapInfoSrc->bmiHeader.biHeight);
	dlgBitmap.m_LUTSize.Format("LUT Size: %d", iColors );

	CString buffer;
	for( int i = 0;i<iColors; ++i ){
		buffer.Format( "%3d.\t%3d\t%3d\t%3d\t\r\n", i, 
			bmiColorsSrc[i].rgbRed,
			bmiColorsSrc[i].rgbGreen,
			bmiColorsSrc[i].rgbBlue);
			dlgBitmap.m_LUT += buffer;
	}

	dlgBitmap.DoModal();
	END_SOURCE_PROCESSING;
}

void CDibView::OnLaborator2Grayscale2()
{
	BEGIN_PROCESSING()

	LPBITMAPINFO pBitmapInfoSrc = (LPBITMAPINFO)lpS;

	if( pBitmapInfoSrc->bmiHeader.biBitCount == 24 ){

		for( int i = 0; i < dwHeight; ++i ){
			for( int  j = 0; j < dwWidth; ++j ){
				BYTE r, g, b;
				getPixelColor24( lpSrc, w, i, j, r, g, b );
				int gray = ( r + g + b ) / 3;
				setPixelColor24( lpDst, w, i, j, gray, gray, gray );
			}
		}
	}
	if( pBitmapInfoSrc->bmiHeader.biBitCount == 8 ){
		for( int i = 0; i < iColors; ++i ){
			int gray = (bmiColorsSrc[i].rgbRed + bmiColorsSrc[i].rgbGreen + bmiColorsSrc[i].rgbBlue ) / 3;
			bmiColorsDst[i].rgbRed = gray;
			bmiColorsDst[i].rgbGreen = gray;
			bmiColorsDst[i].rgbBlue = gray;
		}

		// SORT IT!!
		BYTE g[256];
		for( int k = 0; k < iColors; ++k ){
			g[k] = bmiColorsDst[k].rgbRed;
			bmiColorsDst[k].rgbRed = bmiColorsDst[k].rgbGreen = bmiColorsDst[k].rgbBlue = k;
		}

		for( int i = 0; i < dwHeight; ++i ){
			for( int  j = 0; j < dwWidth; ++j ){
				int k = getPixelColor( lpDst, w, i, j, 8 );
				setPixelColor(lpDst, w, i, j, g[k], 8);
			}
		}
	}

	END_PROCESSING("Grayscale")
}

void CDibView::OnLaborator2Blackwhite()
{
	BEGIN_PROCESSING()
	LPBITMAPINFO pBitmapInfoSrc = (LPBITMAPINFO)lpS;
	CThresholdDlg dlg;
	dlg.DoModal();
	int threshold = dlg.m_Value;

	if( pBitmapInfoSrc->bmiHeader.biBitCount == 8 ){
		for( int i = 0; i < dwHeight; ++i ){
			for( int  j = 0; j < dwWidth; ++j ){
				int k = getPixelColor( lpDst, w, i, j, 8 );
				if( bmiColorsDst[k].rgbBlue < threshold ){
					bmiColorsDst[k].rgbBlue = bmiColorsDst[k].rgbRed = bmiColorsDst[k].rgbGreen = 0;
				} else {
					bmiColorsDst[k].rgbBlue = bmiColorsDst[k].rgbRed = bmiColorsDst[k].rgbGreen = 255;
				}
			}
		}
	}
	END_PROCESSING("Black and White")
}

void CDibView::OnLaborator3Reduce()
{
	OnLaborator3Histograma();
}

void CDibView::OnLaborator3Histograma()
{
	BEGIN_SOURCE_PROCESSING;

	CHistogram histogram;

	CHistogramDlg histogramdlg;
	histogramdlg.m_Histogram = &histogram;

	LPBITMAPINFO pBitmapInfoSrc = (LPBITMAPINFO)lpS;
	histogram.maxValue = 0;
	if( pBitmapInfoSrc->bmiHeader.biBitCount == 8 ){
		for( int i = 0; i < dwWidth; ++i ){
			for( int j = 0; j < dwHeight; ++j ){
				int val = getPixelColor(lpSrc, w, i, j, 8 ) & 0xff;
				histogram.values[val] += 1;
				if( histogram.values[val] > histogram.maxValue ){
					histogram.maxValue = histogram.values[val];
				}
				if( histogram.maxValue < 0 ){
					histogram.maxValue = 0;
				}
			}
		}
	} else {
		return;
	}

	histogramdlg.DoModal();
	END_SOURCE_PROCESSING;
}

