#include "api.h"
#include "motors.hpp"
#include "def.hpp"

// Motor groups for LemLib (using port numbers)
// Negative ports indicate reversed motors
pros::MotorGroup left_motors({-15, -14, -13});  // Front, Middle, Back
pros::MotorGroup right_motors({16, 17, 18});    // Front, Middle, Back

// Intake and output motors
pros::Motor intakeMotor(9, pros::v5::MotorGears::green);
pros::Motor midMotor(10, pros::v5::MotorGears::blue);
pros::Motor outputRoller(1, pros::v5::MotorGears::blue);

// Pneumatics
pros::adi::DigitalOut pistonA('A'); // match loader
pros::adi::DigitalOut pistonB('B');
bool pistonAState(false);
bool pistonBState(false);


// MCL distance sensors 
pros::Distance dist_east(6);
pros::Distance dist_north(4);
pros::Distance dist_south(5);
pros::Distance dist_west(3);

// Intake functions (combined motor)
void intakeForward(void) {
    intakeMotor.move(127);
    midMotor.move(-127);
}

void intakeStop(void) {
    intakeMotor.move(0);
    midMotor.move(0);
}

void intakeBackward(void) {
    intakeMotor.move(-127);
    midMotor.move(127);
}

// Output functions
void outputForward(void) {
    outputRoller.move(127);
}

void outputStop(void) {
    outputRoller.move(0);
}

void outputBackward(void) {
    outputRoller.move(-127);
}

// Controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// IMU sensor on port 1
pros::Imu imu(1);

// LemLib drivetrain configuration
lemlib::Drivetrain drivetrain(
    &left_motors,
    &right_motors,
    WHEEL_BASE_WIDTH, // Track width in inches
    lemlib::Omniwheel::NEW_325, // Wheel type (3.25" omniwheel)
    360, // RPM of drivetrain (blue cartridge = 600 RPM)
    2 // Horizontal drift (tune this value)
);

// Lateral motion controller for LemLib
lemlib::ControllerSettings lateral_controller(
    24, // Proportional gain (kP)
    0, // Integral gain (kI)
    3, // Derivative gain (kD)
    3, // Anti-windup
    1, // Small error range (inches)
    100, // Small error timeout (ms)
    3, // Large error range (inches)
    500, // Large error timeout (ms)
    20 // Slew rate (maximum change in output per 10ms)
);

// Angular motion controller for LemLib
lemlib::ControllerSettings angular_controller(
    4, // Proportional gain (kP)
    0, // Integral gain (kI)
    10, // Derivative gain (kD)
    3, // Anti-windup
    1, // Small error range (degrees)
    100, // Small error timeout (ms)
    3, // Large error range (degrees)
    500, // Large error timeout (ms)
    0 // Slew rate
);

// Odometry sensors (no tracking wheels in this case, using IMU only)
lemlib::OdomSensors sensors(
    nullptr, // Vertical tracking wheel 1
    nullptr, // Vertical tracking wheel 2
    nullptr, // Horizontal tracking wheel 1
    nullptr, // Horizontal tracking wheel 2
    &imu // IMU
);

// LemLib chassis
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);

// Utility functions
int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void togglePistonA(void) {
    pistonAState = !pistonAState;
    pistonA.set_value(pistonAState);
}

void togglePistonB(void) {
    pistonBState = !pistonBState;
    pistonB.set_value(pistonBState);
}

