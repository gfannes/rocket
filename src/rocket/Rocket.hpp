#ifndef HEADER_rocket_Rocket_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Rocket_hpp_ALREADY_INCLUDED

#include "rocket/Shape.hpp"
#include "rocket/V2.hpp"
#include <ostream>

namespace rocket { 
    class Rocket
    {
        public:
            double mass() const {return 10.0e3;}

            Position position{rocket::Earth::radius(), 0.0};
            /* Velocity velocity{0.0, -rocket::Earth::equator_speed()}; */
            Velocity velocity{0.0, 0.0};
            Direction direction{1.0, 0.0};

            Shape shape{0.1, 10.0};

            double speed() const { return velocity.norm(); }

            Force thrust() const
            {
                int n = 10;
                double thr = 2250.0;
                return direction.invert().multiply(thr*n);
            }
            Force drag() const
            {
                Force none;
                return none;
            }

            void process(const Force &force, double dt)
            {
                const auto m = mass();
                auto acceleration = force.multiply(1.0/m);

                velocity += acceleration.multiply(dt);

                position += velocity.multiply(dt);

                normalize_();
            }

            void stream(std::ostream &os) const
            {
                os << C(position)C(velocity)C(direction);
            }

        private:
            void normalize_()
            {
                direction = velocity;
                if (!direction.normalize())
                    direction = Direction{1.0, 0.0};
            }
    };
    std::ostream &operator<<(std::ostream &os, const Rocket &r)
    {
        r.stream(os);
        return os;
    }
} 

#endif
