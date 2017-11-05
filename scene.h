#ifndef SCENE_H
#define SCENE_H

/*#include "intersection_data.h"
#include "ray.h"
#include "lumiere.h"
#include <deque>*/
#include "primitive.h"
#include "plan.h"
#include "lumiere.h"
#include "intersection_data.h"
#include <deque>

#pragma once
class scene
{
	std::deque<primitive*> objets;
	std::deque<lumiere*> lumieres;
	std::deque<plan*> plans;

	BoundingBox * box;
public:
	scene();

	std::deque<primitive*> getObjets();
	void ajouterObjet(primitive * obj);

	std::deque<plan*> getPlans();
	void ajouterPlan(plan * obj);

	std::deque<lumiere*> getLumieres();
	void ajouterLumiere(lumiere * lum);

	void calculateBoundingBoxes();
	std::experimental::optional<intersection_data> intersection(ray * rayon);
	double intensiteLumieres(primitive * primitiveOrigine, intersection_data intersectionCamera);

	~scene();

    //vec3 energyRecu( intersection_data pointSurface, primitive * primitiveSurface, vec3 directionSortant, scene sceneObjets, int qttRayons, int recIndex, int recMax );
    vec3 emission( intersection_data pointSurface, vec3 direction, scene * sceneObjets, int qttRayons, int recIndex, int recMax );
    //vec3 energyEmise( intersection_data pointSurface, primitive * primitiveSurface, vec3 directionSortant, scene sceneObjets, int qttRayons, int recIndex, int recMax );
    vec3 incident( intersection_data pointSurface, vec3 direction, scene * sceneObjets, int qttRayons, int recIndex, int recMax );
};

#endif
