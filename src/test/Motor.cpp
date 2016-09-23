#include "catch.hpp"
#include "rocket/Motor.hpp"
#include "gubg/debug.hpp"

TEST_CASE("Motor tests", "[ut][motor]")
{
    S("test");

    rocket::Motor m(0.15, 0.0338);

    L(C(m.empty_mass()));
    L(C(m.cost()));
    L(C(m.thrust()));
}
