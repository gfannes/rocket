#include "catch.hpp"
#include "rocket/Motor.hpp"
#include "gubg/debug.hpp"

namespace  { 
    template <typename Motor>
        bool print_stats(const std::string &name)
        {
            S("test");
            Motor m(0.15, 0.0338);

            L(C(m.name()));
            L(C(m.empty_mass()));
            L(C(m.full_mass()));
            L(C(m.cost()));
            L(C(m.thrust()));
            L(C(m.burn_time()));
        }
} 

TEST_CASE("SideBurner tests", "[ut][motor]")
{
    S("test");
    print_stats<rocket::SideBurner>("Side burner");
    print_stats<rocket::EndBurner>("End burner");
}
