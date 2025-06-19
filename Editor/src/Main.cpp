
#include "Log.h"
#include "Application.h"

int main() {
	Log::Init();

	LOG_INFO("GBA Texture Editor started...");

	Application::Get().Run();

	LOG_INFO("GBA Texture Editor stopped...");

	return 0;
}
