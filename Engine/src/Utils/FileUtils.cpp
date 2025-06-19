#include "Utils/FileUtils.h"

#include "Log.h"

#include <fstream>

namespace FileUtils {
	std::string ReadText(const std::filesystem::path &filepath) {
		std::ifstream file(filepath, std::ios::in);
		if (!file) {
			LOG_ERROR("Failed to open file {0}", filepath.string());
			return "";
		}

		std::vector<char> buffer(std::filesystem::file_size(filepath));
		file.read(std::data(buffer), std::size(buffer));
		file.close();

		return std::data(buffer);
	}
}
