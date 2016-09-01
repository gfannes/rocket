#ifndef HEADER_rocket_Atmosphere_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Atmosphere_hpp_ALREADY_INCLUDED

#include "rocket/Constants.hpp"
#include <cmath>

namespace rocket { 
    class Atmosphere
    {
        public:
            static double density(double height)
            {
                static const double sea_level = 0.0;
                static const double leo_level = Leo;
                static const double density_sea = 1.29;
                static const double density_leo = 1.0e-9;
                static const double b_ = std::log(density_sea/density_leo)/(sea_level-leo_level);
                static const double a_ = density_sea/std::exp(b_*sea_level);

                if (height < sea_level)
                    return density_sea;
                if (height > leo_level)
                    return density_leo;
                return a_*std::exp(b_*height);
            }
        private:
    };
} 

#endif
