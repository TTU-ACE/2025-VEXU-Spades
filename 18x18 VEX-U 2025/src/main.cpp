//#include <iostream>
#include "main.h"
//#include "robot.cpp"

//#include "robot-config.hpp"
#include "pros/misc.h"
#include "utils.hpp"
#include "robot-config.hpp"
#include "robot.hpp"

ASSET(example_txt) // From static/ folder
ASSET(testPath_txt);


Robot rob;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	rob.chassis.calibrate();
	debugln("Rob-89 Initialized", 1);

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}


void tune_angular() {
	// Tune the angular controller for the chassis
	// set position to x:0, y:0, heading:0
	rob.chassis.setPose(0, 0, 0);
	// turn to face heading 90 with a very long timeout
	rob.chassis.turnToHeading(90, 100000);
	rob.chassis.waitUntilDone();
}

void tune_lateral() {
	// set position to x:0, y:0, heading:0
    rob.chassis.setPose(0, 0, 0);
    // move 48" forwards
    rob.chassis.moveToPoint(0, 48, 10000);
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	debugln("Autonomous", 1);

	rob.chassis.setPose(0, 0, 0);
	//rob.chassis.follow(example_txt, 15, 2000, true); // Replace with path generated from https://path.jerryio.com/
	//rob.chassis.follow(testPath_txt, 15, 20000, true); // Replace with path generated from https://path.jerryio.com/
    // rob.chassis.moveToPoint(20, 20, 5000);

	rob.chassis.moveToPoint(0, 48, 5000);
	rob.chassis.turnToHeading(90, 5000);
	rob.chassis.moveToPoint(30, 48, 5000);
	rob.chassis.turnToHeading(0, 5000);
	rob.chassis.moveToPoint(30, 0, 5000);

    while(rob.chassis.isInMotion()) {
        debugln("Moving chassis to position...");
        pros::delay(500);
    }
	
	debugln("Autonomous Finished.");
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	char msg[100];
	debugln("Opcontrol", 1);

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		
		//rob.setConveyorSpeed(master.get_digital(DIGITAL_R1) ? -conveyorSpeedConstant : 0);
		//rob.setConveyorSpeed(master.get_digital(DIGITAL_R2) ? conveyorSpeedConstant: 0);

		if (master.get_digital(DIGITAL_R1)) 	 {rob.setConveyorSpeed(-conveyorSpeedConstant);}
		else if (master.get_digital(DIGITAL_R2)) {rob.setConveyorSpeed(conveyorSpeedConstant);}
		else 									 {rob.setConveyorSpeed(0);}

		//rob.setIntakeSpeed(master.get_digital(DIGITAL_L1) ? intakeSpeedConstant : 0);
		//rob.setIntakeSpeed(master.get_digital(DIGITAL_L2) ? -intakeSpeedConstant : 0);

		if (master.get_digital(DIGITAL_L1)) 	 {rob.setIntakeSpeed(-intakeSpeedConstant);}
		else if (master.get_digital(DIGITAL_L2)) {rob.setIntakeSpeed(intakeSpeedConstant);}
		else 									 {rob.setIntakeSpeed(0);}
		
		// Set and hold clamp position
		if (master.get_digital(DIGITAL_Y)) 		{rob.setClamp(clampedStatePosition);} //clamp
		else if (master.get_digital(DIGITAL_X)) {rob.setClamp(unclampedStatePosition);} //unclamp
		//if (master.get_digital(DIGITAL_B)) {rob.releaseClamp();} //release for ease of moving

		if (master.get_digital(DIGITAL_UP)) 		{rob.setLift(liftedStatePosition);} //lifted
		else if (master.get_digital(DIGITAL_DOWN))  {rob.setLift(loweredStatePosition);} //lowered

		//check position
		// if(master.get_digital(DIGITAL_A)) {rob.displayClampPosition();}
		// if(master.get_digital(DIGITAL_B)) {rob.returnPositionLift();}
		
		// arcade drive from sticks
        double fwd = master.get_analog(ANALOG_LEFT_Y); 
        double turn = master.get_analog(ANALOG_RIGHT_X);
        rob.arcadeDrive(fwd, turn);

		sprintf(msg, "Fwd (raw): %0.1f, Turn (raw): %0.1f", fwd, turn);
        debugln(msg, 2);  // On line 2

		float hangPos = rob.getHangPosition();
        float clampPos = rob.getClampPosition();
        sprintf(msg, "Clamp (deg): %0.2f, Hang (deg): %0.2f", clampPos, hangPos);
        debugln(msg, 3);  // On line 3

		pros::delay(20); //revert to 20                              
	}
}