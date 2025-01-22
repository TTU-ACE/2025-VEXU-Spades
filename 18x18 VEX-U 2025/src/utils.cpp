#include "utils.hpp"
#include "api.h"

void debugln(const char* msg, int line) {
    pros::lcd::print(line, msg);
    std::cout << msg << std::endl;
}