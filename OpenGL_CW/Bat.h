#pragma once
#ifndef BAT_H
#define BAT_H

#include "Vertex.h"
#include "Enemy.h"
#include "freeglut.h"

class Bat : public Enemy {

public:

	int health;
	bool dead;

	// Constructor 
	Bat(Vertex pos, int w, int h);

	void fly(float deltaTime, GameObject &player);
	void takeDamage();
};

inline void Bat::fly(float deltaTime, GameObject &player) {
	
	if (!dead) {

		// If left of player
		if (position.xPos < player.position.xPos) {
			position.xPos += 0.01 * moveSpeed * deltaTime;
		}
		// If right of player
		else {
			position.xPos -= 0.01 * moveSpeed * deltaTime;
		}

		// If bellow player
		if (position.yPos < player.position.yPos) {
			position.yPos += 0.01 * moveSpeed * deltaTime;
		}
		// If above player
		else {
			position.yPos -= 0.01 * moveSpeed * deltaTime;
		}
	}
	

	// Update sprites
	spriteCount += 0.01 * deltaTime;
	if (spriteCount >= moveSprites.size()) {
		spriteCount = 0;
	}
	activeSprite = moveSprites.at(int(spriteCount));
}

inline void Bat::takeDamage() {
	if (health > 0) {
		health--;
	}
	if (health == 0) {
		dead = true;
	}
}

inline Bat::Bat(Vertex pos, int w, int h) {
	position = pos;
	width = w;
	height = h;
	moveSpeed = 5;
	health = 2;
	dead = false;
}

#endif