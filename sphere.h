#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"
#include "vec3.h"
#include "ray.h"
#include "intersection_data.h"
#include "BoundingBox.h"

#pragma once
class sphere : public primitive
{
protected:
	double rayon;
	vec3 centre;

public:
	sphere(double rayon, vec3 centre, vec3 * albedo, primitive::propriete proprieteSphere, double intensite);
	sphere(double rayon, vec3 * albedo, propriete proprieteSphere, double intensite);		// centre en (0,0,0); intensite = 1;

	void set(double rayon, vec3 centre);
	void setRayon(double rayon);
	void setCentre(vec3 centre);

	double getRayon();
	vec3 getCentre();

	virtual std::experimental::optional<intersection_data> intersection( ray * rayon );
	virtual vec3 normalPoint( vec3 point );
	virtual vec3 getMinValues();
    virtual vec3 getMaxValues();
	bool dedans( vec3 point );

	~sphere();
};

#endif
