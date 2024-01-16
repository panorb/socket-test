#include "core/game_core.hpp"
#include <raylib.h>

int MainLoop() {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Socket Test Game");

	Texture2D player = LoadTexture("assets/Characters/green_character.png");
	
	Rectangle sourceRect{ 0.0f, 0.0f, player.width, player.height };
	Rectangle destRect{ screenWidth / 2.0f, screenHeight / 2.0f, player.width, player.height };
	Vector2 origin = { player.width / 2.0f, player.height / 2.0f };

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTexturePro(player, sourceRect, destRect, origin, 0.0, RAYWHITE );
		EndDrawing();
	}

	UnloadTexture(player);
	return 0;
}
