#ifndef HEADER_ScaleModel_hpp_ALREADY_INCLUDED
#define HEADER_ScaleModel_hpp_ALREADY_INCLUDED

#include "rocket/StagedRocket.hpp"
#include <sstream>
#include <string>

class ScaleModel: public rocket::StagedRocket_crtp<ScaleModel>
{
    public:
        //According to http://hyperphysics.phy-astr.gsu.edu/hbase/airfri2.html, a sphere with size 1m (area 3.1415m*m) made out of water (4188kg)
        //should fall at terminal velocity of 201.287m/s, given C == 0.5 and density air == 1.29kg/m/m/m
        const double c_sphere = 0.5;
        const double c_drop = 0.04;
        const double c_rocket = 0.6;

        const double drag_coef = c_rocket;

        ScaleModel(double pl)
        {
            const double d = 0.0338;
            const double l = 0.15;
            stages.add(rocket::StageType::EndBurner, 1, l, d, false);

            radius = stages.out_diameter()/2.0;
            payload = pl;

            update();
        }

        std::string name() const
        {
            std::ostringstream oss;
            oss << "scale model " << payload << "kg";
            return oss.str();
        }
    private:
};


#endif
