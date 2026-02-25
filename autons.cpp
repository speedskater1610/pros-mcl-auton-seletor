#include "api.h"
#include "autons.hpp"
#include "def.hpp"
#include "motors.hpp"

void red_left_auton() {
    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(90, 2000);
    chassis.waitUntilDone();

    
    /*
    chassis.setPose(0, 0, 0);

    togglePistonB();

    chassis.moveToPoint(0, 5, 10000);
    chassis.waitUntilDone();

    chassis.turnToHeading(-100, 2000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);

    chassis.moveToPoint(0, 28, 10000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(-110, 2000);
    chassis.waitUntilDone();

    intakeForward();
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 15, 10000);
    chassis.waitUntilDone();

    pros::delay(1500);

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -5, 10000);
    chassis.waitUntilDone();

    // chassis.setPose(0, 0, 0);
    // chassis.turnToHeading(-20, 2000);
    // chassis.waitUntilDone();

    togglePistonB();
    togglePistonA();

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -19, 10000, {.forwards = false}, true);
    chassis.waitUntilDone();

    outputForward();

    pros::delay(4000);
    intakeStop();
    outputStop();
    */
}

void red_right_auton() {
    chassis.setPose(0, 0, 0);

    togglePistonB();

    chassis.moveToPoint(0, 5, 10000);
    chassis.waitUntilDone();

    chassis.turnToHeading(100, 2000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);

    chassis.moveToPoint(0, 25, 10000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(110, 2000);
    chassis.waitUntilDone();

    intakeForward();
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 15, 10000);
    chassis.waitUntilDone();

    pros::delay(1500);

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -5, 10000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(20, 2000);
    chassis.waitUntilDone();

    togglePistonB();
    togglePistonA();

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -18, 10000, {.forwards = false}, true);

    outputForward();

    pros::delay(2000);
    intakeStop();
    outputStop();
}

void blue_left_auton() {
    chassis.setPose(0, 0, 0);

    togglePistonB();

    chassis.moveToPoint(0, 5, 10000);
    chassis.waitUntilDone();

    chassis.turnToHeading(-100, 2000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);

    chassis.moveToPoint(0, 28, 10000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(-110, 2000);
    chassis.waitUntilDone();

    intakeForward();
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 15, 10000);
    chassis.waitUntilDone();

    pros::delay(1500);

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -5, 10000);
    chassis.waitUntilDone();

    // chassis.setPose(0, 0, 0);
    // chassis.turnToHeading(-20, 2000);
    // chassis.waitUntilDone();

    togglePistonB();
    togglePistonA();

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -19, 10000, {.forwards = false}, true);
    chassis.waitUntilDone();

    outputForward();

    pros::delay(4000);
    intakeStop();
    outputStop();
}

void blue_right_auton() {
    chassis.setPose(0, 0, 0);

    togglePistonB();

    chassis.moveToPoint(0, 5, 10000);
    chassis.waitUntilDone();

    chassis.turnToHeading(100, 2000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);

    chassis.moveToPoint(0, 25, 10000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(110, 2000);
    chassis.waitUntilDone();

    intakeForward();
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 15, 10000);
    chassis.waitUntilDone();

    pros::delay(1500);

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -5, 10000);
    chassis.waitUntilDone();

    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(20, 2000);
    chassis.waitUntilDone();

    togglePistonB();
    togglePistonA();

    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, -18, 10000, {.forwards = false}, true);

    outputForward();

    pros::delay(2000);
    intakeStop();
    outputStop();

}
