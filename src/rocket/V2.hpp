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

        double norm() const
        {
            return std::sqrt(x*x + y*y);
        }

        V2 invert() const {return V2{-x, -y};}
        V2 multiply(double f) const {return V2{x*f, y*f};}

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
