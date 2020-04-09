
//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle


#include <iostream>
#include <Vector>

#include "Image_Loading/nvImage.h"
#include "freeglut.h"
#include <math.h>
#include <string>
#include "Player.h"
#include "Vertex.h"
#include "Platform.h"
#include "Enemy.h"
#include "Bat.h"



int gameState;

int	mouse_x = 0, mouse_y = 0;
bool LeftPressed = false;
int screenWidth = 960, screenHeight = 540;
int levelWidth = screenWidth * 2;
int levelHeight = screenHeight * 2;
bool keys[256];
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraWidth = screenWidth;
float cameraHeight = screenHeight;

float deltaTime = 0;
float end = 0;
float pauseFactor = 1;

float gravity = 0.008;
float friction = 0.00013;

bool showGrid = false;
bool devMode = false;

int tileWidth = 30;
int tileHeight = tileWidth;

int tilesOnScreenX = screenWidth / tileWidth;
int tilesOnScreenY = screenHeight / tileHeight;

bool ranOutofTime = false;
int timeLimit = 120;
int elapsedTime;
int currentGameTime;
int gameStartTime;
int score;
// Player object
Player player(Vertex(cameraWidth/2, 0));


// Initialise moving platforms
// (Before doing this, should look through the level map, grab positions of each platform)
Platform block(Vertex((cameraWidth / 2) - 10, 90), 90, 30);
Platform block2(Vertex(1140, 600), 90, 30);
Platform block3(Vertex(960, 600), 90, 30);

// Moving platforms
std::vector<Platform*> platforms;

// Initialise enemys
// Slime
Enemy enemy1(Vertex((levelWidth / 2) + 60, 120), 30, 30);
Enemy enemy2(Vertex(210, 240), 40, 30);
Enemy enemy3(Vertex((levelWidth / 2) + 30, 360), 30, 30);
Enemy enemy4(Vertex(1530, 900), 30, 30);
Enemy enemy5(Vertex(1230, 810), 30, 30);

// Bat
Bat* bat1 = new Bat(Vertex((levelWidth / 2) + 60, 120), 20, 10);
Bat* bat2 = new Bat(Vertex((levelWidth / 2) + 600, 120), 20, 10);
Bat* bat3 = new Bat(Vertex(levelWidth - 100, levelHeight -100), 20, 10);

// Slimes
std::vector<Enemy*> enemies;
// Bats
std::vector<Bat*> bats;

// Level textures
GLuint startScreenTexture;
GLuint groundTexture;
GLuint backgroundTexture;
GLuint platformTexture;

// Health textures
GLuint fullHeartTexture;
GLuint emptyHeartTexture;

// Spike texture
GLuint spikeTexture;

// Crate texture
GLuint crateTexture;

// 64x36 tile grid
std::string level[36] = 
{ 
	"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
	"GGGGGGG.........................................................",
	"GGGGGGG.........................................................",
	"G.....G.........................................................",
	"......G.........................................................",
	"G.....G.........................................................",
	"......G.........................................................",
	"G.....G.........................................................",
	"......G.........................................................",
	"G...GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG......GGG",
	".....................G.G...................................GG...",
	"G....................G.G.................................GG.....",
	".....................G.G...............................GG.......",
	"G....................G.G..........G..................GG.........",
	".....................G.G........GG.GG..............GG...........",
	"G.............................GG.....GG..GGGGGGGGGG.............",
	"........GGGGGGG..............G..................................",
	"GGG............................G.................GGG..........GG",
	"GGGGGGG..GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
	"GGGGGGG.........................................................",
	"GGGGGGGG........................................................",
	"G......GG.......................................................",
	"........GG......................................................",
	"G........GG.....................................................",
	"..........GG....................................................",
	"G..........GGG..................................................",
	"................................................................",
	"G.....GGGGGGGGGGGGGGGG.GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG......GGG",
	".....................G.G...................................GG...",
	"GG...................G.G.................................GG.....",
	".....................G.G...............................GG.......",
	"....GGGG.............G.G..........G..................GG.........",
	".....................G.G........GG.GG..............GG...........",
	"G.............................GG.....GG..GGGGGGGGGG.............",
	"........GGGGGGG..............G..................................",
	"GGG...........................................................GG",
};


std::string level2[36] =
{
	"GGGGGGGG...................GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
	"GGGGGGGGGGGG............EEEGGGGGGG..........................GGGG",
	"GGGGGGGGGGGGGGGGG.......EEEGGGGGGG..........................GGGG",
	"GGG.............G..GGGGGGGGGGGGGGG...........GGGGGGGGG......GGGG",
	"G...............G..........G.................G..............GGGG",
	"G.....GG........G..........G.................G..T.........T.GGGG",
	".....GGG........GGGGGG.....G.......GG........G...GGGGGGGGGGGGGGG",
	"G......G........GG.....GGGGG.................G...GGG........GGGG",
	".......G........GG...........................G..............GGGG",
	"G...GGGGGGGGG...GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG.......GGGG",
	"............G...........G..................................GG...",
	"G...........G...........G................................GG.....",
	"............G...........G..............................GG.......",
	"GG..........G...........GC...........................GG.........",
	"............GC..........GCC........................GG...........",
	"............GCC.........GGGGGG..........G..........T............",
	"........GGGGGGGGG............G..................................",
	".............................GGGGG..............................",
	"GG...........................G..................................",
	".............................G...C..............................",
	".............................G...GGGG...........................",
	".............................G.................GG...............",
	"...GG........................GC.................................",
	"...................GGGGGGG...GCC....T.....G.........G...........",
	".......................GGG...GGGGGGG......G.....................",
	"GG.....................GGG................G.............GG......",
	".......................GGG................G..............GG.....",
	"..TCC..........T.......GGG...............GGSSSSSSS..............",
	"...GGGGGGGGGGGG........GGG................GGGGGGGG..............",
	".......................GGG....................GGG...............",
	".......................GGG.....T..........T...GGG...........GG..",
	".......................GGG.....T..........T...GGG..C............",
	"...........T........T..GGG......GGGGGGGGGG....GGG.CC...T........",
	"G......................GGG....GGGGGGGGGGGG....GGGGGGGGG.........",
	"GGG....................GGG....GGGGGGGGGGGG.....................C",
	"GGGSS..................GGGGGSSGGGGGGGGGGGG....................GG",
};



