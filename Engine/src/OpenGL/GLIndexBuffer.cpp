#include "OpenGL/GLIndexBuffer.h"

#include <glad/glad.h>

GLIndexBuffer::GLIndexBuffer(const uint16 *const indices, const uint32 count) : mCount(count) {
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * count, indices, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer() {
	glDeleteBuffers(1, &mId);
}

void GLIndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
}

void GLIndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Ref<GLIndexBuffer> GLIndexBuffer::Create(const uint16 *const indices, const uint32 count) {
	return CreateRef<GLIndexBuffer>(indices, count);
}
