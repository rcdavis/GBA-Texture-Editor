project(Editor)

add_executable(Editor
    src/Application.cpp
    src/Main.cpp
    src/ImGuiWindows/BaseImGuiWindow.cpp
)

target_include_directories(Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Editor PRIVATE Engine)
