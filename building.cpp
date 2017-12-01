#include <algorithm>
#include <linmath.h>
#include <GL/glew.h>
#include "building.h"
#include "util.h"
#include "generator.h"

Building::Building(const BuildingTemplate &properties, const AGenerator* generator)
        : properties(properties)
          , generator(generator)
          , vertices(nullptr)
          , indices(nullptr)
          , nVertices(0)
          , nIndices(0)
          , vao(0)
          , vbo(0) {
    refreshInstances();
}

void Building::createInstance(GridPos pos, bool useRandomTexture) {
    const bool generate = textures.empty() || useRandomTexture;

    int32 textureIndex;
    if (generate) {
        const GLuint textureId = generator->generateTexture(properties);

        textureIndex = (int32) textures.size();
        textures.push_back(textureId);
    } else {
        textureIndex = Util::randRange((int32) textures.size());
    }

    instances.push_back({pos, !generate, textureIndex});
}

void Building::refreshInstances() {
    delete vertices;
    delete indices;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);


    auto generated = generator->generateVertices(properties);
    vertices = generated.vertices;
    nVertices = generated.nVertices;
    indices = generated.indices;
    nIndices = generated.nIndices;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nVertices, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 6));

    if (!instances.empty()) {
        refreshTextures();
    }
}

void Building::refreshTextures() {
    // Delete old textures
    for (GLuint texture : textures) {
        glDeleteTextures(1, &texture);
    }
    textures.clear();

    // Move old instances to a temporary vector
    std::vector<Instance> oldInstances(instances);
    instances.clear();

    // Sort so that instances without randomized texture are created first
    std::sort(
            oldInstances.begin(),
            oldInstances.end(),
            [](Instance a, Instance b) {
                return a.hasRandomTexture != b.hasRandomTexture && !a.hasRandomTexture;
            }
    );

    // Re-create all instances
    for (Instance instance : oldInstances) {
        createInstance(instance.pos, instance.hasRandomTexture);
    }
}

void Building::render(GLint modelMatrixUniformLocation) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    bound = true;

    for (Instance instance : instances) {
        renderInstance(instance, modelMatrixUniformLocation);
    }

    bound = false;
}

void Building::renderInstance(Instance instance, GLint modelMatrixUniformLocation) {
    float x = instance.pos.x * g_gridSizeX;
    float y = 0.0f;
    float z = instance.pos.z * g_gridSizeZ;

    // Create model matrix
    mat4x4 model;
    mat4x4_identity(model);
    mat4x4_translate(model, x, y, z);

    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, (const GLfloat*) model);

    if (!bound) {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }

    glBindTexture(GL_TEXTURE_2D, textures[instance.textureIndex]);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_SHORT, (const void*) indices);
}

Building::~Building() {
    delete vertices;
    delete indices;

    for (GLuint texture : textures) {
        glDeleteTextures(1, &texture);
    }
}
