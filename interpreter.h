#pragma once
#include <QTextEdit>
#include "postfix_handler.h"
#include "interpreter_window.h"

void Interpret(InterpreterWindow& window);

void PushElem(PostfixElem el);
size_t PopVal();
void SetVarAndPop(size_t val);
