#include <iostream>
#include <string>

#include "common.h"
#include "postfix_handler.h"
#include "lex_analyzer.h"
#include "parser.h"


bool BoolExp() {
	if (!LogExp()) {
		return false;
	}
	if (!BoolExp2())
		return false;
	return true;
}

bool BoolExp2() { // the same bool_exp*
	while (lexems_exist() && show_next_lexem().type == LexemType::_or) {
		Lexem l = get_lexem();
		if (!LogExp())
			return false;
		WriteCmd(Cmd::OR);
	}
	return true;

}

bool LogExp() {
	if (!CmpExp())
		return false;
	if (!LogExp2())
		return false;
	return true;
}

bool LogExp2() {
	while (lexems_exist() && show_next_lexem().type == LexemType::_and) {
		Lexem l = get_lexem();
		if (!CmpExp())
			return false;
		WriteCmd(Cmd::AND);
	}
	return true;
}


bool CmpExp() {
	if (!Operand()) {
		return false;
	}
	if (!CmpAndOperand())
		return false;
	return true;
}

bool CmpAndOperand() {
	if (lexems_exist()) {
		Lexem l = get_lexem();
		Cmd cmd;
		if (l.type == LexemType::_comp || l.type == LexemType::_assign) {
			if (l.lex == ">") {
				cmd = Cmd::CMPM ;
			}
			else if (l.lex == "<") {
				cmd = Cmd::CMPL;
			}
			else if (l.lex == "=") {
				cmd = Cmd::CMPE;
			}
			else if (l.lex == "<>") {
				cmd = Cmd::CMPNE;
			}
		}
		else {
			set_error_pos(l.pos);
			set_error_msg("Expected a compare operation");
			return false;
		}
		if (!Operand())
			return false;
		WriteCmd(cmd);
	}
	return true;
}


bool Operand() {
	Lexem l = get_lexem();
	if (l.type == LexemType::_ind) {
		WriteVar(l.lex);
		return true;
	}
	else if (l.type == LexemType::_const) {
		WriteConst(std::stoi(l.lex));
		return true;
	}
	else {
		set_error_pos(l.pos);
		set_error_msg("Expected a identificator or constant");
		return false;
	}
	
}

bool Operators() {
	if (!Operator())
		return false;
	if (!Operators2())
		return false;
	return true;
}

bool Operators2() { // the same operators*
	while (lexems_exist() && show_next_lexem().type == LexemType::_semicolon) {
		Lexem l = get_lexem();
		if (!Operator())
			return false;
	}
	return true;
}

bool Operator() {
	if (!lexems_exist()) {
		set_error_msg("Any operator expected");
		return false;
	}
	Lexem l = get_lexem();
	switch (l.type)
	{
	case LexemType::_ind:
		WriteVar(l.lex);
		if (!lexems_exist() || !(show_next_lexem().lex == "=") ) {
			set_error_pos(l.pos);
			set_error_msg("Expected a assign statement after variable");
			return false;
		}
		l = get_lexem();
		if (!ArithExp())
			return false;
		WriteCmd(Cmd::SET);
		return true;
		break;
	case LexemType::_input:
		l = get_lexem();
		if (!(l.type == LexemType::_ind)) {
			set_error_pos(l.pos);
			set_error_msg("Expected a identificator");
			return false;
		}
		WriteVar(l.lex);
		WriteCmd(Cmd::INPUT);
		return true;
		break;
	case LexemType::_output:
		if (!Operand())
			return false;
		WriteCmd(Cmd::OUT);
		return true;
		break;
	case LexemType::_do:
		int indStart = CurrentPtr(); // PN
		l = get_lexem();
		if (l.type != LexemType::_while) {
			set_error_pos(l.pos);
			set_error_msg("Expected a 'while' keyword");
			return false;
		}
		if (!BoolExp()) {
			return false;
		}
		// тут сформирована часть ПОЛИЗа для булевского выражения сформирована

		// тут формируем переход за цикл если условие ложное
		int indJmp = WriteCmdPtr(-1); // записываем фиктвный адресс (т.к. мы ещё не знаем где у нас конец цикла, т.е куда прыгать)
		WriteCmd(Cmd::JZ);

		if (!Operators()) {
			return false;
		}
		// тут часть ПОЛИЗа для операторов сформирована
		l = get_lexem();
		if (l.type != LexemType::_loop) {
			set_error_pos(l.pos);
			set_error_msg("Expected a 'loop' keyword");
			return false;
		}
		WriteCmdPtr(indStart); // занесли адресс начала цикла
		int indFinish = WriteCmd(Cmd::JMP); // заносим команду безусловного прыжка
		SetCmdPtr(indJmp, indFinish + 1); // перезаписываем адресс выхода из цикла
		return true;
		break;
	}
	set_error_pos(l.pos);
	set_error_msg("Any operator expected");
	return false;
}

bool ArithExp() {
	if (!ArithExp2())
		return false;
	if (!ArithExp1())
		return false;
	return true;
}

bool ArithExp1() {
	while (lexems_exist() && show_next_lexem().type == LexemType::_ao) {
		Lexem l = get_lexem();
		if (!ArithExp())
			return false;
		if (l.lex == "+")
			WriteCmd(Cmd::ADD);
		else if (l.lex == "-")
			WriteCmd(Cmd::SUB);
		else {
			set_error_pos(l.pos);
			set_error_msg("Expected a summary or subtraction operator");
			return false;
		}
	}
	return true;
}

bool ArithExp2() {
	if (!ArithExp4())
		return false;
	if (!ArithExp3())
		return false;
	return true;
}

bool ArithExp3() {
	while (lexems_exist() && ( show_next_lexem().type == LexemType::_mul || show_next_lexem().type == LexemType::_div)) {
		Lexem l = get_lexem();
		if (!ArithExp2())
			return false;
		if (l.lex == "*")
			WriteCmd(Cmd::MUL);
		else if (l.lex == "/") {
			WriteCmd(Cmd::DIV);
		}
		else {
			set_error_pos(l.pos);
			set_error_msg("Expected a multiplication or division operator");
			return false;
		}
	}
	return true;
}

bool ArithExp4() {
	if (show_next_lexem().type == LexemType::_lbracket) {
		Lexem l = get_lexem();
		if (!ArithExp())
			return false;
		if (!lexems_exist()) {
			set_error_pos(l.pos);
			set_error_msg("Expected a right close bracket");
			return false;
		}
		l = get_lexem();
		if (l.type != LexemType::_rbracket) {
			set_error_pos(l.pos);
			set_error_msg("Expected a right close bracket");
			return false;
		}
	}
	else if (!Operand()) {
		return false;
	}
	return true;
}