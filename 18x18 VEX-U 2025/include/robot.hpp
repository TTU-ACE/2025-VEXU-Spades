// robot.hpp
#ifndef ROBOT_HPP
#define ROBOT_HPP

//#pragma once
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
        void setClamp(double position);
        void releaseClamp();

        // Conveyor functions
        void setConveyorSpeed(double speed);

        // Lift functions
        void setLift(double position);
        void releaseLift();
        

        float getClampPosition();
        float getHangPosition();

        // Utility functions
        float getMaxVelocity(pros::Motor motor);

        // drive base and path following
        pros::MotorGroup leftBase;
        pros::MotorGroup rightBase;
        lemlib::Drivetrain drivetrain;

        //chassis
        lemlib::Chassis chassis;

        pros::Motor intakeMotor;
        pros::Motor clampMotor;
        
        pros::MotorGroup conveyor;
        pros::MotorGroup lift;

};

#endif