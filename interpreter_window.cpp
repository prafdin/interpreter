#pragma once
#include "interpreter_window.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

#include <QListWidget>
#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QKeyEvent>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextStream>
#include <QInputDialog>
#include <QDebug>
#include <QDir>

#include "lex_analyzer.h"
#include "parser.h"
#include "postfix_handler.h"
#include "interpreter.h"
#include "common.h"





InterpreterWindow::InterpreterWindow(QWidget* parent) : QWidget(parent)  {
	QGridLayout* grid = new QGridLayout(this);


	grid->setSpacing(2);

	QLabel* labelInput = new QLabel("Input", this);
	QLabel* labelOutput = new QLabel("Output", this);
	

	textInput = new QTextEdit;
	textOutput = new QTextEdit;

	QFont font;
	font.setPointSize(15);
	this->setFont(font);

	QPushButton* inputButton = new QPushButton("Input");
	QPushButton* clearButton = new QPushButton("Clear");

	connect(inputButton, SIGNAL(clicked()), this, SLOT(interpret()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
	
	

	grid->addWidget(labelInput, 0, 0);
	grid->addWidget(labelOutput, 0, 1);
	grid->addWidget(textInput, 1, 0);
	grid->addWidget(textOutput, 1, 1);
	grid->addWidget(inputButton, 2, 0);
	grid->addWidget(clearButton, 2, 1);

	

	setLayout(grid);
}

#include <QColor>

void InterpreterWindow::interpret() {
	std::string str = textInput->toPlainText().toStdString();
	if (!lex_parse(str.c_str())) {
		print_err(str);
		CleanLexems();
		return;
	}
	if (!Operators()) {
		print_err(str);
		CleanPostfix();
		CleanLexems();
		return;
	}
	try {
		Interpret(*this);
	}
	catch (std::logic_error& e) {
		textOutput->setPlainText(QString(e.what()));
	}
	CleanPostfix();
	
}

void InterpreterWindow::clear() {
	textOutput->clear();
}

void InterpreterWindow::PrintMsg(QString& str) {
	textOutput->append(str);
}

size_t InterpreterWindow::ScanNum() {
	QString num = QInputDialog::getText(this, "Enter number",
		"Enter value for variable");

	while (![](std::string str) {return std::all_of(str.begin(), str.end(), std::isdigit); }(num.toStdString())) {
		num = QInputDialog::getText(this, "Enter number",
			"Enter value for variable");
	}

	return std::stoull(num.toStdString());
}

void InterpreterWindow::print_err(std:: string str) {
	int len = strlen(str.c_str());
	int err_pos = get_error_pos();
	if (err_pos == -1) {
		textOutput->append(str.c_str());
		textOutput->append(get_error_msg().c_str());
		return;
	}
	char before_err[255];
	memset(before_err, 0, 255);
	memcpy(before_err, str.c_str(), err_pos);
	char err[2];
	memset(err, 0, 2);
	memcpy(err, str.c_str() + err_pos, 1);
	char after_err[255];
	memset(after_err, 0, 255);
	memcpy(after_err, str.c_str() + err_pos + 1, len - err_pos);

	QColor old_colour(0, 0, 0, 255);
	QColor red(255, 0, 0, 255);

	textOutput->setPlainText(QString(before_err));
	textOutput->moveCursor(QTextCursor::End);
	textOutput->setTextColor(red);
	textOutput->insertPlainText(QString(err));
	textOutput->moveCursor(QTextCursor::End);

	textOutput->moveCursor(QTextCursor::End);
	textOutput->setTextColor(old_colour);
	if (!strlen(after_err))
		textOutput->insertPlainText(QString(" "));
	textOutput->insertPlainText(QString(after_err));
	textOutput->moveCursor(QTextCursor::End);

	textOutput->append(get_error_msg().c_str());
}