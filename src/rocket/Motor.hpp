#ifndef HEADER_rocket_Motor_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Motor_hpp_ALREADY_INCLUDED

#include "rocket/Constants.hpp"

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
        const double material_density = 2810;//kg/m^3: steel has 7850, this value takes the isolation into account (twice as big as the steel)
    } 

    const double exhaust_velocity= 1798.32;//m/s	according to DoD at 137.895146 bar */	

    template <typename Receiver>
        class Motor_crtp
        {
            public:
                Motor_crtp(double length, double in_diameter): length_(length), in_diameter_(in_diameter)
            {
                const auto wall_thickness = wall_thickness_(in_diameter_);
                //Note that octave uses the outside diameter here
                out_diameter_ = in_diameter_ + 2.0*wall_thickness + 4.0*wall_thickness;
            }

                double thrust() const
                {
                    const double isp = 180;//ISP: at 135 bar @ sea level: 186s (170-190)

                    //Propellant consumption a.k.a. propellant burn rate a.k.a m_dot	892.3394983	kg/s
                    const double motor_propellant_burn_rate = receiver_().burning_surface() * galcit::burn_rate_at_135_bar * galcit::density;
                    const double thrust_per_motor = isp * motor_propellant_burn_rate * Gravity;
                    return thrust_per_motor;
                }

                double empty_mass() const
                {
                    const double motor_cylinder_volume = length_ * (out_diameter_*out_diameter_ - in_diameter_*in_diameter_)*Pi/4.0;
                    const double motor_mass_overhead = 1.1;//To account for the mass of the nozzle, fore side flange - TODO: increase this?
                    return motor_cylinder_volume * chamber::material_density * motor_mass_overhead;
                }

                double cost() const
                {
                    const double propellant_grain_mass = receiver_().propellant_volume() * galcit::density;
                    return empty_mass() * chamber::material_price + propellant_grain_mass * galcit::price;
                }

            protected:
                Receiver &receiver_() {return static_cast<Receiver&>(*this);}
                const Receiver &receiver_() const {return static_cast<const Receiver&>(*this);}

                double propellant_grain_square_side_length_() const
                {
                    return std::sqrt(in_diameter_*in_diameter_/2.0);
                }
                //Note that octave uses the outside diameter here
                static double wall_thickness_(double in_diameter)
                {
                    const double pressure = 20000000;//N/m^2	(1 bar = 100 000 N/m^2)
                    const double tensile_strength = 580000000;//N/m^2		(steel or aluminum);
                    const double safety_factor = 1.1;
                    return (pressure * 0.5*in_diameter * safety_factor) / tensile_strength;
                }

                double length_ = 0.0;
                double in_diameter_ = 0.0;
                double out_diameter_ = 0.0;
        };

    class SideBurner: public Motor_crtp<SideBurner>
    {
        public:
            using Motor = Motor_crtp<SideBurner>;

            SideBurner(double length, double in_diameter): Motor(length, in_diameter) {}

            double burning_surface() const
            {
                const double buring_sides = 2.0;
                return buring_sides * length_ * propellant_grain_square_side_length_();
            }
            double propellant_volume() const
            {
                const double tmp = propellant_grain_square_side_length_();
                return tmp*tmp * length_;
            }
    };

    class EndBurner: public Motor_crtp<EndBurner>
    {
        public:
            using Motor = Motor_crtp<EndBurner>;
            EndBurner(double length, double in_diameter): Motor(length, in_diameter) {}

            double burning_surface() const
            {
                return in_diameter_*in_diameter_*Pi/4.0;
            }
            double propellant_volume() const
            {
                return burning_surface() * length_;
            }
    };

} 

#endif