//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void special(int key, int x, int y);
void update();				//called in winmain to update variables

void drawGrid();
void checkKeys();

void drawLevel(std::string level[]);
void drawPlatforms(std::vector<GameObject> platforms);
void drawPlayerHealth();
void drawText(std::string time, float x, float y);
void drawPath(GameObject a, GameObject b);
void drawHitBox(GameObject a);

GLuint loadPNG(char* name);

bool pointToPoint(GameObject a, GameObject b);
bool pointToCircle(GameObject a, GameObject b, float r);
bool rectToRect(GameObject a, GameObject b);

void checkStaticTileCollisions(std::string level[]);
void checkPlatformCollisions(Platform &block);
void checkNonPlayerBounds(GameObject &platform, std::string level[]);
void checkEnemyPlayerCollisions(Enemy &enemy);
void checkBatPlayerCollisions(Bat &enemy);

void loadTextures();
void drawBackground();


							/*************    START OF OPENGL FUNCTIONS   ****************/
void display()
{	

	float start = glutGet(GLUT_ELAPSED_TIME);
	currentGameTime = start;
	deltaTime = (start - end) * pauseFactor;
	end = start;

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();



	// Start screen
	if (gameState == 0) {
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, startScreenTexture);
			glColor4f(0, 0, 0, 0);
			glBegin(GL_POLYGON);
				glTexCoord2f(0, 0); glVertex2f(0, 0);
				glTexCoord2f(1, 0); glVertex2f(cameraWidth, 0);
				glTexCoord2f(1, 1); glVertex2f(cameraWidth , cameraHeight);
				glTexCoord2f(0, 1); glVertex2f(0, cameraHeight);
			glEnd();
		glPopMatrix();
	}
	// Play game
	else if (gameState == 1) {

		// Draw level
		drawLevel(level2);

		// Draw refference grid
		if (showGrid) {
			drawGrid();
		}

		// Draw timer
		drawText(std::to_string(timeLimit - elapsedTime), cameraX + (cameraWidth * 0.9), cameraY + (cameraHeight * 0.9));
		

		// Draw moving platforms
		for each (Platform* platform in platforms)
		{
			platform->render();
			if (devMode) {
				drawHitBox(*platform);
			}
			
		}

		// Draw enemys
		// Slimes
		for each (Enemy* enemy in enemies)
		{
			enemy->render();
			if (devMode) {
				drawHitBox(*enemy);
			}
			
		}

	    // Bats
		for each (Bat* bat in bats)
		{	
			// Draw bat
			if (!bat->dead) {
				bat->render();
			}
			// Draw path & hit box
			if (devMode) {
				drawPath(player, *bat);
				drawHitBox(*bat);
			}

		}

	

		// draw player 
		player.Render();
		if (devMode) {
			drawHitBox(player);
		}

		drawPlayerHealth();
	}
	// End screen
	else if (gameState == 2) {

		cameraX = 0;
		cameraY = 0;

		// calculate score 
		score = (timeLimit - elapsedTime) + (player.health * 20);

		if (ranOutofTime) {
			drawText("YOU RAN OUT OF TIME", cameraWidth / 2 - 100, cameraHeight / 2 + 60);
		}
		else if (player.dead) {
			drawText("YOU DIED", cameraWidth / 2 - 100, cameraHeight / 2 + 60);
		}
		else {
			drawText("YOU WON", cameraWidth / 2 - 100, cameraHeight / 2 + 60);
			drawText("Score: ", cameraWidth / 2 - 100, cameraHeight / 2 - 60);
			drawText(std::to_string(score), cameraWidth / 2 - 30, cameraHeight / 2 - 60);
		}
		drawText("It took you ", cameraWidth / 2 - 100, cameraHeight / 2);
		drawText(std::to_string(elapsedTime), cameraWidth/2, cameraHeight/2 );
		drawText("seconds", cameraWidth / 2 + 30, cameraHeight / 2);
		
	}

	glutSwapBuffers();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;           // to ensure the mouse coordinates match 


	glViewport(0, 0, screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();

	gluOrtho2D(cameraX, cameraX + cameraWidth, cameraY, cameraY + cameraHeight);           // set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void init()
{
	gameState = 0;

	platforms.push_back(&block);
	platforms.push_back(&block2);
	platforms.push_back(&block3);

	enemies.push_back(&enemy1);
	enemies.push_back(&enemy2);
	enemies.push_back(&enemy3);
	enemies.push_back(&enemy4);
	enemies.push_back(&enemy5);
	
	bats.push_back(bat1);
	bats.push_back(bat2);
	bats.push_back(bat3);

	loadTextures();

	glClearColor(0.2, 0.2, 0.2, 0.0);					//sets the clear colour to yellow
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();

	gluOrtho2D(0, cameraWidth, 0, cameraHeight);           // set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void keyDown(unsigned char key, int x, int y){
	//std::cout << key << " Down" << std::endl;
	keys[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
	//std::cout << key << " up" << std::endl;
	keys[key] = false;
}

void checkKeys() {

	// Start screen
	if (gameState == 0) {
		if (keys[' ']) {
			gameStartTime = glutGet(GLUT_ELAPSED_TIME);
			gameState++;
		}
	}
	// Play game
	else if (gameState == 1 && !player.dead) {
		// Move left
		if (keys['a'] && !keys['d']) {
			if (!player.isDucking) {
				player.moveLeft(deltaTime);
			}
		}

		// Move right
		if (keys['d'] && !keys['a']) {
			if (!player.isDucking) {
				player.moveRight(deltaTime);
			}
		}

		if (keys['g']) {
			std::cout << "POSITION: " << player.position << std::endl;
			std::cout << "WIDTH: " << player.width << std::endl;
			std::cout << "HEIGHT: " << player.height << std::endl;
			std::cout << "xVel: " << player.xVel << std::endl;
			std::cout << "yVel: " << player.yVel << std::endl;
			std::cout << "onGround: " << player.onGround << std::endl;
			std::cout << "canJump: " << player.canJump << std::endl;
			std::cout << "MoveSpeed: " << player.moveSpeed << std::endl;
			std::cout << "Health: " << player.health << std::endl;
			std::cout << "Dead: " << player.dead << std::endl;
			std::cout << "Moving right: " << player.movingRight << std::endl;
			std::cout << std::endl;
		}

		// JUMP
		if (keys['w'] && player.onGround && player.canJump) {
			player.canJump = false;
			player.jump(deltaTime);
		}
		else if (!keys['w']) {
			player.canJump = true;
		}

		// DUCK
		if (keys['s'] && player.onGround) {
			player.duck();
		}
		else {
			player.stand();
		}

		// RUN
		if (keys[' '] && player.onGround) {
			//moveSpeed = 7;
			player.run();
		}
		else {
			//player.walk();
		}

		// ATTACK
		if (keys['l']) {
			player.attack();
		}


		// Pause
		if (keys['p']) {
			pauseFactor = 0;
		}
		// Reset to normal speed
		if (keys['o']) {
			pauseFactor = 1;
		}
		// Half speed
		if (keys['[']) {
			pauseFactor = 0.5;
		}

		// DEV MODE
		if (keys['=']) {
			devMode = !devMode;
			showGrid = !showGrid;
		}

	}
	// End screen
	else if (gameState == 2) {

	}

	// Change game state -- (JUST FOR TESTING)
	if (keys['c']) {
		gameState += 1;
		if (gameState == 3) {
			gameState = 0;
		}
		std::cout << gameState << std::endl;
	}

	// Show grid
	if (keys['i']) {
		showGrid = !showGrid;
	}
	

}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:

		break;
	case GLUT_KEY_RIGHT:

		break;
	case GLUT_KEY_UP:

		break;
	case GLUT_KEY_DOWN:

		break;
	}
}

