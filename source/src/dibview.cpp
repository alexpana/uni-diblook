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

// Diblook Headers
#include "stdafx.h"
#include "diblook.h"

#include "dibdoc.h"
#include "dibview.h"
#include "dibapi.h"
#include "mainfrm.h"

// Windows Headers
#include "HRTimer.h"
#include "BitmapInfoDlg.h"
#include "ThresholdDlg.h"

// Personal Headers
#include "Histogram.h"
#include "HistogramDlg.h"
#include "Image.h"

#include "BinaryObject.h"
#include "Contour.h"

// STD Headers
#include <math.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

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

#define CONSTRUCT_SOURCE_IMAGE( img ) \
	Image img( getBppFromColorSpace( iColors ), dwWidth, dwHeight, bmiColorsSrc, iColors, lpSrc );

#define CONSTRUCT_DESTINATION_IMAGE( img ) \
	Image img( getBppFromColorSpace( iColors ), dwWidth, dwHeight, bmiColorsDst, iColors, lpDst );

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

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)


	ON_COMMAND(ID_LABORATOR1_CROPCIRCLE, &CDibView::OnLaborator1Cropcircle)
	ON_COMMAND(ID_LABORATOR2_AFISAREINFO, &CDibView::OnLaborator2Afisareinfo)
	ON_COMMAND(ID_LABORATOR2_GRAYSCALE2, &CDibView::OnLaborator2Grayscale2)
	ON_COMMAND(ID_LABORATOR2_BLACKWHITE, &CDibView::OnLaborator2Blackwhite)
	ON_COMMAND(ID_LABORATOR3_HISTOGRAMA, &CDibView::OnLaborator3Histograma)
	ON_COMMAND(ID_LABORATOR3_REDUCE, &CDibView::OnLaborator3Reduce)
	ON_COMMAND(ID_LABORATOR3_DITHER, &CDibView::OnLaborator3Dither)
	ON_COMMAND(ID_LABORATOR4_BINARYOBJECTSINFORMATION, &CDibView::OnLaborator4BinaryObjectsInformation)
	ON_COMMAND(ID_LABORATOR4_PX, &CDibView::OnLaborator4ProjectionX)
	ON_COMMAND(ID_LABORATOR4_PY, &CDibView::OnLaborator4ProjectionY)
	ON_COMMAND(ID_LABORATOR5_MARKOBJECTS, &CDibView::OnLaborator5MarkObjects)
	ON_COMMAND(ID_LABORATOR6_DRAWCONTOUR, &CDibView::OnLaborator6DrawExternalContour)
	ON_COMMAND(ID_LABORATOR6_DRAWCONTOUR2, &CDibView::OnLaborator6DrawImageContour)

	ON_WM_LBUTTONDBLCLK()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDibView construction/destruction

CDibView::CDibView()
{
}

CDibView::~CDibView()
{
}

static int SELECTED_COLOR;
static BinaryObject SELECTED_OBJECT;

#define II( argx, argy ) ( SELECTED_COLOR == lpSrc[(argx) * w + (argy)] ? 1 : 0 )

