#ifndef HEADER_rocket_Earch_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Earch_hpp_ALREADY_INCLUDED

#include "rocket/Constants.hpp"
#include "rocket/V2.hpp"

namespace rocket { 
    struct Earth
    {
        static double mass() {return 5.9722e24;}
        static double radius() {return 6.371e6;}
        static double equator_speed() {return 2.0*Pi*radius()/(24.0*60.0*60.0);}

        static Force gravity(const Position &p)
        {
            const auto r = p.norm();
            //Do not cross the center of the earth...
            return p.invert().multiply(G*mass()/r/r/r);
        }
    };
} 

#endif
