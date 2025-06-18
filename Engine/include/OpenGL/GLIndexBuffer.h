#pragma once

#include "Core.h"

class GLIndexBuffer {
public:
	GLIndexBuffer(const uint16 *const indices, const uint32 count);
	~GLIndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32 GetCount() const { return mCount; }

	static Ref<GLIndexBuffer> Create(const uint16 *const indices, const uint32 count);

private:
	uint32 mId = 0;
	uint32 mCount = 0;
};
