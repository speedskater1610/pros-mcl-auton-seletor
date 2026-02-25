#pragma once
#include "api.h"
#include "lemlib/api.hpp"

// motor definitions
extern pros::Motor frontMotorRight;
extern pros::Motor backMotorRight;
extern pros::Motor middleMotorRight;
extern pros::Motor backMotorLeft;
extern pros::Motor frontMotorLeft;
extern pros::Motor middleMotorLeft;

// motor groups for LemLib
extern pros::MotorGroup left_motors;
extern pros::MotorGroup right_motors;

// intake and output motors
extern pros::Motor intakeMotor;
extern pros::Motor midMotor;
extern pros::Motor outputRoller;

// pneumatics
extern pros::adi::DigitalOut pistonA;
extern pros::adi::DigitalOut pistonB;
extern bool pistonAState;
extern bool pistonBState;

// controller
extern pros::Controller controller;

// LemLib drivetrain configuration
extern lemlib::Drivetrain drivetrain;

// LemLib chassis
extern lemlib::Chassis chassis;

// imu for odometry
extern pros::Imu imu;

// distance sensors (ports: east=6, north=4, south=5, west=3)
extern pros::Distance dist_east;
extern pros::Distance dist_north;
extern pros::Distance dist_south;
extern pros::Distance dist_west;

// robot dimensions (adjust these to match your robot)
constexpr double WHEEL_DIAMETER    = 3.25; // inches
constexpr double WHEEL_BASE_WIDTH  = 13.5; // inches, distance between left and right wheels
constexpr double GEAR_RATIO        = 0.6;  // external gear ratio, adjust if needed (motors spin a 36t and the wheels are on a 60t)
constexpr double HORIZONTAL_DRIFT  = 2.0;  // drift constant for odometry, tune this
