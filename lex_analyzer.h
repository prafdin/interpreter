#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <map>

enum LexemType { _do, _while,_loop, _and, _or, _input, _output, _comp, _ao, _div, _mul, _ind, _const, _assign, _semicolon, _lbracket,
	_rbracket };


struct Lexem {
	LexemType type;
	std::string lex;
	int pos;
	Lexem(std::string lex, LexemType type, int pos) : type(type), pos(pos), lex(lex) {}
};


bool lex_parse(const char* str);

bool lexems_exist();

Lexem get_lexem();

Lexem show_next_lexem();
