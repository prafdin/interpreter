#include <cctype>
#include <stddef.h>
#include <queue>

#include "lex_analyzer.h"
#include "common.h"

using namespace std;

enum State { S, Al, Nu, Ar, Co1, Co2, lBr, rBr, Se , As, F, E };

queue <Lexem> lexems;

std::map <std::string, LexemType> base_lex = {
	{"do", _do }, {"while", _while } ,{"loop", _loop } , {"and", _and } , {"or", _or } , {"input", _input } , {"output", _output } , {">", _comp } ,
	 {"<", _comp } , {"<>", _comp} , {"=", _assign } , {"+", _ao } , {"-", _ao } , {"/", _div } ,{"*", _mul } , {"ind",_ind},{"const",_const},
	{";" , _semicolon}, {"(", _lbracket}, {")", _rbracket}
};




void add_lex(State prev_state ,const char* text_start,const char* lex_start, const char* lex_end) {
	string lex(lex_start, lex_end);
	ptrdiff_t position = lex_start- text_start ;
	if (base_lex.find(lex) != base_lex.end())
		lexems.push(Lexem(lex, base_lex[lex.c_str()], position));
	else if (prev_state == Al) 
		lexems.push(Lexem(lex, base_lex["ind"], position));
	else if (prev_state == Nu)
		lexems.push(Lexem(lex, base_lex["const"], position));
	// TODO Throw exception 
		
}

bool check_Ar(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
		return true;
	else
		return false;
}

bool lex_parse(const char* str) {
	State st = S, prev_st = S ;
	const char* lex_start = str;
	const char* cur_pos = str;
	bool add = true;
	while (st != E && st != F) {
		add = true;
		prev_st = st;
		switch (st)
		{
	
		case S:
			if (isalpha(*cur_pos)) {
				st = Al;
			}
			else if (isdigit(*cur_pos)) {
				st = Nu;
			}
			else if (isspace(*cur_pos)) {
				st = S;
			}
			else if (check_Ar(*cur_pos)) {
				st = Ar;
			}
			else if (*cur_pos == '<') {
				st = Co1;
			}
			else if (*cur_pos == '>') {
				st = Co2;
			}
			else if (*cur_pos == '=') {
				st = As;
			}
			else if (*cur_pos == '(') {
				st = lBr;
			}
			else if (*cur_pos == ')') {
				st = rBr;
			}
			else if (*cur_pos == ';') {
				st = Se;
			}
			else if (*cur_pos == 0) {
				st = S;
			}
			else st = E;
			add = false;
			break;
		case Al:
			if (isalpha(*cur_pos) || isdigit(*cur_pos)) {
				st = Al;
				add = false;
			}
			else if (isspace(*cur_pos)) {
				st = S;
			}
			else if (check_Ar(*cur_pos)) {
				st = Ar;
			}
			else if (*cur_pos == '=' ) {
				st = As;
			}
			else if (*cur_pos == '<' ) {
				st = Co1;
			}
			else if (*cur_pos == '>') {
				st = Co2;
			}
			else if (*cur_pos == ')') {
				st = rBr;
			}
			else if (*cur_pos == ';') {
				st = Se;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else  st = E;
			break;
		case Nu:
			if (isdigit(*cur_pos)) {
				st = Nu;
				add = false;
			}
			else if (isspace(*cur_pos)) {
				st = S;
			}
			else if (*cur_pos ==  '=') {
				st = As;
			}
			else if (*cur_pos == ';') {
				st = Se;
			}
			else if (check_Ar(*cur_pos)) {
				st = Ar;
			}
			else if (*cur_pos == '<') {
				st = Co1;
			}
			else if (*cur_pos == '>') {
				st = Co2;
			}
			else if (*cur_pos == ')') {
				st = rBr;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else st = E;
			break;
		case Ar:
			if (isspace(*cur_pos)) {
				st = S;
			}
			else if (isdigit(*cur_pos)) {
				st = Nu;
			}
			else if (isalpha(*cur_pos)) {
				st = Al;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else st = E;
			break;
		case Co1:
			if (isspace(*cur_pos)) {
				st = S;
			}
			else if (*cur_pos == '>') {
				st = Co2;
				add = false;
			}
			else if ( isdigit(*cur_pos)) {
				st = Nu;
			}
			else if (isalpha(*cur_pos)) {
				st = Al;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else st = E;
			break;
		case Co2:
			if (isspace(*cur_pos)) {
				st = S;
			}
			else if (isdigit(*cur_pos)) {
				st = Nu;
			}
			else if (isalpha(*cur_pos)) {
				st = Al;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else st = E;
			break;
		case lBr :
			if (isspace(*cur_pos)) {
				st = S;
			}
			else if (isdigit(*cur_pos)) {
				st = Nu;
			}
			else if (isalpha(*cur_pos)) {
				st = Al;
			}
			else if (*cur_pos == '(') {
				st = lBr;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else st = E;
			break;


		case rBr:
			if (isspace(*cur_pos)) {
				st = S;
			}
			else if (*cur_pos == ';') {
				st = Se;
			}
			else if (*cur_pos == ')') {
				st = rBr;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else st = E;
			break;
		case Se:
			if (isspace(*cur_pos)) {
				st = S;
			}
			else if (isalpha(*cur_pos)) {
				st = Al;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else st = E;
			break;
		case As:
			if (isspace(*cur_pos)) {
				st = S;
			}
			else if (isalpha(*cur_pos)) {
				st = Al;
			}
			else if (isdigit(*cur_pos)) {
				st = Nu;
			}
			else if (*cur_pos == '(') {
				st = lBr;
			}
			else if (*cur_pos == 0) {
				st = F;
			}
			else st = E;
			break;
		}
		if (add) {
			add_lex(prev_st, str, lex_start, cur_pos);
		}
		if ((st != prev_st) && (st != S))
			lex_start = cur_pos;
		++cur_pos;
	}
	
	if (st != E)
		return true;
	else {
		set_error_pos( lex_start - str );
		set_error_msg("Invalid syntax");
		return false;
	}
	
}

bool lexems_exist(){
	return lexems.size() != 0;
}

Lexem get_lexem() {
	Lexem lex = lexems.front();
	lexems.pop();
	return lex;
}

Lexem show_next_lexem() {
	return lexems.front();
}

