// Constants
#ifndef MOTOR_CONFIG_HPP
#define MOTOR_CONFIG_HPP

#include "main.h"


struct MotorConfig {
    int PORT_NUMBER;
    pros::v5::MotorGears GEARSET;
    pros::v5::MotorUnits MOTOR_UNITS;
    bool REVERSED;
};

//motor configurations
inline MotorConfig FRONT_LEFT_MOTOR = {1, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig FRONT_RIGHT_MOTOR = {2, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig BACK_LEFT_MOTOR = {3, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig BACK_RIGHT_MOTOR = {4, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};

inline MotorConfig CONVEYOR_LEFT_MOTOR = {5, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig CONVEYOR_RIGHT_MOTOR = {6, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, true};

inline MotorConfig LIFT_LEFT_MOTOR = {7, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig LIFT_RIGHT_MOTOR = {8, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, true};

inline MotorConfig INTAKE_MOTOR = {9, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};

inline MotorConfig CLAMP_MOTOR = {10, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};

//control constants

#endif  // MOTOR_CONFIG_HPP