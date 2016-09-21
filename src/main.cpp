#include "rocket/World.hpp"
#include "rocket/Rocket.hpp"
#include "gubg/gnuplot/Stream.hpp"
#include "gubg/OnlyOnce.hpp"
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
    const int time_data = 0;
    gs.name(time_data, "time data");
    gubg::OnlyOnce name_columns;

    for (double t = 0.0; t < max_t; t += delta_t)
    {
        S("loop");
        L(C(t));

        world.process(delta_t);

        const auto height = rocket.position.norm() - start_position.norm();
        const auto height_km = height/1000.0;
        const auto accel = rocket.acceleration();
        const auto speed = rocket.speed();

        max_height = std::max<double>(max_height, height);
        max_accel = std::max<double>(max_accel, accel);

        const auto pct = 100.0*(height/target_height);
        L(C(pct) << ": " << C(rocket)C(height)C(max_height)C(accel));

        if (name_columns())
            gs.name(time_data, 0, "time (s)").name(time_data, 1, "accel (m/s^2)").name(time_data, 2, "height (km)").name(time_data, 3, "speed (m/s)");
        gs.data(time_data) << t << accel << height_km << speed;
    }
    L(C(max_height)C(max_accel)C(max_accel/rocket::Gravity));

    gs.save("output.gnuplot");

    std::cout << "Everything went OK" << std::endl;
    return 0;
}
