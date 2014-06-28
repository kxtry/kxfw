//AmaranthF的命令行计算器
//
//2008/2/4	v0.1
//可以进行所有带括号的四则运算
//
//2008/2/5	v0.2
//加入了三角与反三角函数（仅弧度）、幂和对数的运算
//对AmaF_Stack（栈）类做了一些补充与修正（增加清栈函数、析构函数、判断栈空函数）
//修正了一些Bug
//
//2008/2/7	v0.3
//加入了异常处理，并给出错误提示
//增加了对形如2 sin3等省略乘号的表达式的支持
//增加了程序说明
//做最后的调试，修正了n多Bug。（头一次体会到原来测试程序比编写程序还要令人抓狂……）
//
//2008/2/14	v1.0
//封装完毕，将所有功能整合到AmaF_Calc类里面。

#include "calc.h"
#include <stack>
#include <list>
#include <cmath>
#include <cerrno>
#include <iostream>

//定义函数的代号
#define SIN 'a'	//三角和反三角函数
#define COS 'b'
#define TAN 'c'
#define ASIN 'd'
#define ACOS 'e'
#define ATAN 'f'
#define LOG 'g'	//10的对数
#define LN 'h'	//自然对数

//定义常数
#define PI 3.145926536	//圆周率
#define EE 2.718281828	//自然对数的底

//定义错误代码
#define IN_SPACE 1
#define DIV_BY_0 2
#define SYNT_ERR 3
#define RNG_ERR 4
#define DOM_ERR 5
#define SYMB_ERR 6

//其他
#define SINGLE 'z'	//单目运算标志


using namespace std;

double AmaF_Calc::amafOperate (double a,char symbol,double b)
{
	switch(symbol)
	{
	case '+':
		return a+b;
	case '-':
		return a-b;
	case '*':
		return a*b;
	case '/':
		if(b==0)	//如果0作除数
			throw DIV_BY_0;
		return a/b;
	case '^':
		return pow(a,b);
	case 'E':
		return a*pow(10.0,b);
	default:
		throw SYMB_ERR;
	}
}

double AmaF_Calc::amafOperate (double a,char symbol)
{
	switch(symbol)
	{
	case SIN:
		return sin(a);
	case COS:
		return cos(a);
	case TAN:
		return tan(a);
	case ASIN:
		return asin(a);
	case ACOS:
		return acos(a);
	case ATAN:
		return atan(a);
	case LOG:
		return log10(a);
	case LN:
		return log(a);
	default:
		throw SYMB_ERR;
	}
}

char AmaF_Calc::amafCompare (char a,char b)
{
	switch(a)
	{
	case '+':
	case '-':
		if(b=='+'||b=='-'||b==')'||b=='$')
			return '>';
		else
			return '<';
	case '*':
	case '/':
		if(b=='('||b=='^'||b=='E'||b>=SIN)
			return '<';
		else
			return '>';
	case '^':
		if(b=='('||b=='E'||b>=SIN)
			return '<';
		else
			return '>';
	case 'E':
		if(b=='(')
			return '<';
		else if(b>=SIN)
			throw SYNT_ERR;
		else
			return '>';
	case '(':
		if(b==')')
			return '=';
		else if(b=='$')
			throw SYNT_ERR;
		else
			return '<';
	case '$':
		if(b=='$')
			return '=';
		else if(b==')')
			throw SYNT_ERR;
		else
			return '<';
	default:
		if(a<SIN||a>LN)
			throw SYNT_ERR;
		else if(b=='('||b=='E'||b=='^'||b>=SIN)
			return '<';
		else
			return SINGLE;
	}
}

char AmaF_Calc::amafFuncSymbol ()
{
	string temp(exp.substr (pos,4));
	if(!temp.find("sin"))
	{
		pos+=2;
		return SIN;
	}
	if(!temp.find("cos"))
	{
		pos+=2;
		return COS;
	}
	if(!temp.find("tan"))
	{
		pos+=2;
		return TAN;
	}
	if(!temp.find("asin"))
	{
		pos+=3;
		return ASIN;
	}
	if(!temp.find("acos"))
	{
		pos+=3;
		return ACOS;
	}
	if(!temp.find("atan"))
	{
		pos+=3;
		return ATAN;
	}
	if(!temp.find("log"))
	{
		pos+=2;
		return LOG;
	}
	if(!temp.find("ln"))
	{
		pos++;
		return LN;
	}

	return exp.at(pos);
}

double AmaF_Calc::amafAtof ()
{
	double conFloat=0;

	if(exp.at(pos)=='e')	//对“e”和“pi”这两个数的操作
	{
		pos++;
		return EE;
	}
	else if(exp.find("pi",pos,2)==pos)
	{
		pos+=2;
		return PI;
	}

	conFloat=exp.at(pos)-('0'-0);		//取整数部分
	pos++;
	for(;amafIsDigit(exp.at(pos))==true;pos++)
	{
		conFloat=10*conFloat+(exp.at(pos)-('0'-0));
	}

	if(exp.at(pos)=='.')	//有小数点的话就继续取小数部分
	{
		double base=1;
		pos++;
		for(;amafIsDigit(exp.at(pos))==true;pos++)
		{
			base/=10;
			conFloat+=base*(exp.at(pos)-('0'-0));
		}
		return conFloat;
	}
	else if((exp.at(pos)==' ')&&amafIsDigit(exp.at(pos+1)))	//判断数字之间是否有空格
		throw IN_SPACE;
	else
		return conFloat;
}