void CDibView::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	BEGIN_SOURCE_PROCESSING;

	CPoint pos = point + GetScrollPosition();

	int x = pos.x;
	int y = dwHeight - pos.y - 1;
	SELECTED_COLOR = lpSrc[y * w + x];

	BinaryObject* pbo = new BinaryObject();
	BinaryObject ob = *pbo;

	// Area
	// -----------------------------------------------------------------
	for( int r = 0; r < dwHeight; ++r ){
		for( int c = 0; c < dwWidth; ++c ){
			ob.area += II(r, c);
		}
	}

	// Perimeter
	// -----------------------------------------------------------------
	ob.perimeter = 0;
	for( int r = 0; r < dwHeight; ++r ){
		for( int c = 0; c < dwWidth; ++c ){
			if( ! II(r, c) )
				continue;

			bool isEdgePixel = false;

			for( int i = max( r - 1, 0 ); i < min( r+2, dwHeight - 1 ); ++i ){
				for( int j = max( c - 1, 0 ); j < min( c+2, dwWidth - 1 ); ++j )
					if( lpSrc[ i * w + j ] != SELECTED_COLOR )
						isEdgePixel = true;
			}

			if( isEdgePixel )
				ob.perimeter += 1;
		}
	}


	// Center of Mass
	// -----------------------------------------------------------------
	for( int r = 0; r < dwHeight; ++r ){
		for( int c = 0; c < dwWidth; ++c ){
			ob.centerOfMass.x += c * II(r, c);
			ob.centerOfMass.y += r * II(r, c);
		}
	}
	ob.centerOfMass.x = (LONG)((float)(ob.centerOfMass.x) * (1.0f / (float)(ob.area)));
	ob.centerOfMass.y = (LONG)((float)(ob.centerOfMass.y) * (1.0f / (float)(ob.area)));

	// Circularity
	// -----------------------------------------------------------------
	ob.thinnessRatio = 4.0f * 3.14152f * (float)ob.area / (float)(ob.perimeter * ob.perimeter);

	// Aspect Ratio
	// -----------------------------------------------------------------
	int minX, minY, maxX, maxY;
	maxX = maxY = 0;
	minY = dwHeight;
	minX = dwWidth;
	for( int r = 0; r < dwHeight; ++r ){
		for( int c = 0; c < dwWidth; ++c ){
			if( ! II( r, c ) ){
				continue;
			}
			if( r < minY ) minY = r;
			if( r > maxY ) maxY = r;
			if( c < minX ) minX = c;
			if( c > maxX ) maxX = c;
		}
	}

	ob.aspectRatio = (float)( maxX - minX + 1 ) / (float)(maxY - minY + 1);

	// Projection
	// -----------------------------------------------------------------
	ob.projectionsXSize = dwWidth;
	ob.projectionsYSize = dwHeight;
	ob.projectionsX = new int[ dwWidth ];
	ob.projectionsY = new int[ dwHeight ];

	for( int r = 0; r < dwHeight; ++r ){
		int count = 0;
		for( int c = 0; c <= dwWidth; ++c ){
			count += II( r, c );
		}
		ob.projectionsY[r] = count;
	}

	for( int c = 0; c < dwWidth; ++c ){
		int count = 0;
		for( int r = 0; r <= dwHeight; ++r ){
			count += II( r, c );
		}
		ob.projectionsX[c] = count;
	}

	// Axis of elongation
	// -----------------------------------------------------------------
	float A = 0.0f;
	float B = 0.0f;
	float C = 0.0f;

	for( int r = 0; r < dwHeight; ++r ){
		for( int c = 0; c < dwWidth; ++c ){
			A += 2 * ( r - ob.centerOfMass.y ) * ( c - ob.centerOfMass.x ) * II( r, c );
			B += ( c - ob.centerOfMass.x ) * ( c - ob.centerOfMass.x ) * II( r, c );
			C += ( r - ob.centerOfMass.y ) * ( r - ob.centerOfMass.y ) * II( r, c );
		}
	}

	ob.elongationAxis = atan2( A, (B-C) ) / 2.0f; // * 180.0f / (2.0f * 3.1416f);

	SELECTED_OBJECT = ob;

	// =================================================================
	//	Display the results
	// =================================================================
	if( x > 0 && x < dwWidth && y > 0 && y < dwHeight ){
		AfxMessageBox(ob.toCString());
	}

	END_SOURCE_PROCESSING;

	CScrollView::OnLButtonDblClk(nFlags, point);
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

CHistogram* getHistogram( BYTE* image, int width, int height ){
	CHistogram* histogram = new CHistogram();
	histogram->maxValue = 0;
	for( int i = 0; i < width; ++i ){
		for( int j = 0; j < height; ++j ){
			int val = getPixelColor(image, width, i, j, 8 ) & 0xff;
			histogram->values[val] += 1;
			if( histogram->values[val] > histogram->maxValue ){
				histogram->maxValue = histogram->values[val];
			}
			if( histogram->maxValue < 0 ){
				histogram->maxValue = 0;
			}
		}
	}
	return histogram;
}

