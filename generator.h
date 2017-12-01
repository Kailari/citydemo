#ifndef DEMO001_GENERATOR_H
#define DEMO001_GENERATOR_H

#include "vertex.h"
#include "buildingtemplate.h"

struct GeneratedVertices {
    Vertex* vertices;
    uint16 nVertices;
    uint16* indices;
    uint16 nIndices;
};

class AGenerator {
public:

    /**
     * Generates vertices and indices for a building.
     * @param buildingTemplate
     * @return
     */
    virtual GeneratedVertices generateVertices(BuildingTemplate buildingTemplate) const = 0;

    /**
     * Generates textures for a building.
     * @param buildingTemplate
     * @return
     */
    virtual GLuint generateTexture(BuildingTemplate buildingTemplate) const = 0;
};

#endif //DEMO001_GENERATOR_H