void mouseMotion(int x, int y)
{
	mouse_y = screenHeight - y;
	mouse_x = x;
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			LeftPressed = true;
		}
		else
			LeftPressed = false;
		break;
	case GLUT_RIGHT_BUTTON:

		break;
	default:
		break;
	}
}

void update()
{	
	

	// Check player input 
	checkKeys();

	// Start screen
	if (gameState == 0) {

	}
	// Play game
	else if (gameState == 1) {


		// End if player is dead
		if (player.dead && player.playerHeight <= 0) {
			gameState++;
		}

		// Get elapsed time
		elapsedTime = (currentGameTime - gameStartTime) / 1000;

		// If timer has run out
		if (timeLimit - elapsedTime <= 0) {
			ranOutofTime = true;
			gameState++;
		}

		// MOVEMENT --------------------------------------------------------

		// move player
		player.move(deltaTime);

		// Move platforms
		for each (Platform* platform in platforms)
		{
			platform->move(deltaTime);
		}

		// Move enemies
		for each (Enemy* enemy in enemies)
		{
			enemy->move(deltaTime);
		}

		// move bats if within agro range
		for each (Bat* bat in bats)
		{
			if (pointToCircle(player, *bat, cameraHeight *1.5)) {
				bat->fly(deltaTime, player);
			}
			
		}

		// Remove dead bats
		for (int i = 0; i < bats.size(); i++)
		{
			if (bats.at(i)->dead) {
				delete bats.at(i);
				bats.erase(bats.begin() +i);
			}
		}

		// PHYSICS ----------------------------------------------------------

		// Apply gravity
		if (player.position.yPos > 0) {
			//reduce gravity for hang time
			if (player.yVel > 0 && keys['w']) {
				gravity = 0.0035;
			}
			else if (player.yVel > 0) {
				gravity = 0.006;
			}
			else {
				gravity = 0.008;
			}
			player.yVel -= gravity * deltaTime;
			if (player.yVel < -player.dropRate) {
				player.yVel = -player.dropRate;
			}
		}
		if (player.position.yPos == 0) {
			player.onGround = true;
			player.yVel = 0;
		}

		// Apply friction when sliding
		if (player.isDucking) {
			if (player.xVel > 0) {
				player.xVel -= friction * deltaTime;
				// ensure xVel stops at 0
				if (player.xVel < 0) {
					player.xVel = 0;
				}
			}
			else if (player.xVel < 0) {
				player.xVel += friction *deltaTime;
				// ensure xVel stops at 0
				if (player.xVel > 0) {
					player.xVel = 0;
				}
			}
		}


		// if not trying to move left, right or sliding, apply friction 
		if (!keys['a'] && !keys['d'] && !player.isDucking && player.onGround) {
			if (player.xVel > 0) {
				player.xVel -= friction * 5 * deltaTime;
				// ensure xVel stops at 0
				if (player.xVel < 0) {
					player.xVel = 0;
				}
			}
			else if (player.xVel < 0) {
				player.xVel += friction * 5 * deltaTime;
				// ensure xVel stops at 0
				if (player.xVel > 0) {
					player.xVel = 0;
				}
			}
		}


		// Carry run momentum 
		if (player.moveSpeed > 5) {
			player.moveSpeed -= 0.01;
		}

		// CLAMP PLAYER & CAMERA --------------------------------------------------

		// Clamp player
		// Too far left
		if (player.position.xPos <= 0) {
			player.position.xPos = 0;
		}
		// Too far right
		if (player.position.xPos + player.width >= levelWidth - 0.111) {
			std::cout << "TOO FAR RIGHT" << std::endl;
			player.position.xPos = levelWidth - player.playerWidth - 0.111;
		}
		// Too far up
		if (player.position.yPos + player.height >= levelHeight - 1.111) {
			player.position.yPos = levelHeight - player.playerHeight - 1.111;
			player.yVel = -0.1;
		}

		// Move camera 
		cameraX = player.position.xPos - (cameraWidth / 2);
		cameraY = player.position.yPos - (cameraHeight / 2);

		// Clamp camera 
		// x-axis
		if (cameraX < 0) {
			cameraX = 0;
		}
		if (cameraX > levelWidth/2) {
			cameraX = levelWidth/2;
		}
		// y-axis
		if (cameraY < 0) {
			cameraY = 0;
		}
		if (cameraY > levelHeight/2) {
			cameraY = levelHeight/2;
		}


		// CHECK COLLISIONS ------------------------------------------------------

		// Check static collisions with player
		checkStaticTileCollisions(level2);


		// Check moving platform collisions with player
		for each (Platform* platform in platforms)
		{
			checkPlatformCollisions(*platform);
		}

		// Check moving platform collisions with map

		for each (Platform* platform in platforms)
		{
			checkNonPlayerBounds(*platform, level2);
		}

		// Check enemy collisions with the map
		for each (Enemy* enemy in enemies)
		{
			checkNonPlayerBounds(*enemy, level2);
		}

		// Check enemy player collisions
		for each (Enemy* enemy in enemies)
		{
			checkEnemyPlayerCollisions(*enemy);
		}

		// Check enemy player collisions
		for each (Bat* bat in bats)
		{
			checkEnemyPlayerCollisions(*bat);
		}

		for each (Bat* bat in bats)
		{
			checkBatPlayerCollisions(*bat);
		}

	}
	// End screen
	else if (gameState == 2) {

	}

	

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();

	gluOrtho2D(cameraX, cameraX + cameraWidth, cameraY, cameraY + cameraHeight);           // set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();


	glutPostRedisplay();
}

