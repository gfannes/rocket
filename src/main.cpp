#include "ScaleModel.hpp"
#include "Fluffy.hpp"
#include "SingleStage.hpp"
#include "MultiStage.hpp"
#include "rocket/World.hpp"
#include "gubg/gnuplot/Stream.hpp"
#include "gubg/OnlyOnce.hpp"
#include "gubg/debug.hpp"
#include <algorithm>
#include <iostream>

namespace  { 
    const int time_data = 0;
    const int pos_data = 1;

    template <typename Rocket, typename Gnuplot>
        void process_world(double max_t, double payload, double delta_t, Gnuplot &gs)
        {
            S("main");

            Rocket rocket(payload);
            gs.name(time_data, rocket.name());
            gs.name(pos_data, rocket.name());
            std::cout << C(rocket.name())C(rocket.mass()) << std::endl;
            const auto start_position = rocket.position;

            using World = rocket::World<Rocket>;
            World world(rocket);

            const double target_height = rocket::Leo;

            double max_height = 0.0;
            double max_accel = 0.0;
            double max_speed = 0.0;
            double distance = 0.0;

            gubg::OnlyOnce name_columns;
            for (double t = 0.0; t < max_t; t += delta_t)
            {
                /* S("pos"); */
                S(nullptr);
                L(C(t));

                if (rocket.position.norm() < start_position.norm())
                    break;

                distance = rocket.position.distance(start_position);

                world.process(delta_t);

                const auto height = rocket.position.norm() - start_position.norm();
                const auto height_km = height/1000.0;
                const auto accel = rocket.acceleration();
                const auto speed = rocket.speed();

                max_height = std::max<double>(max_height, height);
                max_accel = std::max<double>(max_accel, accel);
                max_speed = std::max<double>(max_speed, speed);

                const auto pct = 100.0*(height/target_height);
                L(C(pct) << ": " << C(rocket)C(height)C(max_height)C(accel));

                if (name_columns())
                {
                    gs.name(time_data, 0, "time (s)").name(time_data, 1, "accel (m/s^2)").name(time_data, 2, "height (km)").name(time_data, 3, "speed (decam/s)");
                    gs.name(pos_data, 0, "x (m)").name(pos_data, 1, "y (m)");
                }
                gs.data(time_data) << t << accel << height_km << speed/10.0;
                gs.data(pos_data) << rocket.position.y-start_position.y << rocket.position.x-start_position.x;
                L(C(rocket.position));
            }
            std::cout << C(max_height)C(distance)C(max_speed)C(max_accel)C(max_accel/rocket::Gravity) << std::endl;
        }
} 

int main()
{
    S("main");

    const double delta_t = 0.0001;

    gubg::gnuplot::Stream gs;

    /* for (double d = 0.1; d >= 0.0001; d *= 0.9) */
    /* { */
    /*     process_world<ScaleModel>(100.0, 2.0, d, gs); */
    /*     gs.pause(); */
    /* } */

    gs.pause();
    process_world<ScaleModel>(100.0, 2.0, delta_t, gs);

    gs.pause();
    process_world<ScaleModel>(100.0, 0.2, delta_t, gs);

    /* gs.pause(); */
    /* process_world<Fluffy>(100.0, delta_t, gs); */

    /* gs.pause(); */
    /* process_world<SingleStage>(100.0, delta_t, gs); */

    /* gs.pause(); */
    /* process_world<MultiStage>(800.0, delta_t, gs); */

    gs.save("output.gnuplot");

    std::cout << "Everything went OK" << std::endl;
    return 0;
}
