#include "catch.hpp"
#include "rocket/Motor.hpp"
#include "gubg/debug.hpp"

TEST_CASE("SideBurner tests", "[ut][motor]")
{
    S("test");

    rocket::SideBurner m(0.15, 0.0338);

    L(C(m.empty_mass()));
    L(C(m.cost()));
    L(C(m.thrust()));
}
TEST_CASE("EndBurner tests", "[ut][motor]")
{
    S("test");

    rocket::EndBurner m(0.15, 0.0338);

    L(C(m.empty_mass()));
    L(C(m.cost()));
    L(C(m.thrust()));
}
