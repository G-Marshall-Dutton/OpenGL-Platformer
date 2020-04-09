#pragma once
#ifndef DIMENSION_H
#define DIMENSION_H

#include "Vertex.h"


class Dimension
{

public:

	// Postion of each corner
	// bottom left, bottom right, top right, top left
	Vertex *brp, *trp, *tlp;
	Vertex bl, br, tr, tl;

	float width, height;

	// Constructor
	Dimension();
	Dimension(Vertex position, float w, float h);

	//// Getters
	//Vertex getBl() const;
	//Vertex getBr() const;
	//Vertex getTr() const;
	//Vertex getTl() const;

	//float getWidth() const;
	//float getHeight() const;

	// Setters
	void setBl(float xPos, float yPos);
	void setBr(float xPos, float yPos);
	void setTr(float xPos, float yPos);
	void setTl(float xPos, float yPos);
};
 
// Constructor ------------------------------------------------
inline Dimension::Dimension() {
	bl = Vertex(0, 0);
	br = Vertex(1, 0);
	tr = Vertex(1, 1);
	tl = Vertex(0, 1);
}
inline Dimension::Dimension(Vertex position, float w, float h) {
	width = w;
	height = h;

	std::cout << width << height << std::endl;

	bl = position;
	std::cout << position << std::endl;
	br = Vertex(position.xPos + width, position.yPos);
	std::cout << br << std::endl;
	tr = Vertex(position.xPos + width, position.yPos + height);
	tl = Vertex(position.xPos, position.yPos + height);
}

//// Getters -----------------------------------------------------
//inline Vertex Dimension::getBl() const {
//	return this->bl;
//}
//
//inline Vertex Dimension::getBr() const {
//	return this->br;
//}
//
//inline Vertex Dimension::getTr() const {
//	return this->tr;
//}
//
//inline Vertex Dimension::getTl() const {
//	return this->tl;
//}
//
//inline float Dimension::getWidth() const {
//	return this->width;
//}
//
//inline float Dimension::getHeight() const {
//	return this->height;
//}

// Setters ------------------------------------------------------
inline void Dimension::setBl(float xPos, float yPos) {
	bl.setXPos(xPos);
	bl.setYPos(yPos);
}

inline void Dimension::setBr(float xPos, float yPos) {
	br.setXPos(xPos);
	br.setYPos(yPos);
}

inline void Dimension::setTr(float xPos, float yPos) {
	tr.setXPos(xPos);
	tr.setYPos(yPos);
}

inline void Dimension::setTl(float xPos, float yPos) {
	tl.setXPos(xPos);
	tl.setYPos(yPos);
}
#endif 