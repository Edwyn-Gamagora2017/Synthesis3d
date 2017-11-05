// Raytracer.cpp : Définit le point d'entrée pour l'application console.
//
#include <iostream>
#include <math.h>
#include "vec3.h"
#include "ray.h"
//#include "primitive.h"
//#include "sphere.h"
#include "plan.h"
#include "BoundingBox.h"
//#include "lumiere.h"
#include "scene.h"
#include "utils.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

#define PI 3.14159265
//857 s 4.3
/* fonction printVec
	imprimer les valeurs des coordonnees du vecteur. Utilise le "iostream" por l'impression sur console

	Params => a : vecteur a etre imprime
	Retour => void : le resultat est imprime sur console
*/
void printVec( vec3 a ) {
	cout << a.toString() << endl;
}

/* fonction obtenirDirectionRayon
	creer le vecteur de direction pour le pixel indique (j,i)

	Params => i : ligne du tableau - ecran
	Params => j : column du tableau - ecran
	Params => h : hauteur du tableau - ecran
	Params => w : longueur du tableau - ecran
	Params => fov : 'field of view' de la camera
	Retour => vec3 : la direction du rayon partant de la camera
*/
vec3 obtenirDirectionRayon( int i, int j, int h, int w, int fov ) {
	return (vec3((double)(j - w / 2), (double)(i - h / 2), (double)((-1)*(w / (2 * tan(((fov / 2) * PI / 180.0))))))).normalized();
}

vec3 obtenirCouleur( ray * rayon, scene * sceneObjets, int speculaireRebondi, int qttRayons ){
	std::experimental::optional<intersection_data> intersection_scene = sceneObjets->intersection(rayon);

	if ( intersection_scene ){// && (*intersection_scene).getPrimitiveIntersection() != NULL ) {
        primitive * primitive_resultat = (*intersection_scene).getPrimitiveIntersection();
		vec3 albedo;

		// speculaire
		if ( primitive_resultat->isSpeculaire() && speculaireRebondi < 3 ) {
			ray * rayon_rebondi = primitive_resultat->rebondir( *rayon, (*intersection_scene) );
			albedo = obtenirCouleur(rayon_rebondi, sceneObjets, speculaireRebondi + 1, qttRayons );

			// albedo_reflex = intensite * reflexion + (1-intensite) * albedo_objet
			vec3 albedo_reflex = albedo.multiplication(primitive_resultat->getIntensite());
			// obtenir la couleur de lobjet
			//vec3 albedo_objet = (primitive_resultat->getAlbedo())->multiplication((1-primitive_resultat->getIntensite()));
			//albedo = albedo_reflex.addition(albedo_objet);
			albedo = albedo_reflex;
			if(rayon_rebondi != NULL) delete rayon_rebondi;
		}
		// transparente
		else if (primitive_resultat->isTransparente()) {
			ray * rayon_refraction = primitive_resultat->refraction(*rayon, (*intersection_scene), 1., true );

			if (rayon_refraction != NULL) {
				albedo = obtenirCouleur( rayon_refraction, sceneObjets, speculaireRebondi, qttRayons );
			}
			else {
				albedo = vec3(0, 0, 0);
			}
			if(rayon_refraction != NULL) delete rayon_refraction;
		}
		// diffuse
		else {
            albedo = sceneObjets->emission( (*intersection_scene), rayon->getDirection(), sceneObjets, 0, 0, 2 );
		}

		return albedo;
	}
	else {
		return vec3(0., 0., 0.);
	}
}

