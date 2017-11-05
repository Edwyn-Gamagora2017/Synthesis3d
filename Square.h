#ifndef SQUARE_H
#define SQUARE_H

#include "plan.h"

class Square : public plan
{
    double width, height;

    public:
        Square( vec3 * centre, vec3 * normal, double height, double width, vec3 * albedo, primitive::propriete proprietePrimitive, double intensite );
        virtual ~Square();

        virtual std::experimental::optional<intersection_data> intersection( ray * rayon );
        virtual vec3 normalPoint( vec3 point );
};

#endif // SQUARE_H
