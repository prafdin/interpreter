#pragma once
#include <string>

void set_error_pos(int pos);
void set_error_msg(std::string);
int get_error_pos();
std::string get_error_msg();