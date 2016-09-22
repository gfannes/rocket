#include "catch.hpp"
#include "rocket/Motor.hpp"

TEST_CASE("Motor tests", "[ut][motor]")
{
    rocket::Motor m(0.15, 0.0338);

    REQUIRE(m.thrust() == 0.0);
}
