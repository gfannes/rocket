#ifndef HEADER_ScaleModel_hpp_ALREADY_INCLUDED
#define HEADER_ScaleModel_hpp_ALREADY_INCLUDED

#include "rocket/StagedRocket.hpp"
#include <string>

class ScaleModel: public rocket::StagedRocket_crtp<ScaleModel>
{
    public:
        const double thr_duration = 12.0;
        const double thr_mass = 27.9;
        const double thr_one = 3730.0;

        //According to http://hyperphysics.phy-astr.gsu.edu/hbase/airfri2.html, a sphere with size 1m (area 3.1415m*m) made out of water (4188kg)
        //should fall at terminal velocity of 201.287m/s, given C == 0.5 and density air == 1.29kg/m/m/m
        const double c_sphere = 0.5;
        const double c_drop = 0.04;
        /* const double c_rocket = 0.1; */
        /* const double c_rocket = 0.75; */
        const double c_rocket = 0.9;

        const double drag_coef = c_rocket;

        ScaleModel()
        {
            radius = 0.1;
            payload = 30.0;

            stages.emplace_back(1);

            update();
        }

        std::string name() const {return "scale model";}
    private:
};


#endif
