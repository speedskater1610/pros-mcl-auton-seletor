#include "main.h"
#include "skills.hpp"
#include "autons.hpp"
#include "def.hpp"
#include "motors.hpp"
#include "mcl.hpp"

// enums for auton selection
enum class Alliance {
    RED,
    BLUE,
    SKILLS,
    NONE
};
enum class Side {
    LEFT,
    RIGHT,
    NONE
};

// global vars for auton select
Alliance selected_alliance = Alliance::NONE;
Side selected_side         = Side::NONE;

// screen dimensions
#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 240

// button dimensions and positions
#define BUTTON_HEIGHT    80
#define BUTTON_WIDTH     150
#define SIDE_BUTTON_SIZE 100

// alliance select stage flag
bool alliance_selected = false;

// forward declaration
void auton_selector_task(void);

void draw_alliance_selection(void) {
    pros::screen::erase();

    // red button on the left
    pros::screen::set_pen(pros::c::COLOR_RED);
    pros::screen::fill_rect(20, 80, 20 + BUTTON_WIDTH, 80 + BUTTON_HEIGHT);
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, 95, 110, "RED");

    // blue button in the middle
    pros::screen::set_pen(pros::c::COLOR_BLUE);
    pros::screen::fill_rect(190, 80, 190 + BUTTON_WIDTH, 80 + BUTTON_HEIGHT);
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, 265, 110, "BLUE");

    // skills button on the right
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::fill_rect(360, 80, 360 + BUTTON_WIDTH, 80 + BUTTON_HEIGHT);
    pros::screen::set_pen(pros::c::COLOR_BLACK);
    pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, 435, 110, "SKILLS");

    // title
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::print(pros::text_format_e_t::E_TEXT_LARGE_CENTER, SCREEN_WIDTH / 2, 20, "Select Alliance");
}

void draw_side_selection(void) {
    pros::screen::erase();

    // yellow button for left side
    pros::screen::set_pen(pros::c::COLOR_YELLOW);
    pros::screen::fill_rect(90, 70, 90 + SIDE_BUTTON_SIZE, 70 + SIDE_BUTTON_SIZE);
    pros::screen::set_pen(pros::c::COLOR_BLACK);
    pros::screen::print(pros::text_format_e_t::E_TEXT_LARGE_CENTER, 140, 110, "L");

    // purple button for right side
    pros::screen::set_pen(pros::c::COLOR_PURPLE);
    pros::screen::fill_rect(290, 70, 290 + SIDE_BUTTON_SIZE, 70 + SIDE_BUTTON_SIZE);
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::print(pros::text_format_e_t::E_TEXT_LARGE_CENTER, 340, 110, "R");

    // title
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::print(pros::text_format_e_t::E_TEXT_LARGE_CENTER, SCREEN_WIDTH / 2, 20, "Select Side");
}

void display_selection(void) {
    pros::screen::erase();
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::print(pros::text_format_e_t::E_TEXT_LARGE_CENTER, SCREEN_WIDTH / 2, 80, "Auton Selected:");

    std::string alliance_str;
    if      (selected_alliance == Alliance::RED)    alliance_str = "RED";
    else if (selected_alliance == Alliance::BLUE)   alliance_str = "BLUE";
    else if (selected_alliance == Alliance::SKILLS) alliance_str = "SKILLS";

    std::string side_str = "";
    if      (selected_side == Side::LEFT)  side_str = " - LEFT";
    else if (selected_side == Side::RIGHT) side_str = " - RIGHT";

    pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 120,
                        (alliance_str + side_str).c_str());
}

void wait_for_release(void) {
    while (true) {
        pros::screen_touch_status_s_t status = pros::screen::touch_status();
        if (status.touch_status == pros::E_TOUCH_RELEASED) break;
        pros::delay(20);
    }
    pros::delay(100);
}

