#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <iostream>
class Engine
{
private: 
	
	// private functions
	void initVariables();
	void initWindow();



public: 
	Engine();
	virtual ~Engine();

	//functions
	void update();
	void render();




};

