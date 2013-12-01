// Text.h: interface for the Text class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "Test1View.h"

#if !defined(AFX_TEXT_H__E00CAAC1_FF56_48F5_BDDA_09475A34BA22__INCLUDED_)
#define AFX_TEXT_H__E00CAAC1_FF56_48F5_BDDA_09475A34BA22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TextLine 720
//unsigned char TD1[5]={0};
extern unsigned char TextData[2000];
class Text  
{
public:
	Text();
	virtual ~Text();

public:
	CString FileData;                 //�洢��ǰһ�е�����
    char *ptrStr;                     //ָ��ǰһ��(1Сʱ)�ַ�����ָ��
    unsigned LineNum;
	unsigned TotalLineNum;
    unsigned char Calendar[13];
    
	/*����1-��ȡ�ļ������Ĵ���-����֤*/
    int LineFind(CStdioFile &file){
		int linenum=0;
		file.Seek(0,CFile::begin);
	    //while(file.ReadString(FileData)){   //��ʹ�ø�����ԭ��Ϊ���ļ������һ�����ݣ�������128�ı������ַ���ʱ����ReadString��ȡ��һ���᷵��FALSE����ʵ���϶�ȡ�ǳɹ��ģ����ص�CString�е���������ȷ�ģ���VC6.0�д������Bug��VS2005�У�û�����Bug��
		while (file.ReadString(FileData)!=FALSE || FileData.GetLength()!=0){
			linenum++; 
		}
		return linenum;
	}

	/*�ú���δ�ɹ�ʹ��,��Ϊline��ƫ��byte������������...*/
	int LinePos(CStdioFile &file,unsigned int line){
	    file.Seek(file,line);
	}

	//file-->��ǰ�ı�(4M) ,�ú�����Ҫ�޸ģ�i<TextLineӦ�ø�ΪLineFindģʽ������ʼλ��
	char HeadFind(CStdioFile &file){         
		                              //ָ��ǰһ�����ݵ�ָ��,�����ַ�Ѱ��
		for(int i=0;i<TextLine;i++){  //����720������
		    file.ReadString(FileData);//��ȡһ��,��ȡ��ɺ��Զ�ת����һ����ʼλ��
			ptrStr=FileData.GetBuffer(FileData.GetLength());   //ʹ��ָ��,׼���ַ�����
			if(*ptrStr=='B'){         //����ҵ���ʼλ��
				FileData.ReleaseBuffer();
				return 1;             //������
			}
		}
		FileData.ReleaseBuffer();     
		return 0;                     //���δ�ҵ���ʼλ��,���ش����־0
	}

	//��ȡһ������ʱ�亯�� 
	//strTime-->�����ַ��� pTime-->ָ��洢ʱ�������
	//����ֵ0-->ʧ��,�˴����д�����ʾ,Ҳ���ڵ������ĺ����м���,�������Ѻ�  1-->�ɹ� 
	unsigned int TimePick(CString StrTime,unsigned char *pTime){//��������(0��200)
		unsigned char pCnt=0,dCnt=0; //��ȡʱ���ַ�λ�ü���
		unsigned char Flag=0;        //������ȡ����
		unsigned int  TmpData=0;     //��ʱ��Ž���������
		char *ptrStr=StrTime.GetBuffer(StrTime.GetLength()); //ָ������ַ���ָ��

        if(*ptrStr=='\0'){           //��������ַ���û���κ�����
		    AfxMessageBox("ʱ����ȡ�ַ���Ϊ��,��������");
			return 0;            
		}

		while(*(ptrStr+pCnt)!='D'){  //����������ʼλ֮ǰ������
			if(pCnt>25){             //�������25���ַ�����û���ҵ�������ʼλ
			   AfxMessageBox("ʱ����ȡ�ַ���ʱ��λ����,��������"); 
			   return 0;
			}
			if((*(ptrStr+pCnt)>='0')&&(*(ptrStr+pCnt)<='9')){  //���Ϊ����
				if((*(ptrStr+pCnt+1)==' ')||(*(ptrStr+pCnt+1)=='\0')) //�����һ���ַ����������ַ�,�����˴�������ȡ��������
					Flag=1;           //�ñ�־λ,׼���洢

				TmpData*=10;
                TmpData+=*(ptrStr+pCnt)-'0';               //��ȡ������
			}	

			if(Flag){                 //����˴�������ȡ���
				Flag=0;
				if((TmpData<=59)){    //�����ȡ����������ֵҪ��,ע��˴�û��60,SD��д����ע��
					*(pTime+dCnt++)=(unsigned char)TmpData;  //�洢������
					TmpData=0;
				}
				else{                 //����Ϊ�洢���ݹ���,�����������ݴ洢
					AfxMessageBox("ʱ����ȡ��ֵ����,��������"); //�õ��������ڵ���,ʵ��Ӧ��ɾ��
				    return 0;
				}
			}
			pCnt++;							            //׼����ȡ��һ���ַ�
		}  		
		return 1;                    
	}

