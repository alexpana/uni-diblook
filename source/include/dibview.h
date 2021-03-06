// dibview.h : interface of the CDibView class
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

class CDibView : public CScrollView
{
protected: // create from serialization only
	CDibView();
	DECLARE_DYNCREATE(CDibView)

// Attributes
public:
	CDibDoc* GetDocument()
		{
			ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDibDoc)));
			return (CDibDoc*) m_pDocument;
		}

// Operations
public:

// Implementation
public:
	virtual ~CDibView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView);

	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

	// Printing support
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CDibView)
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);  // user message
	afx_msg void OnProcessingParcurgereSimpla();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLaborator1Cropcircle();
	afx_msg void OnLaborator2Afisareinfo();
	afx_msg void OnLaborator2Grayscale2();
	afx_msg void OnLaborator2Blackwhite();
	afx_msg void OnLaborator3Histograma();
	afx_msg void OnLaborator3Dither();
	afx_msg void OnLaborator3Reduce();
	afx_msg void OnLaborator4BinaryObjectsInformation();
	afx_msg void OnLaborator4ProjectionX();
	afx_msg void OnLaborator4ProjectionY();
	afx_msg void OnLaborator5MarkObjects();
	afx_msg void OnLaborator6DrawContourFromExternalDirections();
	afx_msg void OnLaborator6DrawContourFromExternalDerivative();
	afx_msg void OnLaborator6DrawImageContour();
	afx_msg void OnLaborator6ExportContour();
	afx_msg void OnLaborator7();
	afx_msg void OnLaborator8Statistics();
	afx_msg void OnLaborator8Transform();
	afx_msg void OnLaborator8Normalize();
	afx_msg void OnLaborator9Convolution();
	afx_msg void OnLaborator11Median();
	afx_msg void OnLaborator11Gaussian();
	afx_msg void OnLaborator11BiGaussian();
	afx_msg void OnLaborator12EdgeDetect();
	afx_msg void OnLaborator12CannyPart1();
};

/////////////////////////////////////////////////////////////////////////////

