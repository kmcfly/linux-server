/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author		: lufen
** Date		: 2008-09-11
** Name		: PinYinInput.h
** Version 	: 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include<string.h>
#include<stdio.h>
#include"PinYinInput.h"


//用于记录字母的索引
LetterIndex letIndex[26]={
	{'a',0},
	{'b',5},
	{'c',21},
	{'d',55},
	{'e',76},
	{'f',80},
	{'g',88},
	{'h',106},
	{'i',125},
	{'j',125},
	{'k',139},
	{'l',157},
	{'m',181},
	{'n',201},
	{'o',225},
	{'p',227},
	{'q',244},
	{'r',258},
	{'s',272},
	{'t',306},
	{'u',325},
	{'v',325},
	{'w',325},
	{'x',334},
	{'y',348},
	{'z',363}
};

//用于记录字母组合的索引
PinYinIndex pinYinIndex[399]={
	{"a",0},// 0
	{"ai",12},
	{"an",56},
	{"ang",92},
	{"ao",98},
	{"ba",144},// 5
	{"bai",196},
	{"ban",216},
	{"bang",258},
	{"bao",286},
	{"bei",334},
	{"ben",390},
	{"beng",406},
	{"bi",422},
	{"bian",526},
	{"biao",580},
	{"bie",610},
	{"bin",620},
	{"bing",652},
	{"bo",676},
	{"bu",736},
	{"ca",772},// 21
	{"cai",778},
	{"can",800},
	{"cang",824},
	{"cao",834},
	{"ce",852},
	{"cen",864},
	{"ceng",868},
	{"cha",872},
	{"chai",916},
	{"chan",930},
	{"chang",980},
	{"chao",1032},
	{"che",1056},
	{"chen",1072},
	{"cheng",1114},
	{"chi",1168},
	{"chong",1246},
	{"chou",1268},
	{"chu",1302},
	{"chuai",1358},
	{"chuan",1368},
	{"chuang",1394},
	{"chui",1408},
	{"chun",1424},
	{"chuo",1444},
	{"ci",1454},
	{"cong",1488},
	{"cou",1510},
	{"cu",1518},
	{"cuan",1540},
	{"cui",1554},
	{"cun",1584},
	{"cuo",1594},
	{"da",1622},
	{"dai",1654},
	{"dan",1696},
	{"dang",1746},
	{"dao",1768},
	{"de",1800},
	{"deng",1808},
	{"di",1834},
	{"dia",1908},
	{"dian",1910},
	{"diao",1960},
	{"die",1986},
	{"ding",2018},
	{"diu",2054},
	{"dong",2058},
	{"dou",2096},
	{"du",2118},
	{"duan",2168},
	{"dui",2186},
	{"dun",2200},
	{"duo",2232},
	{"e",2272},
	{"ei",2346},
	{"en",2348},
	{"er",2354},
	{"fa",2382},
	{"fan",2402},
	{"fang",2450},
	{"fei",2482},
	{"fen",2540},
	{"feng",2580},
	{"fou",2622},
	{"fu",2626},
	{"ga",2790},
	{"gai",2804},
	{"gan",2826},
	{"gang",2874},
	{"gao",2898},
	{"ge",2936},
	{"gen",2998},
	{"geng",3010},
	{"gong",3034},
	{"gou",3072},
	{"gu",3114},
	{"gua",3194},
	{"guai",3218},
	{"guan",3224},
	{"guang",3262},
	{"gui",3276},
	{"gun",3332},
	{"guo",3346},
	{"ha",3382},
	{"hai",3386},
	{"han",3408},
	{"hang",3468},
	{"hao",3482},
	{"he",3520},
	{"hei",3578},
	{"hen",3582},
	{"heng",3590},
	{"hong",3604},
	{"hou",3638},
	{"hu",3668},
	{"hua",3754},
	{"huai",3780},
	{"huan",3792},
	{"huang",3848},
	{"hui",3898},
	{"hun",3976},
	{"huo",3996},
	{"ji",4036},
	{"jia",4244},
	{"jian",4316},
	{"jiang",4452},
	{"jiao",4496},
	{"jie",4582},
	{"jin",4664},
	{"jing",4730},
	{"jiong",4806},
	{"jiu",4814},
	{"ju",4866},
	{"juan",4966},
	{"jue",4996},
	{"jun",5050},
	{"ka",5080},
	{"kai",5094},
	{"kan",5120},
	{"kang",5142},
	{"kao",5162},
	{"ke",5178},
	{"ken",5244},
	{"keng",5254},
	{"kong",5260},
	{"kou",5274},
	{"ku",5292},
	{"kua",5316},
	{"kuai",5328},
	{"kuan",5346},
	{"kuang",5352},
	{"kui",5384},
	{"kun",5440},
	{"kuo",5462},
	{"la",5472},
	{"lai",5498},
	{"lan",5522},
	{"lang",5566},
	{"lao",5594},
	{"le",5628},
	{"lei",5644},
	{"leng",5682},
	{"li",5692},
	{"lia",5844},
	{"lian",5846},
	{"liang",5898},
	{"liao",5930},
	{"lie",5970},
	{"lin",5996},
	{"ling",6044},
	{"liu",6100},
	{"long",6142},
	{"lou",6176},
	{"lu",6206},
	{"luan",6326},
	{"lue",6348},
	{"lun",6354},
	{"luo",6370},
	{"m",6422},
	{"ma",6424},
	{"mai",6452},
	{"man",6470},
	{"mang",6506},
	{"mao",6526},
	{"me",6576},
	{"mei",6580},
	{"men",6632},
	{"meng",6646},
	{"mi",6684},
	{"mian",6742},
	{"miao",6770},
	{"mie",6802},
	{"min",6812},
	{"ming",6844},
	{"miu",6868},
	{"mo",6870},
	{"mou",6928},
	{"mu",6946},
	{"n",6988},
	{"na",6990},
	{"nai",7014},
	{"nan",7032},
	{"nang",7052},
	{"nao",7062},
	{"ne",7088},
	{"nei",7092},
	{"nen",7096},
	{"neng",7100},
	{"ni",7102},
	{"nian",7146},
	{"niang",7176},
	{"niao",7178},
	{"nie",7190},
	{"nin",7218},
	{"ning",7220},
	{"niu",7242},
	{"nong",7256},
	{"nou",7268},
	{"nu",7270},
	{"nuan",7292},
	{"nue",7294},
	{"nuo",7296},
	{"o",7312},
	{"ou",7338},
	{"pa",7360},
	{"pai",7380},
	{"pan",7398},
	{"pang",7428},
	{"pao",7446},
	{"pei",7470},
	{"pen",7500},
	{"peng",7506},
	{"pi",7542},
	{"pian",7624},
	{"piao",7644},
	{"pie",7666},
	{"pin",7676},
	{"ping",7700},
	{"po",7726},
	{"pou",7758},
	{"pu",7764},
	{"qi",7812},
	{"qia",7952},
	{"qian",7962},
	{"qiang",8050},
	{"qiao",8090},
	{"qie",8142},
	{"qin",8164},
	{"qing",8208},
	{"qiong",8254},
	{"qiu",8272},
	{"qu",8318},
	{"quan",8384},
	{"que",8426},
	{"qun",8448},
	{"ran",8454},
	{"rang",8468},
	{"rao",8482},
	{"re",8494},
	{"ren",8498},
	{"reng",8532},
	{"ri",8536},
	{"rong",8538},
	{"rou",8568},
	{"ru",8580},
	{"ruan",8620},
	{"rui",8626},
	{"run",8642},
	{"ruo",8646},
	{"sa",8652},
	{"sai",8668},
	{"san",8678},
	{"sang",8692},
	{"sao",8704},
	{"se",8724},
	{"sen",8736},
	{"seng",8738},
	{"sha",8740},
	{"shai",8774},
	{"shan",8778},
	{"shang",8842},
	{"shao",8868},
	{"she",8902},
	{"shen",8940},
	{"sheng",8990},
	{"shi",9020},
	{"shou",9146},
	{"shu",9172},
	{"shua",9264},
	{"shuai",9270},
	{"shuan",9280},
	{"shuang",9288},
	{"shui",9296},
	{"shun",9304},
	{"shuo",9312},
	{"si",9330},
	{"song",9394},
	{"sou",9426},
	{"su",9454},
	{"suan",9498},
	{"sui",9506},
	{"sun",9542},
	{"suo",9558},
	{"ta",9588},
	{"tai",9622},
	{"tan",9654},
	{"tang",9702},
	{"tao",9752},
	{"te",9786},
	{"teng",9794},
	{"ti",9804},
	{"tian",9848},
	{"tiao",9874},
	{"tie",9902},
	{"ting",9912},
	{"tong",9946},
	{"tou",9986},
	{"tu",9998},
	{"tuan",10030},
	{"tui",10040},
	{"tun",10054},
	{"tuo",10068},
	{"wa",10114},
	{"wai",10134},
	{"wan",10138},
	{"wang",10190},
	{"wei",10220},
	{"wen",10338},
	{"weng",10368},
	{"wo",10378},
	{"wu",10412},
	{"xi",10520},
	{"xia",10662},
	{"xian",10704},
	{"xiang",10796},
	{"xiao",10854},
	{"xie",10912},
	{"xin",10986},
	{"xing",11016},
	{"xiong",11060},
	{"xiu",11076},
	{"xu",11110},
	{"xuan",11174},
	{"xue",11226},
	{"xun",11246},
	{"ya",11302},
	{"yan",11358},
	{"yang",11480},
	{"yao",11530},
	{"ye",11592},
	{"yi",11636},
	{"yin",11840},
	{"ying",11906},
	{"yo",11982},
	{"yong",11986},
	{"you",12036},
	{"yu",12116},
	{"yuan",12296},
	{"yue",12364},
	{"yun",12394},
	{"za",12440},
	{"zai",12452},
	{"zan",12470},
	{"zang",12492},
	{"zao",12504},
	{"ze",12534},
	{"zei",12560},
	{"zen",12562},
	{"zeng",12566},
	{"zha",12582},
	{"zhai",12624},
	{"zhan",12640},
	{"zhang",12678},
	{"zhao",12724},
	{"zhe",12754},
	{"zhen",12794},
	{"zheng",12856},
	{"zhi",12898},
	{"zhong",13052},
	{"zhou",13084},
	{"zhu",13130},
	{"zhua",13222},
	{"zhuai",13224},
	{"zhuan",13226},
	{"zhuang",13244},
	{"zhui",13260},
	{"zhun",13278},
	{"zhuo",13286},
	{"zi",13326},
	{"zong",13398},
	{"zou",13420},
	{"zu",13438},
	{"zuan",13460},
	{"zui",13470},
	{"zun",13480},
	{"zuo",13490}
};



