#include "Engine.h"
//	init
void Engine::initVariables() {
	// init basic
	gridSizef = 32.f;
	gridSizeu = static_cast<unsigned>(gridSizef);
	viewSpeed = 200.f;
	dt = 0.f;
	tileSelectorSize = 1;
	// init images
	this->loadAssets();
	
	// init text
	font.loadFromFile(".\\font\\Roboto-Black.ttf");
	gridCoord.setCharacterSize(24);
	gridCoord.setFillColor(sf::Color::Black);
	gridCoord.setFont(font);
	gridCoord.setPosition(100.f, 100.f);
	gridCoord.setString("testtest");
	// init tile selector
	tileSelector = new sf::RectangleShape(sf::Vector2f(gridSizef, gridSizef));
	//tileSelector->setFillColor(sf::Color::Transparent);
	tileSelector->setOutlineColor(sf::Color::Green);
	tileSelector->setOutlineThickness(1.f);
	// init tilemap
	tileMap.resize(64, std::vector<sf::RectangleShape>());
	for (int x = 0; x < 64; x++)
	{
		tileMap[x].resize(64, sf::RectangleShape());
		for (int y = 0; y < 64; y++)
		{
			tileMap[x][y].setSize(sf::Vector2f(gridSizef, gridSizef));
			//tileMap[x][y].setOutlineThickness(1.f);
			//tileMap[x][y].setOutlineColor(sf::Color::Red);
			tileMap[x][y].setFillColor(sf::Color::White);
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
		case sf::Event::KeyReleased:
			if (this->ev.key.code == sf::Keyboard::E)
			{
				expandBrushSize();
			}
			if (this->ev.key.code == sf::Keyboard::Q)
			{
				shrinkBrushSize();
			}
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
	// check mouse
	this->getMouseInfo();
	this->selectTexture();
	this->window->setView(this->window->getDefaultView());
	// snap tile selector to grid
	tileSelector->setPosition(mousePosGrid.x * gridSizef, mousePosGrid.y * gridSizef);
	tileSelector->setTexture(&defaultTexture);
	// draw
	this->paint();
	this->erase();

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
	// zoom cam
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))  //=
	{
		gridSizef += 0.1f;
		// reinit tilemap
		tileMap.resize(64, std::vector<sf::RectangleShape>());
		for (int x = 0; x < 64; x++)
		{
			tileMap[x].resize(64, sf::RectangleShape());
			for (int y = 0; y < 64; y++)
			{
				tileMap[x][y].setSize(sf::Vector2f(gridSizef, gridSizef));
				tileMap[x][y].setOutlineThickness(1.f);
				tileMap[x][y].setOutlineColor(sf::Color::Red);
				//tileMap[x][y].setFillColor(sf::Color::Transparent);
				tileMap[x][y].setPosition(x * gridSizef, y * gridSizef);
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen))  //-
	{
		gridSizef -= 0.1f;
		// init tilemap
		tileMap.resize(64, std::vector<sf::RectangleShape>());
		for (int x = 0; x < 64; x++)
		{
			tileMap[x].resize(64, sf::RectangleShape());
			for (int y = 0; y < 64; y++)
			{
				tileMap[x][y].setSize(sf::Vector2f(gridSizef, gridSizef));
				tileMap[x][y].setOutlineThickness(1.f);
				tileMap[x][y].setOutlineColor(sf::Color::Red);
				//tileMap[x][y].setFillColor(sf::Color::Transparent);
				tileMap[x][y].setPosition(x * gridSizef, y * gridSizef);
			}
		}
	}
}

void Engine::getMouseInfo() {
	mousePosScreen = sf::Mouse::getPosition();
	mousePosWindow = sf::Mouse::getPosition(*this->window);  // why though
	mousePosView = this->window->mapPixelToCoords(mousePosWindow);
	if (mousePosView.x >= 0.f)
	{
		mousePosGrid.x = (mousePosView.x) / gridSizeu;
	}
	if (mousePosView.y >= 0.f)
	{
		mousePosGrid.y = (mousePosView.y) / gridSizeu;
	}
}


void Engine::paint() {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//sf::Texture tempTexture;
		sf::Image copyImage = currentTexture.copyToImage();
		sf::Texture* copyTexture = new sf::Texture();
		copyTexture->loadFromImage(copyImage);
		tileMap[mousePosGrid.x][mousePosGrid.y].setTexture(copyTexture);

		if (tileSelectorSize == 2)
		{
			tileMap[mousePosGrid.x + 1][mousePosGrid.y].setTexture(copyTexture);
			tileMap[mousePosGrid.x ][mousePosGrid.y + 1].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 1].setTexture(copyTexture);
		}
		if (tileSelectorSize == 3)
		{
			tileMap[mousePosGrid.x + 1][mousePosGrid.y].setTexture(copyTexture);
			tileMap[mousePosGrid.x][mousePosGrid.y + 1].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 1].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y].setTexture(copyTexture);
			tileMap[mousePosGrid.x][mousePosGrid.y + 2].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y + 2].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y +1].setTexture(copyTexture);
			tileMap[mousePosGrid.x+1][mousePosGrid.y + 2].setTexture(copyTexture);
		}
	}


}

