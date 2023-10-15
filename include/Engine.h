#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include <iostream>
#include "Inputs.h"
class Engine
{
private: 
	//variables
	float gridSizef;
	unsigned gridSizeu;
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2u mousePosGrid;


	// private functions
	void initVariables();
	void initWindow();



public: 
	Engine();
	virtual ~Engine();
	const bool isRunning() const;
	sf::RectangleShape* shape;
	//window
	sf::RenderWindow* window;
	sf::View view;
	sf::Event ev;
	//functions
	void pollEvents();
	void update();
	void render();




};

