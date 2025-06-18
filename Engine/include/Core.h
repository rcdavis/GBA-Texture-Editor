#pragma once

#include <memory>

typedef unsigned long long uint64;
typedef long long int64;

typedef unsigned int uint32;
typedef int int32;

typedef unsigned short uint16;
typedef short int16;

typedef unsigned char uint8;
typedef char int8;

typedef float f32;
typedef double f64;

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Scope<T> CreateScope(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

template <typename T, typename... Args>
Ref<T> CreateRef(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

template <typename T, typename U>
Ref<T> DynamicCastRef(const Ref<U>& r) { return std::dynamic_pointer_cast<T>(r); }
