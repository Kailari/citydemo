#ifndef DEMO001_BUILDING_H
#define DEMO001_BUILDING_H

#include <GL/glew.h>
#include <vector>
#include "typedef.h"
#include "buildingtemplate.h"
#include "generator.h"
#include "gridpos.h"

const float g_gridSizeX = 10.0f;
const float g_gridSizeZ = 10.0f;
const float g_floorHeight = 3.0f;

// windows per unit
const uint32 g_nWindowsPerFloor = 9;

// window/margin size in pixels
const uint32 g_windowWidth = 10U;
const uint32 g_windowHeight = 3U;
const uint32 g_marginWidth = 3U;
const uint32 g_marginHeight = 4U;

const uint32 g_bottomMargin = g_windowHeight + 2 * g_marginHeight;


class Building {
public:
    Building(const BuildingTemplate &properties, const AGenerator &generator);

    void createInstance(GridPos pos, bool useRandomTexture);

    void refreshVertices();

    void refreshTextures();

    void render();


private:
    struct Instance {
        const GridPos pos;
        const int32 textureIndex;
    };

    const BuildingTemplate properties;
    const AGenerator generator;

    const std::vector<Instance> instances;

    std::vector<GLuint> textures;
    Vertex* vertices;
    uint16* indices;

    uint16 nIndices;
    uint16 nVertices;
};


#endif //DEMO001_BUILDING_H
