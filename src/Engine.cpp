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
	gridCoord.setFillColor(sf::Color::Red);
	gridCoord.setFont(font);
	gridCoord.setPosition(20.f, 20.f);
	gridCoord.setString("testtest");
	// init tile selector
	tileSelector = new sf::RectangleShape(sf::Vector2f(gridSizef, gridSizef));
	//tileSelector->setFillColor(sf::Color::Transparent);
	tileSelector->setOutlineColor(sf::Color::Green);
	tileSelector->setOutlineThickness(1.f);
	// init tilemap
	tileMap.resize(64, std::vector<sf::RectangleShape>());
	tileMapi.resize(64, std::vector<int>());
	for (int x = 0; x < 64; x++)
	{
		tileMap[x].resize(64, sf::RectangleShape());
		tileMapi[x].resize(64, int());
		for (int y = 0; y < 64; y++)
		{
			tileMapi[x][y] = 0;
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
			if (this->ev.key.code == sf::Keyboard::O)
			{
				saveTileMap();
			}
			if (this->ev.key.code == sf::Keyboard::I)
			{
				loadTileMap();
			}
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
	ss << "Current Mouse Grid Coordinate: " << mousePosGrid.x << ", " << mousePosGrid.y << "\n"
		<< "W, A, S, D - Move Cam" << " | " << "Press Equal and Hyphen to Zoom in or out "<<"\n"
		<<"Q, E - Adjust BrushSize" << " | "<< "I, O - Load or Save current Tilemap"<<"\n"
		<<"Left Click to Paint"<<" | Right Click to Erase"<<"\n"
		<<"Press scroll wheel as eyedropper tool"<< " | Press Num Button to Select Tile";
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
	tileSelector->setTexture(&currentTexture);
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
	tileSlotUI();
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
				//tileMap[x][y].setOutlineColor(sf::Color::Red);
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
				//tileMap[x][y].setOutlineColor(sf::Color::Red);
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
		tileMapi[mousePosGrid.x][mousePosGrid.y] = currentTextureID;


		if (tileSelectorSize == 2)
		{
			tileMap[mousePosGrid.x + 1][mousePosGrid.y].setTexture(copyTexture);
			tileMap[mousePosGrid.x ][mousePosGrid.y + 1].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 1].setTexture(copyTexture);
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y] = currentTextureID;
			tileMapi[mousePosGrid.x][mousePosGrid.y + 1] = currentTextureID;
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y + 1] = currentTextureID;
		}
		if (tileSelectorSize == 3)
		{
			tileMap[mousePosGrid.x + 1][mousePosGrid.y].setTexture(copyTexture);
			tileMap[mousePosGrid.x][mousePosGrid.y + 1].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 1].setTexture(copyTexture);
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y] = currentTextureID;
			tileMapi[mousePosGrid.x][mousePosGrid.y + 1] = currentTextureID;
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y + 1] = currentTextureID;
			tileMap[mousePosGrid.x + 2][mousePosGrid.y].setTexture(copyTexture);
			tileMap[mousePosGrid.x][mousePosGrid.y + 2].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y + 2].setTexture(copyTexture);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y +1].setTexture(copyTexture);
			tileMap[mousePosGrid.x+1][mousePosGrid.y + 2].setTexture(copyTexture);
			tileMapi[mousePosGrid.x + 2][mousePosGrid.y] = currentTextureID;
			tileMapi[mousePosGrid.x][mousePosGrid.y + 2] = currentTextureID;
			tileMapi[mousePosGrid.x + 2][mousePosGrid.y + 2] = currentTextureID;
			tileMapi[mousePosGrid.x + 2][mousePosGrid.y + 1] = currentTextureID;
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y + 2] = currentTextureID;
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
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y] = 0;
			tileMapi[mousePosGrid.x][mousePosGrid.y + 1] = 0;
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y + 1] = 0;
		}
		if (tileSelectorSize == 3)
		{
			tileMap[mousePosGrid.x + 1][mousePosGrid.y].setTexture(NULL);
			tileMap[mousePosGrid.x][mousePosGrid.y + 1].setTexture(NULL);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 1].setTexture(NULL);
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y] = 0;
			tileMapi[mousePosGrid.x][mousePosGrid.y + 1] = 0;
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y + 1] = 0;
			tileMap[mousePosGrid.x + 2][mousePosGrid.y].setTexture(NULL);
			tileMap[mousePosGrid.x][mousePosGrid.y + 2].setTexture(NULL);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y + 2].setTexture(NULL);
			tileMap[mousePosGrid.x + 2][mousePosGrid.y + 1].setTexture(NULL);
			tileMap[mousePosGrid.x + 1][mousePosGrid.y + 2].setTexture(NULL);
			tileMapi[mousePosGrid.x + 2][mousePosGrid.y] = 0;
			tileMapi[mousePosGrid.x][mousePosGrid.y + 2] = 0;
			tileMapi[mousePosGrid.x + 2][mousePosGrid.y + 2] = 0;
			tileMapi[mousePosGrid.x + 2][mousePosGrid.y + 1] = 0;
			tileMapi[mousePosGrid.x + 1][mousePosGrid.y + 2] = 0;
		}
	}
}

