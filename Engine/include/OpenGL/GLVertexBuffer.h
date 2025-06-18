#pragma once

#include "OpenGL/GLBufferLayout.h"

class GLVertexBuffer {
public:
	GLVertexBuffer(uint32 size);
	GLVertexBuffer(const f32 *const vertices, const uint32 count);
	~GLVertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void *data, const uint32 size);

	const GLBufferLayout &GetLayout() const { return mLayout; }
	void SetLayout(const GLBufferLayout &layout) { mLayout = layout; }

	static Ref<GLVertexBuffer> Create(uint32 size);
	static Ref<GLVertexBuffer> Create(const f32 *const vertices, uint32 count);

private:
	uint32 mId = 0;
	GLBufferLayout mLayout;
};
