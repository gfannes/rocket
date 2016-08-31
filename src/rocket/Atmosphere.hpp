#ifndef HEADER_rocket_Atmosphere_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Atmosphere_hpp_ALREADY_INCLUDED

#include <cmath>

namespace rocket { 
    class Atmosphere
    {
        public:
            double density(double altitude) const
            {
                if (altitude < sea_)
                    return density_sea_;
                if (altitude > leo_)
                    return density_leo_;
                return a_*std::exp(b_*altitude);
            }
        private:
            const double sea_ = 0.0;
            const double leo_ = 160000.0;
            const double density_sea_ = 1.0;
            const double density_leo_ = 1.0e-9;
            const double b_ = std::log(density_sea_/density_leo_)/(sea_-leo_);
            const double a_ = density_sea_/std::exp(b_*sea_);
    };
} 

#endif
