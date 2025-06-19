#pragma once

#include <filesystem>

#include "Core.h"

class GLTexture {
public:
	GLTexture();
	GLTexture(const std::filesystem::path &filepath);
	~GLTexture();

	uint32 GetId() const { return mId; }
	uint32 GetWidth() const { return mWidth; }
	uint32 GetHeight() const { return mHeight; }

	void Bind(uint32 slot = 0) const;
	void Unbind() const;

	bool IsLoaded() const { return mIsLoaded; }

	bool operator==(const GLTexture &texture) const;

	static Ref<GLTexture> Load(const std::filesystem::path &filepath);

private:
	uint32 mId = 0;
	uint32 mWidth = 0;
	uint32 mHeight = 0;
	uint32 mInternalFormat = 0;
	uint32 mDataFormat = 0;
	bool mIsLoaded = false;
};
