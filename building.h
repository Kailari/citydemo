#ifndef DEMO001_BUILDING_H
#define DEMO001_BUILDING_H

#include <GL/glew.h>
#include <vector>
#include "typedef.h"
#include "buildingtemplate.h"
#include "gridpos.h"

class AGenerator;
struct Vertex;

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
    Building(const BuildingTemplate &properties, const AGenerator* generator);

    virtual ~Building();

    /**
     * Creates a new instance of this building at given position.
     * Does not verify that building can actually be fit there, so always remember
     * checking that before creating instances.
     *
     * @param pos Position where to place the instance. Buildings larger than 1x1 extend towards positive XZ
     * @param useRandomTexture If true, use texture from randomly selected existing instance. Generate new if false.
     */
    void createInstance(GridPos pos, bool useRandomTexture);

    /**
     * Re-generates all instances. Re-generates vertices and textures for every instance.
     */
    virtual void refreshInstances();

    /**
     * Re-generates instances' textures
     */
    virtual void refreshTextures();

    /**
     * Renders all instances
     */
    virtual void render(GLint modelMatrixUniformLocation);

    void clearInstances();

    int32 getSizeX();

    int32 getSizeZ();

protected:
    struct Instance {
        GridPos pos;
        bool hasRandomTexture;
        int32 textureIndex;
    };

    virtual void renderInstance(Instance instance, GLint modelMatrixUniformLocation);


private:

    const BuildingTemplate properties;
    const AGenerator* generator;

    std::vector<Instance> instances;

    std::vector<GLuint> textures;
    Vertex* vertices;
    uint16* indices;

    uint16 nIndices;
    uint16 nVertices;

    GLuint vao, vbo;

    bool bound;
};


#endif //DEMO001_BUILDING_H
