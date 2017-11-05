#ifndef H_LUMIERE
#define H_LUMIERE

#include "vec3.h"
#include "sphere.h"
#include <deque>

#pragma once
class lumiere : public sphere
{
    std::deque< vec3 > points;

public:
	lumiere(vec3 centre, double rayon, vec3 * albedo, double intensite, int qttRayons);

	virtual std::experimental::optional<intersection_data> intersection( ray * rayon );
	virtual vec3 normalPoint( vec3 point );
	virtual vec3 getMinValues();
    virtual vec3 getMaxValues();

	std::deque<vec3> getPointRepresentatif( vec3 direction );

	~lumiere();
};

#endif // H_LUMIERE
