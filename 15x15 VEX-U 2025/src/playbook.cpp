// Autonoumos Playbook
#include "api.h"
#include "playbook.h"
#include "robot-config.hpp"
#include "robot.hpp"

// Make sure to call the macro below for all path files
ASSET(example_txt) // From static/ folder
ASSET(redGrabStake_txt)
ASSET(redDriveToRingFromStake_txt)

extern Robot rob;

void redAuto1() {
    // Grab stake, intake red ring, and place on stake
    // 1) Drive forward to stake
    rob.chassis.setPose(62, 39, 89.989); // First point in file
    rob.raiseClamp();
    rob.chassis.follow(redGrabStake_txt, 15, 5000, false); // Replace with path generated from https://path.jerryio.com/
    // 2) Grab stake
    rob.chassis.waitUntilDone();
    rob.chassis.tank(50, 50);
    pros::delay(250);
    rob.lowerClamp(false, 2000);
    pros::delay(3000);
    //rob.chassis.moveToPose(24, 25, 120, 4000); // Move forward while clamping
    // 3) Drive to red ring
    rob.chassis.turnToHeading(345, 4000, {.maxSpeed = 50});
    rob.chassis.waitUntilDone();
    rob.chassis.follow(redDriveToRingFromStake_txt, 15, 5000, true); // Replace with path generated from https://path.jerryio.com/
    // 4) Intake red ring
    pros::delay(250);
    rob.setIntakeSpeed(INTAKE_SPEED_PCT);
    // 5) Place on stake
    pros::delay(250);
    rob.setHookIntakeSpeed(-HOOK_INTAKE_SPEED_PCT);
    // 6) Reset
    pros::delay(5000);
    rob.stopHookIntake();
    rob.stopIntake();
}