#pragma once

#include "Core.h"

#include <vector>

enum class FramebufferTextureFormat {
	None,
	RGBA8,
	RedInt32,
	Depth24Stencil8,
	Depth = Depth24Stencil8
};

struct FramebufferTextureSpecs {
	constexpr FramebufferTextureSpecs() = default;
	constexpr FramebufferTextureSpecs(const FramebufferTextureFormat format) : textureFormat(format) {}

	FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification {
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecs> &attachments) : attachments(attachments) {}

	std::vector<FramebufferTextureSpecs> attachments;
};

struct FramebufferSpecs {
	FramebufferAttachmentSpecification attachments;
	uint32 width = 0;
	uint32 height = 0;
	uint32 samples = 1;
	bool swapChainTarget = false;
};

class GLFramebuffer {
public:
	GLFramebuffer(const FramebufferSpecs &specs);
	~GLFramebuffer();

	void Invalidate();

	void Bind();
	void Unbind();

	void Resize(const uint32 width, const uint32 height);
	int32 ReadPixel(const uint32 attachmentIndex, const int32 x, const int32 y);

	void ClearAttachment(const uint32 attachmentIndex, const int32 value);

	uint32 GetColorAttachment(const uint32 index = 0) const {
		return mColorAttachments[index];
	}

	const FramebufferSpecs &GetSpecs() const { return mSpecs; }

	static Ref<GLFramebuffer> Create(const FramebufferSpecs &specs);

private:
	std::vector<uint32> mColorAttachments;
	std::vector<FramebufferTextureSpecs> mColorAttachmentSpecs;

	FramebufferSpecs mSpecs;

	uint32 mId = 0;
	uint32 mDepthAttachment = 0;
	FramebufferTextureSpecs mDepthAttachmentSpecs = FramebufferTextureFormat::None;
};
