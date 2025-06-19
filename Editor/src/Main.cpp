
#include "Log.h"
#include "Application.h"

int main() {
	Log::Init();

	LOG_INFO("GBA Texture Editor started...");

	Application app;
	app.Run();

	LOG_INFO("GBA Texture Editor stopped...");

	return 0;
}
