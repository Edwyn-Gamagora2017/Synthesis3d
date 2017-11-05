#include "lumiere.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "utils.h"
using namespace std;

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

lumiere::lumiere( vec3 centre, double rayon, vec3 * albedo, double intensite, int qttRayons ):
    sphere::sphere( rayon, centre, albedo, primitive::diffuse, intensite )
{
    if( qttRayons < 1 ) qttRayons = 1;
	for( int i = 0; i < qttRayons; i++ )
    {
        double r1 = rand()/(double)RAND_MAX;
        double r2 = rand()/(double)RAND_MAX;

        this->points.push_back( vec3(
                                     centre.getX() + 2*this->getRayon()*cos(2*M_PI*r1)*sqrt( r2*(1-r2) ),
                                     centre.getY() + 2*this->getRayon()*sin(2*M_PI*r1)*sqrt( r2*(1-r2) ),
                                     centre.getZ() + this->getRayon()*(1-2*r2)
                               ));
    }
}

std::experimental::optional<intersection_data> lumiere::intersection(ray * rayon)
{
    return sphere::intersection(rayon);
}

vec3 lumiere::normalPoint(vec3 point){
    return sphere::normalPoint(point);
}

vec3 lumiere::getMinValues()
{
    return sphere::getMinValues();
}
vec3 lumiere::getMaxValues()
{
    return sphere::getMaxValues();
}

std::deque<vec3> lumiere::getPointRepresentatif( vec3 direction ){
    // la normal plus proche au rayon
    /*vec3 resultat = this->points[0];
    double scalaire = direction.normalized().produitScalaire( this->normalPoint( resultat ) );
    for( int i=1; i<this->points.size(); i++ )
    {
        double scalaireAux = direction.normalized().produitScalaire( this->normalPoint( this->points[i] ) );
        if( scalaireAux > scalaire )
        {
            scalaire = scalaireAux;
            resultat = this->points[i];
        }
    }
    std::deque<vec3> res;
    res.push_back( resultat );
    return res;*/

    return this->points;
}

lumiere::~lumiere() {}
