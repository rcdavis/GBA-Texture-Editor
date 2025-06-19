#pragma once

#include "Core.h"

#include <vector>

class GLVertexBuffer;
class GLIndexBuffer;

class GLVertexArray {
public:
	GLVertexArray();
	~GLVertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const Ref<GLVertexBuffer> &vertexBuffer);
	void SetIndexBuffer(const Ref<GLIndexBuffer> &indexBuffer);

	const std::vector<Ref<GLVertexBuffer>> &GetVertexBuffers() const { return mVertexBuffers; }
	const Ref<GLIndexBuffer> &GetIndexBuffer() const { return mIndexBuffer; }

	static Ref<GLVertexArray> Create();

private:
	uint32 mId = 0;
	uint32 mVertexBufferIndex = 0;
	std::vector<Ref<GLVertexBuffer>> mVertexBuffers;
	Ref<GLIndexBuffer> mIndexBuffer;
};
