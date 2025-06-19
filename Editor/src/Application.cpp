
#include "Application.h"

#include <assert.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuiFileDialog.h>

#include "Log.h"

#include "OpenGL/GLFramebuffer.h"
#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLShader.h"
#include "OpenGL/GLTexture.h"

static constexpr char* LoadImageFileDialogKey = "LoadImageFileDialogKey";
static constexpr uint32 WindowWidth = 1280;
static constexpr uint32 WindowHeight = 720;

Application::~Application() {
	Shutdown();
}

bool Application::Init() {
	assert(!mImGuiInitialized && "Application is already initialized!");

	glfwSetErrorCallback(GlfwErrorCallback);

	if (!glfwInit()) {
		LOG_CRITICAL("Failed to initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(WindowWidth, WindowHeight, "GBA Texture Editor", nullptr, nullptr);
	if (!mWindow) {
		LOG_CRITICAL("Failed to create GLFW window");
		return false;
	}

	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_CRITICAL("Failed to initialize GLAD");
		return false;
	}

	glfwSwapInterval(1);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	mImGuiInitialized = true;

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	FramebufferSpecs specs;
	specs.attachments = {
		FramebufferTextureFormat::RGBA8,
		FramebufferTextureFormat::Depth
	};
	specs.width = WindowWidth;
	specs.height = WindowHeight;
	mFramebuffer = GLFramebuffer::Create(specs);

	LOG_INFO("Application initialized successfully");

	return true;
}

void Application::Shutdown() {
	mCurrentTexture = nullptr;
	mShader = nullptr;
	mVao = nullptr;
	mFramebuffer = nullptr;

	if (mImGuiInitialized) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		mImGuiInitialized = false;
	}

	glfwTerminate();
	mWindow = nullptr;
}

void Application::Run() {
	if (!Init()) {
		Shutdown();
		return;
	}

	while (!glfwWindowShouldClose(mWindow)) {
		glfwPollEvents();

		Render();
	}

	Shutdown();
}

void Application::Render() {
	RenderScene();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderImGui();

	glfwSwapBuffers(mWindow);
}

void Application::RenderScene() {
	mFramebuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto mousePos = ImGui::GetMousePos();
    mousePos.x -= mViewportBounds[0].x;
    mousePos.y -= mViewportBounds[0].y;

    const glm::vec2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
    mousePos.y = viewportSize.y - mousePos.y;
    mMousePos = glm::vec2((f32)mousePos.x, (f32)mousePos.y);

	mFramebuffer->Unbind();
}

void Application::RenderImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool dockspaceOpen = true;
    static constexpr bool optFullscreen = true;
    static constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if constexpr (optFullscreen)
    {
        const ImGuiViewport* const viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if constexpr (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
    ImGui::PopStyleVar();

    if constexpr (optFullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), dockspaceFlags);

	RenderMainMenuBar();

	RenderLoadImageDialog();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");

    const auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    const auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    const auto viewportOffset = ImGui::GetWindowPos();
    mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    const auto viewportPanelSize = ImGui::GetContentRegionAvail();
    mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    mViewportClickable = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();

    const uint64 texId = (uint64)mFramebuffer->GetColorAttachment();
    ImGui::Image((ImTextureID)texId, viewportPanelSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* const backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

void Application::RenderMainMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				ImGuiFileDialog::Instance()->OpenDialog(LoadImageFileDialogKey, "Choose Image", ".png,.jpg,.bmp");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Application::RenderLoadImageDialog() {
	ImGui::SetNextWindowSize(ImVec2(700, 350), ImGuiCond_FirstUseEver);
	if (ImGuiFileDialog::Instance()->Display(LoadImageFileDialogKey)) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			LOG_INFO("Loading file: {0}...", filePath);
			mCurrentTexture = GLTexture::Load(filePath);
			LOG_INFO("Loaded file: {0}...", filePath);
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void Application::GlfwErrorCallback(int error, const char* description) {
	LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}
