#include "plan.h"
#include "utils.h"

plan::plan(vec3* point, vec3* normal, vec3* albedo, primitive::propriete proprietePrimitive, double intensite)
: primitive( albedo, proprietePrimitive, intensite )
{
    this->point = point;
    this->normal = normal;
}

plan::~plan()
{

}

vec3* plan::getPoint()
{
    return this->point;
}

vec3* plan::getNormal()
{
    return this->normal;
}

std::experimental::optional<intersection_data> plan::intersection(ray * rayon)
{
    // ( p - p0 ). n = 0
    // p = v0 + tv
    // t = ( p0 - v0 ).n / v.n

    // v.n = 0 -> vecteur et plan sont paralleles :
    // ( p0 - v0 ).n = 0 -> vecteur et contenu pour le plan
    // ( p0 - v0 ).n != 0 -> pas dintersection
    // v.n != 0 -> 1 point dintersection
    double scalaire = rayon->getDirection().produitScalaire( *(this->getNormal()) );

    // ignore les vecteurs paralleles
    if( !equalsDouble( scalaire, 0. ) ){
        double numerateur = this->getPoint()->soustraction( rayon->getOrigine() ).produitScalaire( *(this->getNormal()) );
        double t = numerateur/scalaire;

        // ignores solutions negatives
        if(t>0){
            vec3 position = rayon->position( t );
            return intersection_data( position, *(this->getNormal()), t, this );
        }
    }
    return {};
}

vec3 plan::normalPoint(vec3 point)
{
    return *(this->getNormal());
}

vec3 plan::getMinValues()
{
    return vec3();
}
vec3 plan::getMaxValues()
{
    return vec3();
}
