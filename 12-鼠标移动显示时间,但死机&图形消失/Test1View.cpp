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

extern unsigned char TextData[2000];  //全局变量

/////////////////////////////////////////////////////////////////////////////
// CTest1View drawing
#define CorLeftSpace    50            /*坐标线距离左边边框距离*/
#define CorUpSpace      100           /*坐标线距离上边边框距离*/
#define CorDownPos      700           /*坐标线到下边位置,因为比较大,所以需要利用下拉条*/
#define CorRightPos     2000          /*坐标线到右边位置,因为比较大,所以预示后面还有数据*/

void CTest1View::OnDraw(CDC* pDC)
{
	CTest1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
    
    
    CClientDC dc(this);
	m_ptScroll=GetScrollPosition();   //获取当前滑块位置

/******************************1.0添加坐标架构******************************/
    CPen pen,PenDash,PenCoordi;
	
	//画L架构
	pen.CreatePen(PS_SOLID,3,RGB(0,0,0));               //创建一个L坐标粗线的画笔
	CPen *OldPen=dc.SelectObject(&pen);                 //选择该画笔
	dc.MoveTo(CorLeftSpace,CorUpSpace-m_ptScroll.y);    //相对于下滑快的左上角位置
	dc.LineTo(CorLeftSpace,CorDownPos-m_ptScroll.y);    //相对于下滑快画一条竖线
	dc.MoveTo(CorLeftSpace,CorDownPos-m_ptScroll.y);    //相对于下滑快的右下角位置
	dc.LineTo(CorRightPos,CorDownPos-m_ptScroll.y);     //相对于下滑快画一条横线

	//画短刻度线
	PenCoordi.CreatePen(PS_SOLID,3,RGB(0x27,0x40,0x8B));      //创建蓝色虚线,准备画网格
	OldPen=dc.SelectObject(&PenCoordi);
	for(int grid=0;grid<10;grid++){
		dc.MoveTo(CorLeftSpace,640-(grid*60)-m_ptScroll.y);   //以60为1A,每50mA用3个点表示来画线
		dc.LineTo(CorLeftSpace+5,640-(grid*60)-m_ptScroll.y); //所以TextData数组一定要转换成50mA的整数倍
	}

	//画网格
	PenDash.CreatePen(PS_DOT,1,RGB(0,0,255));
    OldPen=dc.SelectObject(&PenDash);
	for(grid=0;grid<10;grid++){
	    dc.MoveTo(CorLeftSpace,640-(grid*60)-m_ptScroll.y);   //横线 50,700为起始点
	    dc.LineTo(CorRightPos,640-(grid*60)-m_ptScroll.y);    //横线 2000,700为终点
	}

	pen.DeleteObject();

	//标数字
	CString s;

	char ScaleTable[3];                        //使用数组存储带显示的字符
	ScaleTable[1]='A';ScaleTable[2]='\0';      //数组第2个&第3个存储固定的数字,'\0'为停止符,否则会出现乱码
	char *pT=ScaleTable;                       //该指针为后续s.Format(pT);调用,因s.Format需传入指针
	for(grid=0;grid<10;grid++){				   //画0—9刻度,因为10A还要计算,后续单独画,这样2种方式就都用到了
		ScaleTable[0]=(0x30+grid);             //转换成可以识别的数组
	    s.Format(pT);                          //转换成字符串
        pDC->TextOut(10,690-grid*60,s);        //指示位置进行显示
	}
	s.Format("10A");     pDC->TextOut(10,90,s);//产生窗口将要输出的文字,此处CString.Format很好
/******************************1.1添加坐标结束******************************/

/******************************2.0根据数据画线******************************/

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
		
		txt1.StartLineFind(FileGlobal,StartTime);     //存储StartTime,鼠标显示时间调用


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


	Text TxtMouse;
    unsigned char CurScrTime[6]={0};//记录当前滑块时间
    unsigned char CurCurTime[6]={0};//记录当前鼠标位置
	//1.打开文件时已经记录了最早时间点
	m_ptScroll=GetScrollPosition();      //2.记录当前滑块位置,准备转化为相对时间点
	TxtMouse.ScrollTime(m_ptScroll.x,CurScrTime);//3.1将滑块位置转化成时间
    TxtMouse.RelativeScrollTime(StartTime,CurScrTime);//3.2获取当前时间
    
    if(point.x>=CorLeftSpace){     //4.若时间点满足要求,进行显示,Y轴后续需计算相对位置
		point.x=point.x-CorLeftSpace;//4.1转换位相对于坐标的0点位置偏移
	    TxtMouse.ScrollTime(point.x,CurCurTime);//4.2将鼠标位置转换成时间,很明显,函数需要改名了
		TxtMouse.RelativeScrollTime(CurScrTime,CurCurTime);//4.3得出现在鼠标时间
	}

    str1.Format("当前时间: %d-%d-%d %d:%d:%d",CurCurTime[0],CurCurTime[1],CurCurTime[2],CurCurTime[3],CurCurTime[4],CurCurTime[5]); //5.显示

	//程序出现死机,图形消失,明天该检查了！！！2013-11-28 

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
