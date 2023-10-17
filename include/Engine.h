#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "sstream"
#include <iostream>
#include <string>

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
	// sprite & texture containers
	//std::vector<sf::Sprite> sprites;
	std::vector<sf::Texture> textures;
	sf::Texture defaultTexture;
	sf::Sprite defaultSprite;
	std::string tempStr;

	// tile selector
	sf::RectangleShape* tileSelector;
	int tileSelectorSize;
	sf::Texture currentTexture;
	int currentTextureID;
	sf::Sprite currentSprite;
	

	//tilemap
	std::vector<std::vector<int>> tileMapi;
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
	void getMouseInfo();
	void paint();
	void erase();
	void expandBrushSize();
	void shrinkBrushSize();
	void loadAssets();
	void selectTexture();

};

