
#include <cmath>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "generator_simple.h"

Building* GeneratorSimple::generate(BuildingTemplate buildingTemplate) {
    // 5 visible faces, 4 vertices per face
    // vertices ==> 5 face x 4 corners = 20
    auto* vertices = new Vertex[20];
    auto* indices = new uint16[35];

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = buildingTemplate.sizeX * g_gridSizeX;
    float d = buildingTemplate.sizeZ * g_gridSizeZ;
    float h = buildingTemplate.nFloors * g_floorHeight;

    unsigned short nV = 0;

    unsigned short nI = 0;
    unsigned short vI = 0;
    for (unsigned short i = 0; i < 6; i++, vI += 4, nI += 6) {
        indices[nI + 0] = vI + 0;
        indices[nI + 1] = vI + 2;
        indices[nI + 2] = vI + 1;
        indices[nI + 3] = vI + 2;
        indices[nI + 4] = vI + 0;
        indices[nI + 5] = vI + 3;
    }

    // TODO: Interleave vertex data to avoid duplicates

    // XY, front
    float nx = 0;
    float ny = 0;
    float nz = -1;
    float uMin = 0 / 2.0f;
    float uMax = 1 / 2.0f;
    vertices[nV++] = {x + 0, y + 0, z + 0, nx, ny, nz, uMin, 0};
    vertices[nV++] = {x + w, y + 0, z + 0, nx, ny, nz, uMax, 0};
    vertices[nV++] = {x + w, y + h, z + 0, nx, ny, nz, uMax, 1};
    vertices[nV++] = {x + 0, y + h, z + 0, nx, ny, nz, uMin, 1};

    // XY, back
    nx = 0;
    ny = 0;
    nz = 1;
    uMin = 0 / 2.0f;
    uMax = 1 / 2.0f;
    vertices[nV++] = {x + w, y + 0, z + d, nx, ny, nz, uMin, 0};
    vertices[nV++] = {x + 0, y + 0, z + d, nx, ny, nz, uMax, 0};
    vertices[nV++] = {x + 0, y + h, z + d, nx, ny, nz, uMax, 1};
    vertices[nV++] = {x + w, y + h, z + d, nx, ny, nz, uMin, 1};

    // ZY, left
    nx = -1;
    ny = 0;
    nz = 0;
    uMin = 1 / 2.0f;
    uMax = 2 / 2.0f;
    vertices[nV++] = {x + 0, y + 0, z + d, nx, ny, nz, uMin, 0};
    vertices[nV++] = {x + 0, y + 0, z + 0, nx, ny, nz, uMax, 0};
    vertices[nV++] = {x + 0, y + h, z + 0, nx, ny, nz, uMax, 1};
    vertices[nV++] = {x + 0, y + h, z + d, nx, ny, nz, uMin, 1};

    // ZY, right
    nx = 1;
    ny = 0;
    nz = 0;
    uMin = 1 / 2.0f;
    uMax = 2 / 2.0f;
    vertices[nV++] = {x + w, y + 0, z + 0, nx, ny, nz, uMin, 0};
    vertices[nV++] = {x + w, y + 0, z + d, nx, ny, nz, uMax, 0};
    vertices[nV++] = {x + w, y + h, z + d, nx, ny, nz, uMax, 1};
    vertices[nV++] = {x + w, y + h, z + 0, nx, ny, nz, uMin, 1};

    // XZ, top
    nx = 0;
    ny = 1;
    nz = 0;
    vertices[nV++] = {x + 0, y + h, z + 0, nx, ny, nz, 0, 0};
    vertices[nV++] = {x + w, y + h, z + 0, nx, ny, nz, 0.01, 0};
    vertices[nV++] = {x + w, y + h, z + d, nx, ny, nz, 0.01, 0.01};
    vertices[nV++] = {x + 0, y + h, z + d, nx, ny, nz, 0, 0.01};

    return new Building(vertices, nV, indices, nI, generateTexture(buildingTemplate));
}

GLuint GeneratorSimple::generateTexture(BuildingTemplate buildingTemplate) {
    // TODO: Generate textures per-face (Generate textures adjacent to a single texture and uv-map accordingly)

    // Calculate properties
    const int32 faceWidth = g_nWindowsPerFloor * (g_windowWidth + g_marginWidth) + g_marginWidth;
    const int32 texWidth = 2 * faceWidth;
    
    const int32 texHeight = (g_windowHeight + g_marginHeight) * buildingTemplate.nFloors + g_marginHeight + g_bottomMargin;

    // Warn if texture width is invalid
    if ((texWidth % 4) != 0) {
        printf("Texture width must be divisible by four! texWidth=%d", texWidth);
    }


    RGBAColor pixels[texHeight][texWidth];
    for (int32 face = 0; face < 2; face++) {
        // Randomize window lights
        std::minstd_rand randomGen((uint64) (glfwGetTime() * 0xFFFF));
        bool lit[buildingTemplate.nFloors][g_nWindowsPerFloor];
        for (int32 y = 0; y < buildingTemplate.nFloors; y++) {
            for (int32 x = 0; x < g_nWindowsPerFloor; x++) {
                lit[y][x] = (randomGen() % 100) < 60;
            }
        }

        // Generate pixels
        for (int32 y = 0; y < texHeight; y++) {
            for (int32 x = 0; x < faceWidth; x++) {
                int32 yy = y - g_bottomMargin;

                // Check if we are on a margin row
                //  - each row is consists of a margin and a window
                //  - thus height of a row is window height + margin height
                //  - we can get y-delta relative to the start of the current row by taking modulus of y by row height
                //  - we know that the wall starts and ends with a margin
                //  - thus if delta is less than margin height, current y-coordinate is on a margin
                const int32 rowHeight = g_windowHeight + g_marginHeight;
                bool isMarginRow = (yy % rowHeight) < g_marginHeight;

                // Check if we are on a margin column, same idea as with rows
                const int32 columnWidth = g_windowWidth + g_marginWidth;
                bool isMarginColumn = (x % columnWidth) < g_marginWidth;

                int32 xx = (face * faceWidth) + x;
                if (yy < 0 || isMarginRow || isMarginColumn) {
                    pixels[y][xx] = {0, 0, 0, 0};
                } else {
                    auto row = (int32) floor(yy / (double) rowHeight);
                    auto column = (int32) floor(x / (double) columnWidth);

                    // TODO: Add randomization to window tint
                    // TODO: Add randomization to window lightness

                    const RGBAColor litColor = {250, 250, 220, 255};
                    const RGBAColor unlitColor = {50, 50, 50, 255};
                    pixels[y][xx] = (lit[row][column] ? litColor : unlitColor);
                }
            }
        }
    }

    // Create the texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*) pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return texture;
}
