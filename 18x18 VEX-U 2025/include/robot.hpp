// robot.hpp
#pragma once
#include "robot-config.hpp"
#include "api.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

class Robot {
public:
    Robot();

    // Drive functions
    void tankDrive(double leftSpeed, double rightSpeed);
    void arcadeDrive(double forwardSpeed, double turnSpeed);

    // Intake functions
    void setIntakeSpeed(double speed);

    // Clamping mechanism functions
    void clamp(double clamped_position, double speed);
    void unclamp(double unclamped_position, double speed);

    // Conveyor functions
    void setConveyorSpeed(double speed);

    // Lift functions
    void raiseLift(double speed);
    void lowerLift(double speed);
    void stopLift();

    // Utility functions
    float getMaxMotorRPM(pros::Motor motor);

private:
    // drive base and path following
    pros::MotorGroup leftBase;
    pros::MotorGroup rightBase;
    lemlib::Drivetrain drivetrain;
    lemlib::Chassis chassis;

    pros::Motor intakeMotor;
    pros::Motor clampMotor;

    pros::MotorGroup conveyor;
    pros::MotorGroup lift;
};