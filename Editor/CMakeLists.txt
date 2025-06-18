project(Editor)

add_executable(Editor
    src/Main.cpp
    glad/src/glad.c
)

target_include_directories(Editor PUBLIC include glad/include)

target_link_libraries(Editor PRIVATE glfw)
