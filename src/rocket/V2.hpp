#ifndef HEADER_rocket_V2_hpp_ALREADY_INCLUDED
#define HEADER_rocket_V2_hpp_ALREADY_INCLUDED

#include "rocket/Constants.hpp"
#include <array>
#include <cmath>
#include <ostream>

namespace rocket { 
    struct V2
    {
        double x = 0.0;
        double y = 0.0;

        V2() = default;
        V2(double x, double y): x(x), y(y) {}

        V2 &operator+=(const V2 &rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        double square_norm() const
        {
            return x*x + y*y;
        }
        double norm() const
        {
            return std::sqrt(square_norm());
        }
        double inprod(const V2 &rhs) const
        {
            return x*rhs.x + y*rhs.y;
        }
        double angle(const V2 &rhs) const
        {
            return std::acos(inprod(rhs)/norm()/rhs.norm());
        }
        double angle() const
        {
            return std::atan2(y, x);
        }
        double distance(const V2 &rhs) const
        {
            const auto dx = x-rhs.x;
            const auto dy = y-rhs.y;
            return std::sqrt(dx*dx + dy*dy);
        }

        V2 invert() const {return V2{-x, -y};}
        V2 multiply(double f) const {return V2{x*f, y*f};}

        V2 rotate(double angle) const
        {
            const auto cos = std::cos(angle);
            const auto sin = std::sin(angle);
            return V2{x*cos + -y*sin, x*sin + y*cos};
        }

        bool project(const V2 &direction)
        {
            const auto sn = direction.square_norm();
            if (sn == Eps)
                return false;
            const auto cross = direction.x*direction.y;
            const auto cross_x = cross*x;
            const auto cross_y = cross*y;
            x = (direction.x*direction.x*x + cross_y                  )/sn;
            y = (cross_x                   + direction.y*direction.y*y)/sn;
            return true;
        }

        bool normalize()
        {
            const auto n = norm();
            if (n < Eps)
                return false;
            x /= n;
            y /= n;
            return true;
        }
    };
    std::ostream &operator<<(std::ostream &os, const V2 &v)
    {
        return os << "[" << v.x << "," << v.y << "]";
    }

    using Position = V2;
    using Velocity = V2;
    using Direction = V2;
    using Force = V2;
} 

#endif
