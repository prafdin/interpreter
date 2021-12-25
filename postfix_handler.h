#pragma once
#include <string>

enum ElemType { elCmd, elVar, elConst, elCmdPtr };
enum Cmd { JMP, JZ, SET, ADD, SUB, AND, OR, CMPE, CMPNE, CMPL, CMPM, OUT, INPUT, MUL, DIV};


struct PostfixElem {
	ElemType type;
	int index;
};

struct Var {
	std::string var;
	int value;
};

int SetConst(int constant);

int CurrentPtr();
int GetConst(int ind);
Var& GetVar(int ind);
Cmd GetCmd(int ind);
PostfixElem GetElem(int ind);

int WriteCmd(Cmd cmd);
int WriteVar(std::string var);
int WriteConst(int constant);
int WriteCmdPtr(int ptr); 
void SetCmdPtr(int ind, int ptr);

void DEBUG();

void CleanPostfix();