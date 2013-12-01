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

extern unsigned char TextData[2000];  //ȫ�ֱ���

/////////////////////////////////////////////////////////////////////////////
// CTest1View drawing
#define CorLeftSpace    50            /*�����߾�����߱߿����*/
#define CorUpSpace      100           /*�����߾����ϱ߱߿����*/
#define CorDownPos      700           /*�����ߵ��±�λ��,��Ϊ�Ƚϴ�,������Ҫ����������*/
#define CorRightPos     2000          /*�����ߵ��ұ�λ��,��Ϊ�Ƚϴ�,����Ԥʾ���滹������*/

void CTest1View::OnDraw(CDC* pDC)
{
	CTest1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
    
    
    CClientDC dc(this);
	m_ptScroll=GetScrollPosition();   //��ȡ��ǰ����λ��

/******************************1.0�������ܹ�******************************/
    CPen pen,PenDash,PenCoordi;
	
	//��L�ܹ�
	pen.CreatePen(PS_SOLID,3,RGB(0,0,0));               //����һ��L������ߵĻ���
	CPen *OldPen=dc.SelectObject(&pen);                 //ѡ��û���
	dc.MoveTo(CorLeftSpace,CorUpSpace-m_ptScroll.y);    //������»�������Ͻ�λ��
	dc.LineTo(CorLeftSpace,CorDownPos-m_ptScroll.y);    //������»��컭һ������
	dc.MoveTo(CorLeftSpace,CorDownPos-m_ptScroll.y);    //������»�������½�λ��
	dc.LineTo(CorRightPos,CorDownPos-m_ptScroll.y);     //������»��컭һ������

	//���̶̿���
	PenCoordi.CreatePen(PS_SOLID,3,RGB(0x27,0x40,0x8B));      //������ɫ����,׼��������
	OldPen=dc.SelectObject(&PenCoordi);
	for(int grid=0;grid<10;grid++){
		dc.MoveTo(CorLeftSpace,640-(grid*60)-m_ptScroll.y);   //��60Ϊ1A,ÿ50mA��3�����ʾ������
		dc.LineTo(CorLeftSpace+5,640-(grid*60)-m_ptScroll.y); //����TextData����һ��Ҫת����50mA��������
	}

	//������
	PenDash.CreatePen(PS_DOT,1,RGB(0,0,255));
    OldPen=dc.SelectObject(&PenDash);
	for(grid=0;grid<10;grid++){
	    dc.MoveTo(CorLeftSpace,640-(grid*60)-m_ptScroll.y);   //���� 50,700Ϊ��ʼ��
	    dc.LineTo(CorRightPos,640-(grid*60)-m_ptScroll.y);    //���� 2000,700Ϊ�յ�
	}

	pen.DeleteObject();

	//������
	CString s;

	char ScaleTable[3];                        //ʹ������洢����ʾ���ַ�
	ScaleTable[1]='A';ScaleTable[2]='\0';      //�����2��&��3���洢�̶�������,'\0'Ϊֹͣ��,������������
	char *pT=ScaleTable;                       //��ָ��Ϊ����s.Format(pT);����,��s.Format�贫��ָ��
	for(grid=0;grid<10;grid++){				   //��0��9�̶�,��Ϊ10A��Ҫ����,����������,����2�ַ�ʽ�Ͷ��õ���
		ScaleTable[0]=(0x30+grid);             //ת���ɿ���ʶ�������
	    s.Format(pT);                          //ת�����ַ���
        pDC->TextOut(10,690-grid*60,s);        //ָʾλ�ý�����ʾ
	}
	s.Format("10A");     pDC->TextOut(10,90,s);//�������ڽ�Ҫ���������,�˴�CString.Format�ܺ�
/******************************1.1����������******************************/

/******************************2.0�������ݻ���******************************/

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
	
	/****************���.txt�ļ�������*****************/
	Text txt;
	txt.ArrayDataCnt = 0;             //ÿ����ȡ����ʱ��Ҫȷ������ֵ��0
	for(int i=0;i<2000;i++)           //����ֵҲҪ��0,����δ��ȡ�����ݻ���ִ���
		TextData[i]=0;
	m_ptScroll=GetScrollPosition();
	//txt.ScrollData(FileGlobal,345678);//345678=4��1��18��
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
	    m_ptrarray.RemoveAll();     //���m_ptrarray������,�˴�Ϊ��2�δ�ʱ����ǰһ������/*ע��˴�m_ptrarray(CPtrArray����)���ܶ�����4M�������*/
	
	    m_GetFilePath=lpOpenFile->GetFileName(); //��ȡ���ļ�·��,����򿪵���
		//CStdioFile File;
		FileGlobal.Open(m_GetFilePath,CFile::modeRead); //���ļ���ʽ,���濪ʼ����

		Text txt1;
		Text txt2;
		txt1.TotalLineNum=txt1.LineFind(FileGlobal);    //�þ䲻��ɾ��,����ScrollData�л����
		txt2.TotalLineNum=txt2.LineFind(FileGlobal);
		
		txt1.StartLineFind(FileGlobal,StartTime);     //�洢StartTime,�����ʾʱ�����


	}
	delete lpOpenFile;
}


/*********************************************************************
��    �ܣ����ڵ���


*********************************************************************/
void CTest1View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Text txt;
	for(int x=0;x<2000;x++){
	    TextData[x]=x%600+100;      //��ֵ100��700֮��
	}
	/*CString S1("1234 is a test");
	int j=S1.GetLength();
	char *ptrStr=S1.GetBuffer(S1.GetLength()); //��Ϊ���ܱ�֤CString��ȡ����ĩβһ��Ϊ0,����Ҫ����ReleaseBuffer( );
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
    unsigned char CurScrTime[6]={0};//��¼��ǰ����ʱ��
    unsigned char CurCurTime[6]={0};//��¼��ǰ���λ��
	//1.���ļ�ʱ�Ѿ���¼������ʱ���
	m_ptScroll=GetScrollPosition();      //2.��¼��ǰ����λ��,׼��ת��Ϊ���ʱ���
	TxtMouse.ScrollTime(m_ptScroll.x,CurScrTime);//3.1������λ��ת����ʱ��
    TxtMouse.RelativeScrollTime(StartTime,CurScrTime);//3.2��ȡ��ǰʱ��
    
    if(point.x>=CorLeftSpace){     //4.��ʱ�������Ҫ��,������ʾ,Y�������������λ��
		point.x=point.x-CorLeftSpace;//4.1ת��λ����������0��λ��ƫ��
	    TxtMouse.ScrollTime(point.x,CurCurTime);//4.2�����λ��ת����ʱ��,������,������Ҫ������
		TxtMouse.RelativeScrollTime(CurScrTime,CurCurTime);//4.3�ó��������ʱ��
	}

    str1.Format("��ǰʱ��: %d-%d-%d %d:%d:%d",CurCurTime[0],CurCurTime[1],CurCurTime[2],CurCurTime[3],CurCurTime[4],CurCurTime[5]); //5.��ʾ

	//�����������,ͼ����ʧ,����ü���ˣ�����2013-11-28 

    CDC *pDC=GetDC();
    //pDC->SetBkColor(RGB(128,128,128));
    pDC->TextOut(0,0,str1);

    GetCursorPos(&pt);
	str2.Format("ȫ������: x=%d,y=%d",pt.x,pt.y);
	pDC->TextOut(0,20,str2);

	CScrollView::OnMouseMove(nFlags, point);
}

void CTest1View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(TRUE);
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}
