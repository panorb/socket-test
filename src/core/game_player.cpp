#include "game_player.hpp"

Player::~Player()
{
	UnloadTexture(texture);
}

void Player::Init() {
	texture = LoadTexture("assets/Characters/green_character.png");
	source_rect = { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}

void Player::Update()
{
	HandleInput();
}

void Player::HandleInput() {
	IsKeyDown(KEY_W) ? position.y -= 30.0f : 0;
	IsKeyDown(KEY_S) ? position.y += 30.0f : 0;
	IsKeyDown(KEY_A) ? position.x -= 30.0f : 0;
	IsKeyDown(KEY_D) ? position.x += 30.0f : 0;
}

void Player::Render()
{
	Rectangle dest_rect = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
	DrawTexturePro(texture, source_rect, dest_rect, Vector2{0.0, 0.0}, 0.0, RAYWHITE);
}

void Player::Write()
{
}

void Player::Read()
{
}
