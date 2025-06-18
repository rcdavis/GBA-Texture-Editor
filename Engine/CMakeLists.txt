project(Engine)

add_compile_definitions(
    SPDLOG_COMPILED_LIB
)

add_library(Engine STATIC
    ImGui/src/imgui.cpp
    ImGui/src/imgui_draw.cpp
    ImGui/src/imgui_widgets.cpp
    ImGui/src/imgui_tables.cpp
    ImGui/src/imgui_demo.cpp
    ImGui/src/imgui_impl_glfw.cpp
    ImGui/src/imgui_impl_opengl3.cpp
    ImGuiFileDialog/src/ImGuiFileDialog.cpp
    spdlog/src/async.cpp
    spdlog/src/bundled_fmtlib_format.cpp
    spdlog/src/cfg.cpp
    spdlog/src/color_sinks.cpp
    spdlog/src/file_sinks.cpp
    spdlog/src/spdlog.cpp
    spdlog/src/stdout_sinks.cpp
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

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
)

target_include_directories(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/include
)

target_link_libraries(Engine PRIVATE glfw)
