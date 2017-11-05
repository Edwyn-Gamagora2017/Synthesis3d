#include "sphere.h"
#include <math.h>
#include <deque>

sphere::sphere(double rayon, vec3 centre, vec3 * albedo, primitive::propriete proprieteSphere, double intensite) : primitive( albedo, proprieteSphere, intensite ) {
	this->set(rayon, centre);
}
sphere::sphere(double rayon, vec3 * albedo, primitive::propriete proprieteSphere, double intensite): primitive( albedo, proprieteSphere, intensite ) {		// centre en (0,0,0); intensite = 1;
	this->set( rayon, vec3(0, 0, 0) );
}

void sphere::set(double rayon, vec3 centre) {
	this->setRayon(rayon);
	this->setCentre(centre);
}
void sphere::setRayon(double rayon) {
	this->rayon = rayon;
}
void sphere::setCentre(vec3 centre) {
	this->centre = centre;
}

double sphere::getRayon() {
	return this->rayon;
}
vec3 sphere::getCentre() {
	return this->centre;
}

std::experimental::optional<intersection_data> sphere::intersection(ray * rayonIntersect) {
	vec3 unitaire = rayonIntersect->getDirection().normalized();
	double scalaire = unitaire.produitScalaire( rayonIntersect->getOrigine().soustraction( this->getCentre() ) );
	double normeCarre = rayonIntersect->getOrigine().soustraction(this->getCentre()).normeCarre();
	double rayonCarre = this->getRayon()*this->getRayon();

	// ax2 + bx + c = 0
	double a = 1.;
	double b = scalaire * 2.;
	double c = normeCarre - rayonCarre;
	// Solutions
	double found = false;

	double racineInterieur = ((b*b) - (4. * a*c));

	if (racineInterieur < 0) {
		return {};
	}
	else
	{
		double racine = sqrt( racineInterieur );
		double t1 = (b*(-1.) + racine) / (2.*a);
		double t2 = (b*(-1.) - racine) / (2.*a);

		if (t2 > 0) {
            vec3 position = rayonIntersect->position(t2);
            vec3 normal = (position.soustraction( this->getCentre() )).normalized();
            return intersection_data( position, normal, t2, this );
		}
		if (t1 > 0) {
            vec3 position = rayonIntersect->position(t1);
            vec3 normal = ((position.soustraction( this->getCentre() )).normalized());
            return intersection_data( position, normal, t1, this );
		}
	}
	return {};
}

vec3 sphere::normalPoint(vec3 point)
{
    return (point.soustraction(this->getCentre())).normalized();
}

vec3 sphere::getMinValues()
{
//std::cout << vec3( this->centre.getX()-this->rayon, this->centre.getY()-this->rayon, this->centre.getZ()-this->rayon ).toString() << std::endl;
    return vec3( this->centre.getX()-this->rayon, this->centre.getY()-this->rayon, this->centre.getZ()-this->rayon );
}
vec3 sphere::getMaxValues()
{
    return vec3( this->centre.getX()+this->rayon, this->centre.getY()+this->rayon, this->centre.getZ()+this->rayon );
}

bool sphere::dedans(vec3 point)
{
    return ( point.soustraction( this->getCentre() ).norme() < this->getRayon() );
}


sphere::~sphere(){}
