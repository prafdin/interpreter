#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "postfix_handler.h"



using namespace std;

static vector<PostfixElem> v;

void DEBUG() {
	for (auto& i : v) {
		cout << i.type << endl;
	}
}

void CleanPostfix()
{
	v.clear();
}



static vector<Cmd> cmd_v;
static vector<Var> var_v;
static vector<size_t> const_v;

int SetConst(size_t constant)
{
	auto it = find_if(const_v.begin(), const_v.end(), [constant](int c) { return c == constant; });
	int ind;
	if (it != const_v.end()) {
		ind = it - const_v.begin()  ;
	}
	else {
		ind = const_v.size();
		const_v.push_back(constant);
	}
	return ind;
}

int CurrentPtr(){
	return v.size();
}

size_t GetConst(int ind) {
	return const_v[ind];
}

Var& GetVar(int ind) {
	return var_v[ind];
}

Cmd GetCmd(int ind)
{
	return cmd_v[ind];
}

PostfixElem GetElem(int ind) {
	return v[ind];
}

int WriteCmd(Cmd cmd){
	auto it = find_if(cmd_v.begin(), cmd_v.end(), [cmd](Cmd& cm) { return cm == cmd; });
	int ind;
	if (it != cmd_v.end()) {
		ind = it - cmd_v.begin();
	}
	else {
		ind = cmd_v.size();
		cmd_v.push_back(cmd);
	}
	
	int pn_index = v.size();
	v.push_back(PostfixElem{ ElemType::elCmd, ind });
	return pn_index;
}

int WriteVar(string var){
	auto it = find_if(var_v.begin(), var_v.end(), [var](Var& v) { return v.var == var; });
	int ind;
	if (it != var_v.end()) {
		ind = it - var_v.begin();
	}
	else {
		ind = var_v.size();
		var_v.push_back(Var{ var,0,false });
	}
	int pn_index = v.size();
	v.push_back(PostfixElem{ ElemType::elVar, ind });
	return pn_index;
}

int WriteConst(int constant){
	auto it = find_if(const_v.begin(), const_v.end(), [constant](int c) { return c == constant; });
	int ind;
	if (it != const_v.end()) {
		ind = it - const_v.begin() ;
	}
	else {
		ind = const_v.size();
		const_v.push_back(constant);
	}
	int pn_index = v.size();
	v.push_back(PostfixElem{ ElemType::elConst, ind });
	return pn_index;
}

int WriteCmdPtr(int ptr){
	int pn_index = v.size();
	v.push_back(PostfixElem{ ElemType::elCmdPtr, ptr });
	return pn_index;
}

void SetCmdPtr(int ind, int ptr){
	v[ind].index = ptr;
}