void Engine::erase() {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		tileMap[mousePosGrid.x][mousePosGrid.y].setTexture(NULL);
		if (tileSelectorSize == 2)
		{
			tileMap[mousePosGrid.x + 1][mousePosGrid.y].setTexture(NULL);
			tileMap[mousePosGrid.x][mousePosGrid.y + 1].setTexture(NULL);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 1].setTexture(NULL);
		}
		if (tileSelectorSize == 3)
		{
			tileMap[mousePosGrid.x + 1][mousePosGrid.y].setTexture(NULL);
			tileMap[mousePosGrid.x][mousePosGrid.y + 1].setTexture(NULL);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 1].setTexture(NULL);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y].setTexture(NULL);
			tileMap[mousePosGrid.x][mousePosGrid.y + 2].setTexture(NULL);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y + 2].setTexture(NULL);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y + 1].setTexture(NULL);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 2].setTexture(NULL);
		}
	}
}

void Engine::loadAssets() {
	
	//defaultSprite.setColor(sf::Color::Transparent);
	defaultTexture.loadFromFile(".\\img\\tiles5.png");
	//defaultTexture.loadFromFile(".\\img\\tiles5.png");
	currentSprite = defaultSprite;
	currentTexture = defaultTexture;
	textures.resize(10, sf::Texture());
	for (int i = 0; i < 10; i++)
	{
		tempStr = (".\\img\\tiles" + std::to_string(i + 1) + ".png");
		std::cout << tempStr;
		textures[i].loadFromFile(tempStr);
	}
}

void Engine::expandBrushSize() {
	tileSelectorSize += 1;
	if (tileSelectorSize >3)
		tileSelectorSize = 3;
	std::cout << "expand"<<tileSelectorSize;
	tileSelector = new sf::RectangleShape(sf::Vector2f(gridSizef * tileSelectorSize, gridSizef * tileSelectorSize));
	tileSelector->setOutlineColor(sf::Color::Green);
	tileSelector->setOutlineThickness(1.f);
}
void Engine::shrinkBrushSize() {
	tileSelectorSize -= 1;
	if (tileSelectorSize < 1)
		tileSelectorSize = 1;
	std::cout << "shrink" << tileSelectorSize;
	tileSelector = new sf::RectangleShape(sf::Vector2f(gridSizef * tileSelectorSize, gridSizef * tileSelectorSize));
	tileSelector->setOutlineColor(sf::Color::Green);
	tileSelector->setOutlineThickness(1.f);
}

void Engine::selectTexture() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		currentTexture = textures[1];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		currentTexture = textures[2];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		currentTexture = textures[3];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		currentTexture = textures[4];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		currentTexture = textures[5];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
	{
		currentTexture = textures[6];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
	{
		currentTexture = textures[7];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
	{
		currentTexture = textures[8];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
	{
		currentTexture = textures[9];
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
	{
		currentTexture = textures[0];
	}
}