
#pragma once

#include <array>

#include "Core.h"
#include "glm/glm.hpp"

struct GLFWwindow;

class GLTexture;
class GLFramebuffer;
class GLVertexArray;
class GLShader;

class Application {
private:
	Application() = default;

public:
	static Application& Get() {
		static Application instance;
		return instance;
	}

	virtual ~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

	GLFWwindow* GetWindow() const { return mWindow; }

	bool Init();

	void Shutdown();

	void Run();

private:
	void Render();
	void RenderScene();
	void RenderImGui();
	void RenderMainMenuBar();

	void RenderLoadImageDialog();

	static void GlfwErrorCallback(int error, const char* description);

private:
	GLFWwindow* mWindow = nullptr;

	glm::vec2 mMousePos = {0.0f, 0.0f};
	glm::vec2 mViewportSize = {0.0f, 0.0f};
	std::array<glm::vec2, 2> mViewportBounds = {
		glm::vec2 {0.0f, 0.0f},
		glm::vec2 {0.0f, 0.0f}
	};

	Ref<GLTexture> mCurrentTexture;

	Ref<GLFramebuffer> mFramebuffer;

	Ref<GLVertexArray> mVao;
	Ref<GLShader> mShader;

	bool mImGuiInitialized = false;
	bool mViewportClickable = false;
};