void drawGrid() {

	int lineDistance = tileWidth;

	glLoadIdentity();

	glBegin(GL_LINES);
	glColor3d(235, 0, 255); 
	for (float x = 0; x < 10000; x += lineDistance)
	{
		glVertex3f(x, 0.0f, 0.0f);
		glVertex3f(x, float(10000), 0.0f);
	}
	for (float y = 0; y < 10000; y += lineDistance)
	{
		glVertex3f(0.0f, y, 0.0f);
		glVertex3f(float(10000), y, 0.0f);
	}
	glEnd();
}

void drawBackground() {
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, backgroundTexture);
			glColor4f(0, 0, 0, 0);
			glBegin(GL_POLYGON);
				glTexCoord2f(0, 0); glVertex2f(0, 0);
				glTexCoord2f(1, 0); glVertex2f(levelWidth, 0);
				glTexCoord2f(1, 1); glVertex2f(levelWidth, levelHeight);
				glTexCoord2f(0, 1); glVertex2f(0, levelHeight);
			glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawLevel(std::string level[]) {

	drawBackground();

	int yCount = 0;
	for (int y = 35; y >= 0; y--)
	{
		for (int x = 0; x < 64; x++)
		{
			int xPos = x*tileWidth;
			int yPos = yCount*tileHeight;

			//std::cout << "x: " << x << ", yCount: " << yCount << std::endl;
			//std::cout << '(' << xPos << '/' << yPos << ')' << std::endl;


			//std::cout << level[y][x] << std::endl;

			if (level[y].at(x) == 'G') {
				//std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
				glPushMatrix();
					glTranslatef(xPos, yPos, 0);
					glEnable(GL_TEXTURE_2D);
						//glColor3f(1.0, 0.2, 0.4);
						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
						glBindTexture(GL_TEXTURE_2D, groundTexture);
						glColor4f(0, 0, 0, 0);
						glBegin(GL_POLYGON);
							glTexCoord2f(0, 0); glVertex2f(0, 0);
							glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
							glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
							glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
						glEnd();
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();
			}
			else if (level[y].at(x) == 'S') {
				glPushMatrix();
					glTranslatef(xPos, yPos, 0);
					glEnable(GL_TEXTURE_2D);
						//glColor3f(1.0, 0.2, 0.4);
						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
						glBindTexture(GL_TEXTURE_2D, spikeTexture);
						glColor4f(0, 0, 0, 0);
						glBegin(GL_POLYGON);
							glTexCoord2f(0, 0); glVertex2f(0, 0);
							glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
							glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
							glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
						glEnd();
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();
			}
			else if (level[y].at(x) == 'C') {
				glPushMatrix();
					glTranslatef(xPos, yPos, 0);
					glEnable(GL_TEXTURE_2D);
						//glColor3f(1.0, 0.2, 0.4);
						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
						glBindTexture(GL_TEXTURE_2D, crateTexture);
						glColor4f(0, 0, 0, 0);
						glBegin(GL_POLYGON);
							glTexCoord2f(0, 0); glVertex2f(0, 0);
							glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
							glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
							glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
						glEnd();
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();
			}
	
		}
		yCount++;
	}
}

