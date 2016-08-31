#ifndef HEADER_rocket_Shape_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Shape_hpp_ALREADY_INCLUDED

namespace rocket { 
    struct Shape
    {
        double drag_coef = 0.0;
        double cross_area = 0.0;
        
        Shape() = default;
        Shape(double coef, double area): drag_coef(coef), cross_area(area) {}
    };
} 

#endif
