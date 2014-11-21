#define MAXWORD 500
#define ResWordNum 14
#define MAXC 50

int linenum=1;
char word[MAXWORD];
int numberoffunction[100];
int varoffunction[100];
int isback=0;
int syms;
char lastfunction[30];
int lastpcode;
int linenumber=1;
FILE *in ,*out;

char *ReserveWord[]=
{
	"int","char","float","void",
	"if", "while", "switch", "case", "default",
	"scanf", "printf" ,"return" ,"const","else"
};

char *Oprate[]=
{					//操作码
	"LDA","LDI","JME","JMC","JMP","CAS",
	"RED","EXF","WRR","INF","ADD","SUB","",
	"MUL","DIV","MUS",">","<","==","!=",">=",
	"<=","STO","CAL","MKF","EXP"
};
struct MyStruct
		{
			int adr;//运行栈入口地址
			int ref;//指向相应的表
			int lev;//在分程序的层次
			int geshu;
		};
union MyUnion
	{
		int valint;
		float valfloat;
		char valchar;
		MyStruct valfunc;
	};
struct Character
{
	char *name;    //符号表的变量名称
	int lev;	   //符号表的层数（在C0文法中为函数中的局部变量和全局变量）	
	int type;		//符号的类型
	int kind;		//是常量变量还是函数名称
	MyUnion f;
};
struct PCO
{
	char opr[5];
	int lev;
	float kind;
	char *str;
};
PCO PCODE[1000];
Character fuhaobiao[100][100];
int canshugeshu[100];
int flagerror;
int lev;//现在所处的层次
int fuhaohang=0,fuhaolie=0;

int interprete();                          //运行栈
int getsyms();									//词法分析程序
int enter(char* name,int type,int kind);			//把变量填入符号表
int constdeclaration();							//分析常量说明部分
int program();									//分析程序
int charcheck();								//检查标识符是否重复定义
int constdefine( int type );					//分析常量定义
int vardefine( int type);						//分析变量说明部分  
int returnfunction( int type)	;				//有返回值的函数
int voidfunction( );							//无返回值的函数定义语句
int statementArray( );							//语句列
int statement( );								//语句
int ifstatement( );								//if条件语句
int whilestatement( );							//while循环语句
int switchstatement( );                         //switch语句
int scanfstatement( );							//scanf读语句
int printfstatement( );							//printf写语句
int returnstatement( );							//return语句
int assignstatement(int a);						//赋值语句
int mainfunction( );							//主函数
int call(int offset);								//函数调用语句
int expression();								//表达式
int gen(char *op,int lev,int x);					//产生一条表达式
int term();										//项
int factor();									//因子
int find(int l);//在符号表中查找标识符是否存在
int find(int l,char *tword);
void error();                                    //错误处理			
/*
	标识符			1				内部字符串
	整数			2				整数值
	浮点数          3               浮点数
	字符			4				字符
	字符串			5				字符串
	变量            6               变量名称
 
    int  			10				---
	char			11				---
	float			12				---
	void			13				---
	if	 	     	14				---
	while			15				---
	switch			16				---
	case        	17				---
	default			18				---
	scanf			19				---
	printf			20				---
	return			21	 			---
	const           22              ---
	else            23              ---
	＋				31				---
	－				32				---
	*				33				---
	／				34				---
	}				35				---
	）				36				---
	（				37				---
	；				38				---
	，				39				---
	{				40				---
	"				41				---
 	"				42				---
  	'				43				---
	'				44				---
	>				45				---
	<				46				---
	==				47				---
	!=              48              ---
	>=				49				---
	<=				50				---	
	=				51				---
	[               52              ---
	]               53              ---
	:               54              ---
	*/

/*
opr 0 14		浮点数加法
opr 0 15		浮点数减法
opr 0 16		浮点数乘法
opr 0 17		浮点数除法

prs 打印字符串
pri 打印整形
prf 打印浮点数
prc 打印字母
rei 读取int
ref	读取float
rec 读取char
*/