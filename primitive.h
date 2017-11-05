#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "vec3.h"
#include "ray.h"
#include "intersection_data.h"
//#include "lumiere.h"
#include <experimental/optional>

class BoundingBox;

class primitive
{
    public:
        enum propriete {
            diffuse,
            speculaire,
            transparente
        };

    protected:
        vec3 * albedo;	// couleur entre [0,1]
        propriete proprietePrimitive;
        double intensite;	// valeur de intensite referent a chaque propriete du materiel : [0,1]

    public:
        primitive( vec3 * albedo, propriete proprietePrimitive, double intensite );
        ~primitive();

        void setAlbedo( vec3 * albedo );
        void setPropriete(propriete proprieteSphere);
        void setIntensite(double intensite);

        vec3 * getAlbedo();
        propriete getPropriete();
        double getIntensite();

        bool isSpeculaire();
        bool isTransparente();

        virtual std::experimental::optional<intersection_data> intersection( ray * rayon ) = 0;
        // returne la direction du vecteur normal associe a un point de la surface de la primitive
        virtual vec3 normalPoint( vec3 point ) = 0;
        virtual vec3 getMinValues() = 0;
        virtual vec3 getMaxValues() = 0;

        virtual ray * rebondir( ray rayon, intersection_data intersection );
        virtual ray * refraction( ray rayon, intersection_data intersection, double indiceExterieur, bool entre );
        static bool compareBoundingBoxesX(primitive * a, primitive * b);
        static bool compareBoundingBoxesY(primitive * a, primitive * b);
        static bool compareBoundingBoxesZ(primitive * a, primitive * b);
};

#endif // PRIMITIVE_H
