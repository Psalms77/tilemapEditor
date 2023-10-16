#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "sstream"
#include <iostream>

class Engine
{
private: 
	//variables
	float gridSizef;
	unsigned gridSizeu;
	float viewSpeed;
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2u mousePosGrid;
	float dt;
	sf::Clock dtClock;
	sf::Text gridCoord;
	sf::Font font;
	sf::RectangleShape* tileSelector;

	//tilemap
	std::vector<std::vector <sf::RectangleShape>> tileMap;
	//window
	sf::RenderWindow* window;
	sf::View view;
	sf::Event ev;
	// private functions
	void initVariables();
	void initWindow();



public: 
	Engine();
	virtual ~Engine();
	const bool isRunning() const;

	//functions
	void pollEvents();
	void update();
	void render();
	void moveCam();



};

