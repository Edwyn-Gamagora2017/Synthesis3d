#include "primitive.h"
#include <math.h>
//#include <algorithm>
#include <iostream>
#include <deque>
//using namespace std;

primitive::primitive(vec3* albedo, propriete proprietePrimitive, double intensite )
{
    this->setAlbedo( albedo );
    this->setPropriete( proprietePrimitive );
    this->setIntensite( intensite );
}

primitive::~primitive(){}

void primitive::setAlbedo(vec3 * albedo)
{
    this->albedo = albedo;
}
void primitive::setPropriete(primitive::propriete proprietePrimitive)
{
	this->proprietePrimitive = proprietePrimitive;
}
void primitive::setIntensite(double intensite)
{
	this->intensite = intensite;
}

vec3 * primitive::getAlbedo()
{
    return this->albedo;
}
primitive::propriete primitive::getPropriete()
{
	return this->proprietePrimitive;
}
double primitive::getIntensite()
{
	return this->intensite;
}

bool primitive::isSpeculaire(){
	return this->proprietePrimitive == primitive::speculaire;
}
bool primitive::isTransparente() {
	return this->proprietePrimitive == primitive::transparente;
}

ray * primitive::rebondir(ray rayon, intersection_data intersection) {
	// vecteur I
	vec3 vecI = rayon.getDirection().normalized();

	// calculer la normal
	vec3 normale = this->normalPoint( intersection.getPosition() );
	vec3 normale_p = normale.multiplication(vecI.produitScalaire(normale));

	// decalage de l'intersection vers la normale
	intersection.setPosition( intersection.getPosition().addition(normale.multiplication(.001)) );

	// creer le rayon
	return new ray(intersection.getPosition(), (normale_p.multiplication(-2.)).addition(vecI));
}

// retour le rayon de sortie de l'primitive : NULL si il ny a pas de refraction
ray * primitive::refraction(ray rayon, intersection_data intersection, double indiceExterieur, bool entre )
{
	// on considere que le indice de refraction de l'exterieur = 1
	double indiceRefraction;
	if (entre) {
		indiceRefraction = (indiceExterieur / this->intensite);
	}
	else {
		indiceRefraction = this->intensite / indiceExterieur;
		// inverse la normal une fois quon est dedans la sphere
        intersection.setNormal((intersection.getNormal()).negative());
	}

	// tr = (ind1 / ind2)*(i-scal(i,n)*n)
	vec3 veci = rayon.getDirection();
	vec3 vecn = intersection.getNormal();
	double scalaire = (veci).produitScalaire(vecn);
	vec3 tR = veci.soustraction( vecn.multiplication( scalaire ));
	double radical = 1.-(pow( indiceRefraction,2 )*pow( (1.-scalaire),2));
/*
std::cout << "Rayon: "<< rayon.getDirection().toString() << std::endl;
std::cout << "Normal:"<< intersection.getNormal().toString() << std::endl;
std::cout << "Escalaire:" << scalaire << std::endl;
std::cout << "Radical:" <<radical << std::endl;
/*/

// sans refraction
/*
vec3 tN = intersection.getNormal().negative();
vec3 vecT = rayon.getDirection();
/*/

/*
	if (radical > 0) {
		// tn = -n*sqrt[1-pow(ind1 / ind2)*pow(1-scal(i,n))]
		vec3 tN = ((intersection.getNormal()).negative()).multiplication(sqrt(radical));
		vec3 vecT = (tR.addition(tN)).normalized();
/*/
//* autre formule
		// t = ( i * ind ) + n * ( ind * cos - sqrt( 1 - ind*ind * ( 1-cos*cos ) ) )
		double cosTeta = veci.produitScalaire( vecn ) / ( veci.norme() * vecn.norme() );
		vec3 t1 = veci.multiplication( indiceRefraction );
		vec3 t2 = vecn.multiplication( (indiceRefraction * cosTeta) - sqrt( 1. - pow( indiceRefraction,2 ) * (1.-pow(cosTeta,2)) ) );
		vec3 vecT = t1.addition( t2 );
		vec3 tN = intersection.getNormal().negative();
//*/
		// decalage de l'intersection vers la normale (en entrant, vers dedans la primitive; en sortant, vers l'exterieur)
        // intersection.setPosition( (intersection.getPosition()).addition(tN.multiplication(0.02)));
		if( entre ){
		    // trouver intersection dans la primitive
		    ray * rayonT = new ray(intersection.getPosition(), vecT);
            std::experimental::optional<intersection_data> intersectionInterne = this->intersection( rayonT );
            delete rayonT;

            // verifie si on est a la limite de la primitive = !foundInterne
//std::cout << intersectionInterne.size() << std::endl;
            if( intersectionInterne ){
//std::cout << "Intersec:" <<intersection.getPosition().toString() << std::endl;
//std::cout << "Interne:" <<intersectionInterne.getPosition().toString() << std::endl;
                // calculer la refraction en sortant
                // la derniere intersection eh la plus distante au cas ou il y a un probleme de precision
                return this->refraction( ray(intersection.getPosition(), vecT), (*intersectionInterne), indiceExterieur, false );
            }
            else{
//std::cout << "Externe:" << std::endl;
                // enlever la decalage
 //               intersection.setPosition( (intersection.getPosition()).addition(tN.multiplication(-0.02)));
//                return new ray( intersection.getPosition(), vecT );
                return NULL;
            }
		}
		else{
            // inversion de la normale
            intersection.setNormal((intersection.getNormal()).negative());

            // decalage vers dehors
            intersection.setPosition( (intersection.getPosition()).addition(tN.multiplication(0.01)));
            return new ray( intersection.getPosition(), vecT );
		}
/*
	}
	else {
		return NULL;
	}
/*/
}

bool primitive::compareBoundingBoxesX(primitive * a, primitive * b) {
    return (a->getMinValues().getX()) < (b->getMinValues().getX());
}
bool primitive::compareBoundingBoxesY(primitive * a, primitive * b) {
    return (a->getMinValues().getY()) < (b->getMinValues().getY());
}
bool primitive::compareBoundingBoxesZ(primitive * a, primitive * b) {
    return (a->getMinValues().getZ()) < (b->getMinValues().getZ());
}