void Engine::loadAssets() {
	
	//defaultSprite.setColor(sf::Color::Transparent);
	defaultTexture.loadFromFile(".\\img\\tiles10.png");
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		if (tileMapi[mousePosGrid.x][mousePosGrid.y] != 0)
		{
			currentTexture = textures[tileMapi[mousePosGrid.x][mousePosGrid.y] - 1 ];
			currentTextureID = tileMapi[mousePosGrid.x][mousePosGrid.y];
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		currentTexture = textures[0];
		currentTextureID = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		currentTexture = textures[1];
		currentTextureID = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		currentTexture = textures[2];
		currentTextureID = 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		currentTexture = textures[3];
		currentTextureID = 4;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		currentTexture = textures[4];
		currentTextureID = 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
	{
		currentTexture = textures[5];
		currentTextureID = 6;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
	{
		currentTexture = textures[6];
		currentTextureID = 7;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
	{
		currentTexture = textures[7];
		currentTextureID = 8;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
	{
		currentTexture = textures[8];
		currentTextureID = 9;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
	{
		currentTexture = textures[9];
		currentTextureID = 0;
	}
}

void Engine::saveTileMap() {
	
	//std::vector<int> row;

	// open a file for outputting the tilemap
	std::ofstream outputfile;
	outputfile.open("tileMapoutput.txt");

	// output the tilemap to the file
	if (outputfile.is_open()) {
		for (int row = 0; row < tileMapi.size(); row++) {
			for (int column = 0; column < tileMapi[row].size(); column++) {
				if (column != tileMapi[row].size() - 1) {
					outputfile << tileMapi[row][column] << " ";
				}
				else {
					outputfile << tileMapi[row][column];
				}
			}
			outputfile << std::endl;
		}
	}

	outputfile.close();

}

void Engine::loadTileMap() {
	std::vector<std::vector<int>> vec;

	std::ifstream file_in("tileMapoutput.txt");
	if (!file_in) {/*error*/ }

	std::string line;
	while (std::getline(file_in, line))
	{
		std::istringstream ss(line);
		std::cout << line;
		vec.emplace_back(std::istream_iterator<int>(ss), std::istream_iterator<int>());
	}
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			std::cout << std::to_string(vec[x][y]);
			;
			if (vec[x][y]> 0)
			{
				tileMapi[x][y] = vec[x][y];
				tileMap[x][y].setTexture(&textures[vec[x][y]]);
			}
			if (vec[x][y] == 0)
			{
				tileMap[x][y].setTexture(&textures[9]);
			}
		}
	}
}

void Engine::tileSlotUI() {

	slotText.setCharacterSize(24.f);
	slotText.setFont(font);
	slotText.setPosition(50.f, 720.f);
	slotText.setFillColor(sf::Color::Blue);
	slotText.setString("Num    1           Num   2           Num   3            Num   4            Num   5           Num   6            Num   7           Num   8           Num   9           Num   0");
	window->draw(slotText);

	tileSlot1 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot1->setOutlineThickness(5.f);
	tileSlot1->setOutlineColor(sf::Color::Blue);
	tileSlot1->setTexture(&textures[0]);
	tileSlot1->setPosition(50.f, 750.f);
	window->draw(*tileSlot1);

	tileSlot2 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot2->setOutlineThickness(5.f);
	tileSlot2->setOutlineColor(sf::Color::Blue);
	tileSlot2->setTexture(&textures[1]);
	tileSlot2->setPosition(200.f, 750.f);
	window->draw(*tileSlot2);

	tileSlot3 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot3->setOutlineThickness(5.f);
	tileSlot3->setOutlineColor(sf::Color::Blue);
	tileSlot3->setTexture(&textures[2]);
	tileSlot3->setPosition(350.f, 750.f);
	window->draw(*tileSlot3);

	tileSlot4 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot4->setOutlineThickness(5.f);
	tileSlot4->setOutlineColor(sf::Color::Blue);
	tileSlot4->setTexture(&textures[3]);
	tileSlot4->setPosition(500.f, 750.f);
	window->draw(*tileSlot4);

	tileSlot5 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot5->setOutlineThickness(5.f);
	tileSlot5->setOutlineColor(sf::Color::Blue);
	tileSlot5->setTexture(&textures[4]);
	tileSlot5->setPosition(650.f, 750.f);
	window->draw(*tileSlot5);

	tileSlot6 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot6->setOutlineThickness(5.f);
	tileSlot6->setOutlineColor(sf::Color::Blue);
	tileSlot6->setTexture(&textures[5]);
	tileSlot6->setPosition(800.f, 750.f);
	window->draw(*tileSlot6);

	tileSlot7 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot7->setOutlineThickness(5.f);
	tileSlot7->setOutlineColor(sf::Color::Blue);
	tileSlot7->setTexture(&textures[6]);
	tileSlot7->setPosition(950.f, 750.f);
	window->draw(*tileSlot7);

	tileSlot8 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot8->setOutlineThickness(5.f);
	tileSlot8->setOutlineColor(sf::Color::Blue);
	tileSlot8->setTexture(&textures[7]);
	tileSlot8->setPosition(1100.f, 750.f);
	window->draw(*tileSlot8);

	tileSlot9 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot9->setOutlineThickness(5.f);
	tileSlot9->setOutlineColor(sf::Color::Blue);
	tileSlot9->setTexture(&textures[8]);
	tileSlot9->setPosition(1250.f, 750.f);
	window->draw(*tileSlot9);

	tileSlot0 = new sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tileSlot0->setOutlineThickness(5.f);
	tileSlot0->setOutlineColor(sf::Color::Blue);
	tileSlot0->setTexture(&textures[9]);
	tileSlot0->setPosition(1400.f, 750.f);
	window->draw(*tileSlot0);
}