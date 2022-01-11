cmake_minimum_required(VERSION 3.20)
project(digital_recognition)

set(CMAKE_CXX_STANDARD 14)

add_executable(digital_recognition digital_recognition/bpnet.cpp
        digital_recognition/bpnet.h
        digital_recognition/data_input.h
        digital_recognition/data_input.cpp
        digital_recognition/layer.h
        digital_recognition/layer.cpp
        digital_recognition/utils.h
        digital_recognition/main.cpp
        )