//void drawPlatforms(std::vector<GameObject> platforms) {
//	for each (GameObject block in platforms)
//	{
//		// Draw block
//		glPushMatrix();
//			glTranslatef(block.position.xPos, block.position.yPos, 0);
//			glEnable(GL_TEXTURE_2D);
//				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//				glBindTexture(GL_TEXTURE_2D, block.activeSprite);
//				glColor4f(1, 0.5, 0, 0);
//				glBegin(GL_POLYGON);
//					glTexCoord2f(0, 0); glVertex2f(0, 0);
//					glTexCoord2f(1, 0); glVertex2f(block.width, 0);
//					glTexCoord2f(1, 1); glVertex2f(block.width, block.height);
//					glTexCoord2f(0, 1); glVertex2f(0, block.height);
//				glEnd();
//			glDisable(GL_TEXTURE_2D);
//		glPopMatrix();
//	}
//}

void drawPlayerHealth() {
	if (player.health > 2) {
		// right most
		// Draw full heart
		glPushMatrix();
			glTranslatef((tileWidth * 0.5) + cameraX + tileWidth *2, cameraY + cameraHeight - tileHeight*1.5, 0);
			glEnable(GL_TEXTURE_2D);
				//glColor3f(1.0, 0.2, 0.4);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glBindTexture(GL_TEXTURE_2D, fullHeartTexture);
				glColor4f(0, 0, 0, 0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0, 0); glVertex2f(0, 0);
					glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
					glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
					glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else {
		// Draw empty heart
		glPushMatrix();
			glTranslatef((tileWidth * 0.5) + cameraX + tileWidth *2, cameraY + cameraHeight - tileHeight*1.5, 0);
			glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glBindTexture(GL_TEXTURE_2D, emptyHeartTexture);
				glColor4f(0, 0, 0, 0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0, 0); glVertex2f(0, 0);
					glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
					glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
					glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (player.health > 1) {
		// middle
		// Draw full heart
		glPushMatrix();
			glTranslatef((tileWidth * 0.5) + cameraX + tileWidth, cameraY + cameraHeight - tileHeight*1.5, 0);
			glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glBindTexture(GL_TEXTURE_2D, fullHeartTexture);
				glColor4f(0, 0, 0, 0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0, 0); glVertex2f(0, 0);
					glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
					glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
					glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else {
		// Draw empty heart
		glPushMatrix();
			glTranslatef((tileWidth * 0.5) + cameraX + tileWidth, cameraY + cameraHeight - tileHeight*1.5, 0);
			glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glBindTexture(GL_TEXTURE_2D, emptyHeartTexture);
				glColor4f(0, 0, 0, 0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0, 0); glVertex2f(0, 0);
					glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
					glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
					glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	if (player.health > 0) {
		// Left most
		// Draw full heart
		glPushMatrix();
			glTranslatef((tileWidth * 0.5) + cameraX, cameraY + cameraHeight - tileHeight*1.5, 0);
			glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glBindTexture(GL_TEXTURE_2D, fullHeartTexture);
				glColor4f(0, 0, 0, 0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0, 0); glVertex2f(0, 0);
					glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
					glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
					glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
					glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else {
		// Draw empty heart
		glPushMatrix();
			glTranslatef((tileWidth * 0.5) + cameraX, cameraY + cameraHeight - tileHeight*1.5, 0);
			glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glBindTexture(GL_TEXTURE_2D, emptyHeartTexture);
				glColor4f(0, 0, 0, 0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0, 0); glVertex2f(0, 0);
					glTexCoord2f(1, 0); glVertex2f(tileWidth, 0);
					glTexCoord2f(1, 1); glVertex2f(tileWidth, tileHeight);
					glTexCoord2f(0, 1); glVertex2f(0, tileHeight);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
 
}

GLuint loadPNG(char* name)
{
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}
	else {
		std::cout << "ERROR LOADING PNG" << std::endl;
	}

	return myTextureID;
}

void loadTextures() {
	// Load player textures ----------------------------------------------
	// Idle
	player.idleSprites.push_back(loadPNG("sprites/adventurer-idle-2-00.png"));
	player.idleSprites.push_back(loadPNG("sprites/adventurer-idle-2-01.png"));
	player.idleSprites.push_back(loadPNG("sprites/adventurer-idle-2-02.png"));
	player.idleSprites.push_back(loadPNG("sprites/adventurer-idle-2-03.png"));

	// Running
	player.runSprites.push_back(loadPNG("sprites/adventurer-run3-00.png"));
	player.runSprites.push_back(loadPNG("sprites/adventurer-run3-01.png"));
	player.runSprites.push_back(loadPNG("sprites/adventurer-run3-02.png"));
	player.runSprites.push_back(loadPNG("sprites/adventurer-run3-03.png"));
	player.runSprites.push_back(loadPNG("sprites/adventurer-run3-04.png"));
	player.runSprites.push_back(loadPNG("sprites/adventurer-run3-05.png"));

	// Jumping
	player.jumpSprites.push_back(loadPNG("sprites/adventurer-jump-00.png"));
	player.jumpSprites.push_back(loadPNG("sprites/adventurer-jump-01.png"));
	player.jumpSprites.push_back(loadPNG("sprites/adventurer-jump-02.png"));
	player.jumpSprites.push_back(loadPNG("sprites/adventurer-jump-03.png"));

	// Falling
	player.fallSprites.push_back(loadPNG("sprites/adventurer-fall-00.png"));
	player.fallSprites.push_back(loadPNG("sprites/adventurer-fall-01.png"));

	// Sliding
	player.slideSprites.push_back(loadPNG("sprites/adventurer-slide-00.png"));
	player.slideSprites.push_back(loadPNG("sprites/adventurer-slide-01.png"));

	// Crouching
	player.crouchSprites.push_back(loadPNG("sprites/adventurer-crouch-00.png"));
	player.crouchSprites.push_back(loadPNG("sprites/adventurer-crouch-01.png"));

	// Attack
	player.attackSprites.push_back(loadPNG("sprites/adventurer-attack1-00.png"));
	player.attackSprites.push_back(loadPNG("sprites/adventurer-attack1-01.png"));
	player.attackSprites.push_back(loadPNG("sprites/adventurer-attack1-02.png"));
	player.attackSprites.push_back(loadPNG("sprites/adventurer-attack1-03.png"));
	player.attackSprites.push_back(loadPNG("sprites/adventurer-attack1-04.png"));


	//Set active sprite
	player.setSprites(player.idleSprites);
	player.activeSprite = player.activeSprites.at(0);

	// Health bar sprites 
	fullHeartTexture = loadPNG("sprites/fullHeart.png");
	emptyHeartTexture = loadPNG("sprites/emptyHeart.png");

	// Load level textures --------------------------------------------

	// Start screen
	startScreenTexture = loadPNG("sprites/startScreen.png");

	// Background
	backgroundTexture = loadPNG("sprites/Background.png");

	// Ground tiles
	groundTexture = loadPNG("sprites/ground.png");

	// Spikes
	spikeTexture = loadPNG("sprites/spikes.png");

	// Crates
	crateTexture = loadPNG("sprites/crate.png");

	// Platform tiles
	for each (Platform* platform in platforms)
	{
		platform->activeSprite = loadPNG("sprites/platform.png");
	}
	

	// Load enemy textures ----------------------------------------------

	// Slimes
	for each (Enemy* enemy in enemies)
	{
		enemy->moveSprites.push_back(loadPNG("sprites/slime-move-0.png"));
		enemy->moveSprites.push_back(loadPNG("sprites/slime-move-1.png"));
		enemy->moveSprites.push_back(loadPNG("sprites/slime-move-2.png"));
		enemy->moveSprites.push_back(loadPNG("sprites/slime-move-3.png"));
	}
	
	// Bats
	for each (Bat* bat in bats)
	{
		bat->moveSprites.push_back(loadPNG("sprites/bat_fly_01.png"));
		bat->moveSprites.push_back(loadPNG("sprites/bat_fly_02.png"));
		bat->moveSprites.push_back(loadPNG("sprites/bat_fly_03.png"));
		bat->moveSprites.push_back(loadPNG("sprites/bat_fly_04.png"));
		bat->moveSprites.push_back(loadPNG("sprites/bat_fly_05.png"));
	}


}

void drawText(std::string time, float x, float y) {

	glColor3f(1, 1, 1);
	glRasterPos2f(x, y);
	for(char c : time)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

}

void drawPath(GameObject a, GameObject b) {
	float xPosA = (a.position.xPos + (a.width / 2));
	float yPosA = (a.position.yPos + (a.height / 2));

	float xPosB = (b.position.xPos + (b.width / 2));
	float yPosB = (b.position.yPos + (b.height / 2));

	glPushMatrix();
		// Draw green line if within attack range
		if (pointToCircle(a, b, 45)) {
			glColor3f(0, 1, 0);
		}
		// Draw red line if within agro range
		else if (pointToCircle(a, b, cameraHeight*1.5)) {
			glColor3f(1, 0, 0);
		}
		// Draw white line
		else {
			glColor3f(1, 1, 1);
		}
		glBegin(GL_LINES);
			glVertex2f(xPosA,yPosA);
			glVertex2f(xPosB, yPosB);
		glEnd();
	glPopMatrix();
}

void drawHitBox(GameObject a) {
	glPushMatrix();
	glColor3f(1, 1, 1);
		glBegin(GL_LINE_LOOP);
			glVertex2f(a.position.xPos, a.position.yPos);
			glVertex2f(a.position.xPos + a.width, a.position.yPos);
			glVertex2f(a.position.xPos + a.width, a.position.yPos + a.height);
			glVertex2f(a.position.xPos, a.position.yPos + a.height);
		glEnd();
	glPopMatrix();
}

// !!! COLLISIONS !!!  -------------------------------------------

// Point to Point collision
// Returns true if point a and b are colliding
bool pointToPoint(GameObject a, GameObject b) {
	return (a.position.xPos == b.position.xPos && a.position.yPos == b.position.yPos);
}

// Point to Circle collision
// Returns true if point a and b are colliding
bool pointToCircle(GameObject a, GameObject b, float r) {
	// differnce in x pos from center of object
	float distX = (a.position.xPos + (a.width/2)) - (b.position.xPos + (b.width/2));
	float distY = (a.position.yPos + (a.height/2)) - (b.position.yPos + (b.height/2));
	float distance = (distX * distX) + (distY * distY);

	return (distance <= (r*r));
}

// Rectangle to rectangle collision
// Returns true if a and b are colliding
bool rectToRect(GameObject a, GameObject b) {
	if (a.position.xPos + a.width >= b.position.xPos &&     // r1 right edge past r2 left
		a.position.xPos <= b.position.xPos + b.width &&       // r1 left edge past r2 right
		a.position.yPos + a.height >= b.position.yPos &&       // r1 top edge past r2 bottom
		a.position.yPos <= b.position.yPos + b.height) {       // r1 bottom edge past r2 top
		return true;
	}
	return false;
}

void checkStaticTileCollisions(std::string level[]) {

	// If falling
	if (player.yVel < 0) {
		// Obtain player bottom left tile based coordinates
		int xPosBl = int((player.position.xPos + 4.999) / tileWidth);
		int yPosBl = int((player.position.yPos + 0.111) / tileHeight);

		// Obtain player bottom right tile based coordinates
		int xPosBr = int(((player.position.xPos + player.playerWidth) - 4.999) / tileWidth);
		int yPosBr = int((player.position.yPos + 0.111) / tileHeight);


		// Check collisions bottom left for ground
		if (level[35 - yPosBl].at(xPosBl) == 'G') {
			// Clamp player position
			player.position.yPos = int(player.position.yPos) + 1;
			player.yVel = 0;
			player.onGround = true;
			//std::cout << "COLLIDING" << std::endl;
		}
		// Check collisions bottom right for ground
		else if (level[35 - yPosBr].at(xPosBr) == 'G') {
			// Clamp player position
			player.position.yPos = int(player.position.yPos) + 1;
			player.yVel = 0;
			player.onGround = true;
			//std::cout << "COLLIDING" << std::endl;
		}
		// Check collisons for spikes
		else if (level[35 - yPosBl].at(xPosBl) == 'S' || level[35 - yPosBr].at(xPosBr) == 'S') {
			player.takeDamage();
			player.jump(deltaTime);
		}
		// Check collisons for End zone
		else if (level[35 - yPosBl].at(xPosBl) == 'E' || level[35 - yPosBr].at(xPosBr) == 'E') {
			gameState++;
		}


	}
	// if Jumping
	else if (player.yVel > 0) {
		// Obtain player top left tile based coordinates
		int xPosTl = int((player.position.xPos + 4.999) / tileWidth);
		int yPosTl = int((player.position.yPos + (player.playerHeight + 0.999)) / tileHeight);

		// Obtain player top right tile based coorinates
		int xPosTr = int(((player.position.xPos + player.playerWidth) - 4.999) / tileWidth);
		int yPosTr = int((player.position.yPos + (player.playerHeight + 0.999)) / tileHeight);

		// Check collisons top left
		if (level[35 - yPosTl].at(xPosTl) == 'G') {
			// Clamp player position
			player.position.yPos = int(player.position.yPos);
			player.yVel = 0;
			//std::cout << "!!!COLLIDING" << std::endl;
		}
		// Check collisons top right
		else if (level[35 - yPosTr].at(xPosTr) == 'G') {
			// Clamp player position
			player.position.yPos = int(player.position.yPos);
			player.yVel = 0;
			//std::cout << "!!!COLLIDING" << std::endl;
		}
	}


	// If moving left
	if (player.xVel < 0) {
		// Obtain player bottom left tile based coordinates
		int xPosBl = int((player.position.xPos + 0.111) / tileWidth);
		int yPosBl = int((player.position.yPos + 3.999) / tileHeight);

		// Obtain player top left tile based coordinates
		int xPosTl = xPosBl;
		int yPosTl = int((player.position.yPos + (player.playerHeight - 0.911)) / tileHeight);

		// Check collisions bottom left
		if (level[35 - yPosBl].at(xPosBl) == 'G') {
			// Clamp player position
			player.position.xPos = int(player.position.xPos) + 1;
			player.xVel = 0;
			std::cout << "BL COLLIDING" << std::endl;
		}
		// Check collisons top left
		else if (level[35 - yPosTl].at(xPosTl) == 'G') {
			// Clamp player position
			player.position.xPos = int(player.position.xPos) ;
			player.xVel = 0;
			std::cout << "TL COLLIDING" << std::endl;
		}
		// Check collisons for spikes
		else if (level[35 - yPosTl].at(xPosTl) == 'S' || level[35 - yPosBl].at(xPosBl) == 'S') {
			player.takeDamage();
			player.jump(deltaTime);
			player.xVel += 0.06;
		}
		// Check collisons for End zone
		else if (level[35 - yPosTl].at(xPosTl) == 'E' || level[35 - yPosBl].at(xPosBl) == 'E') {
			gameState++;
		}
	}
	// If moving right
	else if (player.xVel > 0) {
		// Obtain player bottom right tile based coordinates
		int xPosBr = int((player.position.xPos + player.playerWidth) / tileWidth);
		int yPosBr = int((player.position.yPos + 3.999) / tileHeight);

		// Obtain player top right tile based coorinates
		int xPosTr = xPosBr;
		int yPosTr = int(((player.position.yPos + player.playerHeight) - 0.999) / tileHeight);

		// Check collisions bottom right
		if (level[35 - yPosBr].at(xPosBr) == 'G') {
			// Clamp player position
			player.position.xPos = int(player.position.xPos);
			player.xVel = 0;
			std::cout << "BR COLLIDING" << std::endl;
		}
		// Check collisons top right
		else if (level[35 - yPosTr].at(xPosTr) == 'G') {
			// Clamp player position
			player.position.xPos = int(player.position.xPos);
			player.xVel = 0;
			std::cout << "TR COLLIDING" << std::endl;
		}
		// Check collisons for spikes
		else if (level[35 - yPosTr].at(xPosTr) == 'S' || level[35 - yPosBr].at(xPosBr) == 'S') {
			player.takeDamage();
			player.jump(deltaTime);
			player.xVel -= 0.06;
		}
		// Check collisons for spikes
		else if (level[35 - yPosTr].at(xPosTr) == 'E' || level[35 - yPosBr].at(xPosBr) == 'E') {
			gameState++;
		}
	}

	
	
}

void checkPlatformCollisions(Platform &block) {

		// If the platform is within the screen bounds
		// Check for collision
		if (rectToRect(player, block)) {

			// If left of platform
			if (player.position.xPos + player.width - 0.2 <= block.position.xPos) {
				//std::cout << "LEFT" << std::endl;
				player.xVel = -0.001;
				player.position.xPos = block.position.xPos - player.width;
			}
			// If right of platform
			else if (player.position.xPos >= block.position.xPos + block.width - 0.2) {
				//std::cout << "RIGHT" << std::endl;
				player.xVel = +0.001;
				player.position.xPos = block.position.xPos + block.width;
			}
			// If above platform
			else if (player.position.yPos >= block.position.yPos + block.height - (block.height / 4)) {
				//std::cout << "TOP" << std::endl;
				player.onGround = true;
				player.yVel = 0;

				player.position.yPos = block.position.yPos + block.height;
				if (block.movingRight) {
					player.position.xPos += 0.01 * block.speed * deltaTime;
				}
				else {
					player.position.xPos -= 0.01 * block.speed *deltaTime;
				}

			}
			// If bellow platform
			else if ((player.position.yPos) <= block.position.yPos) {
				//std::cout << "BOTTOM" << std::endl;
				player.position.yPos = block.position.yPos - player.height;
				player.yVel = -0.1;
			}
		}
		//else {
		//	// free up platform refference
		//	player.standingOn = nullptr;
		//}
	
}

void checkNonPlayerBounds(GameObject &platform, std::string level[]) {

	// If moving right
	if (platform.movingRight) {
		// Obtain platform bottom right tile based coordinates
		int xPosBr = int((platform.position.xPos + platform.width) / tileWidth);
		int yPosBr = int(platform.position.yPos / tileHeight);

		// If platform hits either part of the map or a turn around point 
		if (level[35 - yPosBr].at(xPosBr) == 'G' || level[35 - yPosBr].at(xPosBr) == 'T') {
			// Swap directions
			platform.movingRight = !platform.movingRight;
		}
	}
	// If moving left
	else {
		// Obtain platform bottom left tile based coordinates
		int xPosBl = int(platform.position.xPos / tileWidth);
		int yPosBl = int(platform.position.yPos / tileHeight);

		// If platform hits either part of the map or a turn around point 
 		if (level[35 - yPosBl].at(xPosBl) == 'G' || level[35 - yPosBl].at(xPosBl) == 'T') {
			// Swap directions
			platform.movingRight = !platform.movingRight;
		}

	}

	
}

void checkEnemyPlayerCollisions(Enemy &enemy) {
	// If player is colliding with an enemy
	if (rectToRect(player, enemy)) {
		// If falling
		if (player.yVel < 0 && player.position.yPos > enemy.position.yPos + enemy.height/2) {
			player.jump(deltaTime);
		}
		// If to the right of an enemy
		else if (player.position.xPos > enemy.position.xPos) {
			
			player.takeDamage();
			player.jump(deltaTime);
			player.xVel += 0.12;
		
		}
		// If to the left of an enemy
		else {

			player.takeDamage();
			player.jump(deltaTime);
			player.xVel -= 0.12;
		}
	}
}

void checkBatPlayerCollisions(Bat &enemy) {
	if (pointToCircle(player, enemy, 45)) {
		if (player.attacking) {
			if (player.movingRight && enemy.position.xPos >= player.position.xPos) {
				enemy.position.xPos += 70;
				enemy.takeDamage();
			}
			else if (!player.movingRight && enemy.position.xPos <= player.position.xPos) {
				enemy.position.xPos -= 70;
				enemy.takeDamage();
			}
		}
	}
}



/**************** END OPENGL FUNCTIONS *************************/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(200, 200);

	glutCreateWindow("OPENGL CW");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMotion);
	glutIdleFunc(update);

	//add keyboard callback.
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	//add callback for the function keys.
	glutSpecialFunc(special);

	glutMainLoop();

	return 0;
}


