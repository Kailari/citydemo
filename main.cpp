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
#include "util.h"
#include "buildinggrid.h"

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

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

    GLuint program = compileShaderProgram();
    glUseProgram(program);

    GLint projLocation = glGetUniformLocation(program, "proj");
    GLint viewLocation = glGetUniformLocation(program, "view");
    GLint modelLocation = glGetUniformLocation(program, "model");

    mat4x4 proj, view;

    // Build view-matrix
    vec3 eyePos = {25.0f, 100.0f, -35.0f};
    vec3 target = {45.0f, 0.0f, 35.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    mat4x4_look_at(view, eyePos, target, up);

    glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    printf("Window/OpenGL initialized, generating buildings...\n");

    GeneratorSimple generator;
    BuildingGrid grid;
    std::vector<Building*> buildings;
    for (int32 nFloors = 4; nFloors < 13; nFloors++) {
        BuildingTemplate buildingTemplate = {1, 1, nFloors};
        buildings.push_back(new Building(buildingTemplate, &generator));
        grid.registerBuilding(buildings[buildings.size() - 1]);
    }

    grid.refill();


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

        if (glfwGetKey(window, GLFW_KEY_TAB)) {
            for (Building* building : buildings) {
                building->refreshTextures();
            }
        }


        // Camera movement
        const float cameraSpeed = 1.0f;
        eyePos[0] += cameraSpeed * (glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A)); // Strafe
        eyePos[1] += cameraSpeed * (glfwGetKey(window, GLFW_KEY_SPACE) - glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)); // Up/down
        eyePos[2] += cameraSpeed * (glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S)); // Forward

        mat4x4_look_at(view, eyePos, target, up);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, (const GLfloat*) view);

        for (Building* building : buildings) {
            building->render(modelLocation);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (Building* building : buildings) {
        delete building;
    }

    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}