    char* BeginFind(CStdioFile &file){
	    CString Str;
		char *ptrStr;
		//while(file.ReadString(FileData)){   //��ʹ�ø�����ԭ��Ϊ���ļ������һ�����ݣ�������128�ı������ַ���ʱ����ReadString��ȡ��һ���᷵��FALSE����ʵ���϶�ȡ�ǳɹ��ģ����ص�CString�е���������ȷ�ģ���VC6.0�д������Bug��VS2005�У�û�����Bug��
		while (file.ReadString(FileData)!=FALSE || FileData.GetLength()!=0){
			if(*ptrStr=='B')
				return ptrStr;
		}
		return 0;
	}

	//������λ��ת����ʱ��
	void ScrollTime(unsigned int Num,unsigned char *pTime){   //Y M D H M S,û�г�ʼ��,������˵){
		*(pTime+5)=Num%60;    //ȡ����ǰ��
		Num=Num/60;			  //׼����ȡ��
        *(pTime+4)=Num%60;    //ȡ����ǰ��
		Num=Num/60;			  //׼����ȡʱ
		*(pTime+3)=Num%24;    //ȡ����ǰʱ
		*(pTime+2)=Num/24;    //�����ʣ����
	}

	unsigned char TimeCompare(unsigned char *p1,unsigned char *p2){
		for(int i=0;i<6;i++){
		    if(*(p1+i)>*(p2+i))       
				return 1;
			else if(*(p1+i)<*(p2+i))
				return 0;
		}
	    return 1; //������ʱ���붼ƥ�������δ����,˵������ʱ��&�����һ��ʱ�����
	}

	//һСʱ�����ݶԱ� ����ֵ 0-->��һСʱ������  1-->һСʱ�����ݶ�Ӧ��������
	//P1-->����ʱ��     P2-->��ǰһ��ʱ��
	//����2���ַ�����ָ��,���Ϸ���ֵ,ָ�벻��,������ָ���������
	int Time1HourCheck(unsigned char *pt1,unsigned char *pt2){
		unsigned char p1[6]={0};              //����2����ʱ����,�����޸�CurTime��ֵ
		unsigned char p2[6]={0};
		int TimeSubNum=0;                     //����ʱ����ʱ����Ӧ��Second����
		
		for(int i=0;i<6;i++){
		    p1[i]=*(pt1+i);
			p2[i]=*(pt2+i);
		}
        /*�����ʱ����ܴ����С,����ָ�����˴�,�����ڼ���д16:41*/
	    if(*p1>*p2)                           //���������>��ǰһ����
			*(p1+1)=*(p1+1)+12;               //�����·�+12,Ϊ�˼��㷽��
		if(*(p1+1)-*(p2+1)>1)                 //����·ݴ���2����,����������������ܷ���
			return -1;                        //��һСʱ������
		if(*(p1+1)-*(p2+1)==1)                //���������ڵ�ǰ��һ����ʱ��
			*(p1+2)=*(p1+2)+Calendar[*(p2+1)];//��Ϊ����ʱ�����,���Լ�����һ���µ�����
		if(*(p1+2)-*(p2+2)>1)				  //�����ǰ��������>��ǰ�ַ�������1������
			return -1;						  //��һСʱ������
		if(*(p1+2)-*(p2+2)==1)                //���������ڵ�ǰ��һ��ʱ��
			*(p1+3)=*(p1+3)+24;               //����Сʱ+24,Ϊ�˼���
        if(*(p1+3)-*(p2+3)>1)                 //���ʱ��Աȳ���1Сʱ
			return -1;						  //��һСʱ������
		//ע�⣬�������ʱ�����ж�һ����P1>P2
		if(*(p1+3)-*(p2+3)==1)                //���������ڵ�ǰ��һСʱ
			*(p1+4)=*(p1+4)+60;               //�Է���Ϊ��λ,����ʱ���
		TimeSubNum=*(p1+4)*60+*(p1+5);        //��ǰ��ʱ�任���Second����
		TimeSubNum=TimeSubNum-(*(p2+4)*60+*(p2+5));//��ȥ��һ��ʱ��
		return TimeSubNum;  
	}



