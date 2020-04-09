#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"



class Player : public GameObject {


public:
	float playerHeight = 28;
	float playerWidth = 30;
	float moveSpeed = 5;
	float jumpHangCount = 0;
	float jumpHangTime = 5;

	float xVel = 0;
	float yVel = 0;
	float maxVel = 0.13;
	float dropRate = 1.42;

	int duckHeight = playerHeight /1.5;
	int slideWidth = playerHeight;

	int standHeight = playerHeight;
	int standWidth = playerWidth;

	bool isDucking = false;
	bool onGround = true;
	bool canJump = true;
	bool attacking = false;

	bool dead = false;
	bool canTakeDamage = true;
	float invFrames = 5;
	float invCount = 0;
	int health = 3;

	GLuint activeSprite;
	float spriteCount = 0;
	std::vector<GLuint> activeSprites;
	std::vector<GLuint> runSprites;
	std::vector<GLuint> idleSprites;
	std::vector<GLuint> jumpSprites;
	std::vector<GLuint> fallSprites;
	std::vector<GLuint> slideSprites;
	std::vector<GLuint> crouchSprites;
	std::vector<GLuint> attackSprites;


	// Constructor
	Player(Vertex position);

	// Render
	void Render();

	// Move
	void move(float deltaTime);

	// Move right
	void moveRight(float deltaTime);

	// Move left
	void moveLeft(float deltaTime);

	// Jump
	void jump(float deltaTime);

	// Duck
	void duck();

	// Stand
	void stand();

	// Run
	void run();

	// Walk
	void walk();

	// Change sprite set
	void setSprites(std::vector<GLuint> sprites);

	// Take damage
	void takeDamage();

	// Attack
	void attack();


};

inline void Player::run() {
	moveSpeed = 7;
}

inline void Player::walk() {
	moveSpeed = 5;
}

// Duck 
inline void Player::duck() {
	playerHeight = duckHeight;
	// Slide
	if (xVel != 0) {
		playerWidth = slideWidth;
		activeSprites = slideSprites;
	}
	else {
		std::cout << "crouching" << std::endl;
		activeSprites = crouchSprites;
	}
	isDucking = true;
}

// Stand 
inline void Player::stand() {
	playerHeight = standHeight;
	playerWidth = standWidth;
	isDucking = false;
}

// Change sprite set
inline void Player::setSprites(std::vector<GLuint> sprites) {
	activeSprites = sprites;
}

// Take damage
inline void Player::takeDamage() {
	if (canTakeDamage) {

		canTakeDamage = false;
		invCount = invFrames;

		if (health > 0) {
			health--;
		}
		if (health == 0) {
			dead = true;
		}
	}
	
}

inline void Player::attack() {
	attacking = true;
	activeSprites = attackSprites;
}

// Constructor
inline Player::Player(Vertex pos)
{
	width = playerWidth;
	height = playerHeight;
	position = pos;

}

#endif