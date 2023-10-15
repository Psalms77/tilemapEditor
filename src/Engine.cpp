#include "Engine.h"
#include "Inputs.h"
// private functions

//		init
void Engine::initVariables() {
	gridSizef = 100.f;
	gridSizeu = static_cast<unsigned>(gridSizef);
	shape = new sf::RectangleShape(sf::Vector2f(gridSizef, gridSizef));
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

	this->pollEvents();
	mousePosScreen = sf::Mouse::getPosition();
	mousePosWindow = sf::Mouse::getPosition(*this->window);  //why though
	mousePosView = this->window->mapPixelToCoords(mousePosWindow);
	if (mousePosView.x >= 0.f)
	{
		mousePosGrid.x = (mousePosView.y) / gridSizeu;
	}
	if (mousePosView.y >= 0.f)
	{
		mousePosGrid.y = (mousePosView.y) / gridSizeu;
	}
	
	// render
	this->window->clear();
	this->window->setView(this->view);
	// render elements
	//this->window->draw();
	this->window->setView(this->window->getDefaultView());

	// render ui

	this->window->display();
}



void Engine::render() {

	this->window->clear();
	this->window->display();


}



