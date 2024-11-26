// Constants
#ifndef MOTOR_CONFIG_HPP
#define MOTOR_CONFIG_HPP

#include "main.h"


struct MotorConfig {
    int PORT_NUMBER;
    pros::v5::MotorGears GEARSET;
    pros::v5::MotorUnits MOTOR_UNITS;
};

//motor configurations
inline MotorConfig FRONT_LEFT_MOTOR = {1, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig FRONT_RIGHT_MOTOR = {2, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig BACK_LEFT_MOTOR = {3, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig BACK_RIGHT_MOTOR = {4, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig CONVEYOR_LEFT_MOTOR = {5, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig CONVEYOR_RIGHT_MOTOR = {6, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig LIFT_LEFT_MOTOR = {7, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig LIFT_RIGHT_MOTOR = {8, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig INTAKE_MOTOR = {9, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig CLAMP_MOTOR = {10, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

//controller button mapping
const int BUTTON_A = 1;
const int BUTTON_B = 2;
const int BUTTON_X = 3;
const int BUTTON_Y = 4;

const int DPAD_UP = 5;
const int DPAD_DOWN = 6;
const int DPAD_RIGHT = 7;
const int DPAD_LEFT = 8;

const int LEFT_TOP_BUMPER = 9;
const int LEFT_BOTTOM_BUMPER = 10;
const int RIGHT_TOP_BUMPER = 11;
const int RIGHT_BOTTOM_BUMPER = 12;


//control constants





#endif  // MOTOR_CONFIG_HPP