#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "raybox.cpp"
#include "RedBlackTree.cpp"

#include "vec3.h"
#include "primitive.h"
#include <deque>

class BoundingBox
{
    public:
        enum interieurOrderAxe {
            axeX,axeY,axeZ
        };

    protected:
        interieurOrderAxe orderAxe;     // Axe which is used to order the boxes inside
        vec3 maxValues, minValues;      // The minimun and maximun coordinates found inside the box
        primitive * interieur;          // A pointer to the object inside the box, if the box has only one component. If there are more than one, interieur will be null and the box will has boxes inside
        std::deque<BoundingBox> boxes;  // A list of boxes inside the box. If there is only one object inside the box, boxes will be null

    public:
        BoundingBox( std::deque<primitive*> interieur, interieurOrderAxe axe );
        virtual ~BoundingBox();

        // Verify the collision with the box and its children ( if previousIntersection is farest than the intersection with the box )
        std::experimental::optional<intersection_data> intersection( ray * rayon, std::experimental::optional<intersection_data> previousIntersection );

        // Updates min and max values inside the box : check if obj has extreme min or max coordinates
        void updateMinMaxValues( primitive * obj );
        primitive* getInterieur();
        void calculateSubBoxes();
        vec3 getCentre();
        vec3 getDimensions();
        string toString( int level );

    protected:
        // Calculates the intersection between the box cube and a ray
        std::experimental::optional<intersection_data> intersectionBox( ray * rayon );
        // Obtains the minimun coordinates inside the box
        vec3 getMinValues();
        // Obtains the maximun coordinates inside the box
        vec3 getMaxValues();

        //bool operator>( BoundingBox b );
        //static bool compareBoundingBoxes(BoundingBox * a, BoundingBox * b);
};

#endif // BOUNDINGBOX_H
