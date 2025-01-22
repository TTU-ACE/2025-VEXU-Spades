//#include <iostream>
#include "main.h"
//#include "robot.cpp"

//#include "robot-config.hpp"
#include "pros/misc.h"
#include "robot-config.hpp"
#include "robot.hpp"


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
	std::cout<<"init_check_here"<<std::endl;
	pros::lcd::set_text(1, "Hello PROS User!");

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
	//rob.chassis.follow("example_txt", 15, 4000, false); // Replace with path generated from https://path.jerryio.com/
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
	std::cout <<"opcontrol_init_check_here"<< std::endl;

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
		if(master.get_digital(DIGITAL_A)) {rob.displayClampPosition();}
		if(master.get_digital(DIGITAL_B)) {rob.returnPositionLift();}
		
		rob.arcadeDrive(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));

		pros::delay(20); //revert to 20                              
	}
}