	//��ȡ��ǰ���ֹ���λ��Ӧ�ö�ȡ��2000������
	//ScrollNum-->���ֵ�λ��0��2592000
	//OnDraw������m_ptScroll=GetScrollPosition();�Ѿ���û��ֵ�λ��,�ʵ��øñ�������
	unsigned int ScrollData(CStdioFile &file,unsigned int ScrollNum){
        CString Str1; 
		unsigned char TimeLine1[6]={0};  //�洢�ļ���һ��ʱ��
		unsigned char TimeLineB[6]={0};  //�洢�ļ�Begin��ʱ��-�����ʱ��
		unsigned char TimeLine0[6]={0};  //�洢Begin��һ��ʱ��-��Զ��ʱ��-��������ʼʱ��
		unsigned int  LineNum=0;         //���㵱ǰLineλ��,�����ж��Ƿ�Begin�ڵ�һ��
		unsigned char FirstLineFlag=0;   //����Begin�ж��ļ��Ƿ��ڵ�һ��,��������ͬ����

		/*������ݲ�����һ��Ҫ��,������ʾ��Ϣ,��Ϊ��������*/
		//��¼��һ��λ��ʱ��
		file.Seek(0,CFile::begin);       //�˴�һ��Ҫһ�����ļ�ͷλ��,�������������������������λ�ö�ȡ����
		file.ReadString(Str1);           //�ļ���һ������,��¼ʱ�䲢�ж��Ƿ�Ҳ��Beginλ��
        char *ptrStr=Str1.GetBuffer(FileData.GetLength());   //ʹ��ָ��ָ������ַ���,׼�����������ַ�
		if((*ptrStr=='B')||(*(ptrStr+1)=='B')){              //�����һ��Ҳ��Beginλ��
		    FirstLineFlag=1;             //����˳�����
		}
	    TimePick(ptrStr,TimeLine1);      //��һ������ʱ��洢��TimeLine1������,�����Ѱ��Begin������
        
	    //����Beginλ��,����¼��λ��ʱ�������
		unsigned int BeginLineFlag=2;
		if(!FirstLineFlag){              //���Begin���ڵ�һ��λ��
		    while (file.ReadString(Str1)!=FALSE || Str1.GetLength()!=0){
				LineNum++;               //���㵱ǰ����,ע��˴���0�п�ʼ����
				ptrStr=Str1.GetBuffer(FileData.GetLength()); //����ָ��,����������,ԭ��δ֪
				if(*ptrStr=='B'){        //�ҵ�Beginλ��
				    TimePick(ptrStr,TimeLineB);              //Beginʱ��洢��TimeLineB������
					BeginLineFlag=0;
				}  
				if(++BeginLineFlag==2){  //�����һ���ҵ�Beginλ��
					TimePick(ptrStr,TimeLine0);
				    break;
				}
			}
		}
		if(BeginLineFlag==1){              //���Begin�����һ��
		    for(int x=0;x<6;x++)
				TimeLine0[x]=TimeLine1[x]; //�ļ��ĵ�һ������ԭʼ������
		}
		if(BeginLineFlag>2){               //���û���ҵ�Beginλ��,��ô��ʼλ��Ҳ��û���ҵ�
		    AfxMessageBox("δ�ҵ���ʼʱ��,����");
		}
        
		//���㵱ǰScroll��Ӧʱ��
		unsigned char CurTime[6]={0};       //�洢���ֹ�����ʱ��
		ScrollTime(ScrollNum,CurTime);      //��ȡ����ʱ����õ�TempTime��

        //�ӻ������ʱ��,��ʼ���������ʱ�俪ʼ�ľ���ʱ��
		CurTime[5]=CurTime[5]+TimeLine0[5]; //����Second
		if(CurTime[5]/60){                  //���Second>=60
		    CurTime[5]=CurTime[5]%60;       //������ȷ��Second
			CurTime[4]++;                   //Minute����
		}
		CurTime[4]=CurTime[4]+TimeLine0[4]; //����Minute
		if(CurTime[4]/60){                  //���Minute>=60
		    CurTime[4]=CurTime[4]%60;    //������ȷ��Minute
			CurTime[3]++;                   //Hour����
		}
		CurTime[3]=CurTime[3]+TimeLine0[3]; //����Hour
		if(CurTime[3]/24){  //���Hour>=24
		    CurTime[3]=CurTime[3]%24;       //������ȷ��Hour
			CurTime[2]++;   //Day����
		}
        CurTime[2]=CurTime[2]+TimeLine0[2]; //����Day
		if(CurTime[2]>Calendar[TimeLine0[1]]){/*ע������û����TimeLine0[1]*///����·ݳ�����ǰ�·ݵ����Day��
			CurTime[2]=CurTime[2]%Calendar[TimeLine0[1]];//ȡ����ȷ��������
			CurTime[1]++;
		}
		CurTime[1]=CurTime[1]+TimeLine0[1]; /*������û��������,�������Ż�*/
		if(CurTime[1]>12){
		   CurTime[1]=1;
		   CurTime[0]++;
		}
		CurTime[0]=CurTime[0]+TimeLine0[0];//��ȡYear
		
		//˵��:����һ��Ҫ2�����ϣ���Ϊû����1�е������ж�
		//֮����ʹ�ñ�������ģʽ,��Ϊ���ֲ��Ҵ����Ը���..������򵥵Ĳ����㷨��Ҫ����,������Ϊ�������������������޸�
		unsigned char TimeSearch[6]={0};
		int ChkSecNum=0; 
		int LineCntTmp=0;
		Text txt;
		txt.TotalLineNum=txt.LineFind(file);             //ÿ��Textָ����Ҫ���·���,����Ҫ���²�ѯ�ļ�����
        //if(TimeCompare(CurTime,TimeLine1)){    //�����ǰʱ��ݵ�һ��ʱ��
		file.Seek(0,CFile::begin);         //���ļ�ͷλ��
		while((*ptrStr!='B')&&(*(ptrStr+1)!='B')){       //һֱѰ�ҵ�Beginλ��
			file.ReadString(Str1);         //�ļ���һ������
			ptrStr=Str1.GetBuffer(FileData.GetLength()); //ʹ��ָ��ָ������ַ���,׼�����������ַ�		
		    LineCntTmp++;                  //��¼��ǰ�߹�������,�����жϺ����ǵ�һ�л����һ��
		}
		//��ʱStr1�洢��Beginʱ������,ptrStrҲָ�������
		//����ʹ��LineCntTmp��Ϊ�жϱ�־,���˳���ȡ
		//ע��,Begin�����һ�������������
		for(int x=0;x<txt.TotalLineNum;x++){	        //׼����ʼ����
			if(LineCntTmp++>=txt.TotalLineNum){         //���Begin�����һ��
				LineCntTmp=0;              //ֻ��1�λص���ʼλ�õĻ���,���Բ��õ���
				file.Seek(0,CFile::begin); //���ļ�ͷλ��,׼����������
			}

			file.ReadString(Str1);         //������ȡÿһ������
			TimePick(ptrStr,TimeSearch);   //����ʱ��洢��TimeSearch������
			//�����ж�Ӧ�������ȡ����,Ȼ���ж��Ƿ����ȡ1Сʱ�ڵ�����
			if(TimeCompare(CurTime,TimeSearch)){         //��ǰʱ����ڻ���ʱ��
				ChkSecNum=Time1HourCheck(CurTime,TimeSearch); //��������ʱ�����Sec����
				if(ChkSecNum<3600&&ChkSecNum!=(-1)){     //С��1Сʱ����
					HourDataPick(ptrStr,ChkSecNum,1);
				}
			}     
			else{						   //֮���������ж�,����Ϊʱ����ȡ�ﲻ������ȡ��
				ChkSecNum=Time1HourCheck(TimeSearch,CurTime); 
				if(ChkSecNum<3600&&ChkSecNum!=(-1)){     //С��1Сʱ����
					HourDataPick(ptrStr,ChkSecNum,0);
				}
			}
		}
		return 1;
	}

