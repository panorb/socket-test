#pragma once

#include <raylib.h>

struct Player {
	~Player();

	Texture2D texture;
	Rectangle source_rect;

	Vector2 position;

	void Init();
	void Update();
	void Render();
	void Write();
	void Read();

private:
	void HandleInput();
};
