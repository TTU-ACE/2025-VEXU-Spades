#ifndef ROBOT_CONFIG_HPP
#define ROBOT_CONFIG_HPP

#include "main.h"
#include <cstdint>
#include "lemlib/api.hpp"

/**
 * \brief A struct that stores the port, gearset, and motor units for a motor.
 */
struct MotorConfig {
    signed char port;
    pros::v5::MotorGears gearset;
    pros::v5::MotorUnits units;
};

// -----------------------------
// Motor Ports & Configuration
// -----------------------------

// Example: Motor(portNumber, gearset, motorUnits)
inline MotorConfig LEFT_DRIVE_MOTOR_A  = {-20, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig LEFT_DRIVE_MOTOR_B  = {-18, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig LEFT_DRIVE_MOTOR_C  = { 14, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig RIGHT_DRIVE_MOTOR_A = { 10, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig RIGHT_DRIVE_MOTOR_B = {  7, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};
inline MotorConfig RIGHT_DRIVE_MOTOR_C = { -3, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::rotations};

inline MotorConfig HOOK_INTAKE_MOTOR  = { 6, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};
inline MotorConfig INTAKE_MOTOR       = {17, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};
inline MotorConfig HANG_MOTOR_A       = {-15, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};
inline MotorConfig HANG_MOTOR_B       = { 5, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};

inline MotorConfig CLAMP_MOTOR        = { 9, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};
inline MotorConfig TILT_MOTOR         = { 8, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};
inline MotorConfig LBROWN_LEFT_MOTOR  = {19, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};
inline MotorConfig LBROWN_RIGHT_MOTOR = {16, pros::v5::MotorGears::ratio_18_to_1, pros::v5::MotorUnits::degrees};

// Sensors
inline signed char imu_port = 4;

// ----------------------
// Lemlib Drivetrain
// ----------------------

// 1) Example linear motion controller gains
inline lemlib::ControllerSettings linearController(
    20.0,   // kP
    0.0,    // kI
    5,      // kD
    3,      // integral anti-windup
    1,      // small error range (inches)
    100,    // small error timeout (ms)
    3,      // large error range (inches)
    500,    // large error timeout (ms)
    10      // max slew (acceleration)
);

// Example angular motion controller gains
// P and D are tuned
inline lemlib::ControllerSettings angularController(
    8.0,    // kP
    0.0,    // kI
    50,     // kD
    3.0,    // integral anti-windup
    1.0,    // small error range (degrees)
    100,    // small error timeout (ms)
    3.0,    // large error range (degrees)
    500,    // large error timeout (ms)
    0.0     // max slew (acceleration)
);


// Optional input curves for driver control
inline lemlib::ExpoDriveCurve throttleCurve(
    3,     // joystick deadband out of 127
    10,    // minimum output
    1.019  // expo gain
);

inline lemlib::ExpoDriveCurve steerCurve(
    3,
    10,
    1.019
);

// Inertial Sensor on port 4
inline pros::Imu imu(4);

// 3) Odom sensors
// If you do not have tracking wheels or IMU for your small bot, set these to nullptr.
// If you have an inertial sensor, pass its pointer. 
inline lemlib::OdomSensors driveSensors(
    nullptr, // vertical tracking wheel
    nullptr, // second vertical tracking wheel
    nullptr, // horizontal tracking wheel
    nullptr, // second horizontal tracking wheel
    &imu  // inertial sensor pointer (e.g. new pros::Imu(...))
);

// Drivetrain mechanical parameters
//   - trackWidth and wheelDiameter might differ for your small bot. 
//   - "wheelRPM" depends on gearset. For a standard 18:1 gearset, 
//     free speed is ~200 RPM, or 600 RPM if 6:1, etc.
//   - externalGearRatio is 1.0 if there's no external chaining/belting that changes ratio.
inline double SM_BOT_TRACK_WIDTH   = 12.5;                      // measure in inches 
inline double SM_BOT_WHEEL_DIAM    = lemlib::Omniwheel::NEW_4;  // e.g., 4" wheels
inline double SM_BOT_WHEEL_RPM     = 200.0;                     // TODO determine based on gearbox and drivetrain gearset
// TODO - set these to small bot's values

// -----------------------------
// Subsystem Angle Presets
// -----------------------------

// Clamp angles
// Clamp must start from fully pushed in position, therefore it must positively extend for both angles
inline double CLAMP_DOWN_ANGLE = 360; 
inline double CLAMP_UP_ANGLE   = 700;   

// Tilt angles
inline double TILT_UP_ANGLE    = 0.0;
inline double TILT_DOWN_ANGLE  = -180.0;
inline double TILT_INIT_ANGLE  = 0.0;

// Lady Brown (LB) servo angles
inline double LB_LEFT_UP_ANGLE       = 300.0;
inline double LB_LEFT_DOWN_ANGLE     = 0.0;
inline double LB_LEFT_ABUTTON_ANGLE  = 67.25;
inline double LB_LEFT_INIT_ANGLE     = 180.0;

inline double LB_RIGHT_UP_ANGLE      = -300.0;
inline double LB_RIGHT_DOWN_ANGLE    = 0.0;
inline double LB_RIGHT_ABUTTON_ANGLE = -67.25;
inline double LB_RIGHT_INIT_ANGLE    = -180.0;

//lift positions
inline double HANG_LOWERED_POSITION = 100;
inline double HANG_RAISED_POSITION  = 1138.0;

// Hang / Arm speeds
inline int    HANG_SPEED_PCT = 200;
 
// Intake speed
inline int    INTAKE_SPEED_PCT = 100;

// Hook intake speed
inline int    HOOK_INTAKE_SPEED_PCT = 100;

// For ring detection placeholders
inline bool detect_blue_mode = false;
inline bool abort_detection  = false;
inline int GAME_ELEMENT_THRESHOLD = 70;  // example threshold for ring detection

// Toggle states
enum class ButtonToggleState {
    NOT_BEGUN,
    PRESSED,
    FINISHED
};

#endif  // ROBOT_CONFIG_HPP
