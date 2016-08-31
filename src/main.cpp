#include "rocket/World.hpp"
#include "rocket/Rocket.hpp"
#include "gubg/debug.hpp"
#include <iostream>

int main()
{
    S("main");

    using Rocket = rocket::Rocket;
    using World = rocket::World<Rocket>;

    Rocket rocket;
    World world(rocket);

    const double max_t = 0.5;
    const double delta_t = 0.01;

    for (double t = 0.0; t < max_t; t += delta_t)
    {
        S("loop");
        L(C(t));
        world.process(delta_t);
        L(C(rocket));
    }

    std::cout << "Everything went OK" << std::endl;
    return 0;
}
