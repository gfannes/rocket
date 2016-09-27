#ifndef HEADER_SingleStage_hpp_ALREADY_INCLUDED
#define HEADER_SingleStage_hpp_ALREADY_INCLUDED

#include "rocket/StagedRocket.hpp"
#include <string>

class SingleStage: public rocket::StagedRocket_crtp<SingleStage>
{
    public:
        //According to http://hyperphysics.phy-astr.gsu.edu/hbase/airfri2.html, a sphere with size 1m (area 3.1415m*m) made out of water (4188kg)
        //should fall at terminal velocity of 201.287m/s, given C == 0.5 and density air == 1.29kg/m/m/m
        const double c_sphere = 0.5;
        const double c_drop = 0.04;
        /* const double c_rocket = 0.1; */
        /* const double c_rocket = 0.75; */
        const double c_rocket = 0.9;

        const double drag_coef = c_rocket;

        SingleStage()
        {
            //This is used for drag computations
            radius = 0.0972;
            payload = 30.0;

            {
                double r = 0.0972;
                double l = 0.48;
                stages.add(rocket::StageType::EndBurner, 1, l, 2*r, true);
            }

            update();
        }

        std::string name() const {return "single-stage rocket";}
    private:
};


#endif
