//#include <iostream>
#include "main.h"
//#include "robot.cpp"

#include "robot.hpp"
#include "robot-config.hpp"

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
	double speed = 50;
	std::cout <<"opcontrol_init_check_here"<< std::endl;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		
		//Buttonmapping
		/*bool a = master.get_digital(DIGITAL_A);
		bool b = master.get_digital(DIGITAL_B);
		bool x = master.get_digital(DIGITAL_X);
		bool y = master.get_digital(DIGITAL_Y);

		bool r1 = master.get_digital(DIGITAL_R1);
		bool r2 = master.get_digital(DIGITAL_R2);
		bool l1 = master.get_digital(DIGITAL_L1);
		bool l2 = master.get_digital(DIGITAL_L2);

		bool up = master.get_digital(DIGITAL_UP);
		bool down = master.get_digital(DIGITAL_DOWN);*/
		
		if (master.get_digital(DIGITAL_UP)) {rob.raiseLift(speed);}
		else if(master.get_digital(DIGITAL_DOWN)) {rob.lowerLift(speed);}
		else {rob.stopLift();}

		rob.setConveyorSpeed(master.get_digital(DIGITAL_R2) ? -speed : 0);
		rob.setIntakeSpeed(master.get_digital(DIGITAL_R1) ? speed : 0);

		// Set and hold clamp position
		if (master.get_digital(DIGITAL_Y)) 		{rob.setClamp(clampedPosition);}
		else if (master.get_digital(DIGITAL_X)) {rob.setClamp(unclampedPosition);}

		//check position
		//std::cout << rob.returnPositionClamp() << std::endl;
		//std::cout << rob.returnPositionLift() << std::endl;
		if(master.get_digital(DIGITAL_A)) {rob.displayClampPosition();}
		
		rob.arcadeDrive(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));

		//left_mg.move(dir - turn);                      // Sets left motor voltage
		//right_mg.move(dir + turn);                     // Sets right motor voltage

		pros::delay(100); //revert to 20                              
	}
}