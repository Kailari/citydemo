cmake_minimum_required(VERSION 3.8)
project(demo001)

set(CMAKE_CXX_STANDARD 11)

configure_file("shaders/default.frag" "shaders/default.frag" COPYONLY)
configure_file("shaders/default.vert" "shaders/default.vert" COPYONLY)


set(SOURCE_FILES main.cpp building.cpp building.h generator_simple.h generator_simple.cpp generator.h typedef.h vertex.h buildingtemplate.h gridpos.h util.h)
add_executable(demo001 ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories("dependencies/" ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})

target_link_libraries(demo001 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY})
