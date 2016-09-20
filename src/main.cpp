#include "rocket/World.hpp"
#include "rocket/Rocket.hpp"
#include "gubg/gnuplot/Stream.hpp"
#include "gubg/debug.hpp"
#include <algorithm>
#include <iostream>

int main()
{
    S("main");

    using Rocket = rocket::Rocket;
    using World = rocket::World<Rocket>;

    Rocket rocket;
    const auto start_position = rocket.position;
    World world(rocket);

    /* const double max_t = 250.0; */
    /* const double delta_t = 0.1; */
    const double max_t = 50.0;
    const double delta_t = 1.0;
    const double target_height = rocket::Leo;

    double max_height = 0.0;
    double max_accel = 0.0;

    gubg::gnuplot::Stream gs;
    const int height_ix = 0;
    gs.name(height_ix, "height (in meters)");

    for (double t = 0.0; t < max_t; t += delta_t)
    {
        S("loop");
        L(C(t));
        world.process(delta_t);
        const auto height = rocket.position.norm() - start_position.norm();
        max_height = std::max<double>(max_height, height);
        max_accel = std::max<double>(max_accel, rocket.acceleration());
        const auto pct = 100.0*(height/target_height);
        L(C(pct) << ": " << C(rocket)C(height)C(max_height));
        gs.data(height_ix) << t << height;
    }
    L(C(max_height)C(max_accel)C(max_accel/rocket::Gravity));

    gs.save("output.gnuplot");

    std::cout << "Everything went OK" << std::endl;
    return 0;
}
