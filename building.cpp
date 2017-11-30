#include "building.h"

Building::Building(const Vertex* vertices,
                   const unsigned short nVertices,
                   const unsigned short* indices,
                   const unsigned short nIndices,
                   const GLuint texturePtr)
        : vertices(vertices)
          , indices(indices)
          , numVertices(nVertices)
          , numIndices(nIndices)
          , texturePtr(texturePtr) {
    // NO-OP
}

Building::~Building() {
    glDeleteTextures(1, &texturePtr);
    delete vertices;
    delete indices;
}

const Vertex* Building::getVertices() const {
    return vertices;
}

const unsigned short* Building::getIndices() const {
    return indices;
}

const unsigned short Building::getNumVertices() const {
    return numVertices;
}

const unsigned short Building::getNumIndices() const {
    return numIndices;
}

GLuint Building::getTexture() {
    return texturePtr;
}


