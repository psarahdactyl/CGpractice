cmake_minimum_required(VERSION 3.5)
project(hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Matrix3D.cpp
    Matrix3D.h
    Matrix4D.cpp
    Matrix4D.h
    Model.h
    Model.cpp
    Ray.cpp
    Ray.h
    Camera.cpp
    Camera.h
    Sphere.cpp
    Sphere.h
    Vector3D.cpp
    Vector3D.h
    Vector4D.cpp
    Vector4D.h
        Polygon.cpp
        Polygon.h
    sak338_hw1.cpp
        lodepng.cpp
        lodepng.h SMFModel.cpp SMFModel.h)

add_executable(hw1 ${SOURCE_FILES})