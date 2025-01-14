// robot.cpp
#include "robot.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#include "robot-config.hpp"
#include <cstdint>

Robot::Robot() 
    : leftBase({BACK_LEFT_MOTOR.PORT_NUMBER, FRONT_LEFT_MOTOR.PORT_NUMBER}, BACK_LEFT_MOTOR.GEARSET),
      rightBase({FRONT_RIGHT_MOTOR.PORT_NUMBER, BACK_RIGHT_MOTOR.PORT_NUMBER}, BACK_RIGHT_MOTOR.GEARSET),
      drivetrain(       &leftBase, 
                        &rightBase, 
                        15.5,                       // Track width
                        lemlib::Omniwheel::NEW_4,   // Using new 4" omnis
                        360,                        // Drivetrain rpm is 360
                        2
      ),
      chassis(drivetrain, linearController, angularController, driveSensors, &throttleCurve, &steerCurve),  // Chassis for path following

      conveyor({CONVEYOR_LEFT_MOTOR.PORT_NUMBER, CONVEYOR_RIGHT_MOTOR.PORT_NUMBER}, CONVEYOR_LEFT_MOTOR.GEARSET),
      lift({LIFT_LEFT_MOTOR.PORT_NUMBER, LIFT_RIGHT_MOTOR.PORT_NUMBER}, LIFT_LEFT_MOTOR.GEARSET),
      intakeMotor(INTAKE_MOTOR.PORT_NUMBER, INTAKE_MOTOR.GEARSET, INTAKE_MOTOR.MOTOR_UNITS),
      clampMotor(CLAMP_MOTOR.PORT_NUMBER, CLAMP_MOTOR.GEARSET, CLAMP_MOTOR.MOTOR_UNITS) {

    clampMotor.set_zero_position(0);  // Set the zero position of the clamp motor. Robot setup should be done with the clamp in the down position
}

// Tank drive with raw joystick values using lemlib chassis
void Robot::tankDrive(double leftSpeed_raw, double rightSpeed_raw) {
    chassis.tank(leftSpeed_raw, rightSpeed_raw);
}

// Arcade drive with raw joystick values using lemlib chassis
void Robot::arcadeDrive(double forwardSpeed_raw, double turnSpeed_raw) {
    chassis.arcade(forwardSpeed_raw, turnSpeed_raw);
}

// Implementation for setting intake speed
void Robot::setIntakeSpeed(double speed) {
    //set intake at speed
    //std::cout<<"intake"<< std::endl;
    intakeMotor.move_velocity(speed);
}

void Robot::setClamp(double position) {
    //determine state based off input
    //     switch (state) {
    //     case 0: 
    //        clampDesiredState = clampedStatePosition; 
    //     case 1:
    //         clampDesiredState = unclampedStatePosition;

    // }
    const int32_t maxRPM = getMaxMotorRPM(clampMotor);  // Modify if needed
    //get encoder value
    std::cout << "Settting Clamp Position: " << position << std::endl;
    clampMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);  // Hold position when stopped
    clampMotor.move_absolute(position, maxRPM);
}

void Robot::releaseClamp() {
    clampMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);  // Release the brake for free movement
    clampMotor.move_voltage(0);
}

// TODO I believe that the pros::Motor class can hold the position of the motor using a built-in PID controller
// void Robot::updateClamp() {
//     //get encoder value
//     int32_t real_pos = clampMotor.get_position();
//     std::cout << "Clamp Position: " << real_pos << std::endl;

//     // Feed the error into the PID controller
//     int32_t error = clampSetpoint - real_pos;
//     float output = clampPID.update(error);
//     clampMotor.move_velocity(output);
// }

void Robot::setConveyorSpeed(double speed) {
    //set conveyor at speed
    conveyor.move_velocity(speed);
    //std::cout<<"conveyor"<< std::endl;
}

void Robot::setLift(double position) {
    //determine state by user input
    // switch (state) {
    //     case 0:
    //         liftDesiredState = liftedStatePosition;
    //     case 1:
    //         liftDesiredState = loweredStatePosition;
    // }

    const int32_t maxRPM = getMaxMotorRPM(LIFT_RIGHT_MOTOR.PORT_NUMBER);  // Modify if needed
    //get encoder value
    std::cout << "Settting Lift Position: " << position << std::endl;
    lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);  // Hold position when stopped
    lift.move_absolute(position, maxRPM);

	// while (!((lift.get_position() < (liftDesiredState + 5)) && (lift.get_position() > (liftDesiredState - 5)))) {
	//     lift.move_absolute(liftDesiredState, maxRPM);
	//     pros::delay(2);
	// }
}

void Robot::releaseLift() {
    lift.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

/*void Robot::raiseLift(double speed) {
    lift.move_velocity(speed);
    //std::cout<<"raiseLift"<< std::endl;
}

void Robot::lowerLift(double speed) {
    lift.move_velocity(-speed);
    //std::cout<<"lowerLift"<< std::endl;
}

void Robot::stopLift() {
    lift.brake();
    //std::cout<<"stopLift"<< std::endl;
}*/

void Robot::displayClampPosition() {
    std::cout << "Clamp Position: " << clampMotor.get_position() << std::endl;
    //clampMotor.get_position();
}

void Robot::returnPositionLift() {
    std::cout << "Lift Position: " << lift.get_position(1) << std::endl;
    //lift.get_position();
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
