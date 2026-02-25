#include "skills.hpp"
#include "api.h"
#include "autons.hpp"
#include "def.hpp"
#include "motors.hpp"

void skills() {
    chassis.setPose(0, 0, 0);

    togglePistonB();

    chassis.moveToPoint(0, 5, 10000);
    chassis.waitUntilDone();

    chassis.turnToHeading(-100, 2000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);

    chassis.moveToPoint(0, 26, 10000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(-115, 2000);
    chassis.waitUntilDone();

    intakeForward();
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 15, 10000);
    chassis.waitUntilDone();

    pros::delay(1500);

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -6, 10000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(-20, 2000);
    chassis.waitUntilDone();

    togglePistonA();

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -19, 10000, {.forwards = false}, true);
    chassis.waitUntilDone();

    outputForward();

    pros::delay(4000);
    intakeStop();
    outputStop();

    // after main auton 
    // move forwards away from the goal.
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 10, 10000, {.forwards = true}, true);
    chassis.waitUntilDone();

    // turn to the wall
    chassis.setPose(0 ,0, 0);
    chassis.turnToHeading(-115, 2000);
    chassis.waitUntilDone();

    // move slightly towards the wall
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 20, 10000, {.forwards = false}, true);
    chassis.waitUntilDone();

    // turn back to the fcaing perpedickular from the wall
    chassis.setPose(0 ,0, 0);
    chassis.turnToHeading(-115, 2000);
    chassis.waitUntilDone();

    // move back forwards
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 1000, 10000, {.forwards = false}, true);
    chassis.waitUntilDone();

    // MOVE BACKWARDS AND REALIGN TO THE goal
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 5, 10000, {.forwards = false}, true);
    chassis.waitUntilDone();

    chassis.setPose(0 ,0, 0);
    chassis.turnToHeading(-115, 2000);
    chassis.waitUntilDone();
}
