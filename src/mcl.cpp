#include "mcl.hpp"

// global mcl instance
MCLLocalizer* mcl = nullptr;

MCLLocalizer::MCLLocalizer(pros::Distance& east, pros::Distance& north,
                           pros::Distance& south, pros::Distance& west,
                           float start_x, float start_y, float start_theta)
    : east_sensor(east), north_sensor(north),
      south_sensor(south), west_sensor(west),
      est_x(start_x), est_y(start_y), est_theta(start_theta),
      rng(std::random_device{}()) {

    // TODO: replace each offset_x and offset_y with the actual measured distance
    //       in inches from your robot's center to each sensor's face
    mounts = {
        { 0.0f,  -999.0f,  -M_PI / 2.0f }, // east sensor, facing right  (+90 deg from forward)
        { 999.0f,  0.0f,    0.0f          }, // north sensor, facing forward
        {-999.0f,  0.0f,    M_PI          }, // south sensor, facing backward
        { 0.0f,   999.0f,   M_PI / 2.0f  }, // west sensor, facing left   (-90 deg from forward)
    };

    initParticles(start_x, start_y, start_theta, 4.0f);
}

void MCLLocalizer::initParticles(float x, float y, float theta, float spread) {
    particles.resize(NUM_PARTICLES);

    std::normal_distribution<float> dx(x, spread);
    std::normal_distribution<float> dy(y, spread);
    std::normal_distribution<float> dt(theta, 0.08f);

    for (auto& p : particles) {
        p.x      = dx(rng);
        p.y      = dy(rng);
        p.theta  = dt(rng);
        p.weight = 1.0f / NUM_PARTICLES;
    }
}

float MCLLocalizer::expectedDistance(const Particle& p, const SensorMount& mount) {
    // rotate mount offset into world frame
    float c  = cosf(p.theta), s = sinf(p.theta);
    float sx = p.x + c * mount.offset_x - s * mount.offset_y;
    float sy = p.y + s * mount.offset_x + c * mount.offset_y;

    float sensor_angle = p.theta + mount.facing;
    float dx = cosf(sensor_angle);
    float dy = sinf(sensor_angle);

    float min_dist = 1e9f;

    // check intersection with each wall and keep the closest positive hit
    auto check = [&](float t) { if (t > 0.01f) min_dist = std::min(min_dist, t); };

    if (fabsf(dx) > 1e-6f) {
        check((FIELD_SIZE - sx) / dx); // right wall
        check((0.0f       - sx) / dx); // left wall
    }
    if (fabsf(dy) > 1e-6f) {
        check((FIELD_SIZE - sy) / dy); // top wall
        check((0.0f       - sy) / dy); // bottom wall
    }

    return min_dist;
}

void MCLLocalizer::predict(float delta_x, float delta_y, float delta_theta) {
    // tune noise values to match your drivetrain's real world drift
    std::normal_distribution<float> noise_xy(0.0f, 0.4f);
    std::normal_distribution<float> noise_t(0.0f, 0.015f);

    for (auto& p : particles) {
        float c  = cosf(p.theta), s = sinf(p.theta);
        p.x     += c * delta_x - s * delta_y + noise_xy(rng);
        p.y     += s * delta_x + c * delta_y + noise_xy(rng);
        p.theta += delta_theta + noise_t(rng);
    }
}

void MCLLocalizer::update() {
    // convert mm to inches for each sensor
    float read_east  = east_sensor.get()  / 25.4f;
    float read_north = north_sensor.get() / 25.4f;
    float read_south = south_sensor.get() / 25.4f;
    float read_west  = west_sensor.get()  / 25.4f;

    // readings outside this range are likely invalid or out of sensor range
    bool east_valid  = (read_east  > 1.0f && read_east  < 100.0f);
    bool north_valid = (read_north > 1.0f && read_north < 100.0f);
    bool south_valid = (read_south > 1.0f && read_south < 100.0f);
    bool west_valid  = (read_west  > 1.0f && read_west  < 100.0f);

    constexpr float sigma  = 2.0f;
    constexpr float sigma2 = sigma * sigma;

    struct Reading { float value; bool valid; int mount_index; };
    Reading readings[4] = {
        { read_east,  east_valid,  0 },
        { read_north, north_valid, 1 },
        { read_south, south_valid, 2 },
        { read_west,  west_valid,  3 },
    };

    for (auto& p : particles) {
        // discard particles that have wandered outside the field
        if (p.x < 0 || p.x > FIELD_SIZE || p.y < 0 || p.y > FIELD_SIZE) {
            p.weight = 1e-9f;
            continue;
        }

        float log_w = 0.0f;

        for (auto& r : readings) {
            if (!r.valid) continue;
            float expected = expectedDistance(p, mounts[r.mount_index]);
            float err      = r.value - expected;
            log_w += -0.5f * (err * err) / sigma2;
        }

        p.weight *= expf(log_w);
    }

    normalizeWeights();
}

void MCLLocalizer::normalizeWeights() {
    float total = 0.0f;
    for (auto& p : particles) total += p.weight;

    // if all weights collapsed reinitialize around the last known estimate
    if (total < 1e-9f) {
        initParticles(est_x, est_y, est_theta, 8.0f);
        return;
    }

    for (auto& p : particles) p.weight /= total;
}

void MCLLocalizer::resample() {
    // low variance resampling gives better coverage than naive random resampling
    std::vector<Particle> new_particles(NUM_PARTICLES);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f / NUM_PARTICLES);

    float r          = dist(rng);
    float cumulative = particles[0].weight;
    int i            = 0;

    for (int m = 0; m < NUM_PARTICLES; m++) {
        float U = r + (float)m / NUM_PARTICLES;
        while (U > cumulative && i < NUM_PARTICLES - 1) {
            i++;
            cumulative += particles[i].weight;
        }
        new_particles[m]        = particles[i];
        new_particles[m].weight = 1.0f / NUM_PARTICLES;
    }

    particles = new_particles;
}

void MCLLocalizer::estimatePose() {
    // circular mean for theta avoids wraparound errors near 0/2pi
    float sx = 0, sy = 0, sin_t = 0, cos_t = 0;

    for (auto& p : particles) {
        sx    += p.weight * p.x;
        sy    += p.weight * p.y;
        sin_t += p.weight * sinf(p.theta);
        cos_t += p.weight * cosf(p.theta);
    }

    est_x     = sx;
    est_y     = sy;
    est_theta = atan2f(sin_t, cos_t);
}

void MCLLocalizer::step(float delta_x, float delta_y, float delta_theta) {
    predict(delta_x, delta_y, delta_theta);
    update();
    resample();
    estimatePose();
}

lemlib::Pose MCLLocalizer::getPose() {
    // convert theta back to degrees to match lemlib's convention
    return lemlib::Pose(est_x, est_y, est_theta * 180.0f / M_PI);
}