void CDibView::OnLaborator3Histograma()
{
	BEGIN_SOURCE_PROCESSING;

	CHistogram* histogram = nullptr;

	LPBITMAPINFO pBitmapInfoSrc = (LPBITMAPINFO)lpS;
	if( pBitmapInfoSrc->bmiHeader.biBitCount == 8 ){
		histogram = getHistogram( lpSrc, w, dwHeight );
	} else {
		return;
	}

	// display the histogram
	CHistogramDlg histogramdlg;
	histogramdlg.m_Histogram = histogram;
	histogramdlg.DoModal();

	END_SOURCE_PROCESSING;
}

int getNearestHistogramValue( int oldValue, const std::vector<int>& table ){
	int fittest = 0;
	for( auto it = table.begin(); it != table.end(); ++it ){
		if( abs( oldValue - *it ) < abs( oldValue - fittest ) ){
			fittest = *it;
		}
	}
	return fittest;
}

vector<int> getReducedHistogram( CHistogram& hist ){
	static int WH = 5;
	static float TH = 0.0005f;

	std::vector<int> values;
	values.push_back(0);

	for( int k = WH; k < 255 - WH; ++k ){
		float max = 0;
		float average = 0;
		for( int i = k - WH; i < k + WH; ++i ){
			float val = hist.normalizedValue(i);
			if( val > max ){
				max = val;
			}
			average += val;
		}
		average /= (float) 2*WH + 1;

		if( hist.normalizedValue(k) == max && hist.normalizedValue(k) > average + TH ){
			values.push_back(k);
		}
	}
	values.push_back(255);
	return values;
}
void CDibView::OnLaborator3Reduce()
{
	BEGIN_PROCESSING()

	CHistogram hist = *getHistogram(lpSrc, dwWidth, dwHeight );
	vector<int> values = getReducedHistogram( hist );

	for( int i = 0; i < dwHeight; ++i ){
		for( int  j = 0; j < dwWidth; ++j ){
			BYTE color = getPixelColor( lpSrc, w, i, j, 8 );
			setPixelColor( lpDst, w, i, j, getNearestHistogramValue( color, values ), 8 );
		}
	}

	END_PROCESSING("Reduce Colors")
}



void CDibView::OnLaborator4BinaryObjectsInformation()
{
	BEGIN_PROCESSING();

	BinaryObject ob = SELECTED_OBJECT;

	// =================================================================
	// Draw the elongation axis
	// =================================================================

	CDC dc;
	dc.CreateCompatibleDC(0);
	CBitmap bitmap;
	HBITMAP hBitmap = CreateDIBitmap( ::GetDC(0), &((LPBITMAPINFO)lpS)->bmiHeader, CBM_INIT, lpSrc, (LPBITMAPINFO)lpS, DIB_RGB_COLORS );

	bitmap.Attach(hBitmap);

	CBitmap* pTempBmp = dc.SelectObject( &bitmap );
	CPen pen( PS_SOLID, 1, RGB(0, 255, 0 ) );

	CPen* pTempPen = dc.SelectObject(&pen);

	int ox = ob.centerOfMass.x;
	int oy = dwHeight - ob.centerOfMass.y + 1;

	dc.MoveTo( ox, oy );
	dc.LineTo( 
		ox - cos( ob.elongationAxis ) * 200, 
		oy + sin( ob.elongationAxis ) * 200
		);
	dc.MoveTo( ox, oy );
	dc.LineTo( 
		ox + cos( ob.elongationAxis ) * 200, 
		oy - sin( ob.elongationAxis ) * 200
		);

	dc.SelectObject( pTempPen );
	dc.SelectObject( pTempBmp );

	GetDIBits( dc.m_hDC, bitmap, 0, dwHeight, lpDst, (LPBITMAPINFO) lpD,
		DIB_RGB_COLORS );

	END_PROCESSING("Elongation Axis");
}


