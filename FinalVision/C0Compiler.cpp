#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include "C0Complier.h"
using namespace std;

char c;
int codei=1,isfunction=0;
char getnext()
{
	char temp;
	while((temp=fgetc(in))==' ')
		;
	ungetc(temp,in);
	return temp;
}
int error(int i)
{
	flagerror=1;
	printf("%d"" : ",linenumber);
	switch (i)
	{
	case 1:
		printf("没有找到该变量""%s\n",word);
		break;
	case 2:
		printf("此行或上一行缺少分号\n");
		return 0;
	case 3:
		printf("缺少返回值\n");
		return 0;
	case 4:
		printf("void不能有返回值\n");
		return 0;
	case 5:
		printf("返回值类型有误\n");
		return 0;
	case 6:
		printf("缺少(\n");
		return 0;
	case 7:
		printf("参数个数不符合\n");
		return 0;
	case 8:
		printf("变量重复定义\n");
		return 0;
	case 9:
		printf("常量无法赋值\n");
		return 0;
	case 10:
		printf("非法字符\n");
		return 0;
	case 11:
		printf("缺少类型标识符\n");
		return 0;
	case 12:
		printf("缺少变量\n");
		return 0;
	case 13:
		printf("缺少逗号\n");
		return 0;
	case 14:
		printf("缺少单引号\n");
		return 0;
	case 15:
		printf("缺少双引号\n");
		return 0;
	case 16:
		printf("赋值语句赋值类型出错\n");
		return 0;
	case 17:
		printf("数组定义错误\n");
		return 0;
	case 18:
		printf("数组使用错误\n");
		return 0;
	case 19:
		printf("数组缺少]");
		return 0;
	case 20:
		printf("缺少)\n");
		return 0;
	case 21:
		printf("缺少}\n");
		return 0;
	case 22:
		printf("缺少:\n");
		return 0;
	case 24:
		printf("case之后不是可枚举常量\n");
		return 0;
	case 25:
		printf("参数类型不符合\n");
		return 0;
	case 26:
		printf("无返回值无法调用\n");
		return 0;
	default:
		break;
	}
	while((c=fgetc(in))!=';')
				;
	//syms=getsyms();
	return 0;
}
int gen(char *name,int lev,int kind)
{
	PCO temp;
	strcpy(temp.opr,name);
	temp.lev=lev;
	temp.kind=kind;
	PCODE[codei++]=temp;
	return 0;
}
int gen(char *name,int lev,float num)
{
	PCO temp;
	strcpy(temp.opr,name);
	temp.lev=lev;
	temp.kind=num;
	PCODE[codei++]=temp;
	return 0;
}
int gen(char *name,int lev,char *str)
{
	PCO temp;
	strcpy(temp.opr,name);
	temp.lev=lev;
	temp.str=(char*)malloc(sizeof(char)*strlen(str));
	strcpy(temp.str,str);
	PCODE[codei++]=temp;
	return 0;
}
int expression()
{
	int op=syms;
	int x=0, y=0;
	int jioujilu=1;
	syms=getsyms();
	if(syms==31||syms==32)
	{
		int re;
		re=syms;
		syms=getsyms();
		x=term();
		//syms=getsyms();
		jioujilu++;
		if(re==32)
			gen("opr",0,1);
	}
	else
	{
		x=term();
	}
	while(syms==31||syms==32)
	{
		int t;
		t=syms;
		syms=getsyms();
		if(jioujilu%2==0)
		{
			y=term();
			jioujilu++;
		}
		else
		{
			x=y;
			y=term();
			jioujilu++;
		}
		if(t==32)
			gen("opr",0,3);
		else
			gen("opr",0,2);
	}
//	syms=getsyms();
	if(x==13)
		return 13;
	if(x==100||y==100)
		return 100;
	if(x==12||y==12)
		return 12;
	else if (x==10||y==10)
		return 10;
	else
		return 11;
}
int term()
{
	int jioujilu=1;
	int x=0,y=0;
	x=factor();
	while((c=fgetc(in))==' ')
		;
	if(c=='+'||c=='-'||c=='*'||c=='/')
	{
		ungetc(c,in);
		syms=getsyms();
	}
	else
		ungetc(c,in);
	//syms=getsyms();
	jioujilu++;
	//if(c!='+'&&c!='-'&&c!='*'&&c!='/')
		//ungetc(c,in);
	while(syms==33||syms==34)
	{
		int op;
		op=syms;
		if(jioujilu%2==0)
		{
			syms=getsyms();
			y=factor();
			jioujilu++;
		}
		else
		{syms=getsyms();
			x=y;
			y=factor();
			jioujilu++;
		}
		if(op==33)
		{
			gen("opr",0,4);
		}
		else
		{
			gen("opr",0,5);
		}
		while((c=fgetc(in))==' ')
			;
		if(c=='+'||c=='-'||c=='*'||c=='/')
		{
			ungetc(c,in);
			syms=getsyms();
		}
		else
			ungetc(c,in);
	}
	if(x==13)
		return 13;
	else if(x==100||y==100)
		return 100;
	else if(x==12||y==12)
		return 12;
	else if (x==10||y==10)
		return 10;
	else
		return 11;
}
int call(int offset)
{
	syms=getsyms();
	if(syms==37)
	{
		int num=0;
		while(1)
		{
			if((c=fgetc(in))==')')
				break;
			ungetc(c,in);
			num++;
			int t;
			t=expression();
		  //syms=getsyms();
			//if(syms!=39&&syms!=36)
			c=fgetc(in);
			int l=1;
			for(int j=0;j<offset;j++)
			{
				if(fuhaobiao[0][j].kind==2)
					l=l+1;
			}
			if(fuhaobiao[l][num-1].type!=t)
				error(25);

			if(c==')')
				break;
			else if(c==',')
				;
			else
				error(13);

		}
		//syms=getsyms();
		lastpcode=codei;
		if(num==fuhaobiao[0][offset].f.valfunc.geshu)
			gen("cal",num,fuhaobiao[0][offset].f.valfunc.adr);
		else
			error(7);
		return fuhaobiao[0][offset].type;
	}
	else
		error(6);
}
int factor()
{
	if(syms==37)
	{
		//syms=getsyms();
		int p=expression();
		syms=getsyms();
		return p;
	}
	int lev,offset;
	Character temp;
	temp.type=10;
	if(syms==2)
	{
		gen("lit",0,atoi(word));
		return 10;
	}
	else if(syms==4)
	{
		gen("lit",0,word[0]);
		return 11;
	}
	else if(syms==3)
	{
		gen("lit",0,(float)atof(word));
		return 12;
	}
	else if(syms==6)
	{
		int thisoffset,mainoffset;
		thisoffset=find(fuhaohang);
		mainoffset=find(0);
		if(thisoffset!=100)
		{
			offset=thisoffset;
			lev=1;
			if(offset<0)
				temp=fuhaobiao[fuhaohang][-(offset+1)];
			else
				temp=fuhaobiao[fuhaohang][offset];
		}
		else if(mainoffset!=100)
		{
			offset=mainoffset;
			lev=0;
			if(offset<0)
				temp=fuhaobiao[0][-(offset+1)];
			else
				temp=fuhaobiao[0][offset];
		}
		else
		{
			error(1);
			return 100;
		}
		if(temp.kind==0)
		{
			if(temp.type==10)
				gen("lit",lev,temp.f.valint);
			if(temp.type==11)
				gen("lit",lev,temp.f.valchar);
			if(temp.type==12)
				gen("lit",lev,temp.f.valfloat);
		}
		if(temp.kind==1)
		{
			if(temp.type==10)
				gen("lod",lev,offset);
			if(temp.type==11)
				gen("lod",lev,offset);
			if(temp.type==12)
				gen("lod",lev,offset);
		}
		if(temp.kind==2)
		{
			int offset=find(0);
			return 	call(offset);
		}
	}
	return temp.type;
}
int assignstatement()
{
	int lev,offset;
	int thisfind=100,mainfind=100;
	thisfind=find(fuhaohang);
	mainfind=find(0);
	if(thisfind!=100)
	{
		offset=thisfind;
		lev=1;
	}
	else if(mainfind!=100)
	{
		offset=mainfind;
		lev=0;
	}
	else
	{
		error(1);
		return 100;
	}
	int re;
	re=getsyms();
	int q;
	if(thisfind!=100)
		q=fuhaohang;
	else
		q=0;
	if(re==51)
	{
		int t;
		t=expression();
		if(offset >=0)
			{
				if(fuhaobiao[q][offset].kind==0)
					error(9);
				//if(t!=fuhaobiao[q][offset].type)
					//error(16);
				if(fuhaobiao[q][offset].type==10)
					gen("stoi",lev,offset);
				else
					gen("sto",lev,offset);
			}
		else
			{
				if(fuhaobiao[q][-offset-1].kind==0)
					error(9);
				//if(t!=fuhaobiao[q][-offset-1].type)
					//error(16);
				if(fuhaobiao[q][-offset-1].type==10)
					gen("stoi",lev,offset);
				else
					gen("sto",lev,offset);
			}
	}
	else
	{
		printf("赋值语句出现错误");
	}
	char tt;
	tt=getnext();
	if(tt!=';')
	{
		error(2);
		//printf("%d",syms);
	}
	else
		syms=getsyms();
	return 0;
}
int enter(char name[],int type,int kind)
{
	Character temp;
	temp.lev=lev;
	temp.kind=kind;
	temp.name=(char *)malloc(sizeof(char)*50);
	strcpy(temp.name,name);
	temp.type=type;
	if(kind!=2)
	{
		if(type==10)
			temp.f.valint=atoi(word);
		if(type==11)
		{
			temp.f.valchar=word[0];
		}
		if(type==12)
			temp.f.valfloat=atof(word);
	}
	else
		{
			temp.f.valfunc.adr=codei;
			temp.f.valfunc.ref=fuhaohang+1;
			temp.f.valfunc.lev=lev;
	}
	if(type==11)
	{
		if(find(fuhaohang,temp.name)==100)
			fuhaobiao[fuhaohang][fuhaolie++]=temp;
		else
			error(8);
	}
	else
	{
		if(find(fuhaohang)==100)
			fuhaobiao[fuhaohang][fuhaolie++]=temp;
		else
			error(8);
	}
	isfunction=0;
	return 0;
}
int enter(char name[],int type,int kind,int a)
{
	Character temp;
	temp.lev=lev;
	temp.kind=kind;
	temp.name=(char *)malloc(sizeof(char)*50);
	strcpy(temp.name,name);
	temp.type=type;
	if(kind!=2)
	{
		if(type==10)
			temp.f.valint=atoi(word);
		if(type==11)
		{
			temp.f.valchar=word[0];
		}
		if(type==12)
			temp.f.valfloat=atof(word);
	}
	else
		{
			temp.f.valfunc.adr=codei;
			temp.f.valfunc.ref=fuhaohang+1;
			temp.f.valfunc.lev=lev;
	}

	int i=0;
	for(;fuhaobiao[0][i].name!=NULL;i++)
		;
	//fuhaobiao[0][i]=temp;
	if(find(0)==100)
		fuhaobiao[0][i]=temp;
	else
		error(8);
	isfunction=0;
	return 0;
}
int returnfunction( int type)	//有返回值的函数
{
	int re,num=0,flag=0,offset,ret=0;
	fuhaohang++;
	fuhaolie=0;
	offset=find(0);
	while(1)		
	{
		syms=10;
		syms=getsyms();		
		if(syms==36)
		{
			syms=getsyms();
			break;
		}
		if(syms==10||syms==11||syms==12)
		{
			int t=getsyms();
			if(t!=6)
				error(12);			
		}
		else
		{
			error(11);
		}
		if(syms==36)
			break;
		enter(word,syms,1);
		num++;
		int t;

		char tt;
		tt=getnext();
		if(tt==')')
		{
			syms=getsyms();
			break;
		}
		else if(tt==',')
		{
			syms=getsyms();
		}
		else
		{
			error(13);
			break;
		}
		/*if((t=getsyms())==36)
			break;
		else if(t!=39)
			error(13);
		strcpy(word,lastfunction);*/
	}
	fuhaobiao[0][offset].f.valfunc.geshu=num;
	fuhaobiao[0][offset].f.valfunc.adr=codei;
	numberoffunction[fuhaohang]=num;

	if(num!=0)
		syms=getsyms();
					int j;
	while((syms=getsyms())!=35)
	{
		if(syms==10||syms==11||syms==12)
		{
			vardefine(syms);
		}
		else if(syms==22)
			constdeclaration();
		/*else if(syms==20)
			printfstatement();*/
		else if(syms==23)
		{
			int nowcode=codei;
			codei++;
			syms=getsyms();
			statement();
			PCO temp;
			strcpy(temp.opr,"jmp");
			temp.lev=0;
			temp.kind=codei;
			PCODE[nowcode]=temp;
		}
		else if(syms==21)
		{
			//syms=getsyms();
			if(flag==0)
			{
				gen("int",1,fuhaolie);
				flag=1;	

			}/*
			int t=expression();
			if(fuhaobiao[0][offset].type!=t)
				error(5);
			gen("ret",0,lastpcode);*/
			//return 0;
			ret=1;
			returnstatement();
		}
		else
		{
			if(flag==0)
			{
				gen("int",1,fuhaolie);
				flag=1;	
			}
			//syms=getsyms();
			statement();
		}
	}
	if(isback==0)
		error(3);
	else isback=0;
	return 0;
}
int mainfunction()
{
	PCO temp;
	strcpy(temp.opr,"jmp");
	temp.lev=1;
	temp.kind=codei;
	PCODE[0]=temp;
	int i=0;
	for(;fuhaobiao[0][i].name!=NULL;i++)
	{
		;
	}
	gen("int",0,i);
	int re,num=0,flag=0,offset;
	fuhaohang++;
	fuhaolie=0;
	offset=find(0);
	while(1)		
	{
		syms=getsyms();
		getsyms();
		if(syms==36)
			break;
		enter(word,syms,1);
		num++;
		if(getsyms()==36)
			break;
		strcpy(word,lastfunction);
	}
	fuhaobiao[0][offset].f.valfunc.geshu=num;
	fuhaobiao[0][offset].f.valfunc.adr=codei;
	numberoffunction[fuhaohang]=num;
	int j;
	for( j=num;j>0;j--)
		gen("sto",1,j-1);
	if(num!=0)
		syms=getsyms();
	while(syms!=35&&(syms=getsyms())!=35&&c!=-1)
	{
		if(syms==10||syms==11||syms==12)
		{
			vardefine(syms);
		}
		else if(syms==22)
			constdeclaration();
		else if(syms==23)
		{
			syms=getsyms();
			statement();
		}
		else if(syms==21)
		{
			//syms=getsyms();
			expression();
			return 0;
		}
		else
		{
			if(flag==0)
			{
				gen("cal",0,codei+1);
				gen("int",1,fuhaolie);
				flag=1;
			}
			//syms=getsyms();
			statement();
		}
	}
	//printf("%d""缺少返回值",linenum);
	if(c==-1)
		error(21);
	return 0;
}
int voidfunction()
{
	int re,num=0,flag=0,offset;
	fuhaohang++;
	fuhaolie=0;
	offset=find(0);
	while(1)		
	{
		syms=getsyms();
		getsyms();
		if(syms==36)
			break;
		enter(word,syms,1);
		num++;
		if(getsyms()==36)
			break;
		strcpy(word,lastfunction);
	}
	fuhaobiao[0][offset].f.valfunc.geshu=num;
	fuhaobiao[0][offset].f.valfunc.adr=codei;
	numberoffunction[fuhaohang]=num;
	int j;
	if(num!=0)
		syms=getsyms();
	while((syms=getsyms())!=35)
	{
		if(syms==10||syms==11||syms==12)
		{
			vardefine(syms);
		}
		else if(syms==22)
			constdeclaration();
		else if(syms==23)
		{
			int nowcode=codei;
			codei++;
			syms=getsyms();
			statement();
			PCO temp;
			strcpy(temp.opr,"jmp");
			temp.lev=0;
			temp.kind=codei;
			PCODE[nowcode]=temp;
		}
		else if(syms==21)
		{
			syms=getsyms();
			if(syms==38)
				;
			else
			{
				error(4);
				return 0;
			}
		}

		else
		{
			if(flag==0)
			{
				gen("int",1,fuhaolie);
				flag=1;
			}
			//syms=getsyms();
			statement();
		}
	}
				gen("ret",0,lastpcode);
	return 0;
}
int find(int l)
{
	int i;
	int c;
	for(i=0;i<100;i++)
	{
		if(fuhaobiao[l][i].name!=NULL)
		{
			if(strcmp(fuhaobiao[l][i].name,word)==0&&((c=fgetc(in))!='['))
			{
				ungetc(c,in);
				return i;
			}
			else if(strcmp(fuhaobiao[l][i].name,word)==0&&c=='[')
			{
				int re;
				re=expression();
				if(re!=10)
					error(18);
				syms=getsyms();
				if(syms!=53)
					error(19);
				return (-i)-1;
			}
		}
		else 
			return 100;
	}
}
int find(int l,char *tword)
{
	int i;
	int c;
	for(i=0;i<100;i++)
	{
		if(fuhaobiao[l][i].name!=NULL)
		{
			if(strcmp(fuhaobiao[l][i].name,tword)==0&&((c=fgetc(in))!='['))
			{
				ungetc(c,in);
				return i;
			}
			else if(strcmp(fuhaobiao[l][i].name,tword)==0&&c=='[')
			{
				int re;
				re=expression();
				if(re!=10)
					error(18);
				syms=getsyms();
				if(syms!=53)
					error(19);
				return (-i)-1;
			}
		}
		else 
			return 100;
	}
}
int scanfstatement()
{
	int offset,lev;
	Character temp;
	syms=getsyms();
	if(syms==37)
	{
		syms=getsyms();
		while(1)
		{
			find(0);
			if(find(fuhaohang)!=100)
			{
				offset=find(fuhaohang);
				lev=1;
				temp=fuhaobiao[fuhaohang][offset];
			}
			else if(find(0)!=100)
			{
				offset=find(0);
				lev=0;
				temp=fuhaobiao[0][offset];
			}
			else
				printf("%d"," 找不到该标识符",linenum);
			if(temp.type==10)
				gen("rei",0,0);
			else if(temp.type==11)
				gen("rec",0,0);
			else if(temp.type==12)
				gen("ref",0,0);
			gen("sto",lev,offset);
			syms=getsyms();
			if(syms==36)
				break;
			else
				syms=getsyms();
		}
	}
	else
		error(6);
	while((c=fgetc(in))==' ')
		;
	if(c!=';')
	{
		error(2);
		ungetc(c,in);
	}
	else
	{
		ungetc(c,in);
		syms=getsyms();
	}

	return 0;
}
int printfstatement()
{
	syms=getsyms();
	if(syms==37)
	{
		int ta;
		ta=fgetc(in);
		//syms=getsyms();
		if(ta=='"')
		{
			ungetc(ta,in);
			syms=getsyms();
			for(int j=0;j<strlen(word);j++)
			{
				char t;
				t=word[j];
				gen("lit",0,t);
				gen("prc",0,0);
			}
			syms=getsyms();
			if(syms==39)
			{
				int flag;
				flag=expression();
				if(flag==10)
					gen("pri",0,0);
				else if(flag==11)
					gen("prc",0,0);
				else if(flag==12)
					gen("prf",0,0);
			}
			else
				ungetc(c,in);
		}
		else
		{
			int flag;
			ungetc(ta,in);
			flag=expression();
			if(flag==10)
				gen("pri",0,0);
			else if(flag==11)
				gen("prc",0,0);
			else if(flag==12)
				gen("prf",0,0);
			else if(flag==13)
				error(26);
		}
	}
	else
	{
		error(6);
	}
	while((c=fgetc(in))==' ')
		;
	if(c!=')')
	{
		error(20);
		ungetc(c,in);
	}
	else
	{
		ungetc(c,in);
		syms=getsyms();//右括号
		//c=fgetc(in);
		while((c=fgetc(in))==' ')
		;
		if(c!=';')
		{
			error(2);
			ungetc(c,in);
		}
		else
		{
			ungetc(c,in);
			syms=getsyms();//分号
		}
	}
	return 0;
}
int whilestatement()
{
	int opr;
	int nowcode,startcode;
	syms=getsyms();
	startcode=codei;
	if(syms==37)
	{
		expression();
		syms=getsyms();
		if(syms!=36)
		{
			switch (syms)
			{
				case 47:
						opr=8;
						break;
				case 45:
					opr=12;
					break;
				case 46:
					opr=10;
					break;
				case 48:
					opr=9;
					break;
				case 49:  opr=11;
					break;
				case 50: opr=13;
					break;
				default:
					break;
			}
			expression();
			gen("opr",0,opr);
		}
		else
		{
			gen("lit",0,0);
			gen("opr",0,9);
			ungetc(c,in);
		}
		nowcode=codei;
		codei++;
		
		char tt;
		tt=getnext();
		if(tt==')')
		{
			syms=getsyms();
		}
		else
		{
            error(20);
		}

		syms=getsyms();

		statement();
		gen("jmp",0,startcode);
		PCO temp;
		strcpy(temp.opr,"jpc");
		temp.lev=0;
		temp.kind=codei;
		PCODE[nowcode]=temp;
	}
	else
		printf("%d""while缺少参数",linenum);
	return 0;
}
int ifstatement()
{
	int opr;
	int nowcode;
	syms=getsyms();
	if(syms==37)
	{
		expression();
		syms=getsyms();
		if(syms!=36)
		{
			switch (syms)
			{
				case 47:
					opr=8;
					break;
				case 45:
					opr=12;
					break;
				case 46:
					opr=10;
					break;
				case 48:
					opr=9;
					break;
				case 49:  opr=11;
					break;
				case 50: opr=13;
					break;
				default:
					break;
			}
			expression();
			gen("opr",0,opr);
		}
		else
		{
			gen("lit",0,0);
			gen("opr",0,9);
			ungetc(c,in);
		}
		nowcode=codei;
		codei++;
		char tt;
		tt=getnext();
		if(tt==')')
		{
			syms=getsyms();
		}
		else
		{
            error(20);
		}

		syms=getsyms();//吃右括号
		statement();

		PCO temp;
		strcpy(temp.opr,"jpc");
		temp.lev=0;
		temp.kind=codei+1;
		PCODE[nowcode]=temp;
		int nowcode=codei;
		codei++;
		syms=getsyms();

		if(syms==38)
			syms=getsyms();
		if(syms==23)
		{
			syms=getsyms();
			statement();
			strcpy(temp.opr,"jmp");
			temp.lev=0;
			temp.kind=codei;
			PCODE[nowcode]=temp;
		}
		else
		{
			if(syms==35)
				ungetc('}',in);
			else
				statement();
		}
	}
	else
		printf("%d""if缺少参数",linenum);
	return 0;
}
int returnstatement()
{
	isback=1;
	syms=getsyms();
	if(syms==38)
		return 0;
	if(syms!=37)
		error(6);
	int t=expression();
	gen("ret",0,0);
	//syms=getsyms();//)
	char tt;
	tt=getnext();
	if(tt!=')')
	{
		error(20);
		//printf("%d",syms);
	}
	else
	{
		syms=getsyms();
		char tt;
		tt=getnext();
		if(tt!=';')
		{
			error(2);
			//printf("%d",syms);
		}
		else
			syms=getsyms();
	}
//	syms=getsyms();//;
	return 0;
}
int statement()
{
	if(syms==6)
	{
		c=fgetc(in);
		if(c=='(')
		{
			ungetc(c,in);
			factor();
			syms=getsyms();
		}
		else
		{
			ungetc(c,in);
			assignstatement();
		}
	}
	else if(syms==19)
		scanfstatement();
	else if(syms==40)
	{
		syms=getsyms();
		while(syms!=35)
		{
			statement();
			if(syms==38||syms==36||syms==35)
				syms=getsyms();
		}
	}
	else if(syms==16)
		switchstatement();
	else if(syms==20)
		printfstatement();
	else if(syms==14)
		ifstatement();
	else if(syms==15)
		whilestatement();
	else if(syms==21)
		returnstatement();
	return 0;
}
int getsyms()
{
	c=fgetc(in);
	if (c==EOF)
		return 0;
	while(c!=EOF)
	{
		if(isspace(c)==0)
		{

			if(linenum==1)
			{
				fprintf(out,"%d ",linenum);
				linenum++;
			}
			else
			{
				fprintf(out,"\n%d ",linenum);
				linenum++;
			}
		}

		if(isspace(c))
		{
			/*
			linenum++;

			if(c=='\n')
			{
				fputc(c,out);
				//fprintf(out,"%d",linenum);
				fputc(' ',out);
			}
			fputc(c,out);*/
			if(c=='\n')
				linenumber++;
			printf("%c",c);
			c=fgetc(in);
			continue;
		}
		
		if(c==';')
		{
				fprintf(out,"%s","SEMICN");
				fprintf(out,";");
				printf(";");
				//c=fgetc(in);
				return 38;
		}
		if(isalpha(c))
		{
			int i=0;
			word[i++]=c;
			while(isalnum(c)||c=='_')
			{
				c=fgetc(in);
				if(isalnum(c)==0)
				{
						ungetc(c,in);
						break;
				}
				if(i>MAXWORD)
				{
				}
				else
				{
					word[i++]=c;
				}
			}
			word[i]=0;
			int j;
			for( j=0;j<ResWordNum;j++)
			{
				if(strcmp(ReserveWord[j],word)==0)
				{
					char b[100];
					for(int i=0;word[i]!=0;i++)
						b[i]=toupper(word[i]);
					b[i++]='T';
					b[i++]='K';
					b[i++]=0;
					fprintf(out,"%s ",b);
					fprintf(out,"%s",word);
					break;
				}
			}
							printf("%s",word);
			if(j==ResWordNum)
			{
				fprintf(out,"IDEN ");
				fprintf(out,"%s",word);
				return 6;
			}
				else
					return j+10;
			c=fgetc(in);
			continue;
		}
		if(isdigit(c))
		{
			int i=0,flag=0;
			while(isdigit(c)||c=='.')
			{
				word[i++]=c;
				c=fgetc(in);
				if(c=='.')
					flag=1;
			}
			if(isalpha(c))
			{
				printf("error");
			}
			word[i]=0;
			if(flag==1)
			{
					fprintf(out,"FLOATCON ");
								ungetc(c,in);
			fprintf(out,"%s",word);	printf("%s",word);
					return 3;
			}
			else
			{
				fprintf(out,"INTCON ");
							ungetc(c,in);
			fprintf(out,"%s",word);	printf("%s",word);
				return 2;
			}
						
			c=fgetc(in);
			continue;
		}
		if(c=='=')
		{
			char t=fgetc(in);
			if(t!='=')
			{
				fprintf(out,"ASSIGN ");
				fprintf(out,"=");
				printf("=");
				//c=fgetc(in);
				ungetc(t,in);
				return 51;
			}
			else
			{
				fprintf(out,"EQL ");
				fprintf(out,"==");
				printf("==");
				//c=fgetc(in);
				//ungetc(t,in);
				return 47;
			}
		}
		if(c=='!')
		{
			c=fgetc(in);
			printf("!=");
			if(c=='=')
				return 48;
			else
				ungetc(c,in);

		}
		if(c=='-')
		{
			fprintf(out,"MINU ");
			fprintf(out,"-");
			printf("-");
			///c=fgetc(in);
			return 32;
		}
		if(c=='+')
		{
			fprintf(out,"PLUS ");
			fprintf(out,"+");
						printf("+");
			return 31;
		}
		if(c=='/')
		{ 
			fprintf(out,"DIV ");
			fprintf(out,"/");			printf("/");
			return 34;
		}
		if(c=='*')
		{
			fprintf(out,"MUL ");
			fprintf(out,"*");			printf("*");
			//c=fgetc(in);
			return 33;
		}
		if(c=='(')
		{
			fprintf(out,"LPARENT ");
			fprintf(out,"(");			printf("(");
			//c=fgetc(in);
			return 37;
		}
		if(c==')')
		{
			fprintf(out,"RPARENT ");
			fprintf(out,")");			printf(")");
			//c=fgetc(in);
			return 36;
		}
		if(c=='[')
		{
			fprintf(out,"LBRACK ");
			fprintf(out,"[");			printf("[");
			//c=fgetc(in);
			return 52;
		}
		if(c==']')
		{
			fprintf(out,"RPARENT ");
			fprintf(out,"[");			printf("]");
			//c=fgetc(in);
			return 53;
		}
		if(c=='{')
		{
			fprintf(out,"LBRACE ");
			fprintf(out,"{");			printf("{");
			//c=fgetc(in);
			return 40;
		}
		if(c=='}')
		{
			fprintf(out,"RBRACE ");			printf("}");
			fprintf(out,"}");
			//c=fgetc(in);
			return 35;
		}
		if(c==',')
		{
			fprintf(out,"COMMA ");			printf(",");
			fprintf(out,",");
			//c=fgetc(in);
			return 39;
		}
		if(c=='\'')
		{
			c=fgetc(in);
			//printf("\n%d ",linenum);
			//fprintf(out,"\n%d ",linenum);
			//fprintf(out,"CHARCON ");
			//printf("%c",c);
			word[0]=c;
			word[1]=0;
			fprintf(out,"%c",c);
			printf("%c",c);
			c=fgetc(in);
			if(c=='\'')
			{
				/*printf("\n%d ",linenum);
				fprintf(out,"\n%d ",linenum++);
				printf("QMARK ");
				fprintf(out,"QMARK ");
				printf("\'");
				fprintf(out,"\'");*/
			}
			else
			{
				error(15);
			}
			return 4;
			c=fgetc(in);
			continue;
		}
		if(c=='"')
		{
			int j=0;
			c=0;
			while(c!='"')
			{
				c=fgetc(in);
				if(c=='\n')
					error(14);
				word[j++]=c;
				if(j>MAXC)
				{
					printf("error");
				}
			}
			word[--j]=0;
			fprintf(out,"\n%d ",linenum++);
			fprintf(out,"STRCON ");
			fprintf(out,"%s",word);
			printf("%s",word);
			return 5;
		}
		if(c=='>')
		{
			char t=fgetc(in);
			if(t!='=')
			{
				fprintf(out,"GRE ");
				fprintf(out,">");
				//c=fgetc(in);
				ungetc(t,in);
							printf(">");
				return 45;
			}
			else
			{
				fprintf(out,"GEQ ");
				fprintf(out,">=");			printf(">=");
				//c=fgetc(in);
				return 49;
			}
		}
		if(c==':')
			{
							printf(":");
				return 54;
		}
		if(c=='<')
		{
			char t=fgetc(in);
			if(t!='=')
			{
				fprintf(out,"LSS ");
				fprintf(out,"<");
				//c=fgetc(in);			
				printf("<");
				ungetc(t,in);
				return 46;
			}
			else
			{
				fprintf(out,"LEQ ");
				fprintf(out,"<=");			printf("<=");
				//c=fgetc(in);
				return 50;
			}
		}
		if(c>255)
			error(10);
	}
}
int constdeclaration()
{
	int type,re,flag=0;
	char lastword[50];
	re=getsyms();
	while(1)
	{
		
		if(re==38)
			return 0;
		else if (re==10||re==11||re==12)
		{
			type=re;
			while(1)
			{
				re=getsyms();
				if(re==39)
					flag=0;
				else if (re==6)
				{
					strcpy(lastword,word);
					if((re=getsyms()==51))
					{
						re=getsyms();
						if (re==32)
						{
							re=getsyms();
							flag=1;
						}
						if(type==10&&re==2||type==11&&re==4||type==12&&re==3)
							enter(lastword,type,0);
						else
							error(16);
						if(flag==1&&type==12)
							fuhaobiao[fuhaohang][fuhaolie-1].f.valfloat=-fuhaobiao[fuhaohang][fuhaolie-1].f.valfloat;
						if(flag==1&&type==10)
							fuhaobiao[fuhaohang][fuhaolie-1].f.valint=-fuhaobiao[fuhaohang][fuhaolie-1].f.valint;
					}
				}
				else
				{
					if(re==38)
						return 0;
					else
					{
						error(2);
						return 1;
					}
				}
			}
		}
/*		c=fgetc(in);
		while(isspace(c))
			c=fgetc(in);
		if(c==';'||c==',')
			ungetc(c,in);
		else
		{
			ungetc(c,in);
			error(2);
			return 1;
		}*/
		re=getsyms();
	}

	return 0;
}
/*int constdeclaration()
{
	int type,re,flag=0;
	char lastword[50];
	re=getsyms();
	while(1)
	{
		
		if(re==38)
			return 0;
		else if (re==10||re==11||re==12)
			type=re;
		else if(re==39)
			flag=0;
		else if (re==6)
		{
			strcpy(lastword,word);
			if((re=getsyms()==51))
			{
				re=getsyms();
				if (re==32)
				{
					re=getsyms();
					flag=1;
				}
				if(type==10&&re==2||type==11&&re==4||type==12&&re==3)
					enter(lastword,type,0);
				else
					error(16);
				if(flag==1&&type==12)
					fuhaobiao[fuhaohang][fuhaolie-1].f.valfloat=-fuhaobiao[fuhaohang][fuhaolie-1].f.valfloat;
				if(flag==1&&type==10)
					fuhaobiao[fuhaohang][fuhaolie-1].f.valint=-fuhaobiao[fuhaohang][fuhaolie-1].f.valint;
			}
			else
				printf("常量定义等号前面无变量");
		}
/*		c=fgetc(in);
		while(isspace(c))
			c=fgetc(in);
		if(c==';'||c==',')
			ungetc(c,in);
		else
		{
			ungetc(c,in);
			error(2);
			return 1;
		}
		re=getsyms();
	}

	return 0;
}*/
int vardefine(int type)
{
	syms=getsyms();
	while(syms!=38&&syms!=36&&syms!=40&&c!=-1&&c!='\n')
	{
		if(syms==6)
		{
			syms=getsyms();
			if(syms==37)
			{
				enter(word,type,2,0);
				strcpy(lastfunction,word);
				if(type==10||type==11||type==12)
				{
					if(strcmp(word,"main")==0)
						mainfunction();
					else
						returnfunction(type);
				}
				if(type==13)
				{
					if(strcmp(word,"main")==0)
						mainfunction();
					else
						voidfunction();
				}
			}
			else if(syms==52)
			{
				char *lastword;
				lastword=(char*)malloc(sizeof(char)*strlen(word));
				strcpy(lastword,word);
				int t;
				t=getsyms();
				int i=5;
				if(t==2)
				i=atoi(word);
				else
				error(17);
				enter(lastword,type,1);
				for(int j=1;j<i;j++)
				{
					char newword[50];
					char shuzi[5];
					strcpy(newword,lastword);
					sprintf(shuzi,"%d",j);
					strcat(newword,"[");
					strcat(newword,shuzi);
					strcat(newword,"]");
					enter(newword,type,1);
				}
			}
			else
			{
				enter(word,type,1);
			}
			if(syms==38)
				return 0;
			if(syms==35)
				return 0;
		}
		syms=getsyms();
	}
	if(c=='\n')
		error(2);
	return 0;
}
int switchstatement()
{
	//syms=getsyms();
	int from=codei;
	int lastcode;
	syms=getsyms();//左括号
	expression();
	char t;
	t=getnext();
	if(t==')')
		syms=getsyms();//右括号
	else
		error(20);
	syms=getsyms();//左大括号
	syms=getsyms();//第一个case
	if(syms==17)
	{
		expression();
		if(syms!=2&&syms!=3&&syms!=4)
			error(24);
		gen("cas",0,0);
		lastcode=codei;
		codei++;
		char t;
		t=getnext();
		if(t==':')
			syms=getsyms();//冒号
		else
			error(22);
		syms=getsyms();//语句第一个
		statement();
		codei++;
		//syms=getsyms();//分号
		PCO temp;
		strcpy(temp.opr,"jpc");
		temp.lev=0;
		temp.kind=codei;
		PCODE[lastcode]=temp;
		syms=getsyms();
		while(syms==17)
		{
			expression();
			if(syms!=2&&syms!=3&&syms!=4)
				error(24);
			gen("cas",0,0);
			lastcode=codei;
			codei++;
			syms=getsyms();
			syms=getsyms();
			statement();
			codei++;
			syms=getsyms();

			strcpy(temp.opr,"jpc");
			temp.lev=0;
			temp.kind=codei;
			PCODE[lastcode]=temp;
		}
		if(syms==18)
		{
			syms=getsyms();
			if(syms!=54)
				error(22);
			else
				syms=getsyms();
			statement();
			lastcode=codei;	
			syms=getsyms();
		}
	}	else
		printf("switch缺少条件");
	PCO temp;
	strcpy(temp.opr,"jpc");
	temp.lev=0;
	temp.kind=codei;
	PCODE[lastcode]=temp;
	int end=codei;

	strcpy(temp.opr,"jmp");
	temp.lev=0;
	temp.kind=end;
	for(int i=from;i<end;i++)
	{
		if (PCODE[i].str==NULL)
		{
			PCODE[i]=temp;
		}
	}

	return 0;
}
int program()
{
	int flag=0;
	syms=getsyms();
	while(1)
	{
		if(syms==0)
			break;
		else
		{
			if(syms==35)
				syms=getsyms();
			if(syms==22)
				flag=constdeclaration();
			else if(syms==10||syms==11||syms==12||syms==13)
				vardefine(syms);
			else
				statement();
		}
		if(flag==0)
			syms=getsyms();
		else
			flag=0;
	}
	return 0;
}
int interprete()
{
	float interp[1000];
	int lpcode[100];
	int lptop=0;
	for(int j=0;j<100;j++)
		interp[j]=0;
	int top=0,i=0,tail=0;
	for(;i<codei;i++)
	{

		if(strcmp(PCODE[i].opr,"jmp")==0)
			i=PCODE[i].kind-1;
		else if(strcmp(PCODE[i].opr,"lit")==0)
		{
			interp[top++]=PCODE[i].kind;
		}
		else if(strcmp(PCODE[i].opr,"lod")==0)
		{
			if(PCODE[i].kind<0)
			{
				int temp;
				temp=-PCODE[i].kind ;
				temp+=interp[--top];
				if(PCODE[i].lev==0)
				{
					interp[top++]=interp[temp];
				}
				else
				{
					interp[top++]=interp[int(temp+tail)];
				}
			}
			else
			{
			if(PCODE[i].lev==0)
				interp[top++]=interp[int(PCODE[i].kind+1)];
			else
				interp[top++]=interp[int(PCODE[i].kind+1+tail)];
			}

		}
		else if(strcmp(PCODE[i].opr,"sto")==0)
		{
			if(PCODE[i].kind<0)
			{
				int temp;
				temp=-PCODE[i].kind;
				temp+=interp[top-2];
				if(PCODE[i].lev==0)
				{
					interp[temp]=interp[--top];
					--top;
				}
				else
				{
					interp[temp+tail]=interp[--top];
					--top;
				}
			}
			else
			{
			if(PCODE[i].lev==0)
				interp[int(PCODE[i].kind+1)]=interp[--top];
			else
				interp[int(PCODE[i].kind+1+tail)]=interp[--top];
			}
		}
		else if(strcmp(PCODE[i].opr,"stoi")==0)
		{
			if(PCODE[i].kind<0)
			{
				int temp;
				temp=-PCODE[i].kind;
				temp+=interp[top-2];
				if(PCODE[i].lev==0)
				{
					int t;
					t=interp[--top];
					interp[temp]=t;
					--top;
				}
				else
				{
					int t;
					t=interp[--top];
					interp[temp+tail]=t;
					--top;
				}
			}
			else
			{
				int t;
				if(PCODE[i].lev==0)
				{
					t=interp[--top];
					interp[int(PCODE[i].kind+1)]=t;
				}
				else
				{
					t=interp[--top];
					interp[int(PCODE[i].kind+1+tail)]=t;
				}
			}
		}
		else if(strcmp(PCODE[i].opr,"rei")==0)
		{
			int i;
			scanf("%d",&i);
			interp[top++]=i;
		}
		else if(strcmp(PCODE[i].opr,"rec")==0)
		{
			char i[10];
			scanf("%s",i);
			interp[top++]=i[0];
		}
		else if(strcmp(PCODE[i].opr,"ref")==0)
		{
			float i;
			scanf("%f",&i);
			interp[top++]=i;
		}
		else if(strcmp(PCODE[i].opr,"pri")==0)
		{
			printf("%d\n",int(interp[--top]));
		}
		else if(strcmp(PCODE[i].opr,"prc")==0)
		{
			printf("%c",int(interp[--top]));
		}
		else if(strcmp(PCODE[i].opr,"prf")==0)
		{
			printf("%lf\n",interp[--top]);
		}
		else if(strcmp(PCODE[i].opr,"int")==0)
		{
			if(PCODE[i].lev==1)
				top=tail+PCODE[i].kind+2;
			else
			{
				top=tail+2+PCODE[i].kind;
				tail=top;
			}
		}
		else if(strcmp(PCODE[i].opr,"jpc")==0)
		{
			if(interp[--top]==0)
				i=PCODE[i].kind-1;
		}
		else if(strcmp(PCODE[i].opr,"cas")==0)
		{
			if(interp[top-2]==interp[top-1])
					interp[top-1]=1;
				else
					interp[top-1]=0;
		}
		else if(strcmp(PCODE[i].opr,"cal")==0)
		{
			int temp=0,j;
			temp=top-1-PCODE[i].lev;
			for(j=top;j>temp;j--)
			{
				interp[j]=interp[j-1];
			}
			interp[top-PCODE[i].lev]=tail;
			tail=top-PCODE[i].lev;

			/*interp[top--]=tail;
			tail=top;
			int temp=0;
			for(temp=0;temp<PCODE[i].lev;temp++)
			{
				interp[top+1]=interp[top];
				top++;
			}*/


			lpcode[lptop++]=i;
			i=PCODE[i].kind;
			i--;

		}
		else if(strcmp(PCODE[i].opr,"ret")==0)
		{
			int nowtop;
			nowtop=top-1;
			top=tail;
			tail=interp[tail];
			interp[top++]=interp[nowtop];
			//i=interp[0];
			i=lpcode[--lptop];
		}
		else if(strcmp(PCODE[i].opr,"opr")==0)
		{
			switch (int(PCODE[i].kind))
			{
			case 1:
				interp[top-1]=-interp[top-1];
			    break;
			case 2:
				interp[top-2]=interp[top-1]+interp[top-2];
				top--;
				break;
			case 3:
				interp[top-2]=interp[top-2]-interp[top-1];
				top--;
				break;
			case 4:
				interp[top-2]=interp[top-2]*interp[top-1];
				top--;
				break;
			case 5:
				interp[top-2]=interp[top-2]/interp[top-1];
				top--;
				break;
			case 8:
				if(interp[top-2]==interp[top-1])
					interp[top-2]=1;
				else
					interp[top-2]=0;
				top--;
				break;
			case 9:
				if(interp[top-2]!=interp[top-1])
					interp[top-2]=1;
				else
					interp[top-2]=0;
				top--;
				break;
			case 10:
				if(interp[top-2]<interp[top-1])
					interp[top-2]=1;
				else
					interp[top-2]=0;
				top--;
				break;
			case 11:
				if(interp[top-2]>=interp[top-1])
					interp[top-2]=1;
				else
					interp[top-2]=0;
				top--;
				break;
			case 12:
				if(interp[top-2]>interp[top-1])
					interp[top-2]=1;
				else
					interp[top-2]=0;
				top--;
				break;
			case 13:
				if(interp[top-2]<=interp[top-1])
					interp[top-2]=1;
				else
					interp[top-2]=0;
				top--;
				break;
			default:
				break;
			}
		}
	}
	return 0;
}
int main()
{
	char *s;
	s=(char*)malloc(sizeof(char)*100);
	printf("请输入文件名：");
	scanf("%s",s);
	in=fopen(s,"r");//输入的文件
	if(in==NULL)
		printf("输入文件错误");
	out=fopen("10231023_token.txt","w");

	program();
	if(flagerror==0)
	interprete();
	fclose(in);
	fclose(out);
	return 0;
}