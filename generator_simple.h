#ifndef DEMO001_GENERATOR_SIMPLE_H
#define DEMO001_GENERATOR_SIMPLE_H

#include "typedef.h"
#include "building.h"
#include "generator.h"

struct RGBAColor {
    uint8 r, g, b, a;
};

/**
 * Generates simple cubic buildings with little to no detail.
 */
class GeneratorSimple : public AGenerator {
public:
    Building* generateVertices(BuildingTemplate buildingTemplate) override;

    GLuint generateTexture(BuildingTemplate buildingTemplate) override;
};

#endif //DEMO001_GENERATOR_SIMPLE_H
