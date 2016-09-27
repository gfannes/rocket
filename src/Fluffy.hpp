#ifndef HEADER_Fluffy_hpp_ALREADY_INCLUDED
#define HEADER_Fluffy_hpp_ALREADY_INCLUDED

#include "rocket/StagedRocket.hpp"
#include <string>

class Fluffy: public rocket::StagedRocket_crtp<Fluffy>
{
    public:
        //According to http://hyperphysics.phy-astr.gsu.edu/hbase/airfri2.html, a sphere with size 1m (area 3.1415m*m) made out of water (4188kg)
        //should fall at terminal velocity of 201.287m/s, given C == 0.5 and density air == 1.29kg/m/m/m
        const double c_sphere = 0.5;
        const double c_drop = 0.04;
        const double c_rocket = 0.6;

        const double drag_coef = c_rocket;

        Fluffy()
        {
            //This is used for drag computations
            radius = 0.369/2.0;
            payload = 2.0;

            {
                double r = radius;
                double l = 2.1;
                stages.add(rocket::StageType::EndBurner, 1, l, 2*r, true);
                stages.add(rocket::StageType::EndBurner, 1, 0.1, 0.3, true);
                stages.add(rocket::StageType::EndBurner, 1, 0.1, 0.25, true);
                stages.add(rocket::StageType::EndBurner, 1, 0.1, 0.2, true);
                stages.add(rocket::StageType::EndBurner, 1, 0.1, 0.15, true);
                stages.add(rocket::StageType::EndBurner, 1, 0.1, 0.1, true);
                stages.add(rocket::StageType::EndBurner, 1, 0.1, 0.05, true);
            }

            update();
        }

        std::string name() const {return "fluffy";}
    private:
};


#endif
