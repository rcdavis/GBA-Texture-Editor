
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

#include "OpenGL/GLTexture.h"

static constexpr char* LoadImageFileDialogKey = "LoadImageFileDialogKey";

Application::~Application() {
	Shutdown();
}

bool Application::Init() {
	assert(!mImGuiInitialized && "Application is already initialized!");

	glfwSetErrorCallback(GlfwErrorCallback);

	if (!glfwInit()) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(1280, 720, "GBA Texture Editor", nullptr, nullptr);
	if (!mWindow) {
		LOG_ERROR("Failed to create GLFW window");
		return false;
	}

	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("Failed to initialize GLAD");
		return false;
	}

	glfwSwapInterval(1);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	/*ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;*/

	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	mImGuiInitialized = true;

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	LOG_INFO("Application initialized successfully");

	return true;
}

void Application::Shutdown() {
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderImGui();

	glfwSwapBuffers(mWindow);
}

void Application::RenderImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	RenderMainMenuBar();

	if (mCurrentTexture && mCurrentTexture->IsLoaded()) {
		ImGui::Begin("Texture Viewer");
		ImGui::Image((ImTextureID)mCurrentTexture->GetId(), ImVec2(mCurrentTexture->GetWidth(), mCurrentTexture->GetHeight()));
		ImGui::End();
	}

	RenderLoadImageDialog();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
