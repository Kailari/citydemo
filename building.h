#ifndef DEMO001_BUILDING_H
#define DEMO001_BUILDING_H

#include <GL/glew.h>
#include "typedef.h"

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

// pos:     3 float components
// normal:  3 float components
// uv:      2 float components
struct Vertex {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

struct BuildingTemplate {
    const unsigned int sizeX;
    const unsigned int sizeZ;

    const unsigned int nFloors;
};

class Building {
    const Vertex* vertices;

    const unsigned short* indices;
    const unsigned short numVertices;

    const unsigned short numIndices;

    const GLuint texturePtr;

public:

    const Vertex* getVertices() const;
    const unsigned short* getIndices() const;

    const unsigned short getNumVertices() const;

    const unsigned short getNumIndices() const;

    Building(const Vertex* vertices, unsigned short nVertices, const unsigned short* indices, unsigned short nIndices,
             GLuint texturePtr);

    ~Building();

    GLuint getTexture();
};


#endif //DEMO001_BUILDING_H