void CDibView::OnLaborator4ProjectionX()
{
	BEGIN_PROCESSING()
	BinaryObject ob = SELECTED_OBJECT;

	// =================================================================
	// Draw the elongation axis
	// =================================================================

	CDC dc;
	dc.CreateCompatibleDC(0);
	CBitmap bitmap;
	HBITMAP hBitmap = CreateDIBitmap( ::GetDC(0), &((LPBITMAPINFO)lpS)->bmiHeader, CBM_INIT, lpSrc, (LPBITMAPINFO)lpS, DIB_RGB_COLORS );

	bitmap.Attach(hBitmap);

	CBitmap* pTempBmp = dc.SelectObject( &bitmap );
	CPen pen( PS_SOLID, 1, RGB(0, 0, 0 ) );

	CPen* pTempPen = dc.SelectObject(&pen);

	int ox = ob.centerOfMass.x;
	int oy = dwHeight - ob.centerOfMass.y + 1;

	dc.Rectangle(0, 0, dwWidth, dwHeight);

	for( int i = 0; i < ob.projectionsXSize; ++i ){
		dc.MoveTo( i, 0 );
		dc.LineTo( i, ob.projectionsX[i] );

	}

	dc.SelectObject( pTempPen );
	dc.SelectObject( pTempBmp );

	GetDIBits( dc.m_hDC, bitmap, 0, dwHeight, lpDst, (LPBITMAPINFO) lpD,
		DIB_RGB_COLORS );

	END_PROCESSING("Projection X");
}

void CDibView::OnLaborator4ProjectionY()
{
	BEGIN_PROCESSING()

	BinaryObject ob = SELECTED_OBJECT;

	// =================================================================
	// Draw the elongation axis
	// =================================================================

	CDC dc;
	dc.CreateCompatibleDC(0);
	CBitmap bitmap;
	HBITMAP hBitmap = CreateDIBitmap( ::GetDC(0), &((LPBITMAPINFO)lpS)->bmiHeader, CBM_INIT, lpSrc, (LPBITMAPINFO)lpS, DIB_RGB_COLORS );

	bitmap.Attach(hBitmap);

	CBitmap* pTempBmp = dc.SelectObject( &bitmap );
	CPen pen( PS_SOLID, 1, RGB(0, 0, 0 ) );

	CPen* pTempPen = dc.SelectObject(&pen);

	int ox = ob.centerOfMass.x;
	int oy = dwHeight - ob.centerOfMass.y + 1;

	dc.Rectangle(0, 0, dwWidth, dwHeight);

	for( int i = 0; i < ob.projectionsYSize; ++i ){
		dc.MoveTo( 0, i );
		dc.LineTo( ob.projectionsY[i], i );

	}

	dc.SelectObject( pTempPen );
	dc.SelectObject( pTempBmp );

	GetDIBits( dc.m_hDC, bitmap, 0, dwHeight, lpDst, (LPBITMAPINFO) lpD,
		DIB_RGB_COLORS );

	END_PROCESSING("Projection Y");
}

