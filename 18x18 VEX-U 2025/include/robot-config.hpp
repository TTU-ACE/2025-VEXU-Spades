// Constants 
#ifndef ROBOT_CONFIG_HPP
#define ROBOT_CONFIG_HPP

#include "main.h"
#include "lemlib/api.hpp"
//#include "robot-config.hpp"
#include <cstdint>


struct MotorConfig {
    signed char PORT_NUMBER;
    pros::v5::MotorGears GEARSET;
    pros::v5::MotorUnits MOTOR_UNITS;
};

//=============================================================================
// Drive Train and Chassis Configuration
//=============================================================================

//motor configurations
inline MotorConfig FRONT_LEFT_MOTOR =   {-3, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig FRONT_RIGHT_MOTOR =  {8, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig BACK_LEFT_MOTOR =    {-1, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig BACK_RIGHT_MOTOR =   {6, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

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
// PID Controllers
//=============================================================================

//https://lemlib.readthedocs.io/en/stable/api/utils.html#pid
// create a PID
// lemlib::PID clampPid(5, // kP
//                 0.02, // kI
//                 20, // kD
//                 0.01,// integral anti windup range
//                 true // don't reset integral when sign of error flips
// ); 

// lemlib::PID liftPid(5, // kP
//                 0.01, // kI
//                 20, // kD
//                 5, // integral anti windup range
//                 false // don't reset integral when sign of error flips
// ); 
// Initialize robot components

//=============================================================================
// Subsystems
//=============================================================================

inline MotorConfig CONVEYOR_LEFT_MOTOR = {5, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig CONVEYOR_RIGHT_MOTOR = {-7, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig LIFT_LEFT_MOTOR = {-8, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig LIFT_RIGHT_MOTOR = {10, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig INTAKE_MOTOR = {2, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig CLAMP_MOTOR = {9, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};

//control constants
inline float clampedStatePosition = 0;
inline float unclampedStatePosition = 240;
//float clampCurrentPosition;
inline float clampDesiredState;

inline double liftedStatePosition = 1121;
inline double loweredStatePosition = 50;
inline double liftDesiredState;
// inline double liftPosition;

inline double clampSpeedConstant = 50;
inline double liftSpeedConstant = 50;
inline double conveyorSpeedConstant = 50;
inline double intakeSpeedConstant = 50;

//inline double deadband = 0;


#endif  // ROBOT_CONFIG_HPP
