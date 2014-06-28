#ifndef CALC_H
#define CALC_H

#include <string>

using namespace std;

class AmaF_Calc
{
protected:
	string exp;	//记录用户输入的表达式
	int pos;	//记录扫描到的位置
	double amafOperate(double a,char symbol,double b);	//双目运算函数
	double amafOperate(double a,char symbol);	//重载的单目运算函数
	char amafCompare(char a,char b);	//比较运算符的优先级
	bool amafIsDigit(char c)	//判断是不是数字或常量
	{if((c<='9'&&c>='0')||(c=='e')||(c=='p'))return true;return false;}
	char amafFuncSymbol();	//将用户输入的函数转换成字符型记号
	double amafAtof();	//获得一个实数，并设定扫描位置

public:
	AmaF_Calc():exp(""),pos(0){}	//构造函数
	void help();	//帮助
	void evalExpression(string);	//求解函数
};

#endif