	//ptr-->ָ��ǰһ�����ݵ�ָ��,��Ϊ�����Ѿ��������ָ��,���Բ���Ҫ�ڴ���File��
	//gap-->��ǰ������curTime����ĸ���,����ChkSecNum
	//Cur-->�����жϴ洢��ʽ 1-��ǰʱ�����Lineʱ�� 0-��ǰʱ��С��Lineʱ��
	int ArrayDataCnt;							//����������ȡ�±�
    int HourDataPick(char *ptr,int gap,char Cur){ 
		int LineDataCnt=0;						//һ����(���3600)���ݲ����±�
	    unsigned int  pCnt=0;                   //һ�������ֽڲ����±�
		unsigned char Flag=0;					//��ȡһ�����ݱ��
		unsigned int  TmpData=0;				//��ʱ��Ž���������
		int           ArrayDataCntFlag=1;       //ArrayDataCnt��һ�θ�ֵ��ǣ���������ϸ�÷�

		while(*(ptr+pCnt)!='D'){             //Ѱ��������ʼ���
		    pCnt++;								//׼��������һ���±�
			if(pCnt>=23){                       //���Ѱ��23���ַ���δ�ҵ�(ʵ���Ϊ23)
				AfxMessageBox("δ�ҵ�����");
				return 0;                       //����0,ʵ��δ�ҵ����ݱ�ʶ
			}
		}

		pCnt++;                                 //���ַ�Ϊ'D',��ת����һ���ַ�

		while(*(ptr+pCnt)!='\0'){				//������������,ֱ�����ݽ���
			if((*(ptr+pCnt)>='0')&&(*(ptr+pCnt)<='9')){  //���Ϊ����
				if((*(ptr+pCnt+1)==' ')||(*(ptr+pCnt+1)=='\0')) //�����һ���ַ����������ַ�,�����˴�������ȡ��������
					Flag=1;						//�ñ�־λ,׼���洢

				TmpData*=10;
                TmpData+=*(ptr+pCnt)-'0';       //��ȡ������
			}	

			/*���������£�gap��Ҫͬʱ����ǰ��û�����������~*/
			if(Flag){							//���1��Number��ȡ���
				Flag=0;
				LineDataCnt++;                  //��¼��ǰ��ȡ�����ݸ���
				if(Cur){                        //�����ǰʱ���Lineʱ��
					if(TmpData<=200){			//�����ȡ����������ֵ
						if(LineDataCnt>gap){    //����Cur����,��ʼ�洢
							TextData[ArrayDataCnt++]=(unsigned char)TmpData; //�洢������	
						}                       //��Ϊ��һ�����ʱ��������������,����ArrayDataCnt���ÿ����±�����
						TmpData=0;
					}
					else{   					//����Ϊ�洢���ݹ���,�����������ݴ洢
						TmpData=0;				//ͬ����Ҫ��0,��Ӱ����һ��������ȡ
						AfxMessageBox("�洢������ֵ����");  //�õ��������ڵ���,ʵ��Ӧ��ɾ��
					}
				}
				else{                           //��ǰʱ�䣼Lineʱ��
					if(ArrayDataCntFlag){		//�����һ�δ洢������
						if(ArrayDataCnt>gap){	//��������Ѿ���¼���ݣ����Ҷ���gap�Ĳ��
						    AfxMessageBox("������ȡ���Ǵ���"); return 0;
						}
					    ArrayDataCntFlag=0;     //���治�ڴ洢
						ArrayDataCnt=gap;       //��ǰgap���������±꣬׼���ڸû�����ֱ����ȡ
					}  /*������Ҫ����һ��ArrayDataCnt>2000����,����Ͳ������ж���*/
					if((TmpData<=200)){         //�����ȡ����������ֵҪ��
						if((ArrayDataCnt<2000)){//������С������Ӧ��¼ֵ
							TextData[ArrayDataCnt++]=(unsigned char)TmpData;    //�洢������	
						}
						TmpData=0;
					}
					else{   					//����Ϊ�洢���ݹ���,�����������ݴ洢
						TmpData=0;				//ͬ����Ҫ��0,��Ӱ����һ��������ȡ
						AfxMessageBox("�洢������ֵ����");  //�õ��������ڵ���,ʵ��Ӧ��ɾ��
					}
				}
			}
			pCnt++;							            //׼����ȡ��һ���ַ�
		}
		return 1;
	}