void auton_selector_task(void) {
    draw_alliance_selection();
    wait_for_release();

    while (true) {
        pros::screen_touch_status_s_t status = pros::screen::touch_status();

        if (status.touch_status == pros::E_TOUCH_PRESSED) {
            int x = status.x;
            int y = status.y;

            if (!alliance_selected) {
                if (y >= 80 && y <= 160) {
                    if (x >= 20 && x <= 170) {
                        // red selected
                        selected_alliance = Alliance::RED;
                        alliance_selected = true;
                        wait_for_release();
                        draw_side_selection();
                    } else if (x >= 190 && x <= 340) {
                        // blue selected
                        selected_alliance = Alliance::BLUE;
                        alliance_selected = true;
                        wait_for_release();
                        draw_side_selection();
                    } else if (x >= 360 && x <= 510) {
                        // skills selected, no side needed
                        selected_alliance = Alliance::SKILLS;
                        selected_side     = Side::NONE;
                        alliance_selected = true;
                        wait_for_release();
                        display_selection();
                        break;
                    }
                }
            } else {
                if (y >= 70 && y <= 170) {
                    if (x >= 90 && x <= 190) {
                        // left side selected
                        selected_side = Side::LEFT;
                        wait_for_release();
                        display_selection();
                        break;
                    } else if (x >= 290 && x <= 390) {
                        // right side selected
                        selected_side = Side::RIGHT;
                        wait_for_release();
                        display_selection();
                        break;
                    }
                }
            }
        }
        pros::delay(20);
    }
}

void on_center_button(void) {
    selected_alliance = Alliance::NONE;
    selected_side     = Side::NONE;
    alliance_selected = false;
    auton_selector_task();
}

void initialize(void) {
    pros::lcd::initialize();
    pros::lcd::register_btn1_cb(on_center_button);
    togglePistonB();

    // calibrate the imu
    pros::screen::erase();
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 100, "Calibrating IMU...");
    imu.reset();
    while (imu.is_calibrating()) pros::delay(10);

    pros::screen::erase();
    pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 100, "IMU Calibrated!");
    pros::delay(500);

    // calibrate LemLib chassis
    chassis.calibrate();
    chassis.setPose(0, 0, 0);
    togglePistonB();

    // initialize mcl with all four distance sensors
    // starting pose assumes robot begins at field center facing forward
    // adjust start_x, start_y, start_theta to match your actual starting position
    mcl = new MCLLocalizer(
        dist_east, dist_north, dist_south, dist_west,
        72.0f, 72.0f, 0.0f // start_x, start_y, start_theta in inches/radians
    );

    // run mcl update loop in a background task at 20hz
    pros::Task mcl_task([] {
        lemlib::Pose last = chassis.getPose();

        while (true) {
            lemlib::Pose curr = chassis.getPose();

            // compute world frame delta from lemlib odometry
            float dtheta   = (curr.theta - last.theta) * M_PI / 180.0f;
            float dx_world = curr.x - last.x;
            float dy_world = curr.y - last.y;

            // rotate into robot local frame for the predict step
            float c        = cosf(-last.theta * M_PI / 180.0f);
            float s        = sinf(-last.theta * M_PI / 180.0f);
            float dx_local = c * dx_world - s * dy_world;
            float dy_local = s * dx_world + c * dy_world;

            mcl->step(dx_local, dy_local, dtheta);

            // uncomment to correct lemlib odometry with mcl estimate
            // chassis.setPose(mcl->getPose());

            last = curr;
            pros::delay(50);
        }
    });
    
    // run the auton selector
CheckScreen:
    if (pros::c::lcd_is_initialized()) {
        auton_selector_task();
    } else goto CheckScreen;
}

void autonomous(void) {
    pros::screen::erase();
    pros::screen::set_pen(pros::c::COLOR_WHITE);
    pros::screen::print(pros::text_format_e_t::E_TEXT_LARGE_CENTER, SCREEN_WIDTH / 2, 80, "Starting Auton");
    pros::delay(500);

    if (selected_alliance == Alliance::RED) {
        if (selected_side == Side::LEFT) {
            pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 120, "Running Red Left");
            red_left_auton();
        } else if (selected_side == Side::RIGHT) {
            pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 120, "Running Red Right");
            red_right_auton();
        }
    } else if (selected_alliance == Alliance::BLUE) {
        if (selected_side == Side::LEFT) {
            pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 120, "Running Blue Left");
            blue_left_auton();
        } else if (selected_side == Side::RIGHT) {
            pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 120, "Running Blue Right");
            blue_right_auton();
        }
    } else if (selected_alliance == Alliance::SKILLS) {
        pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 120, "Running Skills");
        skills();
    } else {
        pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 120, "Running Skills (default)");
        skills();
    }

    pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, SCREEN_WIDTH / 2, 160, "Auton Complete");
}

void disabled(void) {}
void competition_initialize(void) {}

void opcontrol(void) {}
