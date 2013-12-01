// Test1View.h : interface of the CTest1View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST1VIEW_H__D9C9C781_0509_446D_A412_620488BD8AC3__INCLUDED_)
#define AFX_TEST1VIEW_H__D9C9C781_0509_446D_A412_620488BD8AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTest1View : public CScrollView
{
protected: // create from serialization only
	CTest1View();
	DECLARE_DYNCREATE(CTest1View)

// Attributes
public:
	CTest1Doc* GetDocument();
    CStdioFile FileGlobal;   //
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest1View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CPoint m_ptOrigin;
	CPoint m_ptScroll;
	virtual ~CTest1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	CPtrArray m_ptrarray;
// Generated message map functions
protected:
	//{{AFX_MSG(CTest1View)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFileOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Test1View.cpp
inline CTest1Doc* CTest1View::GetDocument()
   { return (CTest1Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST1VIEW_H__D9C9C781_0509_446D_A412_620488BD8AC3__INCLUDED_)
