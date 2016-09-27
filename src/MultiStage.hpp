#ifndef HEADER_MultiStage_hpp_ALREADY_INCLUDED
#define HEADER_MultiStage_hpp_ALREADY_INCLUDED

#include "rocket/StagedRocket.hpp"
#include <string>

class MultiStage: public rocket::StagedRocket_crtp<MultiStage>
{
    public:
        //According to http://hyperphysics.phy-astr.gsu.edu/hbase/airfri2.html, a sphere with size 1m (area 3.1415m*m) made out of water (4188kg)
        //should fall at terminal velocity of 201.287m/s, given C == 0.5 and density air == 1.29kg/m/m/m
        const double c_sphere = 0.5;
        const double c_drop = 0.04;
        const double c_rocket = 0.9;

        const double drag_coef = c_rocket;

        MultiStage()
        {
            //This is used for drag computations
            radius = 1.0;
            payload = 10.0e3;

            {
                double r = 0.0972;
                double l = 0.6;
                stages.add(rocket::StageType::EndBurner, 1000, l, 2*r, true);
                stages.add(rocket::StageType::EndBurner, 800,  l, 2*r, true);
                stages.add(rocket::StageType::EndBurner, 350,  l, 2*r, true);
            }

            update();
        }

        std::string name() const {return "multi-stage rocket";}
    private:
};


#endif
