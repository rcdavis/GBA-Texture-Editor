#pragma once

#include <filesystem>
#include <string>

namespace FileUtils {
	std::string ReadText(const std::filesystem::path &filepath);
}
