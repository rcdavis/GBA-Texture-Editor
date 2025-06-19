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
    src/OpenGL/GLFramebuffer.cpp
    src/OpenGL/GLIndexBuffer.cpp
    src/OpenGL/GLShader.cpp
    src/OpenGL/GLTexture.cpp
    src/OpenGL/GLVertexArray.cpp
    src/OpenGL/GLVertexBuffer.cpp
    src/Utils/FileUtils.cpp
    src/Log.cpp
)

target_compile_definitions(Engine PUBLIC
    GLFW_INCLUDE_NONE
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Engine PUBLIC
        LOGGING_ENABLED
    )
endif()

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glm/include
)

target_include_directories(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/include
)

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(Engine PRIVATE
    glfw
    spdlog::spdlog
    OpenGL::GL
)
