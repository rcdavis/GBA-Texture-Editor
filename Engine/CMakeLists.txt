project(Engine)

add_library(Engine STATIC
    ImGui/src/imgui.cpp
    ImGui/src/imgui_draw.cpp
    ImGui/src/imgui_widgets.cpp
    ImGui/src/imgui_tables.cpp
    ImGui/src/imgui_demo.cpp
    ImGui/src/imgui_impl_glfw.cpp
    ImGui/src/imgui_impl_opengl3.cpp
    ImGuiFileDialog/src/ImGuiFileDialog.cpp
    glad/src/glad.c
    stb/src/stb_image.c
)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/include
)

target_include_directories(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/include
)

target_link_libraries(Engine PRIVATE glfw)
