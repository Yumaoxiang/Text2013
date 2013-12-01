// Test1View.cpp : implementation of the CTest1View class
//

#include "stdafx.h"
#include "Test1.h"

#include "Test1Doc.h"
#include "Test1View.h"
#include "Text.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTest1View

IMPLEMENT_DYNCREATE(CTest1View, CScrollView)

BEGIN_MESSAGE_MAP(CTest1View, CScrollView)
	//{{AFX_MSG_MAP(CTest1View)
	ON_WM_HSCROLL()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest1View construction/destruction

CTest1View::CTest1View()
{
	// TODO: add construction code here
    m_ptOrigin=0;
	m_ptScroll=0;
}

CTest1View::~CTest1View()
{
}

BOOL CTest1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

extern unsigned char TextData[2000];  //唯一的全局变量
/////////////////////////////////////////////////////////////////////////////
// CTest1View drawing

void CTest1View::OnDraw(CDC* pDC)
{
	CTest1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
    CString s;
	s.Format("Origin Test");   //产生窗口将要输出的文字,此处CString.Format很好
	pDC->TextOut(0,50,s);
    
    CClientDC dc(this);
	m_ptScroll=GetScrollPosition();
	//dc.MoveTo(0,m_ptScroll.x/37500);
	//dc.LineTo(m_ptScroll.x+1200,(m_ptScroll.x+1200)/37500);

	unsigned char Table[2000];
	for(int x=0;x<2000;x++){
	    Table[x]=x%255;      
	}
	dc.MoveTo(0,Table[0]-m_ptScroll.y);
	for(x=0;x<1000;x++){
    	dc.LineTo(x,Table[x]*3-m_ptScroll.y);
	}

	dc.MoveTo(0,TextData[0]-m_ptScroll.y);
	for(x=0;x<2000;x++){
    	dc.LineTo(x,TextData[x]-m_ptScroll.y);
	}

	
	//dc.LineTo(500,600);
	//dc.LineTo(600,600);
}

void CTest1View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 2594000;   //60*60*24*30+2000=2594000
	sizeTotal.cy = 5000;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CTest1View printing

BOOL CTest1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTest1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTest1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTest1View diagnostics

#ifdef _DEBUG
void CTest1View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTest1View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTest1Doc* CTest1View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest1Doc)));
	return (CTest1Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

void CTest1View::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    
	if (nSBCode == SB_THUMBTRACK) 
    { 
        SCROLLINFO lpScrollInfo; 
        GetScrollInfo(SB_HORZ, &lpScrollInfo); 
        nPos = lpScrollInfo.nTrackPos; 
    } 
	
	/****************添加.txt文件处理函数*****************/
	Text txt;
	txt.ArrayDataCnt = 0;             //每次提取数据时需要确定该数值清0
	for(int i=0;i<2000;i++)           //数组值也要清0,否则未读取的数据会出现错误
		TextData[i]=0;
	m_ptScroll=GetScrollPosition();
	//txt.ScrollData(FileGlobal,345678);//345678=4天1分18秒
    txt.ScrollData(FileGlobal,m_ptScroll.x);


	Invalidate(TRUE);
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTest1View::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog *lpOpenFile;
	CString sFileFilter;
	CString m_GetFilePath;

	sFileFilter="Data File(*.txt)|*.txt|All Files(*.*)|*.*||";
	lpOpenFile=new CFileDialog(TRUE,"","",OFN_FILEMUSTEXIST,sFileFilter,NULL);
    
	if(lpOpenFile->DoModal()==IDOK){	
	    m_ptrarray.RemoveAll();     //清除m_ptrarray中数据,此处为第2次打开时覆盖前一次数据/*注意此处m_ptrarray(CPtrArray类型)可能读不了4M大的数据*/
	
	    m_GetFilePath=lpOpenFile->GetFileName(); //获取该文件路径,下面打开调用
		//CStdioFile File;
		FileGlobal.Open(m_GetFilePath,CFile::modeRead); //打开文件方式,下面开始处理

		Text txt1;
		Text txt2;
		txt1.TotalLineNum=txt1.LineFind(FileGlobal);    //该句不能删除,后面ScrollData中会调用
		txt2.TotalLineNum=txt2.LineFind(FileGlobal);
		
		//		Text::TotalLineNum=txt1.LineFind(FileGlobal);
		//char Check=0;

		//txt.DataPick(File,3,5);
		//txt.ScrollData(FileGlobal,0);
        /*CString FileTemp;     //验证定位,存储字符串
		for(int k=0;k<3;k++){
		  File.ReadString(FileTemp);
		  File.Seek(0,CFile::begin);
		}*/
        
		
		/*Check=txt.HeadFind(File);
        if(!Check)
			MessageBox("未找到头文件位置");*/

		//Check=txt.DataCheck();

		//int temp=txt.LineFind(FileGlobal);


	}
	delete lpOpenFile;
}


/*********************************************************************
功    能：用于调试


*********************************************************************/
void CTest1View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Text txt;
	for(int x=0;x<2000;x++){
	    TextData[x]=x%600+100;      //数值100到700之间
	}
	/*CString S1("1234 is a test");
	int j=S1.GetLength();
	char *ptrStr=S1.GetBuffer(S1.GetLength()); //因为不能保证CString读取数据末尾一定为0,所以要调用ReleaseBuffer( );
	char a=*(ptrStr+13);
    if(*(ptrStr-1)=='t'){
	   int i=0;
	}
    S1.ReleaseBuffer();*/

	CScrollView::OnLButtonDown(nFlags, point);
}

void CTest1View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString str1,str2;
    CPoint pt;
	RECT rect;
	rect.left=0;rect.top=0;rect.right=200;rect.bottom=30;
	InvalidateRect(&rect);
	UpdateWindow();
    str1.Format("当前窗口坐标：x=%d,y=%d",point.x,point.y);
    CDC *pDC=GetDC();
    //pDC->SetBkColor(RGB(128,128,128));
    pDC->TextOut(0,0,str1);

    GetCursorPos(&pt);
	str2.Format("全屏坐标: x=%d,y=%d",pt.x,pt.y);
	pDC->TextOut(0,20,str2);

	CScrollView::OnMouseMove(nFlags, point);
}

void CTest1View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(TRUE);
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}
