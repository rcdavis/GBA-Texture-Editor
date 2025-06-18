cmake_minimum_required(VERSION 3.10)

project(GBATextureEditor VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(ImGui)
add_subdirectory(Editor)

set(CMAKE_BUILD_TYPE Debug)
