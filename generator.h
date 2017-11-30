#ifndef DEMO001_GENERATOR_H
#define DEMO001_GENERATOR_H

#include "building.h"

class AGenerator {
public:
    virtual Building* generate(BuildingTemplate buildingTemplate) = 0;
    virtual GLuint generateTexture(BuildingTemplate buildingTemplate) = 0;
};

#endif //DEMO001_GENERATOR_H
