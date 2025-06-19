#pragma once

#include <functional>
#include <string>

class BaseImGuiWindow {
public:
	BaseImGuiWindow() = default;
	BaseImGuiWindow(const std::string &title, bool isOpen = false);
	virtual ~BaseImGuiWindow();

	const std::string &GetTitle() const { return mTitle; }
	void SetTitle(const std::string &title) { mTitle = title; }

	bool IsOpen() const { return mIsOpen; }
	void Open(bool isOpen) { mIsOpen = isOpen; }

	virtual void RenderMenuItem(std::function<void()> onClick = nullptr);
	void Render();

protected:
	virtual void OnRender() = 0;

protected:
	std::string mTitle;
	bool mIsOpen = false;
};
