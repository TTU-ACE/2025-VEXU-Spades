#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "api.h"
#include "lemlib/api.hpp"
#include <iostream>

/**
 * \brief Class to encapsulate all robot subsystems and state.
 */
class Robot {
public:
    Robot();

    // --- Drivetrain / Movement ---
    void arcadeDrive(double forward, double turn);
    void tankDrive(double leftVel, double rightVel);

    // --- Subsystem Controls ---
    void setIntakeSpeed(double pct);
    void stopIntake();

    void setHookIntakeSpeed(double pct);
    void stopHookIntake();

    void raiseHang();
    void lowerHang();
    void stopHang();

    void clampIt();   // toggles clamp up/down
    void spinClampToAngle(double angle);

    void spinTiltToAngle(double angle);
    void spinLBLeftToAngle(double angle, bool wait = true);
    void spinLBRightToAngle(double angle, bool wait = true);

    // Moves the Lady Browns down, up, or to “A-button” angle
    void LBDown();
    void LBInit();
    void LBToAButton();

    // Resets or toggles the clamp
    void setIsClamped(bool val);
    bool getIsClamped() const;

    // AI Vision or ring detection placeholder
    void handleRingDetection();

    // Utility
    float getMaxVelocity(pros::Motor motor);

    // Public Subsystems
    lemlib::Chassis chassis;

private:
    // -------------- Motors --------------
    pros::Motor intakeMotor;
    pros::Motor hookIntakeMotor;

    // Hang is made of two motors, group them
    //pros::Motor hangMotorA;
    //pros::Motor hangMotorB;
    pros::MotorGroup hang;

    // Single clamp, tilt, lady brown motors
    pros::Motor clampMotor;
    pros::Motor tiltMotor;
    pros::Motor lbLeftMotor;
    pros::Motor lbRightMotor;

    // Drive groups and Lemlib chassis for path following
    pros::MotorGroup leftBase;
    pros::MotorGroup rightBase;
    lemlib::Drivetrain drivetrain;

    // Sensors
    pros::Imu imu;
    lemlib::OdomSensors driveSensors;

    // -------------- States --------------
    bool isClamped;  // track clamp state

    // -------------- Private Helpers --------------
    void spinMotorTo(pros::Motor& motor, double angleDeg, bool waitForCompletion = true);
};

#endif
