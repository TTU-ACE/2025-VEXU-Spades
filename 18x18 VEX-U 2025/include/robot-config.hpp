// Constants
#ifndef ROBOT_CONFIG_HPP
#define ROBOT_CONFIG_HPP

#include "main.h"
#include "lemlib/api.hpp"


struct MotorConfig {
    signed char PORT_NUMBER;
    pros::v5::MotorGears GEARSET;
    pros::v5::MotorUnits MOTOR_UNITS;
    bool REVERSED;
};

//=============================================================================
// Drive Train and Chassis Configuration
//=============================================================================

//motor configurations
inline MotorConfig FRONT_LEFT_MOTOR =   {1, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig FRONT_RIGHT_MOTOR =  {2, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig BACK_LEFT_MOTOR =    {3, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig BACK_RIGHT_MOTOR =   {4, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};

// lateral motion controller
inline lemlib::ControllerSettings linearController(     10,     // proportional gain (kP)
                                                        0,      // integral gain (kI)
                                                        3,      // derivative gain (kD)
                                                        3,      // anti windup
                                                        1,      // small error range, in inches
                                                        100,    // small error range timeout, in milliseconds
                                                        3,      // large error range, in inches
                                                        500,    // large error range timeout, in milliseconds
                                                        20      // maximum acceleration (slew)
);

// angular motion controller
inline lemlib::ControllerSettings angularController(    2,     // proportional gain (kP)
                                                        0,     // integral gain (kI)
                                                        10,    // derivative gain (kD)
                                                        3,     // anti windup
                                                        1,     // small error range, in degrees
                                                        100,   // small error range timeout, in milliseconds
                                                        3,     // large error range, in degrees
                                                        500,   // large error range timeout, in milliseconds
                                                        0      // maximum acceleration (slew)
);

// sensors for odometry
inline lemlib::OdomSensors driveSensors(    nullptr, // vertical tracking wheel
                                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                                            nullptr, // horizontal tracking wheel
                                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                                            nullptr  // inertial sensor
);

// input curve for throttle input during driver control
inline lemlib::ExpoDriveCurve throttleCurve(    3, // joystick deadband out of 127
                                                10, // minimum output where drivetrain will move out of 127
                                                1.019 // expo curve gain
);

// input curve for steer input during driver control
inline lemlib::ExpoDriveCurve steerCurve(   3, // joystick deadband out of 127
                                            10, // minimum output where drivetrain will move out of 127
                                            1.019 // expo curve gain
);


//=============================================================================
// Subsystems
//=============================================================================

inline MotorConfig CONVEYOR_LEFT_MOTOR = {6, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig CONVEYOR_RIGHT_MOTOR = {7, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, true};

inline MotorConfig LIFT_LEFT_MOTOR = {8, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};
inline MotorConfig LIFT_RIGHT_MOTOR = {21, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, true};

inline MotorConfig INTAKE_MOTOR = {5, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};

inline MotorConfig CLAMP_MOTOR = {9, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations, false};


//control constants

#endif  // ROBOT_CONFIG_HPP