void CDibView::OnLaborator3Dither()
{
	BEGIN_PROCESSING()

	CHistogram hist = *getHistogram(lpSrc, dwWidth, dwHeight );
	vector<int> values = getReducedHistogram( hist );

	for( int i = 0; i < dwHeight; ++i ){
		for( int  j = 0; j < dwWidth; ++j ){
			BYTE color = getPixelColor( lpSrc, w, i, j, 8 );
			setPixelColor( lpDst, w, i, j, getNearestHistogramValue( color, values ), 8 );
		}
	}

	LPBITMAPINFO pBitmapInfoSrc = (LPBITMAPINFO)lpS;

	if( pBitmapInfoSrc->bmiHeader.biBitCount != 8 ){
		// display a message
		return;
	}

	for( int i = 0; i < dwHeight; ++i ){
		for( int  j = 0; j < dwWidth; ++j ){
			BYTE source_color = getPixelColor(lpSrc, w, i, j, 8 );
			setPixelColor( lpDst, w, i, j, source_color, 8 );
		}
	}

	for( int y = dwHeight - 1; y >= 0; --y ){
		for( int  x = 0; x < dwWidth; ++x ){
			float error;
			BYTE old_color = getPixelColor(lpDst, w, y, x, 8 );
			BYTE new_color = getNearestHistogramValue( old_color, values );
			setPixelColor(lpDst, w, y, x, new_color, 8 );
			error = (float)(old_color - new_color);
			error = min( error, 255.0f );
			error = max( error, 0.0f );
			BYTE color;

			if( x < dwWidth-1 ){
				BYTE scale = (BYTE)((7.0f / 16.0f) * error);
				old_color = getPixelColor(lpDst, w, y, x+1, 8 );
				setPixelColor( lpDst, w, y, x+1, old_color + scale, 8 );
			}

			if( y > 0 && x < dwWidth - 1 ){
				color = (BYTE)((float)getPixelColor(lpDst, w, y-1, x+1, 8 ) + ((3.0 / 16.0) * error));
				setPixelColor( lpDst, w, y-1, x+1, color, 8 );
			}

			if( y > 0 ){
				color = (BYTE)((float)getPixelColor(lpDst, w, y-1, x, 8 ) + ((5.0 / 16.0) * error));
				setPixelColor( lpDst, w, y-1, x, color, 8 );
			}

			if( y > 0 && x < dwWidth - 1 ){
				color = (BYTE)((float)getPixelColor(lpDst, w, y-1, x+1, 8 ) + ((1.0 / 16.0) * error));
				setPixelColor( lpDst, w, y-1, x+1, color, 8 );
			}
		}
	}

	//delete hist;
	END_PROCESSING("Dither")
}

int Parent( int a, int* parent ){
	int p = a;
	while( parent[p] != p ){
		p = parent[p];
	}
	return p;
}

void NewPair( int a, int b, int *parent ){
	parent[Parent(b, parent)] = Parent(a, parent);
}

int NewLabel(){
	static int label = 1;
	return label++;
}

void MarkObjects( const Image& src, Image& dst ){
	int* pixelLabels = new int[src.GetWidth() * src.GetHeight()];
	memset( pixelLabels, 0, src.GetWidth() * src.GetHeight() * sizeof( int ) );
	int w = src.GetWidth();

	int new_label = 1;

	int* labelTree = new int[256];
	for( int i = 0; i < 256; ++i ){
		labelTree[i] = i;
	}

	for( int i = src.GetHeight() - 2; i >= 0; --i ){
		for( int j = 1; j < src.GetWidth() - 1; ++j ){

			if( src.GetLutIndex( i, j ) == 0 ){
				int X = src.GetLutIndex( i, j );
				int A = src.GetLutIndex( i, j - 1 );
				int B = src.GetLutIndex( i + 1, j - 1 );
				int C = src.GetLutIndex( i + 1, j );
				int D = src.GetLutIndex( i + 1, j + 1 );
				int X_Label = Parent( pixelLabels[i * w + j], labelTree );
				int A_Label = Parent( pixelLabels[i * w + j - 1], labelTree );
				int B_Label = Parent( pixelLabels[(i+1) * w + j-1], labelTree );
				int C_Label = Parent( pixelLabels[(i+1) * w + j], labelTree );
				int D_Label = Parent( pixelLabels[(i+1) * w + j+1], labelTree );

				// 1, 2
				if( B == 0 ){
					X_Label = B_Label;

					if( C != 0 && D == 0 )
					{
						NewPair( B_Label, D_Label, labelTree );
					}
					pixelLabels[i * w + j] = X_Label;
					continue;
				}

				// 3, 4, 5
				if( A == 0 ){
					X_Label = A_Label;

					if( C == 0 ){
						NewPair( A_Label, C_Label, labelTree );
					} else {
						if( D == 0 ){
							NewPair( A_Label, D_Label, labelTree );
						}
					}
					pixelLabels[i * w + j] = X_Label;
					continue;
				}

				// 6
				if( C == 0 ){
					X_Label = C_Label;
					pixelLabels[i * w + j] = X_Label;
					continue;
				}

				// 7
				if( D == 0 ){
					X_Label = D_Label;
					pixelLabels[i * w + j] = X_Label;
					continue;
				}

				// 8
				X_Label = new_label++;
				pixelLabels[i * w + j] = X_Label;
			}
		}
	}

	for( int i = 0; i < src.GetHeight(); ++i ){
		for( int j = 0; j < src.GetWidth(); ++j ){
			int index = i * w + j;
			int value = Parent( pixelLabels[i * w + j], labelTree );
			bool cond = src.GetLutIndex( i, j ) == 0;
			if( cond ){
				dst.SetPixelLUTIndex( i, j, value );
			}
		}
	}

	// Add some pretty colors to the destination LUT
	srand (time(NULL));
	for( int i = 1; i < 254; ++i ){
		Color c( rand() % 255, rand() % 255, rand() % 255 );
		dst.SetLUTColor( i, c );
	}
}

