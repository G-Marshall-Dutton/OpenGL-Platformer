#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "freeglut.h"

class Enemy : public GameObject {
public:

	int moveSpeed = 12;

	GLuint activeSprite;
	float spriteCount = 0;
	std::vector<GLuint> moveSprites;


	//Constructor
	Enemy();
	Enemy(Vertex pos, int w, int h);

	//Render
	void render();

	// Move 
	void move(float deltaTime);
};

inline void Enemy::render() {
	glPushMatrix();
		glTranslatef(position.xPos, position.yPos, 0);
		glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, activeSprite);
			glBegin(GL_POLYGON);
			if (!movingRight) {
				glTexCoord2f(0, 0); glVertex2f(-4, 0);
				glTexCoord2f(1, 0); glVertex2f(width + 4, 0);
				glTexCoord2f(1, 1); glVertex2f(width + 4, height +16);
				glTexCoord2f(0, 1); glVertex2f(-4, height +16);
			}
			else {
				glTexCoord2f(1, 0); glVertex2f(-4, 0);
				glTexCoord2f(0, 0); glVertex2f(width+4, 0);
				glTexCoord2f(0, 1); glVertex2f(width+4, height+16);
				glTexCoord2f(1, 1); glVertex2f(-4, height+16);
			}
				
			glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

inline void Enemy::move(float deltaTime) {
	if (movingRight) {
		position.xPos += 0.01 * moveSpeed * deltaTime;
	}
	else {
		position.xPos -= 0.01 * moveSpeed * deltaTime;
	}

	// Update sprites
	spriteCount += 0.01 * deltaTime;
	if (spriteCount >= moveSprites.size()) {
		spriteCount = 0;
	}
	activeSprite = moveSprites.at(int(spriteCount));
}

inline Enemy::Enemy(Vertex pos, int w, int h) {
	position = pos;
	width = w;
	height = h;
}

inline Enemy::Enemy() {
	position = Vertex();
	width = 0;
	height = 0;
}

#endif