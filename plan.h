#ifndef PLAN_H
#define PLAN_H

#include "primitive.h"
#include "BoundingBox.h"

class plan : public primitive
{
    protected :
        vec3 * point;
        vec3 * normal;

    public:
        plan( vec3 * point, vec3 * normal, vec3 * albedo, primitive::propriete proprietePrimitive, double intensite );
        ~plan();

        vec3 * getPoint();
        vec3 * getNormal();

        virtual std::experimental::optional<intersection_data> intersection( ray * rayon );
        virtual vec3 normalPoint( vec3 point );
        virtual vec3 getMinValues();
        virtual vec3 getMaxValues();
};

#endif // PLAN_H
