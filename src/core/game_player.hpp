#pragma once

#include <raylib.h>

#include "game_object.hpp"

struct Player : public GameObject {
	~Player();

	Texture2D texture_;
	Rectangle source_rect_;

	Vector2 position_;

	void Init();
	void Update();
	void Render();
	void Write();
	void Read();

private:
	void HandleInput();
};
