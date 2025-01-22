#include "utils.hpp"
#include "api.h"

void debugln(const char* msg) {
    pros::lcd::print(2, msg);
    std::cout << msg << std::endl;
}