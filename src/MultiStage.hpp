#ifndef HEADER_MultiStage_hpp_ALREADY_INCLUDED
#define HEADER_MultiStage_hpp_ALREADY_INCLUDED

#include "rocket/StagedRocket.hpp"
#include <string>

class MultiStage: public rocket::StagedRocket_crtp<MultiStage>
{
    public:
        const double thr_duration = 15.0;
        const double thr_mass = 35.0;
        const double thr_one = 3730.0;

        //According to http://hyperphysics.phy-astr.gsu.edu/hbase/airfri2.html, a sphere with size 1m (area 3.1415m*m) made out of water (4188kg)
        //should fall at terminal velocity of 201.287m/s, given C == 0.5 and density air == 1.29kg/m/m/m
        const double c_sphere = 0.5;
        const double c_drop = 0.04;
        const double c_rocket = 0.75;

        const double drag_coef = c_rocket;


        MultiStage()
        {
            radius = 1.0;
            payload = 10.0e3;

            //Reaches 164km with max_g of 7
            stages = {Stage(1000), Stage(800), Stage(350)};

            update();
        }

        std::string name() const {return "multi-stage rocket";}
    private:
};


#endif
