#include "main.h"
#include "utils.hpp"
#include "playbook.h"
#include "robot.hpp"
#include "robot-config.hpp"
#include <cmath>

// Create a global Robot instance
Robot rob;
bool initialized_subsystems;


// Toggle-tracking for the Y button clamp logic
ButtonToggleState yButtonState = ButtonToggleState::NOT_BEGUN;
bool wasYPressedLast = false;

/**
 * Initialize code. Runs once after program starts.
 */
void initialize() {
    pros::lcd::initialize();
    rob.chassis.calibrate();
    //rob.spinTiltToAngle(180);
    // pros::lcd::set_text(1, "Small Bot: Hello from PROS!");
    // std::cout << "initialize here" << std::endl;
    debugln("15-Cubed Initialized", 1);
    initialized_subsystems = false;

    // The Robot constructor automatically sets up motors and starting positions
    // Additional custom init can be done here if needed
}

/**
 * Disabled code (not used here but required by PROS for comp)
 */
void disabled() {
    initialized_subsystems = false;
    // RELEASE SUBSYSTEMS
    rob.clampMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rob.tiltMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rob.lbLeftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rob.lbRightMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

/**
 * Competition-specific init
 */
void competition_initialize() {
    // If you have a pre-auton routine, calibrations, etc.
    initialized_subsystems = false;
}

void tune_angular() {
    // Tune the angular controller for the chassis
    // set position to x:0, y:0, heading:0
    rob.chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    rob.chassis.turnToHeading(-90, 100000);
    rob.chassis.waitUntilDone();
}

void tune_lateral() {
    // Tune the lateral controller for the chassis
    // set position to x:0, y:0, heading:0
    rob.chassis.setPose(0, 0, 0);
    // move to x:0, y:20 with a very long timeout
    rob.chassis.moveToPoint(0, 48, 100000);
    rob.chassis.waitUntilDone();
}

// TODO make async
void tiltInit() {
    rob.spinTiltToAngle(TILT_DOWN_ANGLE);
    pros::delay(500);
    rob.tiltMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rob.tiltMotor.move_voltage(0);
    initialized_subsystems = true;
    pros::delay(500);
    rob.tiltMotor.tare_position();
}

/**
 * Autonomous code
 */
void autonomous() {
    debugln("Autonomous", 1);
    // Put tilt in down position
    if(!initialized_subsystems) {
        tiltInit();
        rob.raiseClamp();
    }

    rob.LBInit();

    // Red Auto 1 =============================================================
    redAuto1();
    //==========================================================================

    // Blue Auto 1 ============================================================
    // blueAuto1();
    //==========================================================================

    debugln("Autonomous Finished.");

}

/**
 * OpControl (teleop) code
 */
void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    // std::cout << "opcontrol here" << std::endl;
    char msg[100];
    debugln("Opcontrol", 1);
    // Put tilt in down position
    if(!initialized_subsystems) {
        tiltInit();
        rob.raiseClamp();
    }

    // Turn off tilt motor
    rob.tiltMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rob.tiltMotor.move_voltage(0);
    rob.LBInit();

    // Track first-time teleop actions if needed
    // bool teleopStarted = false;

    while(true) {
        // if(!teleopStarted) {
        //     // example “move to init position”
        //     rob.LBInit();
        //     teleopStarted = true;
        // }

        // arcade drive from sticks
        double fwd = master.get_analog(ANALOG_LEFT_Y); 
        double turn = master.get_analog(ANALOG_RIGHT_X);
        rob.arcadeDrive(fwd, turn);
        // std::cout << "Fwd: " << fwd << ", Turn: " << turn << std::endl;

        sprintf(msg, "Fwd (raw): %0.1f, Turn (raw): %0.1f", fwd, turn);
        debugln(msg, 2);  // On line 2

        float hangPos = rob.getHangPosition();
        float clampPos = rob.getClampPosition();
        sprintf(msg, "Clamp (deg): %0.2f, Hang (deg): %0.2f", clampPos, hangPos);
        debugln(msg, 3);  // On line 3

        float tiltPos = rob.getTiltPosition();
        sprintf(msg, "Tilt (deg): %0.2f", tiltPos);
        debugln(msg, 4);  // On line 4

        auto pose = rob.chassis.getPose();
        sprintf(msg, "X: %0.1f, Y: %0.1f, H: %0.1f", pose.x, pose.y, pose.theta);  
        debugln(msg, 5);  // On line 5

        // L1 => intake forward, L2 => intake reverse
        if(master.get_digital(DIGITAL_L1))      {rob.setIntakeSpeed(-INTAKE_SPEED_PCT);} 
        else if(master.get_digital(DIGITAL_L2)) {rob.setIntakeSpeed(INTAKE_SPEED_PCT);} 
        else                                    {rob.stopIntake();}

        // R1 => hook intake forward, R2 => hook intake reverse
        if(master.get_digital(DIGITAL_R1))      {rob.setHookIntakeSpeed(HOOK_INTAKE_SPEED_PCT);} 
        else if(master.get_digital(DIGITAL_R2)) {rob.setHookIntakeSpeed(-HOOK_INTAKE_SPEED_PCT);} 
        else                                    {rob.stopHookIntake();}

        // UP => raise hang, DOWN => lower hang
        if(master.get_digital(DIGITAL_UP))        {rob.raiseHang();} 
        else if(master.get_digital(DIGITAL_DOWN)) {rob.lowerHang();} 
        else                                      {rob.stopHang();}


        // Left => Lady Browns down
        // if(master.get_digital(DIGITAL_LEFT)) {rob.LBDown();}
        // if(master.get_digital(DIGITAL_LEFT)) {
        //     rob.clampMotor.move_velocity(-50);
        //     rob.clampMotor.tare_position();
        // }
        // // if(master.get_digital(DIGITAL_RIGHT)) {rob.LBUp();};
        // else if(master.get_digital(DIGITAL_RIGHT)) {
        //     rob.clampMotor.move_velocity(50);
        //     rob.clampMotor.tare_position();
        // }
        // Single-press toggling of clamp with Y
        bool yPressedNow = master.get_digital(DIGITAL_Y);
        if(yPressedNow && !wasYPressedLast) {
            // just changed from not pressed to pressed
            if(yButtonState == ButtonToggleState::NOT_BEGUN) {
                yButtonState = ButtonToggleState::PRESSED;
            }
        }
        else if(!yPressedNow && wasYPressedLast) {
            // just changed from pressed to not pressed
            if(yButtonState == ButtonToggleState::PRESSED) {
                yButtonState = ButtonToggleState::FINISHED;
                // we interpret that as a single press
                rob.clampIt();
            }
        }
        else if(yButtonState == ButtonToggleState::FINISHED) {
            // reset to allow next press
            yButtonState = ButtonToggleState::NOT_BEGUN;
        }
        wasYPressedLast = yPressedNow;

        // A => move Lady Browns to A-button angle
        if(master.get_digital(DIGITAL_A)) {rob.LBToLoadPos();}

        // B => move Lady Browns to init angle
        if(master.get_digital(DIGITAL_B)) {rob.LBInit();}

        // X => toggle detect_blue_mode
        if(master.get_digital_new_press(DIGITAL_X)) {
            if(detect_blue_mode) {
                abort_detection = true;
                detect_blue_mode = false;
            } else {
                detect_blue_mode = true;
                abort_detection  = false;
            }
        }

        // If detection is toggled on, handle it
        rob.handleRingDetection();

        pros::delay(20);
    }
}
