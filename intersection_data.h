#include "vec3.h"
//#include "primitive.h"

class primitive;

#pragma once
class intersection_data
{
	vec3 position;
	vec3 normal;
	double t;
	primitive * primitiveIntersection;

public:
	intersection_data(vec3 position, vec3 normal, double t, primitive * primitiveIntersection);
	~intersection_data();

	void setPosition(vec3 position);
	void setNormal(vec3 normal);
	void setT(double t);

	vec3 getPosition();
	vec3 getNormal();
	double getT();
	primitive * getPrimitiveIntersection();
};

