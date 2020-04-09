#include "Player.h"
#include "freeglut.h"

void Player::Render() {
	// Player object
	//glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
		glTranslatef(position.xPos , position.yPos, 0);
		glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, activeSprite);
			//glColor4f(0, 0, 0, 1);
			glBegin(GL_POLYGON);
			if (movingRight) {
				glTexCoord2f(0, 0); glVertex2f(-16, 0);
				glTexCoord2f(1, 0); glVertex2f(playerWidth+16 , 0);
				glTexCoord2f(1, 1); glVertex2f(playerWidth+16, playerHeight +8);
				glTexCoord2f(0, 1); glVertex2f(-16, playerHeight +8);
			}
			else {
				glTexCoord2f(1, 0); glVertex2f(-16, 0);
				glTexCoord2f(0, 0); glVertex2f(playerWidth+16, 0);
				glTexCoord2f(0, 1); glVertex2f(playerWidth+16, playerHeight+8);
				glTexCoord2f(1, 1); glVertex2f(-16, playerHeight+8);
			}
				
			glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

// Move right
// Increase xVel
void Player::moveRight(float deltaTime) {
	movingRight = true;
	// Set running sprites active
	if (onGround) {
		setSprites(runSprites);
	}
	if (xVel < 0) {
		xVel = -0.0001;
	}
	xVel += 0.001;
	if (xVel > maxVel) {
		xVel = maxVel;
	}

}

// Move left
// Decrease xVel
void Player::moveLeft(float deltaTime) {
	movingRight = false;
	// Set running sprites active
	if (onGround) {
		setSprites(runSprites);
	}
	
	if (xVel > 0) {
		xVel = 0.0001;
	}
	xVel -= 0.001;
	if (xVel < -maxVel) {
		xVel = -maxVel;
	}
}

// Applies xVel & yVel to players position
void Player::move(float deltaTime) {
	position.setXPos(position.xPos + xVel * moveSpeed * deltaTime);
	position.setYPos(position.yPos + yVel * deltaTime);

	// Keep player from falling through bottom of level
	if (position.yPos < 0) {
		position.yPos = 0;
	}

	// Set falling sprites
	if (!onGround && yVel < 0) {
		activeSprites = fallSprites;
	}

	// Reset idle sprites
	if (xVel == 0 && yVel ==0 && !isDucking && !attacking) {
		activeSprites = idleSprites;
	}

	// Update sprites
	spriteCount += 0.002 * deltaTime * moveSpeed;
	if (spriteCount >= activeSprites.size()) {
		if (attacking) {
			attacking = false;
		}
		spriteCount = 0;
	}
	activeSprite = activeSprites.at(int(spriteCount));

	// Invincibilty frames
	if (invCount > 0) {
		invCount -= 0.1;
	}
	else {
		invCount = 0;
		canTakeDamage = true;
	}

	// Die
	if (dead && playerHeight > 0) {
		playerHeight -= 0.1;
		playerWidth -= 0.1;
	}
}

// Jump
void Player::jump(float deltaTime) {

	setSprites(jumpSprites);
	onGround = false;
	yVel = 1;
}

