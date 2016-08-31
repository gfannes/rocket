#ifndef HEADER_rocket_Drag_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Drag_hpp_ALREADY_INCLUDED

namespace rocket { 
    template <typename Rocket>
        double drag(const Rocket &rocket, double density)
        {
            const auto speed = rocket.speed();
            return 0.5 * density * (speed*speed) * rocket.shape.cross_area * rocket.shape.drag_coef;
        }
} 

#endif
