// robot.cpp
#include "robot.hpp"
#include "robot-config.hpp"

Robot::Robot() {
    // Initialize robot components if needed
    // backLeftMotor = pros::Motor(BACK_LEFT_MOTOR);
    // backRightMotor = pros::Motor(BACK_RIGHT_MOTOR);
    // frontLeftMotor = pros::Motor(FRONT_LEFT_MOTOR);
    // frontRightMotor = pros::Motor(FRONT_RIGHT_MOTOR);

    // intakeMotor = pros::Motor(INTAKE_MOTOR);
    // clampMotor = pros::Motor(CLAMP_MOTOR);

    // leftConveyorMotor = pros::Motor(CONVEYOR_LEFT_MOTOR);
    // rightConveyorMotor = pros::Motor(CONVEYOR_RIGHT_MOTOR);

    // leftLiftMotor = pros::Motor(LIFT_LEFT_MOTOR);
    // rightLiftMotor = pros::Motor(LIFT_RIGHT_MOTOR);

    //motor groups
    // leftBase = pros::MotorGroup(frontLeftMotor, backLeftMotor);
    // rightBase = pros::MotorGroup(frontRightMotor,backRightMotor);

    // conveyor = pros::MotorGroup(leftConveyorMotor, rightConveyorMotor);
    // lift = pros::MotorGroup(leftLiftMotor, rightLiftMotor);
}

// Implementation for tank drive with RPM values
void Robot::tankDrive(double leftSpeed_rpm, double rightSpeed_rpm) {
    // backLeftMotor.move_velocity(leftSpeed_rpm);
    // frontLeftMotor.move_velocity(leftSpeed_rpm);
    // backRightMotor.move_velocity(rightSpeed_rpm);
    // frontRightMotor.move_velocity(rightSpeed_rpm);

    leftBase.move_velocity(leftSpeed_rpm);
    rightBase.move_velocity(rightSpeed_rpm);
}

void Robot::arcadeDrive(double forwardSpeed_rpm, double turnSpeed_rpm) {
    // Implementation for arcade drive
    // Calculate left and right motor speeds based on forward and turn speeds
    double leftSpeed_rpm = forwardSpeed_rpm + turnSpeed_rpm;
    double rightSpeed_rpm = forwardSpeed_rpm - turnSpeed_rpm;

    // Get max RPM of motors for normalization if out of bounds
    float maxRPM = getMaxMotorRPM(backLeftMotor);  
    
    // Normalize speeds if they exceed maxRPM
    double maxInput = std::max(std::abs(leftSpeed_rpm), std::abs(rightSpeed_rpm));
    if (maxInput > maxRPM) {
        leftSpeed_rpm = (leftSpeed_rpm / maxInput) * maxRPM;
        rightSpeed_rpm = (rightSpeed_rpm / maxInput) * maxRPM;
    }

    // Set motor speeds for left and right sides in RPM
    // backLeftMotor.move_velocity(leftSpeed_rpm);
    // frontLeftMotor.move_velocity(leftSpeed_rpm);
    // backRightMotor.move_velocity(rightSpeed_rpm);
    // frontRightMotor.move_velocity(rightSpeed_rpm);

    leftBase.move_velocity(leftSpeed_rpm);
    rightBase.move_velocity(rightSpeed_rpm);
}

// Implementation for setting intake speed
void Robot::setIntakeSpeed(double speed) {
    //set intake at speed
    intakeMotor.move_velocity(speed);
}

void Robot::clamp(double clamped_position, double speed) {
    //get encoder value
    double position = clampMotor.get_position();

    //checks if current position equals desired position
    if (position != clamped_position) {
        //move to position based on absolute location
        clampMotor.move_absolute(clamped_position, speed);
    }
}

void Robot::unclamp(double unclamped_position, double speed) {
    //get encoder value
    double position = clampMotor.get_position();

    //checks if current position equals desired position
    if (position != unclamped_position) {
        //move to position based on absolute location
        clampMotor.move_absolute(unclamped_position, speed);
    }
}

void Robot::setConveyorSpeed(double speed) {
    //set conveyor at speed
    conveyor.move_velocity(speed);
}

void Robot::raiseLift(double speed) {
    
}

void Robot::lowerLift(double speed) {
    
}

void Robot::stopLift() {
    
}

/**
 * Get the maximum RPM of a motor based on its gearset
 */
float Robot::getMaxMotorRPM(pros::Motor motor) {
    float maxRPM;

    //maxRPM is dependent on the gearset of the motors
    switch (motor.get_gearing()) {
        case pros::MotorGears::ratio_36_to_1:
            maxRPM = 100;
            break;
        case pros::MotorGears::ratio_18_to_1:
            maxRPM = 200;
            break;
        case pros::MotorGears::ratio_6_to_1:
            maxRPM = 600;
            break;
        default:
            maxRPM = 200; // Default to a common gearset if unknown
            break;
    }

    return maxRPM;
}