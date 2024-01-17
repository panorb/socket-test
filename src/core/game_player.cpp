#include "game_player.hpp"

Player::~Player()
{
	UnloadTexture(texture_);
}

void Player::Init() {
	texture_ = LoadTexture("assets/Characters/green_character.png");
	source_rect_ = { 0, 0, static_cast<float>(texture_.width), static_cast<float>(texture_.height) };
}

void Player::Update()
{
	HandleInput();
}

void Player::HandleInput() {
	IsKeyDown(KEY_W) ? position_.y -= 30.0f : 0;
	IsKeyDown(KEY_S) ? position_.y += 30.0f : 0;
	IsKeyDown(KEY_A) ? position_.x -= 30.0f : 0;
	IsKeyDown(KEY_D) ? position_.x += 30.0f : 0;
}

void Player::Render()
{
	Rectangle dest_rect = { position_.x, position_.y, static_cast<float>(texture_.width), static_cast<float>(texture_.height) };
	DrawTexturePro(texture_, source_rect_, dest_rect, Vector2{0.0, 0.0}, 0.0, RAYWHITE);
}

void Player::Write()
{
}

void Player::Read()
{
}
