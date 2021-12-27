#include <stack>
#include <stdexcept>

#include <QTextEdit>
#include <QString>

#include "postfix_handler.h"
#include "interpreter.h"
#include "interpreter_window.h"
using namespace std;

stack<PostfixElem> st;

int MAX_COUNT_ITERATIONS = 50000;

void Interpret(InterpreterWindow& window) {
	int count_iteration = 0;
	int tmp;
	int cur_indx = 0;
	int max_indx = CurrentPtr();
	while (cur_indx < max_indx) {
		++count_iteration;
		if (count_iteration == MAX_COUNT_ITERATIONS) {
			throw std::logic_error("Count of cycle iteration too much");
		}
		PostfixElem cur_elem = GetElem(cur_indx);
		if (cur_elem.type == ElemType::elCmd) {
			Cmd cmd = GetCmd(cur_elem.index);
			switch (cmd)
			{
			case JMP:
				cur_indx = PopVal();
				break;
			case JZ:
				tmp = PopVal();
				if (PopVal()) cur_indx++; else cur_indx = tmp;
				break;
			case SET:
				tmp = PopVal();
				SetVarAndPop(tmp);
				++cur_indx;
				break;
			case ADD:
				tmp = PopVal();
				tmp = SetConst(tmp + PopVal());
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case SUB:
				tmp = PopVal();
				tmp = SetConst(PopVal() - tmp);
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case AND:
				tmp = PopVal();
				tmp = SetConst(tmp && PopVal());
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case OR:
				tmp = PopVal();
				tmp = SetConst(tmp ||  PopVal());
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case CMPE:
				tmp = PopVal();
				tmp = SetConst(tmp == PopVal());
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case CMPNE:
				tmp = PopVal();
				tmp = SetConst(tmp != PopVal());
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case CMPL:
				tmp = PopVal();
				tmp = SetConst(tmp > PopVal());
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case CMPM:
				tmp = PopVal();
				tmp = SetConst(tmp < PopVal());
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case OUT:
				char buf[50];
				sprintf(buf, "%zu", PopVal());
				window.PrintMsg(QString(buf));
				++cur_indx;
				break;
			case INPUT:
				tmp = window.ScanNum();				
				SetVarAndPop(tmp);
				++cur_indx;
				break;
			case MUL:
				tmp = PopVal();
				tmp = SetConst(tmp * PopVal());
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			case DIV:
				tmp = PopVal();
				tmp = SetConst( PopVal()/ tmp);
				PushElem(PostfixElem{ ElemType::elConst, tmp });
				++cur_indx;
				break;
			default:
				break;
			}
		}
		else {
			PushElem(GetElem(cur_indx));
			cur_indx++;
		}
	}

}

void PushElem(PostfixElem el) {
	st.push(el);
}
size_t PopVal() {
	PostfixElem el = st.top();
	st.pop();

	if (el.type == ElemType::elConst) {
		return GetConst(el.index);
	}
	else if (el.type == ElemType::elVar) {
		Var var = GetVar(el.index);
		if (!var.initialized)
			throw std::logic_error(std::string("Var ") + var.var + std::string(" not initialized"));
		return var.value;
	}
	else if (el.type == ElemType::elCmdPtr) {
		return el.index;
	}
	else {
		// TODO Check el type ; if non var or const -  throw except
	}
}

void SetVarAndPop(size_t val) {
	PostfixElem el = st.top();
	st.pop();
	GetVar(el.index).initialized = true;
	GetVar(el.index).value = val;
}
