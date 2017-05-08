#
# This is the CMake build configuration for MiniTrace on Unix-like platform
# 
# Author: Minsi Chen
#
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(MiniTrace)

FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

SET(SRC_FILES
	Box.cpp
	Triangle.cpp
	Camera.cpp
	Material.cpp
	Ray.cpp
	Vector3.cpp
	Light.cpp
	Plane.cpp
	RayTracer.cpp
	Sphere.cpp
	Scene.cpp
	)

INCLUDE_DIRECTORIES( 
	${GLUT_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	)

ADD_EXECUTABLE(minitracer main.cpp 
	${SRC_FILES}
	)

TARGET_LINK_LIBRARIES(minitracer
	${GLUT_glut_LIBRARY}
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	)