/****************************************************************
**功           能  :	
**输入参数:					
**输出参数:	
**返  回   值 :	
*****************************************************************/	  
CPinYinInput* CPinYinInput::Instance()
{
	static CPinYinInput s_pinYinInput;
	return &s_pinYinInput;
}


/****************************************************************
**功           能  :	构造函数 	
**输入参数:					
**输出参数:	
**返  回   值 :	
*****************************************************************/	   
CPinYinInput::CPinYinInput()
{
	memset(m_Inputstr, 0, PinYinNUM);
	m_Inputnum = 0;
	m_WordNum = 0;
	m_PageNum = 1;
	m_WordBuffer = NULL;
	m_WordBufferLen = 0;
	memset(m_displaybuf, 0, PY_DISP_BUFF_LEN);
	m_dispLen = 0;

	m_pFilePath = NULL;
}


/****************************************************************
**功           能  :	析构函数 	
**输入参数:					
**输出参数:	
**返  回   值 :	
*****************************************************************/	   
CPinYinInput:: ~CPinYinInput()
{
	//if(m_WordBuffer != NULL)
	//{
	//	delete []m_WordBuffer;
	//	m_WordBuffer = NULL;
	//}
	Quit();
}

bool CPinYinInput::Initial(const char* path)
{
	Quit();

	m_pFilePath = new char[512];
	memset(m_pFilePath, 0, 512);

	if (NULL != path)
	{
		strcpy(m_pFilePath, path);
		int len = strlen(m_pFilePath);
#ifdef __ENVIRONMENT_WIN32__
		if ('/' == m_pFilePath[len - 1] || '\\' == m_pFilePath[len - 1] )
		{
			m_pFilePath[len - 1] = '\0';
		}
		strcat(m_pFilePath, "\\ui\\fontlib_cn_s.txt");
#else
		if ('/' == m_pFilePath[len - 1])
		{
			m_pFilePath[len - 1] = '\0';
		}
		strcat(m_pFilePath, "/ui/fontlib_cn_s.txt");
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(m_pFilePath, "ui\\fontlib_cn_s.txt");
#else
		strcpy(m_pFilePath, "ui/fontlib_cn_s.txt");
#endif

		return false;
	}

	return true;
}

