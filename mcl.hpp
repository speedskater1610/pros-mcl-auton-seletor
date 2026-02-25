#pragma once
#include "lemlib/api.hpp"
#include "pros/distance.hpp"
#include <vector>
#include <random>
#include <cmath>

constexpr float FIELD_SIZE = 144.0f; // inches, vex high stakes field is 12ft x 12ft

struct Particle {
    float x, y, theta;
    float weight;
};

struct SensorMount {
    float offset_x, offset_y; // offset from robot center in inches
    float facing;             // angle relative to robot front in radians
};

class MCLLocalizer {
public:
    static constexpr int NUM_PARTICLES = 300;

    pros::Distance& east_sensor;
    pros::Distance& north_sensor;
    pros::Distance& south_sensor;
    pros::Distance& west_sensor;

    std::vector<Particle> particles;
    std::mt19937 rng;

    float est_x, est_y, est_theta;

    std::vector<SensorMount> mounts;

    MCLLocalizer(pros::Distance& east, pros::Distance& north,
                 pros::Distance& south, pros::Distance& west,
                 float start_x, float start_y, float start_theta);

    void initParticles(float x, float y, float theta, float spread);
    float expectedDistance(const Particle& p, const SensorMount& mount);
    void predict(float delta_x, float delta_y, float delta_theta);
    void update();
    void normalizeWeights();
    void resample();
    void estimatePose();
    void step(float delta_x, float delta_y, float delta_theta);

    lemlib::Pose getPose();
};

// global MCL instance, initialized in initialize()
extern MCLLocalizer* mcl;
