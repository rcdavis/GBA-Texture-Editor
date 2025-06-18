project(Editor)

add_executable(Editor
    src/Main.cpp
)

target_include_directories(Editor PUBLIC include)

target_link_libraries(Editor PRIVATE ImGui)
