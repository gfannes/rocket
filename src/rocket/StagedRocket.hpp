#ifndef HEADER_rocket_StagedRocket_hpp_ALREADY_INCLUDED
#define HEADER_rocket_StagedRocket_hpp_ALREADY_INCLUDED

#include "rocket/Shape.hpp"
#include "rocket/Motor.hpp"
#include "rocket/Earth.hpp"
#include "rocket/V2.hpp"
#include "gubg/debug.hpp"
#include <list>
#include <ostream>
#include <cassert>

namespace rocket { 
    template <typename Receiver>
        class StagedRocket_crtp
        {
            public:
                Position position{rocket::Earth::radius(), 0.0};
                /* Velocity velocity{0.0, -rocket::Earth::equator_speed()}; */
                Velocity velocity{0.0, 0.0};
                Direction direction{1.0, 0.0};

                double radius = 0.0;

                Shape shape;

                struct Stage
                {
                    double nr_thr = 0.0;

                    double duration = 0.0;
                    double thr_duration = 0.0;
                    double thr_one = 0.0;
                    double mass_one = 0.0;
                    double mass() const {return (nr_thr*mass_one)*(duration/thr_duration);}
                    double thrust() const {return nr_thr*thr_one;}
                    Stage(double nr_thr): nr_thr(nr_thr) {}
                };
                std::list<Stage> stages;

                //Call this after setting the thruster parameters or the stages or the shape
                //Might include more settings in the future
                void update()
                {
                    for (auto &stage: stages)
                    {
                        stage.duration = receiver_().thr_duration;
                        stage.thr_duration = receiver_().thr_duration;
                        stage.thr_one = receiver_().thr_one;
                        stage.mass_one = receiver_().thr_mass;
                    }
                    shape.drag_coef = receiver_().drag_coef;
                    shape.cross_area = Pi*radius*radius;
                }

                double payload = 0.0;
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
                Receiver &receiver_() {return static_cast<Receiver&>(*this);}
                const Receiver &receiver_() const {return static_cast<const Receiver&>(*this);}
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
    template <typename Receiver>
        std::ostream &operator<<(std::ostream &os, const StagedRocket_crtp<Receiver> &v)
        {
            v.stream(os);
            return os;
        }
} 

#endif
