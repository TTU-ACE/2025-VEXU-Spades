#include "main.h"
#include "utils.hpp"
#include "robot.hpp"
#include "robot-config.hpp"
#include <cmath>

ASSET(example_txt) // From static/ folder

// Create a global Robot instance
Robot rob;

// Toggle-tracking for the Y button clamp logic
ButtonToggleState yButtonState = ButtonToggleState::NOT_BEGUN;
bool wasYPressedLast = false;

/**
 * Initialize code. Runs once after program starts.
 */
void initialize() {
    pros::lcd::initialize();
    rob.chassis.calibrate();
    // pros::lcd::set_text(1, "Small Bot: Hello from PROS!");
    // std::cout << "initialize here" << std::endl;
    debugln("Small Bot Initialized", 1);

    // The Robot constructor automatically sets up motors and starting positions
    // Additional custom init can be done here if needed
}

/**
 * Disabled code (not used here but required by PROS for comp)
 */
void disabled() {}

/**
 * Competition-specific init
 */
void competition_initialize() {
    // If you have a pre-auton routine, calibrations, etc.
}

/**
 * Autonomous code
 */
void autonomous() {
    debugln("Autonomous", 1);
    // // Example: clamp down and move forward
    // rob.clampIt();   
    // rob.arcadeDrive(100, 0); // forward
    // pros::delay(1000);
    // rob.arcadeDrive(0, 0);   // stop
    // pros::delay(500);
    //rob.chassis.follow(example_txt, 15, 10000, false); // Replace with path generated from https://path.jerryio.com/

    // rob.chassis.moveToPoint(0, 20, 5000);

    // while(rob.chassis.isInMotion()) {
    //     debugln("Moving chassis to position...");
    //     pros::delay(500);
    // }


    // Tune the angular controller for the chassis
    // set position to x:0, y:0, heading:0
    rob.chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    rob.chassis.turnToHeading(90, 100000);
    rob.chassis.waitUntilDone();


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
        double turn = -master.get_analog(ANALOG_RIGHT_X);
        rob.arcadeDrive(fwd, turn);
        // std::cout << "Fwd: " << fwd << ", Turn: " << turn << std::endl;

        sprintf(msg, "Fwd (raw): %0.1f, Turn (raw): %0.1f", fwd, turn);
        debugln(msg, 2);  // On line 2

        float hangPos = rob.getHangPosition();
        float clampPos = rob.getClampPosition();
        sprintf(msg, "Clamp (deg): %0.2f, Hang (deg): %0.2f", clampPos, hangPos);
        debugln(msg, 3);  // On line 3

        // L1 => intake forward, L2 => intake reverse
        if(master.get_digital(DIGITAL_L1))      {rob.setIntakeSpeed(INTAKE_SPEED_PCT);} 
        else if(master.get_digital(DIGITAL_L2)) {rob.setIntakeSpeed(-INTAKE_SPEED_PCT);} 
        else                                    {rob.stopIntake();}

        // R1 => hook intake forward, R2 => hook intake reverse
        if(master.get_digital(DIGITAL_R1))      {rob.setHookIntakeSpeed(HOOK_INTAKE_SPEED_PCT);} 
        else if(master.get_digital(DIGITAL_R2)) {rob.setHookIntakeSpeed(-HOOK_INTAKE_SPEED_PCT);} 
        else                                    {rob.stopHookIntake();}

        // UP => raise hang, DOWN => lower hang
        if(master.get_digital(DIGITAL_UP))        {rob.raiseHang();} 
        else if(master.get_digital(DIGITAL_DOWN)) {rob.lowerHang();} 
        else                                      {rob.stopHang();}

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

        // Left => Lady Browns down
        if(master.get_digital(DIGITAL_LEFT)) {rob.LBDown();}

        if(master.get_digital(DIGITAL_RIGHT)) {rob.LBUp();};

        // A => move Lady Browns to A-button angle
        if(master.get_digital(DIGITAL_A)) {rob.LBToAButton();}

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
