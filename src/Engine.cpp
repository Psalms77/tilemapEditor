#include "Engine.h"

// private functions

//		init
void Engine::initVariables() {
	gridSizef = 32.f;
	gridSizeu = static_cast<unsigned>(gridSizef);
	viewSpeed = 200.f;
	dt = 0.f;
	font.loadFromFile(".\\font\\Roboto-Black.ttf");
	gridCoord.setCharacterSize(24);
	gridCoord.setFillColor(sf::Color::White);
	gridCoord.setFont(font);
	gridCoord.setPosition(100.f, 100.f);
	gridCoord.setString("testtest");
	//shape = new sf::RectangleShape(sf::Vector2f(gridSizef, gridSizef));
	tileSelector = new sf::RectangleShape(sf::Vector2f(gridSizef, gridSizef));
	tileSelector->setFillColor(sf::Color::Transparent);
	tileSelector->setOutlineColor(sf::Color::Green);
	tileSelector->setOutlineThickness(1.f);
	tileMap.resize(64, std::vector<sf::RectangleShape>());

	for (int x = 0; x < 64; x++)
	{
		tileMap[x].resize(64, sf::RectangleShape());
		for (int y = 0; y < 64; y++)
		{
			tileMap[x][y].setSize(sf::Vector2f(gridSizef, gridSizef));
			tileMap[x][y].setOutlineThickness(1.f);
			tileMap[x][y].setOutlineColor(sf::Color::Red);
			tileMap[x][y].setFillColor(sf::Color::Transparent);
			tileMap[x][y].setPosition(x * gridSizef, y * gridSizef);
		}
	}



	this->window = nullptr;
}
void Engine::initWindow() {
	this->window = new sf::RenderWindow(sf::VideoMode(1600, 900), "2d Tilemap Editor");
	this->view.setSize(1600.f, 900.f);
	this->view.setCenter(this->window->getSize().x /2.f, this->window->getSize().y / 2.f);
}




Engine::Engine() {
	this->initVariables();
	this->initWindow();
	
}
Engine::~Engine() {
	delete this->window;
}


// accessors
const bool Engine::isRunning() const{
	return this->window->isOpen();
}


// functions

void Engine::pollEvents() {
	while (this -> window -> pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
			{
				this->window->close();
			}
			break;
		}
	}
}


void Engine::update() {
	
	dt = dtClock.restart().asSeconds();

	// grid coordination
	std::stringstream ss;
	ss << "Current Grid Coordinate: " << mousePosGrid.x << ", " << mousePosGrid.y << "\n";
	gridCoord.setString(ss.str());

	this->window->setView(this->view);
	this->pollEvents();
	// move camera
	this->moveCam();

	mousePosScreen = sf::Mouse::getPosition();
	mousePosWindow = sf::Mouse::getPosition(*this->window);  //why though
	mousePosView = this->window->mapPixelToCoords(mousePosWindow);
	if (mousePosView.x >= 0.f)
	{
		mousePosGrid.x = (mousePosView.x) / gridSizeu;
	}
	if (mousePosView.y >= 0.f)
	{
		mousePosGrid.y = (mousePosView.y) / gridSizeu;
	}
	this->window->setView(this->window->getDefaultView());
	// snap tile selector info
	tileSelector->setPosition(mousePosGrid.x * gridSizef, mousePosGrid.y * gridSizef);



}



void Engine::render() {
	// render
	this->window->clear();
	this->window->setView(this->view);
	// render elements
	//this->window->draw();
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			window->draw(tileMap[x][y]);
		}
	}
	//this->window->draw(*shape);
	window->draw(*tileSelector);
	this->window->setView(this->window->getDefaultView());

	// render ui
	window->draw(gridCoord);

	this->window->display();
	//this->window->clear();
	//this->window->display();


}

void Engine::moveCam() {

	// move camera
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))  //left
	{
		view.move(-viewSpeed * dt, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))  //right
	{
		view.move(viewSpeed * dt, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))  //up
	{
		view.move(0.f, -viewSpeed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))  //down
	{
		view.move(0.f, viewSpeed * dt);
	}



}