void AmaF_Calc::help ()
{
	cout<<"\n本计算器拥有带括号的基本四则运算、三角与反三角函数运算、幂和对数运算等功能。\n\n"
		"输入说明：\n"
		"数字0~9，小数点“.”\n"
		"基本运算符：+ - * / ( )\n"
		"科学计数法输入：E\n"
		"三角函数：sin,cos,tan,asin（反正弦，下同）,acos,atan\n"
		"幂与对数（10为底或e为底）：^,log,ln\n"
		"圆周率与自然对数的底：pi,e\n"
		"命令：help：显示本说明；exit：退出程序。\n"<<endl;
	cout<<"补充说明：\n"
		"1 乘号在特定情况下可以省略，如7(3+2)或者2sin3的形式。\n"
		"2 上述的函数以及常数必须用小写字母（除了“E”）原样输入。\n"
		"3 相邻数字之间不允许有空格，但其余位置不作限制，如sin ( 2  ^2+1   )。\n"
		"4 负数须使用括号括起来。\n"
		"5 科学计数法的输入：2.1E3代表2100（即2.1*10^3）。\n"
		"6 三角函数运算采用弧度制。\n"
		"7 目前计算精度比较低（最多4位小数有效），而且由于计算机的存储原理，结果可能会存在误差。\n"
		"8 程序给出的错误提示可能会有些不正常……如果这种情况发生，请忽略它，自己检查所输入的表达式。\n"<<endl;
	cout<<endl;
}

void AmaF_Calc::evalExpression (string inexp)
{
	exp=inexp;
	pos=0;
	stack<char,list<char>> symbols;	//用来存放运算符的栈
	stack<double,list<double>> figures;	//用来存放数的栈

	exp+='$';	//添加结束符
	symbols.push ('$');	//起始字符入栈

	try
	{
		for(;exp.at(pos)!='$' || symbols.top ()!='$';)	//开始逐个扫描
		{
			if(amafIsDigit(exp.at(pos))==true)	//若是数的话，入栈
			{
				figures.push (amafAtof());
				if(errno==EDOM)	//判断自变量是否非法
					throw DOM_ERR;
				if(errno==ERANGE)	//判断是否溢出
					throw RNG_ERR;
			}

			else if((pos!=0)&&(exp.at(pos)=='-')&&((exp.at(pos-1)=='(') ||(exp.at(pos-1)=='E')))	//带负号的数入栈
			{
				pos++;
				figures.push (-1.0*amafAtof());
				if(errno==EDOM)
					throw DOM_ERR;
				if(errno==ERANGE)
					throw RNG_ERR;
			}

			else if(exp.at(pos)==' ')	//跳过空格字符
				pos++;

			else
			{
				bool prevIsDigit=false;	//记录前一个字符是否是数字
				if(pos!=0&&exp.at(pos-1)>='0'&&exp.at(pos-1)<='9')
					prevIsDigit=true;
				char conSymbol=amafFuncSymbol();
				if(prevIsDigit&&(conSymbol>=SIN||conSymbol=='('))	//以此判断是否是形如“2sin 3”形式的输入
					symbols.push ('*');	//若是的话，乘号入栈
				switch(amafCompare(symbols.top (),conSymbol))	//比较栈顶运算赋与读入的运算符优先级
				{
				case '<':
					symbols.push (conSymbol);	//前者优先级较低的话，入栈，继续扫描
					pos++;
					break;
				case '=':	//括号内的运算计算完毕时，左括号出栈
					symbols.pop ();
					pos++;
					break;
				case '>':	//优先级高的话，取出上一个运算符，并对取出的两个数进行运算，结果入栈
					{
						double temp1=figures.top ();
						figures.pop ();
						double temp2=amafOperate(figures.top (),symbols.top (),temp1);
						figures.pop ();
						figures.push (temp2);
						symbols.pop ();
						break;
					}
				case SINGLE:	//如果单目运算函数优先级较高，取第一个数进行运算，结果入栈
					{
						double temp=amafOperate(figures.top (),symbols.top ());
						figures.pop ();
						symbols.pop ();
						figures.push (temp);
						break;
					}
				}
			}
		}
		if(errno==EDOM)
			throw DOM_ERR;
		else if(errno==ERANGE)
			throw RNG_ERR;
		
		cout<<exp<<"\b = "<<figures.top ()<<endl;
		figures.pop ();
		cout<<endl;
	}
	catch(int errorCode)	//异常处理
	{
		switch(errorCode)
		{
		case SYNT_ERR:
			cout<<"语法错误！请参照说明。\n"<<endl;
			break;
		case IN_SPACE:
			cout<<"数字之间不允许有空格！\n"<<endl;
			break;
		case DIV_BY_0:
			cout<<"除数不能为零！\n"<<endl;
			break;
		case RNG_ERR:
			errno=0;
			cout<<"浮点数太大！\n"<<endl;
			break;
		case DOM_ERR:
			errno=0;
			cout<<"某个函数的自变量非法！\n"<<endl;
			break;
		case SYMB_ERR:
			cout<<"表达式中有非法字符！\n"<<endl;
			break;
		}
	}
}

#if 0

#include "calc.h"
#include <iostream>

using namespace std;

int main()
{
	AmaF_Calc calculator;
	string exp;

	cout<<"////AmaranthF的计算器////"<<'\n'<<endl;
	calculator.help();

	while(1)
	{
		cout<<"请输入表达式：";

		char str[150];
		cin.getline(str,150);
		exp=str;

		if(exp.compare ("exit")==0)
		{
			cout<<"感谢您的使用！"<<endl;
			break;
		}
		else if(exp.compare ("help")==0)
		{
			calculator.help();
			continue;
		}

		calculator.evalExpression(exp);
	}

	return 0;
}
#endif