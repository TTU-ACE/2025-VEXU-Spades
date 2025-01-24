#include "robot.hpp"
#include "robot-config.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

Robot::Robot()
    : intakeMotor(INTAKE_MOTOR.port, INTAKE_MOTOR.gearset, INTAKE_MOTOR.units),
      hookIntakeMotor(HOOK_INTAKE_MOTOR.port, HOOK_INTAKE_MOTOR.gearset, HOOK_INTAKE_MOTOR.units),

      //hangMotorA(HANG_MOTOR_A.port, HANG_MOTOR_A.gearset, HANG_MOTOR_A.units),
      //hangMotorB(HANG_MOTOR_B.port, HANG_MOTOR_B.gearset, HANG_MOTOR_B.units),
      hang({HANG_MOTOR_A.port, HANG_MOTOR_B.port}, HANG_MOTOR_A.gearset, HANG_MOTOR_A.units),

      clampMotor(CLAMP_MOTOR.port, CLAMP_MOTOR.gearset, CLAMP_MOTOR.units),
      tiltMotor(TILT_MOTOR.port, TILT_MOTOR.gearset, TILT_MOTOR.units),
      lbLeftMotor(LBROWN_LEFT_MOTOR.port, LBROWN_LEFT_MOTOR.gearset, LBROWN_LEFT_MOTOR.units),
      lbRightMotor(LBROWN_RIGHT_MOTOR.port, LBROWN_RIGHT_MOTOR.gearset, LBROWN_RIGHT_MOTOR.units),

      // Create MotorGroups for the drive base
      leftBase({LEFT_DRIVE_MOTOR_A.port, LEFT_DRIVE_MOTOR_B.port, LEFT_DRIVE_MOTOR_C.port}, LEFT_DRIVE_MOTOR_A.gearset, LEFT_DRIVE_MOTOR_A.units),
      rightBase({RIGHT_DRIVE_MOTOR_A.port, RIGHT_DRIVE_MOTOR_B.port, RIGHT_DRIVE_MOTOR_C.port}, RIGHT_DRIVE_MOTOR_A.gearset, RIGHT_DRIVE_MOTOR_A.units),

      // Create a Lemlib Drivetrain
      drivetrain(
         &leftBase, 
         &rightBase,
         SM_BOT_TRACK_WIDTH,            // Track width in inches
         SM_BOT_WHEEL_DIAM,
         SM_BOT_WHEEL_RPM,               // Wheel RPM (200 for 18:1)
         0
      ),

      // Create the Chassis object
      chassis(
         drivetrain,
         linearController,      // From robot-config
         angularController,     // From robot-config
         driveSensors,          // From robot-config
         &throttleCurve,        // Optional joystick expo curve
         &steerCurve            // Optional joystick expo curve
      ),

      isClamped(false)
{
     // ----------------------
    // Set motor reversing
    // ----------------------

    // If intake or other motors need reversing, do so here:
    // intakeMotor.set_reversed(false);
    // hookIntakeMotor.set_reversed(false);
    // hangMotorA.set_reversed(false);
    // hangMotorB.set_reversed(false);
    // clampMotor.set_reversed(false);
    // tiltMotor.set_reversed(false);
    // lbLeftMotor.set_reversed(false);
    // lbRightMotor.set_reversed(false);


    // Configure motors that need special brake modes or torque
    clampMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    tiltMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    lbLeftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    lbRightMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    //hangMotorA.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    //hangMotorB.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    hang.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);


    // If your small bot initialization includes setting initial angles:
    clampMotor.tare_position();
    tiltMotor.tare_position();
    lbLeftMotor.tare_position();
    lbRightMotor.tare_position();

    // Move to your initial positions
    // spinTiltToAngle(TILT_INIT_ANGLE);
    // pros::delay(750);

    // Clamp up
    // clampMotor.move_absolute(CLAMP_UP_ANGLE, getMaxVelocity(clampMotor));
    // spinClampToAngle(CLAMP_UP_ANGLE);
    // pros::delay(500);

    // Lady Brown arms down
    // lbLeftMotor.move_absolute(LB_LEFT_DOWN_ANGLE, getMaxVelocity(lbLeftMotor));
    // lbRightMotor.move_absolute(LB_RIGHT_DOWN_ANGLE, getMaxVelocity(lbRightMotor));

    // Move them
    // spinLBLeftToAngle(LB_LEFT_DOWN_ANGLE, false);
    // spinLBRightToAngle(LB_RIGHT_DOWN_ANGLE, true);
    // pros::delay(750);

    // could do more setup if needed
}

// --------------------------------------------------------------------
// Driving with Lemlib
// --------------------------------------------------------------------

// Arcade drive using Lemlib's chassis
void Robot::arcadeDrive(double forward, double turn) {
    chassis.arcade(forward, turn);
}

// Tank drive using Lemlib's chassis
void Robot::tankDrive(double leftVel, double rightVel) {
    chassis.tank(leftVel, rightVel);
}

/**
 * Sets the intake motor velocity
 */
void Robot::setIntakeSpeed(double pct) {
    intakeMotor.move_velocity(pct * 6.0); 
    // PROS move_velocity is in RPM by default, 
    // Multiply by 6 to approximate 600RPM = 100% if using 18:1 gearset
}
void Robot::stopIntake() {
    intakeMotor.move_velocity(0);
}

/**
 * Sets the hook intake motor velocity
 */
void Robot::setHookIntakeSpeed(double pct) {
    hookIntakeMotor.move_velocity(pct * 6.0);
}
void Robot::stopHookIntake() {
    hookIntakeMotor.move_velocity(0);
}

