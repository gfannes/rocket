#ifndef HEADER_rocket_World_hpp_ALREADY_INCLUDED
#define HEADER_rocket_World_hpp_ALREADY_INCLUDED

#include "rocket/Earth.hpp"
#include "rocket/Atmosphere.hpp"
#include "gubg/debug.hpp"

namespace rocket { 
    template <typename Rocket>
        class World
        {
            public:
                World(Rocket &r): rocket_(r) {}

                void process(double dt)
                {
                    S("world");
                    const double height = rocket_.position.norm()-Earth::radius();
                    const double mass = rocket_.mass();
                    L(C(height)C(mass));
                    const auto drag = rocket_.drag(Atmosphere::density(height));
                    const auto thrust = rocket_.thrust();
                    const auto gravity = Earth::gravity(rocket_.position).multiply(mass);
                    V2 force;
                    force += drag;
                    force += thrust;
                    force += gravity;
                    L(C(drag)C(thrust)C(gravity) <<  " => " << C(force));

                    rocket_.process(force, dt);
                }

            private:
                Rocket &rocket_;
        };
} 

#endif
