#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QTextEdit>
#include <QTextStream>


class InterpreterWindow : public QWidget {

    Q_OBJECT // for our slots

public:
    InterpreterWindow(QWidget* parent = 0);
	void PrintMsg(QString& str);
	size_t ScanNum();

private slots:
	void interpret();
	void clear();

private:
	QTextEdit* textInput;
	QTextEdit* textOutput;
	void print_err(std::string str);


};