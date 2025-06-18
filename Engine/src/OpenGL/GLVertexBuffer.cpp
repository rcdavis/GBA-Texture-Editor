#include "OpenGL/GLVertexBuffer.h"

#include <glad/glad.h>

GLVertexBuffer::GLVertexBuffer(uint32 size) {
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

GLVertexBuffer::GLVertexBuffer(const f32 *const vertices, const uint32 count) {
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * count, vertices, GL_STATIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer() {
	glDeleteBuffers(1, &mId);
}

void GLVertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, mId);
}

void GLVertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::SetData(const void *data, const uint32 size) {
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

Ref<GLVertexBuffer> GLVertexBuffer::Create(uint32 size) {
	return CreateRef<GLVertexBuffer>(size);
}

Ref<GLVertexBuffer> GLVertexBuffer::Create(const f32 *const vertices, uint32 count) {
	return CreateRef<GLVertexBuffer>(vertices, count);
}
