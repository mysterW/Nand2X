#pragma once 

#include"stdafx.h"
typedef std::string EXP;
typedef std::string STATIC;
//typedef int POINTER;
#define PUSH "push"
#define POP "pop"
#define ADD "add"
#define SUB "sub"
#define NEG "neg"
#define EQUAL "eq"
#define GREATER_THAN "gt"
#define LOWER_THAN "lt"
#define AND "and"
#define OR "or"
#define NOT "not"
#define SP "SP"
#define LCL "LCL"
#define ARG "ARG"
#define THI "THIS"
#define THA "THAT"
#define TMP "5"
#define STA "16"
#define R0 "R0"
#define R1 "R1"
#define R2 "R2"
#define R3 "R3"
#define R4 "R4"
#define R5 "R5"
#define R6 "R6"
#define R7 "R7"
#define R8 "R8"
#define R9 "R9"
#define R10 "R10"
#define R11 "R11"
#define R12 "R12"
#define R13 "R13"
#define R14 "R14"
#define R15 "R15"
#define SETA(a) (asmfile<<'@'<<(a)<<endl)
#define ASSIGN_A(a) (asmfile<<"A="<<(a)<<endl)
#define ASSIGN_M(a) (asmfile<<"M="<<(a)<<endl)
#define ASSIGN_D(a) (asmfile<<"D="<<(a)<<endl)
#define TONOT(a,b) (asmfile<<(a)<<'='<<'!'<<(b)<<endl)
#define TONEG(a,b) (asmfile<<(a)<<'='<<'-'<<(b)<<endl)
#define TOAND(a,b,c) (asmfile<<(a)<<'='<<(b)<<'&'<<(c)<<endl)
#define TOOR(a,b,c) (asmfile<<(a)<<'='<<(b)<<'|'<<(c)<<endl)
#define TOADD(a,b,c) (asmfile<<(a)<<'='<<(b)<<'+'<<(c)<<endl)
#define TOSUB(a,b,c) (asmfile<<(a)<<'='<<(b)<<'-'<<(c)<<endl)
#define JUMPWHEN(a,b) asmfile<<(a)<<';'<<(b)<<endl
#define JGT "JGT"
#define JEQ "JEQ"
#define JLT "JLT"
#define JGE "JGE"
#define JNE "JNE"
#define JLE "JLE"
#define JMP "JMP"
#define REGA 'A'
#define REGM 'M'
#define REGD 'D'
#define REGAM "AM"
#define REGAD "AD"
#define REGMD "MD"
#define REGAMD "AMD"
#define CONSTANT "constant"
#define LOCAL "local"
#define ARGUMENT "argument"
#define THIS "this"
#define THAT "that"
#define STATIC "static"
#define TEMP "temp"

// VMtranslator类的声明

class VMtranslator
{
protected:
	std::ifstream vmfile;     //输入文件
	std::ofstream asmfile;     //输出文件
	int cursor=0;           //指出当前文件光标
	EXP advance();    //前进并返回下一行经预处理的代码,文件到达末尾时返回空字符串。
	bool precompile(EXP& expression);    //代码预处理，去除多余空格以及注释，有代码则返回真
	void parser(EXP exp);       //语句解释器，调用相关的写代码程式,并输出语法错误在屏幕上
	void writePush(EXP block,int number);              //写push代码           
	void writePop(EXP block, int number);              //写pop代码
	void writeSingleArithmetic(EXP instruction);      //写单因子计算代码
	void writeDoubleArithmetic(EXP instruction);      //写双因子计算代码
public:
	VMtranslator(std::string vmdir,std::string asmdir) {             //构造函数，制定输入及输出文件
		vmfile.open(vmdir);
		asmfile.open(asmdir);
	}
	void translate();		//翻译器主体
};