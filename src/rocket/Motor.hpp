#ifndef HEADER_rocket_Motor_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Motor_hpp_ALREADY_INCLUDED

#include "rocket/Constants.hpp"
#include <ostream>
#include <cmath>

namespace rocket { 

    //GALCIT 61-C properties
    namespace galcit { 
        const double burn_rate_at_135_bar = 0.04;//m/s
        const double density = 1780;//kg/m^3
        const double price = 4;//euro/kg
    } 

    //Chamber properties
    namespace chamber { 
        const double material_price = 3;//dollar/kg
        const double material_density_steel_insulated = 2810;//kg/m^3: steel has 7850, this value takes the isolation into account (twice as big as the steel)
        const double material_density_steel = 7850;//kg/m^3
        /* const double pressure = 20000000;//N/m^2	(1 bar = 100 000 N/m^2) */
        const double pressure = 18000000;//N/m^2	(1 bar = 100 000 N/m^2)
        /* const double tensile_strength = 580000000;//N/m^2		(steel or aluminum); */
        const double tensile_strength = 600000000;//N/m^2		(steel or aluminum);
        const double safety_factor = 1.1;

        //Note that octave uses the outside diameter here
        inline double wall_thickness(double in_diameter)
        {
            return (pressure * 0.5*in_diameter * safety_factor) / tensile_strength;
        }
    } 

    const double exhaust_velocity= 1798.32;//m/s	according to DoD at 137.895146 bar */	

    template <typename Receiver>
        class Motor_crtp
        {
            public:
                Motor_crtp(double length, double in_diameter, bool insulated): length_(length), in_diameter_(in_diameter), insulated_(insulated)
            {
                const auto wall_thickness = chamber::wall_thickness(in_diameter_);
                //Note that octave uses the outside diameter here
                out_diameter_ = in_diameter_ + 2.0*wall_thickness;
                if (insulated)
                    out_diameter_ += 4.0*wall_thickness;
            }

                double out_diameter() const {return out_diameter_;}
                double in_diameter() const {return in_diameter_;}

                double thrust() const
                {
                    const double isp = 186;//ISP: at 135 bar @ sea level: 186s (170-190)

                    //Propellant consumption a.k.a. propellant burn rate a.k.a m_dot	892.3394983	kg/s
                    const double motor_propellant_burn_rate = receiver_().burning_surface() * galcit::burn_rate_at_135_bar * galcit::density;
                    const double thrust_per_motor = isp * motor_propellant_burn_rate * Gravity;
                    return thrust_per_motor;
                }

                double burn_time() const
                {
                    return receiver_().burn_depth()/galcit::burn_rate_at_135_bar;
                }

                double empty_mass() const
                {
                    const double motor_cylinder_volume = length_ * (out_diameter_*out_diameter_ - in_diameter_*in_diameter_)*Pi/4.0;
                    const double motor_mass_overhead = 1.1;//To account for the mass of the nozzle, fore side flange - TODO: increase this?
                    return motor_cylinder_volume * (insulated_ ? chamber::material_density_steel_insulated : chamber::material_density_steel) * motor_mass_overhead;
                }
                double propellant_mass() const
                {
                    return receiver_().propellant_volume() * galcit::density;
                }
                double full_mass() const
                {
                    return empty_mass() + propellant_mass();
                }

                double cost() const
                {
                    return empty_mass() * chamber::material_price + propellant_mass() * galcit::price;
                }

            protected:
                Receiver &receiver_() {return static_cast<Receiver&>(*this);}
                const Receiver &receiver_() const {return static_cast<const Receiver&>(*this);}

                bool insulated_ = false;
                double length_ = 0.0;
                double in_diameter_ = 0.0;
                double out_diameter_ = 0.0;
        };

    class SideBurner: public Motor_crtp<SideBurner>
    {
        private:
            const double burning_sides_ = 2.0;

        public:
            using Motor = Motor_crtp<SideBurner>;

            SideBurner(double length, double in_diameter, bool insulated): Motor(length, in_diameter, insulated) {}

            const char *name() const {return "SideBurner";}

            double burning_surface() const
            {
                return burning_sides_ * length_ * propellant_grain_square_side_length_();
            }
            double burn_depth() const
            {
                return propellant_grain_square_side_length_()/burning_sides_;
            }
            double propellant_volume() const
            {
                const double tmp = propellant_grain_square_side_length_();
                return tmp*tmp * length_;
            }

        private:
            double propellant_grain_square_side_length_() const
            {
                return std::sqrt(in_diameter_*in_diameter_/2.0);
            }
    };

    class EndBurner: public Motor_crtp<EndBurner>
    {
        public:
            using Motor = Motor_crtp<EndBurner>;

            EndBurner(double length, double in_diameter, bool insulated): Motor(length, in_diameter, insulated) {}

            const char *name() const {return "EndBurner";}

            double burning_surface() const
            {
                return in_diameter_*in_diameter_*Pi/4.0;
            }
            double burn_depth() const
            {
                return length_;
            }
            double propellant_volume() const
            {
                return burning_surface() * length_;
            }
    };

} 

#endif
