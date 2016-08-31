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
                    L(C(height));
                    V2 force;
                    /* force += rocket_.thrust(); */
                    /* L(C(force)); */
                    /* force += rocket_.drag(); */
                    /* L(C(force)); */
                    force += Earth::gravity(rocket_.position).multiply(rocket_.mass());
                    L(C(force));

                    rocket_.process(force, dt);
                }

            private:
                Rocket &rocket_;
        };
} 

#endif
