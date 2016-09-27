#ifndef HEADER_rocket_StagedRocket_hpp_ALREADY_INCLUDED
#define HEADER_rocket_StagedRocket_hpp_ALREADY_INCLUDED

#include "rocket/Shape.hpp"
#include "rocket/Stages.hpp"
#include "rocket/Earth.hpp"
#include "rocket/V2.hpp"
#include "gubg/debug.hpp"
#include <ostream>
#include <cassert>

namespace rocket { 
    template <typename Receiver>
        class StagedRocket_crtp
        {
            private:
                static constexpr const char *logns = nullptr;

            public:
                Position position{rocket::Earth::radius(), 0.0};
                /* Velocity velocity{0.0, -rocket::Earth::equator_speed()}; */
                Velocity velocity{0.0, 0.0};
                Direction start_direction = position.rotate(0.25*Pi);
                Direction direction{1.0, 0.0};

                StagedRocket_crtp()
                {
                    start_direction.normalize();
                    direction = start_direction;
                }

                double radius = 0.0;

                Shape shape;

                Stages stages;

                //Call this after setting the thruster parameters or the stages or the shape
                //Might include more settings in the future
                void update()
                {
                    shape.drag_coef = receiver_().drag_coef;
                    shape.cross_area = Pi*radius*radius;
                }

                double payload = 0.0;
                double mass() const
                {
                    double m = payload;
                    m += stages.mass();
                    return m;
                }

                double speed() const { return velocity.norm(); }
                double acceleration() const {return accel_;}

                Force thrust() const
                {
                    return direction.multiply(stages.thrust());
                }
                Force drag(double density) const
                {
                    S(logns);
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
                    S("accel");
                    /* S(nullptr); */
                    const auto m = mass();
                    auto acceleration = force.multiply(1.0/m);

                    const auto slow = 5.0;
                    if (velocity.norm() <= slow)
                    {
                        L("before " << C(acceleration)C(direction)C(force));
                        const auto ok = acceleration.project(direction);
                        assert(ok);
                        L("after  " << acceleration);
                    }

                    accel_ = acceleration.norm();
                    L(C(accel_)C(acceleration.angle())C(direction.angle()));

                    velocity += acceleration.multiply(dt);

                    position += velocity.multiply(dt);

                    stages.process(dt);

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
                    const auto slow = 5.0;
                    if (velocity.norm() <= slow)
                    {
                        //We are flying slow: assume some guidance to keep us upright
                        direction = position.rotate(0.25*Pi);
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
