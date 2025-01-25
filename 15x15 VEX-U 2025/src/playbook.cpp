// Autonoumos Playbook
#include "api.h"
#include "playbook.h"
#include "robot-config.hpp"
#include "robot.hpp"

// Make sure to call the macro below for all path files
ASSET(example_txt) // From static/ folder
// Blue Auto 1
ASSET(blueGrabStake_txt)
ASSET(blueBackUp_txt)
// Red Auto 1
ASSET(redGrabStake_txt)
ASSET(redBackUp_txt)

extern Robot rob;

void blueAuto1() {
    // Grab stake, intake red ring, and place on stake
    // 1) Drive forward to stake
    rob.chassis.setPose(51.465, 31.203, 90); // First point in file
    rob.raiseClamp();
    rob.chassis.follow(blueGrabStake_txt, 15, 2500, false);
    
    // 2) Grab stake and place preloaded blue stake
    rob.chassis.waitUntilDone();
    rob.lowerClamp(false, 1000);
    pros::delay(250);
    rob.chassis.follow(blueBackUp_txt, 15, 2000, true);
    rob.setHookIntakeSpeed(-HOOK_INTAKE_SPEED_PCT); // Hit the stake with backward motion in case it is in the way to prevent jamming
    rob.setIntakeSpeed(INTAKE_SPEED_PCT); // Start intake
    
    // 3) Reset
    pros::delay(3000);
    rob.stopHookIntake();
    rob.stopIntake();
}

void redAuto1() {
    // Grab stake, intake red ring, and place on stake
    // 1) Drive forward to stake
    rob.chassis.setPose(51.465, 31.203, 270); // First point in file
    rob.raiseClamp();
    rob.chassis.follow(redGrabStake_txt, 15, 3000, false);
    
    // 2) Grab stake
    rob.chassis.waitUntilDone();
    rob.lowerClamp(false, 1000);
    pros::delay(250);
    rob.chassis.follow(redBackUp_txt, 15, 2000, true);
    rob.setHookIntakeSpeed(-HOOK_INTAKE_SPEED_PCT); // Hit the stake with backward motion in case it is in the way to prevent jamming
    rob.setIntakeSpeed(INTAKE_SPEED_PCT); // Start intake

    // 3) Reset
    pros::delay(3000);
    rob.stopHookIntake();
    rob.stopIntake();
}