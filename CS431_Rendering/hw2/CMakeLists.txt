cmake_minimum_required(VERSION 3.5)
project(hw2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Camera.cpp
    Camera.h
    lodepng.cpp
    lodepng.h
    Matrix3D.cpp
    Matrix3D.h
    Matrix4D.cpp
    Matrix4D.h
    Model.cpp
    Model.h
    Polygon.cpp
    Polygon.h
    Ray.cpp
    Ray.h
    sak338_hw2.cpp
    SMFModel.cpp
    SMFModel.h
    Sphere.cpp
    Sphere.h
    Transformation.cpp
    Transformation.h
    Vector3D.cpp
    Vector3D.h
    Vector4D.cpp
    Vector4D.h Light.cpp Light.h)

add_executable(hw2 ${SOURCE_FILES})