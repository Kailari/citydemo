#ifndef DEMO001_GENERATOR_H
#define DEMO001_GENERATOR_H

#include "vertex.h"
#include "buildingtemplate.h"

class AGenerator {
public:
    struct GeneratedVertices {
        Vertex* vertices;
        uint16* indices;
    };

    virtual GeneratedVertices generateVertices(BuildingTemplate buildingTemplate) = 0;
    virtual GLuint generateTexture(BuildingTemplate buildingTemplate) = 0;
};

#endif //DEMO001_GENERATOR_H
