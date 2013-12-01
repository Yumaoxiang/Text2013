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
	CString FileData;                 //存储当前一行的数据
    char *ptrStr;                     //指向当前一行(1小时)字符串的指针
    unsigned LineNum;
	unsigned TotalLineNum;
    unsigned char Calendar[13];
    
	/*函数1-读取文件行数寄存器-已验证*/
    int LineFind(CStdioFile &file){
		int linenum=0;
		file.Seek(0,CFile::begin);
	    //while(file.ReadString(FileData)){   //不使用该语句的原因为当文件的最后一行数据，正好是128的倍数个字符的时候，用ReadString读取，一定会返回FALSE。但实际上读取是成功的，返回的CString中的数据是正确的！（VC6.0中存在这个Bug，VS2005中，没有这个Bug）
		while (file.ReadString(FileData)!=FALSE || FileData.GetLength()!=0){
			linenum++; 
		}
		return linenum;
	}

	/*该函数未成功使用,因为line是偏移byte数而不是行数...*/
	int LinePos(CStdioFile &file,unsigned int line){
	    file.Seek(file,line);
	}

	//file-->当前文本(4M) ,该函数需要修改，i<TextLine应该改为LineFind模式查找起始位置
	char HeadFind(CStdioFile &file){         
		                              //指向当前一行数据的指针,用于字符寻找
		for(int i=0;i<TextLine;i++){  //遍历720行数据
		    file.ReadString(FileData);//读取一行,读取完成后自动转到下一行起始位置
			ptrStr=FileData.GetBuffer(FileData.GetLength());   //使用指针,准备字符查找
			if(*ptrStr=='B'){         //如果找到起始位置
				FileData.ReleaseBuffer();
				return 1;             //返回真
			}
		}
		FileData.ReleaseBuffer();     
		return 0;                     //如果未找到起始位置,返回错误标志0
	}

	//提取一行数据时间函数 
	//strTime-->该行字符串 pTime-->指向存储时间的数组
	//返回值0-->失败,此处因有错误提示,也可在调用它的函数中加入,这样更友好  1-->成功 
	unsigned int TimePick(CString StrTime,unsigned char *pTime){//解析数据(0—200)
		unsigned char pCnt=0,dCnt=0; //提取时间字符位置计数
		unsigned char Flag=0;        //数据提取变量
		unsigned int  TmpData=0;     //临时存放解析的数据
		char *ptrStr=StrTime.GetBuffer(StrTime.GetLength()); //指向该行字符串指针

        if(*ptrStr=='\0'){           //如果该行字符串没有任何数据
		    AfxMessageBox("时间提取字符串为空,请检查数据");
			return 0;            
		}

		while(*(ptrStr+pCnt)!='D'){  //查找数据起始位之前的数据
			if(pCnt>25){             //如果查找25个字符后仍没有找到数据起始位
			   AfxMessageBox("时间提取字符串时间位过长,请检查数据"); 
			   return 0;
			}
			if((*(ptrStr+pCnt)>='0')&&(*(ptrStr+pCnt)<='9')){  //如果为数字
				if((*(ptrStr+pCnt+1)==' ')||(*(ptrStr+pCnt+1)=='\0')) //如果下一个字符不是数字字符,表明此次数字提取即将结束
					Flag=1;           //置标志位,准备存储

				TmpData*=10;
                TmpData+=*(ptrStr+pCnt)-'0';               //提取该数据
			}	

			if(Flag){                 //如果此次数据提取完成
				Flag=0;
				if((TmpData<=59)){    //如果提取数据满足数值要求,注意此处没有60,SD卡写入需注意
					*(pTime+dCnt++)=(unsigned char)TmpData;  //存储该数据
					TmpData=0;
				}
				else{                 //否则为存储数据过大,或发生错误数据存储
					AfxMessageBox("时间提取数值过大,请检查数据"); //该弹出框用于调试,实际应可删除
				    return 0;
				}
			}
			pCnt++;							            //准备提取下一个字符
		}  		
		return 1;                    
	}

    char* BeginFind(CStdioFile &file){
	    CString Str;
		char *ptrStr;
		//while(file.ReadString(FileData)){   //不使用该语句的原因为当文件的最后一行数据，正好是128的倍数个字符的时候，用ReadString读取，一定会返回FALSE。但实际上读取是成功的，返回的CString中的数据是正确的！（VC6.0中存在这个Bug，VS2005中，没有这个Bug）
		while (file.ReadString(FileData)!=FALSE || FileData.GetLength()!=0){
			if(*ptrStr=='B')
				return ptrStr;
		}
		return 0;
	}

	//将滑块位置转化成时间
	void ScrollTime(unsigned int Num,unsigned char *pTime){   //Y M D H M S,没有初始化,后面在说){
		*(pTime+5)=Num%60;    //取出当前秒
		Num=Num/60;			  //准备提取分
        *(pTime+4)=Num%60;    //取出当前分
		Num=Num/60;			  //准备提取时
		*(pTime+3)=Num%24;    //取出当前时
		*(pTime+2)=Num/24;    //算出所剩天数
	}

	unsigned char TimeCompare(unsigned char *p1,unsigned char *p2){
		for(int i=0;i<6;i++){
		    if(*(p1+i)>*(p2+i))       
				return 1;
			else if(*(p1+i)<*(p2+i))
				return 0;
		}
	    return 1; //年月日时分秒都匹配完成仍未返回,说明滑块时间&最早第一行时间相等
	}

	//一小时内数据对比 返回值 0-->非一小时内数据  1-->一小时内数据对应的数据数
	//P1-->滑块时间     P2-->当前一行时间
	//传入2个字符串的指针,符合返回值,指针不变,不符合指针继续搜索
	int Time1HourCheck(unsigned char *pt1,unsigned char *pt2){
		unsigned char p1[6]={0};              //利用2个临时变量,放置修改CurTime数值
		unsigned char p2[6]={0};
		int TimeSubNum=0;                     //两个时间点的时间差对应的Second个数
		
		for(int i=0;i<6;i++){
		    p1[i]=*(pt1+i);
			p2[i]=*(pt2+i);
		}
        /*传入的时间可能大可能小,这里指考虑了大,吃晚饭在继续写16:41*/
	    if(*p1>*p2)                           //如果滑块年>当前一行年
			*(p1+1)=*(p1+1)+12;               //滑块月份+12,为了计算方便
		if(*(p1+1)-*(p2+1)>1)                 //如果月份大于2个月,这种情况正常不可能发生
			return -1;                        //非一小时内数据
		if(*(p1+1)-*(p2+1)==1)                //如果滑块大于当前行一个月时间
			*(p1+2)=*(p1+2)+Calendar[*(p2+1)];//因为滑块时间更大,所以加入上一个月的日期
		if(*(p1+2)-*(p2+2)>1)				  //如果当前滑块天数>当前字符串天数1天以上
			return -1;						  //非一小时内数据
		if(*(p1+2)-*(p2+2)==1)                //如果滑块大于当前行一天时间
			*(p1+3)=*(p1+3)+24;               //滑块小时+24,为了计算
        if(*(p1+3)-*(p2+3)>1)                 //如果时间对比超过1小时
			return -1;						  //非一小时内数据
		//注意，下面对于时间差的判断一定是P1>P2
		if(*(p1+3)-*(p2+3)==1)                //如果滑块大于当前行一小时
			*(p1+4)=*(p1+4)+60;               //以分钟为单位,计算时间差
		TimeSubNum=*(p1+4)*60+*(p1+5);        //当前的时间换算成Second个数
		TimeSubNum=TimeSubNum-(*(p2+4)*60+*(p2+5));//减去另一个时间
		return TimeSubNum;  
	}



	//获取当前滑轮滚动位置应该读取的2000个数据
	//ScrollNum-->滑轮的位置0—2592000
	//OnDraw函数中m_ptScroll=GetScrollPosition();已经获得滑轮的位置,故调用该变量即可
	unsigned int ScrollData(CStdioFile &file,unsigned int ScrollNum){
        CString Str1; 
		unsigned char TimeLine1[6]={0};  //存储文件第一行时间
		unsigned char TimeLineB[6]={0};  //存储文件Begin点时间-最近的时间
		unsigned char TimeLine0[6]={0};  //存储Begin下一行时间-最远的时间-真正的起始时间
		unsigned int  LineNum=0;         //计算当前Line位置,用于判断是否Begin在第一行
		unsigned char FirstLineFlag=0;   //用于Begin判断文件是否在第一行,后续做不同处理

		/*如果数据不满足一定要求,弹出提示信息,作为后续补丁*/
		//记录第一行位置时间
		file.Seek(0,CFile::begin);       //此处一定要一定到文件头位置,否则若调用其他函数可能造成位置读取错误
		file.ReadString(Str1);           //文件第一行数据,记录时间并判断是否也是Begin位置
        char *ptrStr=Str1.GetBuffer(FileData.GetLength());   //使用指针指向此行字符串,准备查找特殊字符
		if((*ptrStr=='B')||(*(ptrStr+1)=='B')){              //如果第一行也是Begin位置
		    FirstLineFlag=1;             //后续顺序查找
		}
	    TimePick(ptrStr,TimeLine1);      //第一行数据时间存储到TimeLine1数组中,下面该寻找Begin处数据
        
	    //查找Begin位置,并记录该位置时间和行数
		unsigned int BeginLineFlag=2;
		if(!FirstLineFlag){              //如果Begin不在第一行位置
		    while (file.ReadString(Str1)!=FALSE || Str1.GetLength()!=0){
				LineNum++;               //计算当前行数,注意此处从0行开始算起
				ptrStr=Str1.GetBuffer(FileData.GetLength()); //重新指向,否则发生错误,原因未知
				if(*ptrStr=='B'){        //找到Begin位置
				    TimePick(ptrStr,TimeLineB);              //Begin时间存储到TimeLineB数组中
					BeginLineFlag=0;
				}  
				if(++BeginLineFlag==2){  //如果上一次找到Begin位置
					TimePick(ptrStr,TimeLine0);
				    break;
				}
			}
		}
		if(BeginLineFlag==1){              //如果Begin在最后一行
		    for(int x=0;x<6;x++)
				TimeLine0[x]=TimeLine1[x]; //文件的第一行是最原始的数据
		}
		if(BeginLineFlag>2){               //如果没有找到Begin位置,那么起始位置也就没有找到
		    AfxMessageBox("未找到起始时间,请检查");
		}
        
		//计算当前Scroll对应时间
		unsigned char CurTime[6]={0};       //存储滑轮滚动的时间
		ScrollTime(ScrollNum,CurTime);      //提取出的时间放置到TempTime中

        //从滑轮相对时间,开始计算从最早时间开始的绝对时间
		CurTime[5]=CurTime[5]+TimeLine0[5]; //计算Second
		if(CurTime[5]/60){                  //如果Second>=60
		    CurTime[5]=CurTime[5]%60;       //计算正确的Second
			CurTime[4]++;                   //Minute自增
		}
		CurTime[4]=CurTime[4]+TimeLine0[4]; //计算Minute
		if(CurTime[4]/60){                  //如果Minute>=60
		    CurTime[4]=CurTime[4]%60;    //计算正确的Minute
			CurTime[3]++;                   //Hour自增
		}
		CurTime[3]=CurTime[3]+TimeLine0[3]; //计算Hour
		if(CurTime[3]/24){  //如果Hour>=24
		    CurTime[3]=CurTime[3]%24;       //计算正确的Hour
			CurTime[2]++;   //Day自增
		}
        CurTime[2]=CurTime[2]+TimeLine0[2]; //计算Day
		if(CurTime[2]>Calendar[TimeLine0[1]]){/*注意这里没有用TimeLine0[1]*///如果月份超过当前月份的最大Day数
			CurTime[2]=CurTime[2]%Calendar[TimeLine0[1]];//取出正确的日期数
			CurTime[1]++;
		}
		CurTime[1]=CurTime[1]+TimeLine0[1]; /*这里面没有算闰年,后续需优化*/
		if(CurTime[1]>12){
		   CurTime[1]=1;
		   CurTime[0]++;
		}
		CurTime[0]=CurTime[0]+TimeLine0[0];//提取Year
		
		//说明:数据一定要2行以上，因为没有做1行的数据判断
		//之所以使用遍历查找模式,因为二分查找代码稍复杂..而且最简单的查找算法需要排序,所以作为后续代码质量提升在修改
		unsigned char TimeSearch[6]={0};
		int ChkSecNum=0; 
		int LineCntTmp=0;
		Text txt;
		txt.TotalLineNum=txt.LineFind(file);             //每个Text指针需要重新分配,所以要重新查询文件行数
        //if(TimeCompare(CurTime,TimeLine1)){    //如果当前时间≥第一行时间
		file.Seek(0,CFile::begin);         //到文件头位置
		while((*ptrStr!='B')&&(*(ptrStr+1)!='B')){       //一直寻找到Begin位置
			file.ReadString(Str1);         //文件第一行数据
			ptrStr=Str1.GetBuffer(FileData.GetLength()); //使用指针指向此行字符串,准备查找特殊字符		
		    LineCntTmp++;                  //记录当前走过多少行,用于判断后面是第一行或最后一行
		}
		//此时Str1存储了Begin时的数据,ptrStr也指向该数据
		//下面使用LineCntTmp作为判断标志,如何顺序读取
		//注意,Begin下面的一行是最早的数据
		for(int x=0;x<txt.TotalLineNum;x++){	        //准备开始遍历
			if(LineCntTmp++>=txt.TotalLineNum){         //如果Begin在最后一行
				LineCntTmp=0;              //只有1次回到起始位置的机会,所以不用担心
				file.Seek(0,CFile::begin); //到文件头位置,准备遍历查找
			}

			file.ReadString(Str1);         //遍历读取每一行数据
			TimePick(ptrStr,TimeSearch);   //数据时间存储到TimeSearch数组中
			//首先判断应该如何提取数据,然后判断是否该提取1小时内的数据
			if(TimeCompare(CurTime,TimeSearch)){         //当前时间大于滑块时间
				ChkSecNum=Time1HourCheck(CurTime,TimeSearch); //计算两个时间相隔Sec个数
				if(ChkSecNum<3600&&ChkSecNum!=(-1)){     //小于1小时数据
					HourDataPick(ptrStr,ChkSecNum,1);
				}
			}     
			else{						   //之所以这样判断,是因为时间提取里不容易提取±
				ChkSecNum=Time1HourCheck(TimeSearch,CurTime); 
				if(ChkSecNum<3600&&ChkSecNum!=(-1)){     //小于1小时数据
					HourDataPick(ptrStr,ChkSecNum,0);
				}
			}
		}
		return 1;
	}

	//ptr-->指向当前一行数据的指针,因为上面已经有了这个指针,所以不需要在传入File了
	//gap-->当前数组与curTime相隔的个数,就是ChkSecNum
	//Cur-->用于判断存储方式 1-当前时间大于Line时间 0-当前时间小于Line时间
	int ArrayDataCnt;							//数组数据提取下标
    int HourDataPick(char *ptr,int gap,char Cur){ 
		int LineDataCnt=0;						//一行中(最大3600)数据查找下标
	    unsigned int  pCnt=0;                   //一行数据字节查找下标
		unsigned char Flag=0;					//提取一个数据标记
		unsigned int  TmpData=0;				//临时存放解析的数据
		int           ArrayDataCntFlag=1;       //ArrayDataCnt第一次赋值标记，后面有详细用法

		while(*(ptr+pCnt)!='D'){             //寻找数据起始标号
		    pCnt++;								//准备查找下一个下标
			if(pCnt>=23){                       //如果寻找23个字符仍未找到(实际最长为23)
				AfxMessageBox("未找到数据");
				return 0;                       //返回0,实际未找到数据标识
			}
		}

		pCnt++;                                 //该字符为'D',故转到下一个字符

		while(*(ptr+pCnt)!='\0'){				//遍历该数据组,直到数据结束
			if((*(ptr+pCnt)>='0')&&(*(ptr+pCnt)<='9')){  //如果为数字
				if((*(ptr+pCnt+1)==' ')||(*(ptr+pCnt+1)=='\0')) //如果下一个字符不是数字字符,表明此次数字提取即将结束
					Flag=1;						//置标志位,准备存储

				TmpData*=10;
                TmpData+=*(ptr+pCnt)-'0';       //提取该数据
			}	

			/*吃晚饭在想下，gap需要同时考虑前面没有填入的数据~*/
			if(Flag){							//如果1个Number提取完成
				Flag=0;
				LineDataCnt++;                  //记录当前提取的数据个数
				if(Cur){                        //如果当前时间≥Line时间
					if(TmpData<=200){			//如果提取数据满足数值
						if(LineDataCnt>gap){    //到达Cur数据,开始存储
							TextData[ArrayDataCnt++]=(unsigned char)TmpData; //存储该数据	
						}                       //因为上一行最近时间才有所需的数据,所以ArrayDataCnt不用考虑下标跳变
						TmpData=0;
					}
					else{   					//否则为存储数据过大,或发生错误数据存储
						TmpData=0;				//同样需要清0,不影响下一次数据提取
						AfxMessageBox("存储数据数值过大");  //该弹出框用于调试,实际应可删除
					}
				}
				else{                           //当前时间＜Line时间
					if(ArrayDataCntFlag){		//如果第一次存储行数据
						if(ArrayDataCnt>gap){	//如果数组已经记录数据，而且多余gap的差距
						    AfxMessageBox("数据提取覆盖错误"); return 0;
						}
					    ArrayDataCntFlag=0;     //后面不在存储
						ArrayDataCnt=gap;       //当前gap赋给数组下标，准备在该基础上直接提取
					}  /*这里需要补充一个ArrayDataCnt>2000返回,下面就不用再判断了*/
					if((TmpData<=200)){         //如果提取数据满足数值要求
						if((ArrayDataCnt<2000)){//数据量小于数组应记录值
							TextData[ArrayDataCnt++]=(unsigned char)TmpData;    //存储该数据	
						}
						TmpData=0;
					}
					else{   					//否则为存储数据过大,或发生错误数据存储
						TmpData=0;				//同样需要清0,不影响下一次数据提取
						AfxMessageBox("存储数据数值过大");  //该弹出框用于调试,实际应可删除
					}
				}
			}
			pCnt++;							            //准备提取下一个字符
		}
		return 1;
	}

	/*数据提取函数,不需要提取整个一行,而是针对每一行的不同位置进行后续提取*/
	/*返回值: 0-->出现异常数据  其他数据-->正常*/
	/*file-->文件指针  Line-->读取的行数  Pos-->该行的具体位置,从该位置提取后面的数据*/
    unsigned int DataPick(CStdioFile &file,unsigned int Line,unsigned int Pos){//解析数据(0—200)
		unsigned int  pCnt=0,dCnt=0;  //字符个数查找计数
        unsigned int  TmpData=0;      //临时存放解析的数据
		unsigned int  DataCnt=0;      //数组中存放临时数据下标
		unsigned char Flag=0;         //提取一个数据标记
        CString s_LineData;           //存储一行数据的字符串变量

		if(Line<1||Line>721){         //选择输入Line的区间,文本文件从第1行开始算起(不是0行,后面会-1)
		    AfxMessageBox("读取行数超过阈值,请检查");
			return 0;
		}
		if(Pos>3600){                 //选择每一行的阈值
		    AfxMessageBox("单行数据查找超过阈值,请检查");
			return 0;
		}

		ptrStr=FileData.GetBuffer(FileData.GetLength());/*该指针准备替换成局部变量*///使用指针,准备字符查找

		file.Seek(Line-1,CFile::begin); //第一个参数,找到文件的起始位置，从起始位置开始偏移行数
        file.ReadString(s_LineData);  //第2个参数,从该位置读取一行文件

		while(*(ptrStr+pCnt)!='D'){   //寻找数据起始标号
		    pCnt++; 
			if(pCnt>=23){             //如果寻找23个字符仍未找到(实际最长为23)
				AfxMessageBox("未找到数据");
				return 0;             //返回0,实际未找到数据标识
			}
		}
		
		pCnt++;                       //该字符为'D',故转到下一个字符

		while(*(ptrStr+pCnt)!='\0'){  //遍历该数据组,直到数据结束
			if((*(ptrStr+pCnt)>='0')&&(*(ptrStr+pCnt)<='9')){  //如果为数字
				if((*(ptrStr+pCnt+1)==' ')||(*(ptrStr+pCnt+1)=='\0')) //如果下一个字符不是数字字符,表明此次数字提取即将结束
					Flag=1;           //置标志位,准备存储

				TmpData*=10;
                TmpData+=*(ptrStr+pCnt)-'0';           //提取该数据
			}	

			if(Flag){                 //如果此次数据提取完成
				Flag=0;
				if((TmpData<=200)&&(++dCnt>=Pos)){     //如果提取数据满足数值&位置要求
					TextData[DataCnt++]=(unsigned char)TmpData;                      //存储该数据
					TmpData=0;
				}
				else                  //否则为存储数据过大,或发生错误数据存储
					AfxMessageBox("存储数据过大");      //该弹出框用于调试,实际应可删除
			}
			pCnt++;							            //准备提取下一个字符
		}
		return dCnt;                    
	}
};

//unsigned char  Text::TD;
#endif // !defined(AFX_TEXT_H__E00CAAC1_FF56_48F5_BDDA_09475A34BA22__INCLUDED_)
