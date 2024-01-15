#include "core/game_core.hpp"
#include <raylib.h>

int MainLoop() {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Socket Test Game");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		EndDrawing();
	}

	return 0;
}
