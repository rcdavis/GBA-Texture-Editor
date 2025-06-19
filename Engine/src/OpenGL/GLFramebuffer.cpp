#include "OpenGL/GLFramebuffer.h"

#include "Log.h"

#include <glad/glad.h>

#include <cassert>

static constexpr uint32 s_MaxFramebufferSize = 8192;

namespace Utils {
	static constexpr GLenum TextureTarget(const bool multisampled) {
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(const bool multisampled, uint32 *const outId, const uint32 count) {
		glGenTextures(count, outId);
	}

	static void BindTexture(const bool multisampled, const uint32 id) {
		glBindTexture(TextureTarget(multisampled), id);
	}

	static void AttachColorTexture(
		const uint32 id,
		const int32 samples,
		const GLenum internalFormat,
		const GLenum format,
		const uint32 width,
		const uint32 height,
		const int32 index)
	{
		const bool multisampled = samples > 1;
		if (multisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	static void AttachDepthTexture(
		const uint32 id,
		const int32 samples,
		const GLenum format,
		const GLenum attachmentType,
		const uint32 width,
		const uint32 height)
	{
		const bool multisampled = samples > 1;
		if (multisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	static bool IsDepthFormat(const FramebufferTextureFormat format) {
		switch (format)
		{
		case FramebufferTextureFormat::Depth24Stencil8:
			return true;
		}

		return false;
	}

	static GLenum FramebufferTextureFormatToGL(const FramebufferTextureFormat format) {
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:
			return GL_RGBA8;

		case FramebufferTextureFormat::RedInt32:
			return GL_RED_INTEGER;
		}

		assert(false && "Cannot convert FB format to GL type");
		return 0;
	}
}

GLFramebuffer::GLFramebuffer(const FramebufferSpecs &specs) :
	mColorAttachments(),
	mColorAttachmentSpecs(),
	mSpecs(specs),
	mId(0),
	mDepthAttachment(0),
	mDepthAttachmentSpecs(FramebufferTextureFormat::None)
{
	for (auto &spec : mSpecs.attachments.attachments) {
		if (!Utils::IsDepthFormat(spec.textureFormat))
			mColorAttachmentSpecs.emplace_back(spec);
		else
			mDepthAttachmentSpecs = spec;
	}

	Invalidate();
}

GLFramebuffer::~GLFramebuffer() {
	glDeleteFramebuffers(1, &mId);
	glDeleteTextures((GLsizei)std::size(mColorAttachments), std::data(mColorAttachments));
	glDeleteTextures(1, &mDepthAttachment);
}

void GLFramebuffer::Invalidate() {
	if (mId) {
		glDeleteFramebuffers(1, &mId);
		glDeleteTextures((GLsizei)std::size(mColorAttachments), std::data(mColorAttachments));
		glDeleteTextures(1, &mDepthAttachment);

		mColorAttachments.clear();
		mDepthAttachment = 0;
	}

	glGenFramebuffers(1, &mId);
	glBindFramebuffer(GL_FRAMEBUFFER, mId);

	const bool multisample = mSpecs.samples > 1;

	if (!std::empty(mColorAttachmentSpecs)) {
		mColorAttachments.resize(std::size(mColorAttachmentSpecs));
		Utils::CreateTextures(multisample, std::data(mColorAttachments), (GLsizei)std::size(mColorAttachments));

		for (size_t i = 0; i < std::size(mColorAttachments); ++i) {
			Utils::BindTexture(multisample, mColorAttachments[i]);
			switch (mColorAttachmentSpecs[i].textureFormat)
			{
			case FramebufferTextureFormat::RGBA8:
				Utils::AttachColorTexture(mColorAttachments[i], mSpecs.samples, GL_RGBA8, GL_RGBA, mSpecs.width, mSpecs.height, (GLsizei)i);
				break;

			case FramebufferTextureFormat::RedInt32:
				Utils::AttachColorTexture(mColorAttachments[i], mSpecs.samples, GL_R32I, GL_RED_INTEGER, mSpecs.width, mSpecs.height, (GLsizei)i);
				break;
			}
		}
	}

	if (mDepthAttachmentSpecs.textureFormat != FramebufferTextureFormat::None) {
		Utils::CreateTextures(multisample, &mDepthAttachment, 1);
		Utils::BindTexture(multisample, mDepthAttachment);
		switch (mDepthAttachmentSpecs.textureFormat)
		{
		case FramebufferTextureFormat::Depth24Stencil8:
			Utils::AttachDepthTexture(mDepthAttachment, mSpecs.samples,
									  GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, mSpecs.width, mSpecs.height);
			break;
		}
	}

	if (std::size(mColorAttachments) > 1) {
		assert(std::size(mColorAttachments) <= 4 && "GL Framebuffer Color attachments greater than 4");
		constexpr GLenum buffers[4] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3
		};
		glDrawBuffers((GLsizei)std::size(mColorAttachments), std::data(buffers));
	} else if (std::empty(mColorAttachments)) {
		glDrawBuffer(GL_NONE);
	}

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Framebuffer is incomplete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, mId);
	glViewport(0, 0, mSpecs.width, mSpecs.height);
}

void GLFramebuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::Resize(const uint32 width, const uint32 height) {
	if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
		LOG_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
		return;
	}

	mSpecs.width = width;
	mSpecs.height = height;

	Invalidate();
}

int32 GLFramebuffer::ReadPixel(const uint32 attachmentIndex, const int32 x, const int32 y) {
	assert(attachmentIndex < std::size(mColorAttachments) && "attachmentIndex is out of bounds for color attachments");

	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int pixelData = 0;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}

void GLFramebuffer::ClearAttachment(const uint32 attachmentIndex, const int32 value) {
	assert(attachmentIndex < std::size(mColorAttachments) && "attachmentIndex is out of bounds for color attachments");

	auto &spec = mColorAttachmentSpecs[attachmentIndex];
	glClearTexImage(mColorAttachments[attachmentIndex], 0,
					Utils::FramebufferTextureFormatToGL(spec.textureFormat), GL_INT, &value);
}

Ref<GLFramebuffer> GLFramebuffer::Create(const FramebufferSpecs &specs) {
	return CreateRef<GLFramebuffer>(specs);
}