/**
 * Hang motor group
 */
void Robot::raiseHang() {
    // Move both hang motors up
    //hangMotorA.move_velocity(HANG_SPEED_PCT * 6.0);
    //hangMotorB.move_velocity(HANG_SPEED_PCT * 6.0);
    hang.move_velocity(HANG_SPEED_PCT);
    //hang.move_absolute(HANG_RAISED_POSITION, HANG_SPEED_PCT);
}

void Robot::lowerHang() {
    // Move both hang motors down
    //hangMotorA.move_velocity(-HANG_SPEED_PCT * 6.0);
    //hangMotorB.move_velocity(-HANG_SPEED_PCT * 6.0);
    hang.move_velocity(-HANG_SPEED_PCT);
    //hang.move_absolute(HANG_LOWERED_POSITION, HANG_SPEED_PCT);
}

void Robot::stopHang() {
    //hangMotorA.move_velocity(0);
    //hangMotorB.move_velocity(0);
    hang.move_velocity(0);
}

/**
 * Toggles clamp: if open, close it; if closed, open it
 */
void Robot::clampIt() {
    if (!isClamped) {
        lowerClamp();
    } else {
        raiseClamp();
    }
}

void Robot::raiseClamp() {
    spinClampToAngle(CLAMP_UP_ANGLE);
    isClamped = false;
}

void Robot::lowerClamp() {
    spinClampToAngle(CLAMP_DOWN_ANGLE);
    isClamped = true;
}

/**
 * Move clamp to specific angle
 */
void Robot::spinClampToAngle(double angle) {
    clampMotor.move_absolute(angle, 100); 
}

/**
 * Tilt to a specific angle
 */
void Robot::spinTiltToAngle(double angle) {
    tiltMotor.move_absolute(angle, 100);
    // while (fabs(tiltMotor.get_position() - angle) > 5) {
    //     pros::delay(20);
    // }
}

/**
 * Move lady brown left motor to angle
 */
void Robot::spinLBLeftToAngle(double angle, bool wait) {
    lbLeftMotor.move_absolute(angle, 100);
    if (wait) {
        while (fabs(lbLeftMotor.get_position() - angle) > 5) {
            pros::delay(20);
        }
    }
}

/**
 * Move lady brown right motor to angle
 */
void Robot::spinLBRightToAngle(double angle, bool wait) {
    lbRightMotor.move_absolute(angle, 100);
    if (wait) {
        while (fabs(lbRightMotor.get_position() - angle) > 5) {
            pros::delay(20);
        }
    }
}

/**
 * Lady Brown arms down
 */
void Robot::LBDown() {
    spinLBLeftToAngle(LB_LEFT_DOWN_ANGLE, false);
    spinLBRightToAngle(LB_RIGHT_DOWN_ANGLE, false);
}

void Robot::LBUp() {
    spinLBLeftToAngle(LB_LEFT_UP_ANGLE, false);
    spinLBRightToAngle(LB_RIGHT_UP_ANGLE, false);
}

/**
 * Lady Brown arms init
 */
void Robot::LBInit() {
    spinLBLeftToAngle(LB_LEFT_INIT_ANGLE, false);
    spinLBRightToAngle(LB_RIGHT_INIT_ANGLE, false);
}

/**
 * Lady Brown arms to "A button" angle
 */
void Robot::LBToAButton() {
    spinLBLeftToAngle(LB_LEFT_ABUTTON_ANGLE, false);
    spinLBRightToAngle(LB_RIGHT_ABUTTON_ANGLE, false);
}

// For toggling clamp in other places if needed
void Robot::setIsClamped(bool val) {
    isClamped = val;
}
bool Robot::getIsClamped() const {
    return isClamped;
}

/**
 * Placeholder for ring detection logic
 */
void Robot::handleRingDetection() {
    // Because PROS does not natively support the same AiVision calls as VEXcode Python,
    // you’d either integrate a 3rd-party library or stub the code here.
    // For example:
    if (detect_blue_mode && !abort_detection) {
        // 1) Acquire camera image
        // 2) Look for color signature
        // 3) If ring width > GAME_ELEMENT_THRESHOLD => do something
        // ...
        // If ring found:
        //pros::delay(1500);
        stopHookIntake();
        LBInit(); // or LBDown / LBUp
        detect_blue_mode = false;
    }
}

// Returns position of hang in degrees
float Robot::getHangPosition()
{
    return hang.get_position(0); // Get position of one of the motors
}

// Returns position of clamp in degrees
float Robot::getClampPosition()
{
    return clampMotor.get_position(0);
}

float Robot::getTiltPosition()
{
    return tiltMotor.get_position();
}

/**
 * Get the maximum velocity of a motor based on its gearset and units
 */
float Robot::getMaxVelocity(pros::Motor motor) {
    float maxVelocity;

    // maxVelocity is dependent on the gearset of the motors
    switch (motor.get_gearing()) {
        case pros::MotorGears::ratio_36_to_1:
            maxVelocity = 100;
            break;
        case pros::MotorGears::ratio_18_to_1:
            maxVelocity = 200;
            break;
        case pros::MotorGears::ratio_6_to_1:
            maxVelocity = 600;
            break;
        default:
            maxVelocity = 200; // Default to a common gearset if unknown
            break;
    }

    // Adjust maxVelocity based on the motor units
    if (motor.get_encoder_units() == pros::v5::MotorUnits::degrees) {
        maxVelocity *= 360.0 / 60.0; // Convert RPM to degrees per second
    }

    return maxVelocity;
}
