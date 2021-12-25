#include <string>

#include "common.h"

static int error_pos;
static std::string error_msg;

void set_error_pos(int pos) {
	error_pos = pos;
}
void set_error_msg(std::string msg) {
	error_msg = msg;
}

int get_error_pos() {
	return error_pos;
}

std::string get_error_msg() {
	return error_msg;
}