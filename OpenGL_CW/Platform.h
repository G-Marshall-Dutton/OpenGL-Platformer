#pragma once
#ifndef PLATFORM_H
#define PLATFORM_H

#include "GameObject.h"
#include "freeglut.h"

class Platform : public GameObject {

public:

	float speed = 7;
	GLuint activeSprite;

	// Constructor 
	Platform(Vertex pos, int w, int h);

	void render();

	void move(float delatTime);

	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
};

inline void Platform::render() {
	glPushMatrix();
		glTranslatef(position.xPos, position.yPos, 0);
		glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, activeSprite);
			glColor4f(0, 0, 0, 0);
			glBegin(GL_POLYGON);
				glTexCoord2f(0, 0); glVertex2f(-10, 0);
				glTexCoord2f(1, 0); glVertex2f(width+10, 0);
				glTexCoord2f(1, 1); glVertex2f(width+10, height+10);
				glTexCoord2f(0, 1); glVertex2f(-10, height+10);
			glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

inline void Platform::moveRight(float deltaTime) {
	position.xPos += 0.01 * speed * deltaTime;
}

inline void Platform::moveLeft(float deltaTime) {
	position.xPos -= 0.01 * speed * deltaTime;
}

inline void Platform::move(float deltaTime) {
	if (movingRight) {
		position.xPos += 0.01 * speed * deltaTime;
	}
	else{
		position.xPos -= 0.01 * speed *deltaTime;
	}
}

inline Platform::Platform(Vertex pos, int w, int h) {
	position = pos;
	width = w;
	height = h;

}


#endif