#include "game_core.hpp"
#include "game_player.hpp"

const double MS_PER_UPDATE = 1000.0 / 25.0;

constexpr int screenWidth = 800;
constexpr int screenHeight = 450;
size_t updateFrame = 0;

Player player;

int MainLoop() {
	InitWindow(screenWidth, screenHeight, "Socket Test Game");
	SetTargetFPS(60);
	SetWindowState(FLAG_VSYNC_HINT);

	player.Init();

	double previous = GetTime() * 1000;
	double lag = 0.0;

	while (!WindowShouldClose()) {
		double current = GetTime() * 1000;
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		while (lag >= MS_PER_UPDATE) {
			Update();
			
			lag -= MS_PER_UPDATE;
		}

		Render();
	}

	CloseWindow();
	return 0;
}

void Update() {
	spdlog::info("Update");
	player.Update();

	updateFrame = (++updateFrame) % 1000;
}

void Render() {
	spdlog::info("Render");

	BeginDrawing();
	ClearBackground(RAYWHITE);
	player.Render();
	DrawFPS(10, 10);

	
	std::string updateFrameNum = "Update Frame: ";
	updateFrameNum += std::to_string(updateFrame);

	DrawText(updateFrameNum.c_str(), 10, 30, 20, BLACK);
	EndDrawing();
}
