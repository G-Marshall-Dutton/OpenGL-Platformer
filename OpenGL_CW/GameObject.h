#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vertex.h"
#include "Dimension.h"
#include "Image_Loading/nvImage.h"
#include <Vector>

class GameObject {


public:
	// Bottom left 
	Vertex position;

	int width;
	int height;

	bool movingRight = true;

	// Constructors
	GameObject();
	//GameObject(Dimension dimension);
	GameObject(Vertex position, int width, int height);

	// Getters
	//Vertex getPosition();
	//Dimension getDimension();
	float getWidth() const;
	float getHeight() const;
	// Setters
	void setPosition(Vertex postition);
	//void setDimension(Dimension dimension);

};

// Constructors
inline GameObject::GameObject() {
	position = Vertex();
	//dimension = Dimension();
}

//inline GameObject::GameObject(Dimension d) {
//	dimension = d;
//	position = dimension.bl;
//
//}

inline GameObject::GameObject(Vertex pos, int w, int h) {
	position = pos;
	width = w;
	height = h;
	std::cout << "W " << width << "H " << height << std::endl;
}

// Getters
//inline Vertex GameObject::getPosition()  {
//	return this->position;
//}
//
//inline Dimension GameObject::getDimension()  {
//	return dimension;
//}
//
inline float GameObject::getWidth() const {
	return width;
}

inline float GameObject::getHeight()  const {
	return height;
}

// Setters
inline void GameObject::setPosition(Vertex pos) {
	position = pos;
}

//inline void GameObject::setDimension(Dimension d) {
//	dimension = d;
//}
#endif