void CPinYinInput::Quit()
{
	if(m_WordBuffer != NULL)
	{
		delete []m_WordBuffer;
		m_WordBuffer = NULL;
	}

	if(m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	m_WordBufferLen = 0;
	m_dispLen = 0;
}

/****************************************************************
**功           能  :	设置从键盘上获取字符
**输入参数:	str: 输入的字符			
**输出参数:	无	
**返  回   值 :	改变m_Inputstr 成员的 值 
*****************************************************************/	   
void CPinYinInput::SetChar(const char *str, BYTE len)
{
	BYTE i = 0;
	if(str == NULL)
	{
		return;
	}
	
	//判断是否是合法串
	for(i = 0; i < len; i++)
	{
		if((str[i] >='a')&&(str[i]<='z'))
		{
			m_Inputstr[i] = str[i];
		}
	}
	if( (i == len) &&(m_Inputstr[0] != 0))
	{
		m_WordNum = 0;
		m_Inputstr[len] = 0;
		GetPinYinInfo();
		GetDisplayPageInfo(1);
		m_PageNum = 1;
	}

}

/****************************************************************
**功           能  :	设置从键盘上获取数字
**输入参数:	str: 输入的字符			
**输出参数:	无	
**返  回   值 :	改变m_Inputnum 成员的 值 
*****************************************************************/	   
void CPinYinInput::SetNumber(const char str)
{
	if((str>='0')&&(str<='9'))
	{
		m_Inputnum = str - '0';
	}
}

/****************************************************************
**功           能  :	获取用户输入字符串的拼音相关信息并
				返回汉字的个数和汉字的首地址
**输入参数:	m_Inputstr:用户输入的字符串			
**输出参数:	m_WordNum:当前拼音汉字的总个数
**返  回   值 :	无
*****************************************************************/	   
void CPinYinInput::GetPinYinInfo(void)
{	
	unsigned int start =0,end = 0,i = 0;
	BYTE flag = 0;
	ULONG offset = 0;

	if(m_Inputstr[0]== 'z')
	{
		start = letIndex[25].index;
		end = 399;
	}
	else
	{
		start = letIndex[m_Inputstr[0]-'a'].index;
		end = letIndex[m_Inputstr[0]-'a'+1].index;
	}

	//查找字符串
	for(i = start; i<end; i++)
	{
		if(strcmp(m_Inputstr, pinYinIndex[i].pinYin) == 0)
		{
			flag = 1;
			break;
		}	
	}

	if(flag == 1)
	{	
		start = i;
		end = i+1;		
		offset = pinYinIndex[start].index;
		if(end == 399)
		{
			m_WordNum = 13518- pinYinIndex[start].index;
		}
		else
		{
			m_WordNum = pinYinIndex[end].index - pinYinIndex[start].index;
		}
		
		GetChineseInfo(offset);
	}
}

/****************************************************************
**功           能  :	动态申请当前拼音所有汉字的空间
**输入参数:	无		
**输出参数:	无
**返  回   值 :	true：成功false:失败
*****************************************************************/	   
BOOL CPinYinInput::SetCNBuffer(void)
{
	if(m_WordNum != 0)
	{
		if(m_WordNum + 1 > m_WordBufferLen)
		{
			m_WordBufferLen = m_WordNum + 1;

			if(m_WordBuffer)
			{
				delete [] m_WordBuffer;
			}

			m_WordBuffer = new char[m_WordBufferLen];
		}

		memset(m_WordBuffer, 0, m_WordBufferLen);
		
		if(m_WordBuffer == NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

/****************************************************************
**功           能  :	从文件中获取到汉前拼音所对应的所有
				汉字
**输入参数:	offset: 当前汉字的首地址		
**输出参数:	无
**返  回   值 :	无
*****************************************************************/	   
void CPinYinInput::GetChineseInfo(ULONG offset)
{
	if(SetCNBuffer() == false)
	{
		return;
	}

#ifdef __ENVIRONMENT_WIN32__
	FILE *fp = fopen(m_pFilePath/*"..\\ui\\fontlib.txt"*/,"rb");
#else
	FILE *fp = fopen(m_pFilePath/*"/mnt/mtd/src/chinese_font/fontlib.txt"*/,"rb");
#endif
	
	if(fp == NULL)
	{
		printf("open fontlib file err!");
		return;
	}

	fseek(fp, offset, 0);	
	fread(m_WordBuffer,sizeof(char),m_WordNum,fp);
	*(m_WordBuffer+m_WordNum) = 0;

	fclose(fp);
	fp = NULL;
			
}


/****************************************************************
**功           能  :	获得每页显示的汉字信息
**输入参数:	page: 当前需要显示的页		
**输出参数:	无
**返  回   值 :	m_displaybuf:当前页需要显示的汉字当前定
				为一次显示7个汉字page 是基于1 的
*****************************************************************/	   
void CPinYinInput::GetDisplayPageInfo(unsigned int page)
{
	if(m_WordNum == 0)
	{
		return;
	}
	
	unsigned short  disnum = 0,modnum = 0;
	disnum = m_WordNum/CNDISNUM;
	modnum = m_WordNum%CNDISNUM;
	
	memset(m_displaybuf,0,PY_DISP_BUFF_LEN);
	m_dispLen = 0;

	if(disnum == 0)
	{
		memcpy(m_displaybuf,(char *)m_WordBuffer ,modnum);
		m_dispLen = modnum;
	}
	else
	{
		if(page > disnum)
		{
			memcpy(m_displaybuf,(char *)(m_WordBuffer + (page-1)*CNDISNUM),modnum);
			m_dispLen = modnum;
		}
		else 
		{	
			memcpy(m_displaybuf,(char *)(m_WordBuffer + (page-1)*CNDISNUM),CNDISNUM);
			m_dispLen = CNDISNUM;
		}
	}
}

/****************************************************************
**功           能  :	获得当前汉字的所有页数
**输入参数:	无
**输出参数:	无
**返  回   值 :	返回当页数
*****************************************************************/	   
unsigned short CPinYinInput::GetMaxPage(void)
{
	unsigned short disnum = 0,modnum = 0;

	if(m_WordNum!=0)
	{
		disnum = m_WordNum/CNDISNUM;
		modnum = m_WordNum%CNDISNUM;

		if((disnum == 0)&&(modnum !=0))
		{
			disnum = 1;
		}
		else if((disnum !=0) && (modnum !=0))
		{
			disnum = disnum +1;
		}
	}
	
	return disnum;
}


/****************************************************************
**功           能  :	获取上一页的信息
**输入参数:	无
**输出参数:	无
**返  回   值 :	无
*****************************************************************/	   
bool CPinYinInput::GetPageUp(void)
{
	if(m_PageNum - 1 <= 0)
	{
		m_PageNum = 1;
		return false;
	}
	else
	{
		m_PageNum -= 1;
		GetDisplayPageInfo(m_PageNum);
		return true;
	}
}

/****************************************************************
**功           能  :	获取下一页的信息
**输入参数:	无
**输出参数:	无
**返  回   值 :	无
*****************************************************************/	   
bool CPinYinInput::GetPageDown(void)
{
	unsigned short  maxpage = 0;

	maxpage = GetMaxPage();
	
	if(maxpage)
	{	
		if(m_PageNum + 1 > maxpage)
		{
			return false;
		}
		else
		{
			m_PageNum += 1;
			GetDisplayPageInfo(m_PageNum);

			return true;
		}
	}

	return false;
}


/****************************************************************
**功           能  :	设置当前页并获取当前页的汉字
**输入参数:	page:页数
**输出参数:	无
**返  回   值 :	返回当页数
*****************************************************************/	   
void CPinYinInput::SetCurrPageInfo(unsigned short page)
{
	unsigned short  maxpage = 0;

	if(m_WordNum != 0)
	{
		maxpage = GetMaxPage ();

		if(page>maxpage)
		{
			return;
		}
		else
		{
			GetDisplayPageInfo(page);	
		}
	}
}

void CPinYinInput::ClearTextBuff()
{
	memset(m_displaybuf, 0, PY_DISP_BUFF_LEN);
}

/****************************************************************
**功           能  :	获得当前字母所对应的汉字串
**输入参数:	*str:当前拼接的串	
**输出参数:	无
**返  回   值 :	无
*****************************************************************/	   
void CPinYinInput::GetText(char * str)
{
	if((str == NULL)||(m_dispLen <= 0))
	{
		return;
	}

	//ucs2 to utf-8
	char displayWord[PY_DISP_BUFF_LEN] = {0};
	unsigned short* pWchar = reinterpret_cast<unsigned short*>(m_displaybuf);

	unsigned short c;
	int index = 0;
	int out_index = 0;
	c = pWchar[index++];
	while(c)
	{
		displayWord[out_index++] = 0x30 + index;

		if(c < 0x080)
		{
			displayWord[out_index++] = static_cast<unsigned char>(c);
		}
		else if(c < 0x800)
		{
			displayWord[out_index++] = 0xc0 | (c >> 6);
			displayWord[out_index++] = 0x80 | (c & 0x3f);
		}
		else
		{
			displayWord[out_index++] = 0xe0 | (c >> 12);
			displayWord[out_index++] = 0x80 | ((c >> 6) & 0x3f);
			displayWord[out_index++] = 0x80 | (c & 0x3f);
		}
		displayWord[out_index++] = 0x20;
		displayWord[out_index++] = 0x20;

		c = pWchar[index++];
		if(index * 2 > m_dispLen)
		{
			break;
		}
	}
	displayWord[out_index] = 0x00;

	strcpy(str,displayWord);
}


/****************************************************************
**功           能  :	获得所输入数字所对应的汉字
**输入参数:	*str: 当前查找到的汉字	
**输出参数:	无
**返  回   值 :	无
****************************************************************/	   
void CPinYinInput::GetSingleText(char * str)
{
	char displayWord[32]={0};
	BYTE number = 0;
	
	if((str == NULL) ||(m_dispLen <= 0)||((m_Inputnum <=0)
		||(m_Inputnum>CNDISNUM/2) ||(m_Inputnum > m_dispLen/2)))
	{
		return;
	}

	number =(m_Inputnum-1)*2;

	unsigned short c = *reinterpret_cast<unsigned short*>(m_displaybuf + number);
	//sprintf(strtmp,"%c%c",m_displaybuf[number],m_displaybuf[number+1]);
	
	int out_index = 0;
	if(c < 0x080)
	{
		displayWord[out_index++] = static_cast<unsigned char>(c);
	}
	else if(c < 0x800)
	{
		displayWord[out_index++] = 0xc0 | (c >> 6);
		displayWord[out_index++] = 0x80 | (c & 0x3f);
	}
	else
	{
		displayWord[out_index++] = 0xe0 | (c >> 12);
		displayWord[out_index++] = 0x80 | ((c >> 6) & 0x3f);
		displayWord[out_index++] = 0x80 | (c & 0x3f);
	}

	displayWord[out_index] = 0x0;

	strcpy(str, displayWord);

	//当用户选择完一个汉字后需要重新
	//输入新的拼音选择新的汉字
	if(strlen(str))
	{
		memset(m_displaybuf,0,PY_DISP_BUFF_LEN);
	}
}


