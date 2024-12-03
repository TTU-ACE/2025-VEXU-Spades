// robot.hpp
#pragma once
#include "robot-config.hpp"
#include "api.h"

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
    // Private members if needed
    pros::Motor backLeftMotor;
    pros::Motor backRightMotor;
    pros::Motor frontLeftMotor;
    pros::Motor frontRightMotor;
    pros::Motor intakeMotor;
    pros::Motor clampMotor;
    pros::Motor leftConveyorMotor;
    pros::Motor rightConveyorMotor;
    pros::Motor leftLiftMotor;
    pros::Motor rightLiftMotor;

    pros::MotorGroup leftBase;
    pros::MotorGroup rightBase;
    pros::MotorGroup conveyor;
    pros::MotorGroup lift;
};