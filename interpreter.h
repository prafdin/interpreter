#pragma once
#include <QTextEdit>
#include "postfix_handler.h"
#include "interpreter_window.h"

void Interpret(InterpreterWindow& window);

void PushElem(PostfixElem el);
int PopVal();
void SetVarAndPop(int val);
