
#pragma once

struct GLFWwindow;

class Application {
public:
	Application() = default;
	virtual ~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

	bool Init();

	void Shutdown();

	void Run();

private:
	void Render();
	void RenderImGui();
	void RenderMainMenuBar();

	void RenderLoadImageDialog();

	static void GlfwErrorCallback(int error, const char* description);

private:
	GLFWwindow* mWindow = nullptr;
	bool mImGuiInitialized = false;
};
