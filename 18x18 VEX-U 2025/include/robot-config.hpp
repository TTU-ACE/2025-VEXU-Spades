// Constants 
#ifndef ROBOT_CONFIG_HPP
#define ROBOT_CONFIG_HPP

#include "main.h"
#include "lemlib/api.hpp"
//#include "robot-config.hpp"
#include <cstdint>

/**
 * \brief A struct that stores the port, gearset, and motor units for a motor.
 */
struct MotorConfig {
    signed char port;
    pros::v5::MotorGears gearset;
    pros::v5::MotorUnits units;
};

//=============================================================================
// Motor Ports & Configuration
//=============================================================================

inline MotorConfig FRONT_LEFT_MOTOR =   {-10, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig FRONT_RIGHT_MOTOR =  {1, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig BACK_LEFT_MOTOR =    {-7, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig BACK_RIGHT_MOTOR =   {4, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig CONVEYOR_LEFT_MOTOR = {-8, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig CONVEYOR_RIGHT_MOTOR = {3, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig LIFT_LEFT_MOTOR = {6, pros::v5::MotorGears::ratio_36_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig LIFT_RIGHT_MOTOR = {-5, pros::v5::MotorGears::ratio_36_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig INTAKE_MOTOR = {2, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig CLAMP_MOTOR = {9, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};



//=============================================================================
// Lemlib Drive Train Configuration
//=============================================================================

// lateral motion controller
inline lemlib::ControllerSettings linearController(     100,     // proportional gain (kP)
                                                        0,      // integral gain (kI)
                                                        60,      // derivative gain (kD)
                                                        3,      // anti windup
                                                        1,      // small error range, in inches
                                                        100,    // small error range timeout, in milliseconds
                                                        3,      // large error range, in inches
                                                        500,    // large error range timeout, in milliseconds
                                                        40      // maximum acceleration (slew)
);

// angular motion controller
// P and D are tuned
inline lemlib::ControllerSettings angularController(    13,     // proportional gain (kP)
                                                        0,      // integral gain (kI)
                                                        120,    // derivative gain (kD)
                                                        3,      // anti windup
                                                        0.5,    // small error range, in degrees
                                                        100,    // small error range timeout, in milliseconds
                                                        1.5,    // large error range, in degrees
                                                        500,    // large error range timeout, in milliseconds
                                                        0       // maximum acceleration (slew)
);

inline pros::Imu imu(11);

// sensors for odometry
inline lemlib::OdomSensors driveSensors(    nullptr, // vertical tracking wheel
                                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                                            nullptr, // horizontal tracking wheel
                                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                                            &imu     // inertial sensor
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
// Constants
//=============================================================================

// 5) Drivetrain mechanical parameters
//   - trackWidth and wheelDiameter might differ for your small bot. 
//   - "wheelRPM" depends on gearset. For a standard 18:1 gearset, 
//     free speed is ~200 RPM, or 600 RPM if 6:1, etc.
//   - externalGearRatio is 1.0 if there's no external chaining/belting that changes ratio.
inline double LG_BOT_TRACK_WIDTH   =   15.5;    // measure in inches 
inline double LG_BOT_WHEEL_DIAM    =   lemlib::Omniwheel::NEW_4;
inline double LG_BOT_WHEEL_RPM     =   360;    // TODO fix for 5:3 drivetrain and correct gearset
inline double LG_BOT_EXT_GEAR_RATIO=   2;


//control constants
inline float clampedStatePosition = 0;
inline float unclampedStatePosition = 200;
inline float clampDesiredState;

inline double liftedStatePosition = 2242;
inline double loweredStatePosition = 50;
inline double liftDesiredState;

//inline double clampSpeedConstant = 50;
//inline double liftSpeedConstant = 50;
inline double conveyorSpeedConstant = 130;
inline double intakeSpeedConstant = 150;

//inline double deadband = 0;

#endif  // ROBOT_CONFIG_HPP
