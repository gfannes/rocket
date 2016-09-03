#ifndef HEADER_rocket_Rocket_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Rocket_hpp_ALREADY_INCLUDED

#include "rocket/Shape.hpp"
#include "rocket/V2.hpp"
#include "gubg/debug.hpp"
#include <list>
#include <ostream>
#include <cassert>

namespace rocket { 
    class Rocket
    {
        public:
            Position position{rocket::Earth::radius(), 0.0};
            /* Velocity velocity{0.0, -rocket::Earth::equator_speed()}; */
            Velocity velocity{0.0, 0.0};
            Direction direction{1.0, 0.0};

            /* const double radius = 1.0; */
            const double radius = 0.1;

            //According to http://hyperphysics.phy-astr.gsu.edu/hbase/airfri2.html, a sphere with size 1m (area 3.1415m*m) made out of water (4188kg)
            //should fall at terminal velocity of 201.287m/s, given C == 0.5 and density air == 1.29kg/m/m/m
            const double c_sphere = 0.5;
            const double c_drop = 0.04;
            /* const double c_rocket = 0.1; */
            /* const double c_rocket = 0.75; */
            const double c_rocket = 0.9;

            /* const double c = c_sphere; */
            const double c = 2*c_rocket;
            Shape shape{c, Pi*radius*radius};

            static double thr_duration()
            {
                return 12;
                return 15.0;
            };
            static double thr_mass()
            {
                return 27.96;
                return 35.0;
            }
            static double thr_one()
            {
                return 3730.0;
            }
            struct Stage
            {
                double duration = thr_duration();
                double nr_thr = 0.0;
                double mass() const {return (nr_thr*thr_mass())*(duration/thr_duration());}
                double thrust() const {return nr_thr*thr_one();}
                Stage(double nr_thr): nr_thr(nr_thr) {}
            };
            //Reaches 164km with max_g of 7
            /* std::list<Stage> stages = {Stage(1000), Stage(800), Stage(350)}; */
            std::list<Stage> stages = {Stage(1)};

            /* const double payload = 10.0e3; */
            const double payload = 30.0;
            double mass() const
            {
                double m = payload;
                for (const auto &stage: stages)
                    m += stage.mass();
                return m;
            }

            double speed() const { return velocity.norm(); }
            double acceleration() const {return accel_;}

            Force thrust() const
            {
                if (stages.empty())
                    //No more fuel
                    return Force{};
                const auto &stage = stages.front();
                return direction.multiply(stage.thrust());
            }
            Force drag(double density) const
            {
                S("drag");
                const auto s = speed();
                L(C(s)C(s*s)C(density));
                if (s <= 1.0)
                    return Force{};
                auto force = velocity.invert();
                const auto ok = force.normalize();
                assert(ok);
                return force.multiply(0.5 * density * (s*s) * shape.cross_area * shape.drag_coef);
            }

            void process(const Force &force, double dt)
            {
                const auto m = mass();
                auto acceleration = force.multiply(1.0/m);
                accel_ = acceleration.norm();

                velocity += acceleration.multiply(dt);

                position += velocity.multiply(dt);

                if (!stages.empty())
                {
                    auto &stage = stages.front();
                    if (stage.duration <= dt)
                        //This stage is burnt: drop it
                        stages.pop_front();
                    else
                        //This stage is still burning, decrease its burn time
                        stage.duration -= dt;
                }

                normalize_();
            }

            void stream(std::ostream &os) const
            {
                os << C(position)C(speed())C(velocity)C(direction)C(acceleration());
            }

        private:
            void normalize_()
            {
                const auto slow = 1.0;
                if (velocity.norm() <= slow)
                {
                    //We are flying slow: assume some guidance to keep us upright
                    direction = position;
                }
                else
                {
                    //We have some speed: the rocket flies in that direction
                    direction = velocity;
                }
                const auto ok = direction.normalize();
                assert(ok);
            }

            double accel_ = 0.0;
    };
    std::ostream &operator<<(std::ostream &os, const Rocket &r)
    {
        r.stream(os);
        return os;
    }
} 

#endif
