cmake_minimum_required(VERSION 3.18)
project(PUF_Toolkit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

file(GLOB PUF_BCH_Decoder_Sources PUF_BCH_Decoder/*.cpp)
add_executable(PUF_BCH_Decoder ${PUF_BCH_Decoder_Sources})

file(GLOB PUF_BCH_Encoder_Sources PUF_BCH_Encoder/*.cpp)
add_executable(PUF_BCH_Encoder ${PUF_BCH_Encoder_Sources})

file(GLOB PUF_Toolkit_Sources PUF_Toolkit/*.cpp)
add_executable(PUF_Toolkit ${PUF_Toolkit_Sources})
