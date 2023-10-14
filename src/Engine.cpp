#include "Engine.h"

// private functions

//		init
void Engine::initVariables() {
	this->window = nullptr;
}
void Engine::initWindow() {
	this->window = new sf::RenderWindow(sf::VideoMode(1600, 900), "2d Tilemap Editor");
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



}



void Engine::render() {

	this->window->clear();
	this->window->display();


}



