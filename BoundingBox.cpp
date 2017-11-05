#include "BoundingBox.h"
#include <algorithm>    // std::sort

// If interieur parameter has only one item, the bounding box will have only this item as this.interieur and a null this.boxes
// else, this.boxes will be composed of other boxes and this.interieur equals to null
BoundingBox::BoundingBox( std::deque<primitive*> interieur, interieurOrderAxe axe )
{
    if( interieur.size() > 0 ){
        this->orderAxe = axe;

        // order list based on the axe
        //sort( interieur.begin(), interieur.end(), (axe==interieurOrderAxe::axeX?primitive::compareBoundingBoxesX:(axe==interieurOrderAxe::axeY?primitive::compareBoundingBoxesY:primitive::compareBoundingBoxesZ)) );
        sort( interieur.begin(), interieur.end(), (axe==axeX?primitive::compareBoundingBoxesX:(axe==axeY?primitive::compareBoundingBoxesY:primitive::compareBoundingBoxesZ)) );

        // obtain the max and min values
        this->minValues = interieur[0]->getMinValues();
        this->maxValues = interieur[0]->getMaxValues();
        for( int i = 1; i < interieur.size(); i++ )
        {
            this->updateMinMaxValues( interieur[i] );
        }

        // create boxes (more than one object in interieur parameter) or interieur (just one object in interieur parameter)
        if( interieur.size() == 1 )
        {
            // create box to this primitive
            this->interieur = interieur[0];
        }
        else
        {
            this->interieur = nullptr;

            int amountBoxes = 2;
            int amountObjPerBox = interieur.size()/amountBoxes;
            // Create the boxes inside : divide the list of objects
            for(int i = 0; i < amountBoxes; i++)
            {
                std::deque<primitive*> subList;
                // inserting the range of objects from interieur into subList. the last sublist will have the rest of the list
                for( int j = i*amountObjPerBox; j < (i==amountBoxes-1?interieur.size():(i+1)*amountObjPerBox); j++ ){
                    subList.push_back( interieur[j] );
                }
                // Generating new boxes : the will be ordered based on the next axe
                this->boxes.push_back( BoundingBox( subList, (axe==axeX?axeY:(axe==axeY?axeZ:axeX)) ) );
            }
        }
    }
}

BoundingBox::~BoundingBox(){}

// Verify the collision with the box and its children ( if previousIntersection is farest than the intersection with the box )
std::experimental::optional<intersection_data> BoundingBox::intersection(ray * rayon, std::experimental::optional<intersection_data> previousIntersection )
{
    // Intersection to external box Cube
    std::experimental::optional<intersection_data> intersectBox = this->intersectionBox( rayon );
    // Calculate internal intersection if the external one if smaller than the previousIntersection
    if( intersectBox && ( !previousIntersection || (*intersectBox).getT() < (*previousIntersection).getT() ) )
    {
        // If the box has only one child, check its intersection
        if( this->interieur != NULL )
        {
            std::experimental::optional<intersection_data> interieurIntersection = this->interieur->intersection( rayon );
            if( interieurIntersection && ( !previousIntersection || (*interieurIntersection).getT() < (*previousIntersection).getT() ) )
            {
                return interieurIntersection;
            }
        }
        // If the box has boxes as children, check their intersection
        else
        {
            std::experimental::optional<intersection_data> intersection_minimun = previousIntersection;//( vec3(), vec3(), 0, NULL );

            for (int i = 0; i < this->boxes.size(); i++)
            {
                std::experimental::optional<intersection_data> intersectChild = this->boxes[i].intersection( rayon, intersection_minimun );

                if( intersectChild && ( !intersection_minimun || (*intersectChild).getT() < (*intersection_minimun).getT() ) ){
                    intersection_minimun = intersectChild;
                }
            }
            if( !previousIntersection || (*intersection_minimun).getT() < (*previousIntersection).getT() ){
                return intersection_minimun;
            }
        }
    }

    return {};
}

// Calculates the intersection between the box cube and a ray
std::experimental::optional<intersection_data> BoundingBox::intersectionBox( ray * rayon )
{
    // Using external library to calculate the intersection
    // Instantiating box in the external library format based on its min and max values
    Vec3f boundInf( minValues.getX(), minValues.getY(), minValues.getZ() );
    Vec3f boundSup( maxValues.getX(), maxValues.getY(), maxValues.getZ() );
    AABBox * intersectBox = new AABBox( boundInf, boundSup );

    // Creating a ray in the external library format
    Vec3f orig( rayon->getOrigine().getX(), rayon->getOrigine().getY(), rayon->getOrigine().getZ() );
    Vec3f direc( rayon->getDirection().getX(), rayon->getDirection().getY(), rayon->getDirection().getZ() );
    Ray r( orig, direc );

    float t;
    std::experimental::optional<intersection_data> result = {};
    if( intersectBox->intersect( r, t ) )
    {
        vec3 pos = rayon->position( t );
        result = intersection_data( pos, vec3(0,0,0), t, NULL );
    }
    if(intersectBox != NULL) delete intersectBox;

    return result;
}

// Updating min and max values inside the box : check if obj has extreme min or max coordinates
void BoundingBox::updateMinMaxValues( primitive * obj )
{
    vec3 objMinValues = obj->getMinValues();
    vec3 objMaxValues = obj->getMaxValues();
    if( minValues.getX() > objMinValues.getX() ){ minValues.setX( objMinValues.getX() ); }
    if( maxValues.getX() < objMaxValues.getX() ){ maxValues.setX( objMaxValues.getX() ); }
    if( minValues.getY() > objMinValues.getY() ){ minValues.setY( objMinValues.getY() ); }
    if( maxValues.getY() < objMaxValues.getY() ){ maxValues.setY( objMaxValues.getY() ); }
    if( minValues.getZ() > objMinValues.getZ() ){ minValues.setZ( objMinValues.getZ() ); }
    if( maxValues.getZ() < objMaxValues.getZ() ){ maxValues.setZ( objMaxValues.getZ() ); }
}

primitive* BoundingBox::getInterieur()
{
    return this->interieur;
}

vec3 BoundingBox::getMinValues()
{
    return this->minValues;
}
vec3 BoundingBox::getMaxValues()
{
    return this->maxValues;
}

/*bool BoundingBox::operator>( BoundingBox b )
{
    return (this->minValues.getX()) > (b.minValues.getX());
}*/

vec3 BoundingBox::getCentre()
{
    vec3 dimensions = this->getDimensions();
    return vec3( this->minValues.getX()+dimensions.getX()/2, this->minValues.getY()+dimensions.getY()/2, this->minValues.getZ()+dimensions.getZ()/2 );
}
vec3 BoundingBox::getDimensions()
{
    return vec3( this->maxValues.getX()-this->minValues.getX(), this->maxValues.getY()-this->minValues.getY(), this->maxValues.getZ()-this->minValues.getZ() );
}

string BoundingBox::toString( int level )
{
    string result = "";
    for(int i=0; i<level; i++){
        result += "-";
    }
    if( this->interieur != nullptr ){
        result += " OBJ\n";
    }
    else{
        result += " BOX\n";
        for( int i=0; i<this->boxes.size(); i++ ){
            result += this->boxes[i].toString( level+1 );
        }
    }
    return result;
}
