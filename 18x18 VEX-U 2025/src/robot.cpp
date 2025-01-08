// robot.cpp
#include "robot.hpp"
#include "robot-config.hpp"

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
    // Initialize robot components
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

void Robot::clamp(double clamped_position, double speed) {
    //get encoder value
    double position = clampMotor.get_position();
    //std::cout<<"clamp"<< std::endl;

    //checks if current position equals desired position
    if (position != clamped_position) {
        //move to position based on absolute location
        clampMotor.move_absolute(clamped_position, speed);
    }
}

void Robot::unclamp(double unclamped_position, double speed) {
    //get encoder value
    double position = clampMotor.get_position();
   // std::cout<<"unclamp"<< std::endl;

    //checks if current position equals desired position
    if (position != unclamped_position) {
        //move to position based on absolute location
        clampMotor.move_absolute(unclamped_position, speed);
    }
}

void Robot::setConveyorSpeed(double speed) {
    //set conveyor at speed
    conveyor.move_velocity(speed);
    //std::cout<<"conveyor"<< std::endl;
}

void Robot::raiseLift(double speed) {
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
}

void Robot::returnPositionClamp() {
    std::cout << "Clamp Position: " << clampMotor.get_position() << std::endl;
    //clampMotor.get_position();
}

void Robot::returnPositionLift() {
    std::cout << "Lift Position: " << lift.get_position() << std::endl;
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