int main()
{
    srand(1213);
	/*


	TP01 - Partie 04


	*/
	// --------------------------------------------------------------
	// --------------------------------------------------------------
	// Test :: Mirroir
	//*
	vec3 origine = vec3(0, 0, 0);

	scene * scene1 = new scene();

/* TESTE PLAN
    // objets
    scene1.ajouterObjet(new sphere(30, vec3(0, -20, -95), couleurToAlbedo(vec3(255, 0, 0)), sphere::diffuse, 1));		                // sphere rouge - centre
    // murs
    scene1.ajouterObjet(new plan( new vec3(0., -20., -50.), new vec3( 0.,1.,0. ), new vec3( 1.,1.,1. ), primitive::diffuse, 0.5 ));    // plan blanc - sol
    // lumieres
	scene1.ajouterLumiere(new lumiere( vec3(0, 40, -95), vec3(255, 255, 255), 1) );		// lumiere jaune
/*/
//* SCENE
    scene1->ajouterObjet( new sphere(10, vec3(0, -20, -50), couleurToAlbedo(vec3(255, 0, 0)), primitive::diffuse, 0.1));		// sphere
    scene1->ajouterObjet( new sphere(3, vec3(4, 4, -50), couleurToAlbedo(vec3(255, 0, 0)), primitive::diffuse, 0.1));		// sphere
	//scene1->ajouterObjet(new sphere(5, vec3(10, -5, -50), couleurToAlbedo(vec3(255, 255, 0)), primitive::speculaire, 1));				// sphere petite - jaune
	// murs
	scene1->ajouterPlan(new plan(new vec3(0., -20., 0.), new vec3( 0.,1.,0. ), couleurToAlbedo(vec3( 0.,0.,255. )), primitive::diffuse, 0.1));   // sol
	scene1->ajouterPlan(new plan(new vec3(0., +20., 0.), new vec3( 0.,-1.,0. ), couleurToAlbedo(vec3( 50.,150.,1. )), primitive::diffuse, 0.1));  // plafond
	scene1->ajouterPlan(new plan(new vec3(+20., 0., 0.), new vec3( -1.,0.,0. ), couleurToAlbedo(vec3( 95.,255.,95. )), primitive::diffuse, 0.1));  // gauche
	scene1->ajouterPlan(new plan(new vec3(-20., 0., 0.), new vec3( 1.,0.,0. ), couleurToAlbedo(vec3( 255.,95.,255. )), primitive::diffuse, 0.1));  // droite
	scene1->ajouterPlan(new plan(new vec3(0., 0., -60.), new vec3( 0.,0.,1. ), couleurToAlbedo(vec3( 255.,255.,255. )), primitive::diffuse, 0.1));  // fond
	scene1->ajouterPlan(new plan(new vec3(0, 0., 10.), new vec3( 0.,0.,-1. ), couleurToAlbedo(vec3( 150.,150.,150. )), primitive::diffuse, 0.1));  // arriere
	// lumieres
	scene1->ajouterLumiere( new lumiere( vec3(0, 20, -40), 1., couleurToAlbedo(vec3( 255.,255.,255. )), 300, 5) );		// lumiere jaune

	/*scene1.ajouterObjet(new BoundingBox(10,5,5,new vec3(0, 10., -50)));  // arriere
	scene1.ajouterObjet(new BoundingBox(10,5,5,new vec3(0, 0., -50)));  // arriere
	scene1.ajouterObjet(new BoundingBox(10,5,5,new vec3(10, 0., -50)));  // arriere*/
//*/

    scene1->calculateBoundingBoxes();

	// Tableau
	int w = 512;
	int h = 512;
	w = 64;
	h = 64;
	int fov = 60;

	// initialisation
	pixel ** colors = new pixel*[h];
	for (int i = 0; i<h; i++)
		colors[i] = new pixel[w];

	// Execute les rayons
	bool resultat = false;
	double t = 0.;
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
//int i = h/2+1;
//int j = w/2+1;
//int i = 10;
//int j = 52;
			vec3 direction = obtenirDirectionRayon(i, j, h, w, fov);
			ray * rayCamera = new ray(origine, direction);

			// definir la couleur
			colors[i][j].set(
                albedoToCouleur( obtenirCouleur( rayCamera, scene1, 0, 0 ) )
			);
//std::cout << "COR " << i << " " << j << std::endl;

			if( rayCamera != NULL )
                delete rayCamera;
		}
		// v / 2.2. * 255.

	// sauvegarde dans le fichier "sphere.ppm"
	save_img("images/bounding", w, h, colors);
	//*/

	// arreter le logiciel, afin de regarder les resultats. sinon, il se fermera automatiquement
	/*
	int x;
	cin >> x;
	/*/

    return 0;
}
