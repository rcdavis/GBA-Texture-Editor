project(Engine)

add_library(Engine STATIC
    src/imgui.cpp
    src/imgui_draw.cpp
    src/imgui_widgets.cpp
    src/imgui_tables.cpp
    src/imgui_demo.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
    src/ImGuiFileDialog.cpp
    glad/src/glad.c
    stb/src/stb_image.c
)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

target_include_directories(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/include
)

target_link_libraries(Engine PRIVATE glfw)
