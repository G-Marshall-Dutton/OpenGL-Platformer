#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <iomanip>

class Vertex {

public:

	float xPos, yPos;

	// Constructor
	Vertex();
	Vertex(float xPos, float yPos);

	//// Getters 
	//float getXPos() const;
	//float getYPos() const;

	// Setters
	void setXPos(float xPos);
	void setYPos(float yPos);

};

// Constructor
inline Vertex::Vertex() {
	xPos = 0;
	yPos = 0;
}

inline Vertex::Vertex(float x, float y) {
	xPos = x;
	yPos = y;
}

//// Getters
//inline float Vertex::getXPos() const{
//	return xPos;
//}
//
//inline float Vertex::getYPos() const {
//	return yPos;
//}

// Setters
inline void Vertex::setXPos(float x){
	xPos = x;
}

inline void Vertex::setYPos(float y) {
	yPos = y;
}

// I/O stream operators
inline 
std::ostream& operator<<(std::ostream& os, const Vertex &v)
{
	return os << "(" << v.xPos << ", " << v.yPos << ")";
}

#endif