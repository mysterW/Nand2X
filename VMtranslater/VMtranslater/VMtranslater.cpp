// VMtranslater.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VMtranslator.h"
using namespace std;

int main(int argc, char* argv[])
{
	string vmfile;
	string asmfile;
	if (argc == 1)
	{
		cerr << "Lack of Parameters:Please Spcify your file" << endl;
		return 0;
	}
	else if (argc == 2)
	{
		vmfile = argv[1];
		ifstream tryinput(vmfile);
		if (!tryinput)
		{
			cerr << "No such a file or directory:The file name should not be with space" << endl;
			return 0;
		}
		auto pos = vmfile.rfind('.');
		if (pos== string::npos)
		{
			cerr << "The file is not .vm file" << endl;
			return 0;
		}
		else
		{
			asmfile=vmfile.substr (0, pos);
			asmfile.append(".asm");
		}
	}
	else if (argc == 3)
	{
		vmfile = argv[1];
		asmfile = argv[2];
	}
	else
	{
		cerr << "Unexpected Parameters.The file name should not be with space " << endl;
		return 0;
	}
	VMtranslator vm(vmfile, asmfile);
	vm.translate();
	getchar();
	return 0;
}

void VMtranslator::translate() {
	EXP expression;
	while (!((expression=advance()).empty()))
	{
		parser(expression);
	}

}

EXP VMtranslator::advance()
{
	EXP expression;
	do {
		getline(vmfile,expression);
		cursor++;
	} while ((!precompile(expression)) && (!vmfile.eof()));    //判断是否有效语句，最后一行，若不是有效语句，则会返回空字符串；
	return expression;
}

bool VMtranslator::precompile(EXP& expression) {
	auto posofNote = expression.find("//");                  //去除注释
	if (posofNote != expression.npos)
		expression.erase(posofNote);
	for (int pos=0,end = expression.size(); pos != end; pos++)            //去除多余空格
	{
		if (expression[pos]==' ')
		{
			if (pos == 0)                  //去除起始空格
			{
				int i = 0;
				for (; i != expression.size() && expression[i] == ' '; i++);
				expression.erase(0, i);
			}
			else if (expression[pos - 1] != ' ')                   //去除中间多余空格及尾后空格
			{
				int i = pos;
				for (; i != expression.size() && expression[i] == ' '; i++);
				if (i == expression.size() && expression[i - 1] == ' ')    //去除尾后空格
				{
					expression.erase(pos);
					pos--;
				}
				else
					expression.erase(pos + 1, i - pos - 1);
			}
			else                                   //向前去除空格
			{
				int i = pos;
				for (; i != 0 && expression[i] == ' '; i--);
				expression.erase(i, pos + 1);
				pos = i;
			}
		}
		end = expression.size();              //更新字符串尾
	}
	if (expression.empty())
		return false;
	return true;
}

void VMtranslator::parser(EXP exp)
{
	istringstream expression(exp);
	EXP command;
	EXP arg1;
	//STATIC staticArg2; 
	int numberArg2;
	expression >> command;
	if (command == PUSH)
	{
		expression >> arg1;
		expression >> numberArg2;
		if (!expression.fail())
			writePush(arg1,numberArg2);
		//else
			//writePush(staticArg2);
	}
	else if (command == POP)
	{
		expression >> arg1;
		expression >> numberArg2;
		if (!expression.fail())
			writePop(arg1,numberArg2);
		//else
			//writePop(staticArg2);
	}
	else if (command == NEG || command == NOT)
	{
		writeSingleArithmetic(command);
	}
	else if (command == ADD || command == SUB || command == EQUAL ||
		command == GREATER_THAN || command == LOWER_THAN || command == AND || command == OR)
	{
		writeDoubleArithmetic(command);
	}
	else
	{
		cerr << "line " << cursor << " errorC0001: syntax error:undefined symbol" << endl;
	}
	return;
}

void VMtranslator::writePush(EXP block, int number)
{
	if (block == CONSTANT)
	{
		SETA(number);
		ASSIGN_D(REGA); 
	}
	else
	{
		if (block == LOCAL) {
			SETA(LCL);
			ASSIGN_D(REGM);
		}
		else if (block == ARGUMENT)
		{
			SETA(ARG);
			ASSIGN_D(REGM);
		}
		else if (block == THIS)
		{
			SETA(THI);
			ASSIGN_D(REGM);
		}
		else if (block == THAT)
		{
			SETA(THA);
			ASSIGN_D(REGM);
		}
		else if (block == TEMP)
		{
			SETA(TMP);
			ASSIGN_D(REGA);
		}
		else if (block == STATIC)
		{
			SETA(STA);
			ASSIGN_D(REGA);
		}
		SETA(number);
		TOADD(REGA, REGD, REGA);
		ASSIGN_D(REGM);
	}
	SETA(SP);
	ASSIGN_A(REGM);
	ASSIGN_M(REGD);
	TOADD(REGD, REGA, "1");
	SETA(SP);
	ASSIGN_M(REGD);
}



void VMtranslator::writePop(EXP block,int number)
{
	SETA(number);
	ASSIGN_D(REGA);
	if (block == LOCAL) {
		SETA(LCL);
		TOADD(REGA, REGD, REGM);
	}
	else if (block == ARGUMENT)
	{
		SETA(ARG);
		TOADD(REGA, REGD, REGM);
	}
	else if (block == THIS)
	{
		SETA(THI);
		TOADD(REGA, REGD, REGM);
	}
	else if (block == THAT)
	{
		SETA(THA);
		TOADD(REGA, REGD, REGM);
	}
	else if (block == TEMP)
	{
		SETA(TMP);
		TOADD(REGA, REGD, REGA);
	}
	else if (block == STATIC)
	{
		TOADD(REGA, REGD, REGA);
	}
	ASSIGN_D(REGA);
	SETA(R13);
	ASSIGN_M(REGD);
	SETA(SP);
	TOSUB(REGAM, REGM, "1");
	ASSIGN_D(REGM);
	SETA(R13);
	ASSIGN_A(REGM);
	ASSIGN_M(REGD);
}
void VMtranslator::writeSingleArithmetic(EXP instruction)
{

}
void VMtranslator::writeDoubleArithmetic(EXP instruction)
{
	SETA(SP);
	TOSUB(REGA,REGM,"1");
	ASSIGN_D(REGM);
	TOSUB(REGA, REGA, "1");
	if (instruction == ADD)
	{
		TOADD(REGM, REGM, REGD);
	}
	else if (instruction == SUB)
	{
		TOSUB(REGM, REGM, REGD);
	}
	TOADD(REGD, REGA, "1");
	SETA(SP);
	ASSIGN_M(REGD);
}