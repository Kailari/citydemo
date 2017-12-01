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
    GeneratedVertices generateVertices(BuildingTemplate buildingTemplate) const override;

    GLuint generateTexture(BuildingTemplate buildingTemplate) const override;
};

#endif //DEMO001_GENERATOR_SIMPLE_H
