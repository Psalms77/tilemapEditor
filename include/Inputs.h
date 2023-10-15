#pragma once
#include "Engine.h"
class Inputs
{
public: 
 	Inputs();
	~Inputs();
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2u mousePosGrid;

};

