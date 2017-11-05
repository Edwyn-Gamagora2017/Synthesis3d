#include "Square.h"

Square::Square(vec3* centre, vec3* normal, double height, double width, vec3* albedo, primitive::propriete proprietePrimitive, double intensite) : plan( centre, normal, albedo, proprietePrimitive, intensite )
{
    this->height = height;
    this->width = width;
}

Square::~Square()
{

}

std::experimental::optional<intersection_data> Square::intersection(ray * rayon)
{
    std::experimental::optional<intersection_data> planIntersection = plan::intersection( rayon );

    if( planIntersection &&
           (*planIntersection).getPosition().getX() < this->point->getX()+this->width/2 &&
           (*planIntersection).getPosition().getX() > this->point->getX()-this->width/2 &&
           (*planIntersection).getPosition().getY() < this->point->getX()+this->height/2 &&
           (*planIntersection).getPosition().getY() > this->point->getX()-this->height/2
           )
        return planIntersection;

    return {};
}

vec3 Square::normalPoint(vec3 point)
{
    plan::getNormal();
}
