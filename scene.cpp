#include "scene.h"
#include "utils.h"
#include <iostream>
#include <deque>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

scene::scene(){}

std::deque<primitive *> scene::getObjets(){
	return this->objets;
}
void scene::ajouterObjet(primitive * obj){

	this->objets.push_back( obj );
}

std::deque<plan *> scene::getPlans(){
	return this->plans;
}
void scene::ajouterPlan(plan * obj){
	this->plans.push_back( obj );
}

std::deque<lumiere*> scene::getLumieres() {
	return this->lumieres;
}
void scene::ajouterLumiere(lumiere * lum)
{
	this->lumieres.push_back(lum);
	this->ajouterObjet( lum );
}

void scene::calculateBoundingBoxes(){
    /*std::deque<primitive*> units = this->objets;
    sort( units.begin(), units.end(), BoundingBox::compareBoundingBoxes );*/
    this->box = new BoundingBox( this->objets, BoundingBox::axeX );
}

/*std::deque<BoundingBox*> cutBoundingBox( BoundingBox * bBoxOrigine )
{
    for( int i=0; i<bBoxOrigine->getInterieur().size(); i++ )
    {

    }
}*/

std::experimental::optional<intersection_data> scene::intersection(ray * rayon) {
	std::experimental::optional<intersection_data> intersection_minimun = {};

    std::experimental::optional<intersection_data> intersectBoundingBox = this->box->intersection(rayon, {});

    if( intersectBoundingBox ){
        intersection_minimun = intersectBoundingBox;
    }

	for (int i = 0; i < this->plans.size(); i++)
	{
        std::experimental::optional<intersection_data> intersection_aux = this->plans[i]->intersection(rayon);
		if ( intersection_aux ) {
			if ( !intersection_minimun || (*intersection_aux).getT() < (*intersection_minimun).getT() ) {
				intersection_minimun = intersection_aux;
			}
		}
	}

    return intersection_minimun;
}

scene::~scene(){}

vec3 scene::emission( intersection_data pointSurface, vec3 direction, scene * sceneObjets, int qttRayons, int recIndex, int recMax )
{
    std::deque< lumiere* > lumieres = sceneObjets->getLumieres();
    double Le = pointSurface.getPrimitiveIntersection()->getIntensite();

    // decalage
    pointSurface.setPosition( pointSurface.getPosition().addition( pointSurface.getNormal().multiplication(.01) ) );
    vec3 integralIncident(0,0,0);
    vec3 integralLampes(0,0,0);
    if( recIndex < recMax ){
        for( int i = 0; i < lumieres.size(); i++ )
        {
            vec3 directionObjectif = lumieres[i]->getCentre().soustraction( pointSurface.getPosition() ).normalized();

            vec3 Li = this->incident( pointSurface, directionObjectif, sceneObjets, qttRayons, recIndex, recMax );
            double g = directionObjectif.produitScalaire( pointSurface.getNormal() );
            vec3 f = pointSurface.getPrimitiveIntersection()->getAlbedo()->multiplication( g );

            integralLampes = integralLampes.addition( Li.multiplication( f.division( g ) ) );
        }
        for( int i = 0; i < qttRayons; i++ )
        {
            // generer directions aleatoires bien distribues
            double r1 = rand()/(double)RAND_MAX;
            double r2 = rand()/(double)RAND_MAX;

            vec3 directionObjectif = vec3(
                          cos( 2*M_PI*r1 ) * sqrt( 1.-r2 ),
                          sin( 2*M_PI*r1 ) * sqrt( 1.-r2 ),
                          sqrt( r2 )
            ).normalized();
            //directionObjectif = directionObjectif.changeBase( pointSurface->getNormal() );

            // inversion au cas ou la direction eh vers l'origine
            if( directionObjectif.produitScalaire( pointSurface.getNormal() ) < 0. )
            {
                directionObjectif = directionObjectif.negative();
            }

            vec3 Li = this->incident( pointSurface, directionObjectif, sceneObjets, qttRayons, recIndex, recMax );
            double g = directionObjectif.produitScalaire( pointSurface.getNormal() );
            vec3 f = pointSurface.getPrimitiveIntersection()->getAlbedo()->multiplication( g );

            integralIncident = integralIncident.addition( Li.multiplication( f.division( g ) ) );
        }
        if(qttRayons > 0) integralIncident = integralIncident.division( qttRayons );
    }

    return integralIncident.addition( integralLampes ).addition( vec3(Le,Le,Le) );
}

vec3 scene::incident( intersection_data pointSurface, vec3 direction, scene * sceneObjets, int qttRayons, int recIndex, int recMax )
{
    ray * rayonVersObjectif = new ray( pointSurface.getPosition(), direction );
    double G = 0;
    double V = 0;
    vec3 Lo(0,0,0);

    std::experimental::optional<intersection_data> intersection_sortant = sceneObjets->intersection( rayonVersObjectif );

    delete rayonVersObjectif;

    if( intersection_sortant )// && (*intersection_sortant).getPrimitiveIntersection() != NULL )
    {
        primitive * primitive_resultat = (*intersection_sortant).getPrimitiveIntersection();

        V = 1;
        vec3 positionObjectif = (*intersection_sortant).getPosition();
        vec3 normalObjectif = primitive_resultat->normalPoint( positionObjectif );

        if( (*intersection_sortant).getT() < 1 )  // intersect entre objets
            G = 0;
        else
            G = direction.produitScalaire( pointSurface.getNormal() ) * direction.negative().produitScalaire( normalObjectif )/pow((*intersection_sortant).getT(), 2);

        Lo = this->emission( intersection_data( positionObjectif, normalObjectif, (*intersection_sortant).getT(), primitive_resultat ), direction, sceneObjets, qttRayons, recIndex + 1, recMax );
    }

    return Lo.multiplication(V*G);
}