	/*������ȡ����,����Ҫ��ȡ����һ��,�������ÿһ�еĲ�ͬλ�ý��к�����ȡ*/
	/*����ֵ: 0-->�����쳣����  ��������-->����*/
	/*file-->�ļ�ָ��  Line-->��ȡ������  Pos-->���еľ���λ��,�Ӹ�λ����ȡ���������*/
    unsigned int DataPick(CStdioFile &file,unsigned int Line,unsigned int Pos){//��������(0��200)
		unsigned int  pCnt=0,dCnt=0;  //�ַ��������Ҽ���
        unsigned int  TmpData=0;      //��ʱ��Ž���������
		unsigned int  DataCnt=0;      //�����д����ʱ�����±�
		unsigned char Flag=0;         //��ȡһ�����ݱ��
        CString s_LineData;           //�洢һ�����ݵ��ַ�������

		if(Line<1||Line>721){         //ѡ������Line������,�ı��ļ��ӵ�1�п�ʼ����(����0��,�����-1)
		    AfxMessageBox("��ȡ����������ֵ,����");
			return 0;
		}
		if(Pos>3600){                 //ѡ��ÿһ�е���ֵ
		    AfxMessageBox("�������ݲ��ҳ�����ֵ,����");
			return 0;
		}

		ptrStr=FileData.GetBuffer(FileData.GetLength());/*��ָ��׼���滻�ɾֲ�����*///ʹ��ָ��,׼���ַ�����

		file.Seek(Line-1,CFile::begin); //��һ������,�ҵ��ļ�����ʼλ�ã�����ʼλ�ÿ�ʼƫ������
        file.ReadString(s_LineData);  //��2������,�Ӹ�λ�ö�ȡһ���ļ�

		while(*(ptrStr+pCnt)!='D'){   //Ѱ��������ʼ���
		    pCnt++; 
			if(pCnt>=23){             //���Ѱ��23���ַ���δ�ҵ�(ʵ���Ϊ23)
				AfxMessageBox("δ�ҵ�����");
				return 0;             //����0,ʵ��δ�ҵ����ݱ�ʶ
			}
		}
		
		pCnt++;                       //���ַ�Ϊ'D',��ת����һ���ַ�

		while(*(ptrStr+pCnt)!='\0'){  //������������,ֱ�����ݽ���
			if((*(ptrStr+pCnt)>='0')&&(*(ptrStr+pCnt)<='9')){  //���Ϊ����
				if((*(ptrStr+pCnt+1)==' ')||(*(ptrStr+pCnt+1)=='\0')) //�����һ���ַ����������ַ�,�����˴�������ȡ��������
					Flag=1;           //�ñ�־λ,׼���洢

				TmpData*=10;
                TmpData+=*(ptrStr+pCnt)-'0';           //��ȡ������
			}	

			if(Flag){                 //����˴�������ȡ���
				Flag=0;
				if((TmpData<=200)&&(++dCnt>=Pos)){     //�����ȡ����������ֵ&λ��Ҫ��
					TextData[DataCnt++]=(unsigned char)TmpData;                      //�洢������
					TmpData=0;
				}
				else                  //����Ϊ�洢���ݹ���,�����������ݴ洢
					AfxMessageBox("�洢���ݹ���");      //�õ��������ڵ���,ʵ��Ӧ��ɾ��
			}
			pCnt++;							            //׼����ȡ��һ���ַ�
		}
		return dCnt;                    
	}
};

//unsigned char  Text::TD;
#endif // !defined(AFX_TEXT_H__E00CAAC1_FF56_48F5_BDDA_09475A34BA22__INCLUDED_)