void DrawContour( Image& img, Contour& contour, int colorLutIndex = 0 ){
	Vector2 p;
	while( contour.HasNext() ){
		p = contour.NextPosition();
		img.SetPixelLUTIndex(p.y, p.x, colorLutIndex);
	}
}

Contour ReadContour( string filename ){
	Contour c;
	ifstream fin( filename );
	int x, y, n, d;

	fin >> x >> y;
	c.SetStartPosition( Vector2( x, y ) );

	fin >> n;

	for( int i = 0; i < n; ++i ){
		fin >> d;
		c.AddDirection( d );
	}

	return c;
}

Contour ConstructContour( const Image& img ){
	Contour c;

	// Find the starting pixel
	int startx = -1, starty = -1;
	for( int x = 0; x < img.GetHeight() && startx == -1; ++x ){
		for( int y = 0; y < img.GetWidth() && starty == -1; ++y ){
			if( img.GetLutIndex(x, y) == 0 ){
				startx = x;
				starty = y;
			}
		}
	}

	c.SetStartPosition( Vector2( startx, starty ) );

	int count = 0, direction = 7;
	int cx = startx, cy = starty;
	while( cx != startx || cy != starty || count == 0 ){
		for( int i = 7; i >= 0; --i ){
			if( img.GetLutIndex( cx + c.GetDelta(i).x, cy + c.GetDelta(i).y ) == 0 ){
				c.AddDirection( i );
				break;
			}
			cx += c.GetDelta(i).x;
			cy += c.GetDelta(i).y;
		}
		++count;
	}

	return c;
}

void CDibView::OnLaborator5MarkObjects(){
	BEGIN_PROCESSING();

	CONSTRUCT_SOURCE_IMAGE( imgSrc );
	CONSTRUCT_DESTINATION_IMAGE( imgDst );

	MarkObjects( imgSrc, imgDst );

	END_PROCESSING("Mark Objects");
}

void CDibView::OnLaborator6DrawExternalContour(){
	BEGIN_SOURCE_PROCESSING;

	CONSTRUCT_SOURCE_IMAGE( imgSrc );
	// Open File
	string filename = "c:/Users/Alex/Dropbox/Universitate/Anul3/Sem_2_Procesare_Imagini/Imagini/border_tracing/reconstruct.txt";

	Contour contour = ReadContour( filename );

	DrawContour( imgSrc, contour );

	END_SOURCE_PROCESSING;
}

void CDibView::OnLaborator6DrawImageContour(){
	BEGIN_PROCESSING();

	CONSTRUCT_SOURCE_IMAGE( imgSrc );
	CONSTRUCT_DESTINATION_IMAGE( imgDst );

	Contour c = ConstructContour( imgSrc );
	imgDst.Clear();
	DrawContour( imgDst, c );

	END_PROCESSING("Trace Contour");
}
