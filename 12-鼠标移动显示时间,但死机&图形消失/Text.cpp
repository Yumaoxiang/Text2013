// Text.cpp: implementation of the Text class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test1.h"
#include "Text.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
unsigned char TextData[2000]={0}; //���ÿ����Ҫ��ʾ��2000������,ע��,����һ��ȫ�ֱ���,������ע��

Text::Text()
{
    LineNum=0;
	TotalLineNum=0;
	Calendar[0]=0;
	Calendar[1]=31;Calendar[2]=28;Calendar[3]=31;Calendar[4]=30;
	Calendar[5]=31;Calendar[6]=30;Calendar[7]=31;Calendar[8]=31;
	Calendar[9]=30;Calendar[10]=31;Calendar[11]=30;Calendar[12]=31;
	ArrayDataCnt=0;//�����¼�±�
}

Text::~Text()
{

}
