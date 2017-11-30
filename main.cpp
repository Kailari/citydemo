#include <cstdio>
#include <string>
#include <ios>
#include <fstream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "linmath.h"
#include "building.h"
#include "generator_simple.h"

static const struct {
    float x, y;
    float r, g, b;
} vertexData[3] = {
        {-0.6f, -0.4f, 1.f, 0.f, 0.f},
        {0.6f,  -0.4f, 0.f, 1.f, 0.f},
        {0.f,   0.6f,  0.f, 0.f, 1.f}
};


static std::string loadShaderCode(const char* filePath) {
    std::string code;
    std::ifstream stream(filePath, std::ios::in);
    if (stream.is_open()) {
        std::string line;
        while (getline(stream, line)) {
            code += "\n" + line;
        }
        stream.close();
    } else {
        fprintf(stdout, "Could not open stream!");
    }

    return code;
}

static GLuint compileShaderProgram() {
    std::string fragmentShaderCode = loadShaderCode("shaders/default.frag");
    std::string vertexShaderCode = loadShaderCode("shaders/default.vert");

    GLuint vertexShader, fragmentShader, program;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertSrcPtr = vertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &vertSrcPtr, nullptr);
    glCompileShader(vertexShader);

    GLint result = GL_FALSE;
    int infoLogLen;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLen);
    if (infoLogLen > 0) {
        std::vector<char> errorMsg(infoLogLen + 1);
        glGetShaderInfoLog(vertexShader, infoLogLen, nullptr, &errorMsg[0]);
        printf("ERROR!\n");
        fprintf(stdout, "%s\n", &errorMsg[0]);
    }


    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragSrcPtr = fragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &fragSrcPtr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLen);
    if (infoLogLen > 0) {
        std::vector<char> errorMsg(infoLogLen + 1);
        glGetShaderInfoLog(fragmentShader, infoLogLen, nullptr, &errorMsg[0]);
        printf("ERROR!\n");
        fprintf(stdout, "%s\n", &errorMsg[0]);
    }


    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
    if (infoLogLen > 0) {
        std::vector<char> errorMsg(infoLogLen + 1);
        glGetProgramInfoLog(program, infoLogLen, nullptr, &errorMsg[0]);
        printf("ERROR!\n");
        fprintf(stdout, "%s\n", &errorMsg[0]);
    }

    return program;
}


int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Initializing GLFW failed!\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(640, 640, "yee", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Opening GLFW window failed!\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = static_cast<GLboolean>(true);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Initializing GLEW failed!\n");
        return -1;
    }


    GeneratorSimple generator;
    BuildingTemplate buildingTemplate = {1, 1, 6};

    Building* building = generator.generate(buildingTemplate);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * building->getNumVertices(), building->getVertices(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 6));


    GLuint program = compileShaderProgram();
    glUseProgram(program);

    GLint projLocation = glGetUniformLocation(program, "proj");
    GLint viewLocation = glGetUniformLocation(program, "view");
    GLint modelLocation = glGetUniformLocation(program, "model");


    mat4x4 proj;
    mat4x4 view;

    // Build camera-matrix (store it temporarily to view)
    vec3 eyePos = {0.0f, 25.0f, 0.0f};
    vec3 target = {0.0f, 0.0f, 35.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    mat4x4_look_at(view, eyePos, target, up);

    glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    int width = -1;
    int height = -1;
    bool running = true;
    while (running) {
        if (glfwWindowShouldClose(window)) {
            running = false;
        }

        int newWidth, newHeight;
        glfwGetFramebufferSize(window, &newWidth, &newHeight);
        if (newWidth != width || newHeight != height) {
            width = newWidth;
            height = newHeight;

            glViewport(0, 0, width, height);
            float ratio = width / (float) height;

            mat4x4_perspective(proj, 90.0f, ratio, 0.001f, 1000.0f);
            //mat4x4_ortho(proj, -ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
            glUniformMatrix4fv(projLocation, 1, GL_FALSE, (const GLfloat*) proj);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
            delete building;
            building = generator.generate(buildingTemplate);

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * building->getNumVertices(), building->getVertices(),
                         GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 3));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 6));
        }

        // Create model matrix
        mat4x4 model, offsetTrans, trans, scale, rot;
        mat4x4_identity(model);

        mat4x4_identity(scale);

        mat4x4_identity(rot);
        mat4x4_rotate_Y(rot, rot, (float) glfwGetTime());

        mat4x4_translate(trans, target[0], target[1], target[2]);

        mat4x4_translate(offsetTrans, -g_gridSizeX / 2.0f, 0.0f, -g_gridSizeZ / 2.0f);

        mat4x4_mul(model, model, scale);
        mat4x4_mul(model, model, trans);
        mat4x4_mul(model, model, rot);
        mat4x4_mul(model, model, offsetTrans);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, (const GLfloat*) view);
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, (const GLfloat*) model);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBindTexture(GL_TEXTURE_2D, building->getTexture());
        glDrawElements(GL_TRIANGLES, building->getNumIndices(), GL_UNSIGNED_SHORT, (const void*) building->getIndices());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete building;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}