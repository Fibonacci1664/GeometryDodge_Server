#include "Level.h"

Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs) : Screen(hwnd, in, gs)
{
	isDebugMode = true;

	initLevel();

	if (isDebugMode)
	{
		initDebugMode();
	}
}

Level::~Level()
{
	if (player1)
	{
		delete player1;
		player1 = nullptr;
	}

	if (ui)
	{
		delete ui;
		ui = nullptr;
	}

	for (int i = 0; i < asteroids.size(); ++i)
	{
		if (asteroids[i])
		{
			delete asteroids[i];
			asteroids[i] = nullptr;
		}
	}

	if (conn)
	{
		delete conn;
		conn = nullptr;
	}
}

void Level::initLevel()
{
	initConnection();
	initBackground();
	initUI();
	initPlayer();
	initAsteroids();
}

void Level::initConnection()
{
	conn = new Connection();
	conn->createTCPListner();
	// Happens once here, then every update loop thereafter
	conn->checkConnections();
}

void Level::initDebugMode()
{
	// Player debug collision box
	player1ColBox.setFillColor(sf::Color(0, 0, 0, 0));
	player1ColBox.setOutlineColor(sf::Color::Red);
	player1ColBox.setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	player1ColBox.setPosition(sf::Vector2f(player1->getCollisionBox().left, player1->getCollisionBox().top));
	player1ColBox.setSize(sf::Vector2f(player1->getCollisionBox().width, player1->getCollisionBox().height));

	if (player2)
	{
		player2ColBox.setFillColor(sf::Color(0, 0, 0, 0));
		player2ColBox.setOutlineColor(sf::Color::Yellow);
		player2ColBox.setOutlineThickness(1.0f);

		player2ColBox.setPosition(sf::Vector2f(player2->getCollisionBox().left, player2->getCollisionBox().top));
		player2ColBox.setSize(sf::Vector2f(player2->getCollisionBox().width, player2->getCollisionBox().height));
	}

	// Create a new asteroid collision box
	asteroidColBoxes.push_back(sf::RectangleShape());

	// Asteroids debug collision boxes
	asteroidColBoxes[0].setFillColor(sf::Color(0, 0, 0, 0));
	asteroidColBoxes[0].setOutlineColor(sf::Color::Magenta);
	asteroidColBoxes[0].setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	asteroidColBoxes[0].setPosition(sf::Vector2f(asteroids[0]->getCollisionBox().left, asteroids[0]->getCollisionBox().top));
	asteroidColBoxes[0].setSize(sf::Vector2f(asteroids[0]->getCollisionBox().width, asteroids[0]->getCollisionBox().height));
}

void Level::initBackground()
{
	loadTexture();
	bgTexture.setSmooth(true);
	bgSprite.setTexture(bgTexture);
	//bgSprite.setOrigin(bgSprite.getTextureRect().width * 0.5f, bgSprite.getTextureRect().height * 0.5f);
	////bgSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
	bgSprite.setScale(5.0f, 2.8125f);
}

void Level::initUI()
{
	ui = new UI;
	currentWave = ui->getWaveCount();
}

void Level::initPlayer()
{
	player1 = new Player(1, window, input);
	/*player2 = new Player(2, window, input);
	player2->getPlayerSprite()->setPosition(300.0f, 300.0f);*/
}

void Level::initAsteroids()
{
	asteroids.push_back(new Asteroid(window));
}

void Level::spawnNewAsteroid()
{
	// If we get here it means the wave count was incremented, so spawn an asteroid
	asteroids.push_back(new Asteroid(window));
	createNewAsteroidColBox();
}

void Level::checkCurrentWave()
{
	if (currentWave != ui->getWaveCount())
	{
		spawnNewAsteroid();
		currentWave = ui->getWaveCount();
	}
}

void Level::handleInput(float dt)
{
	//player1->handleInput(dt);
	//player2->handleInput(dt);
}

void Level::update(float dt)
{
	PlayerDataMsg* msg = conn->checkConnections();

	ui->update(dt);

	//player1->networkUpdate(dt, conn);

	if (msg)
	{
		player1->update(dt, msg);
	}
	
	//player2->networkUpdate(dt, conn);

	if (isDebugMode)
	{
		// Update the players collision box
		player1ColBox.setPosition(sf::Vector2f(player1->getCollisionBox().left, player1->getCollisionBox().top));

		if (player2)
		{
			player2ColBox.setPosition(sf::Vector2f(player2->getCollisionBox().left, player2->getCollisionBox().top));
		}

		// Update all the asteroids collision boxes
		for (int i = 0; i < asteroidColBoxes.size(); ++i)
		{
			asteroidColBoxes[i].setPosition(sf::Vector2f(asteroids[i]->getCollisionBox().left, asteroids[i]->getCollisionBox().top));
		}
	}

	checkCurrentWave();

	// Update all the asteroids
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i]->update(dt);
	}

	//checkCollisions();
}

void Level::render()
{
	beginDraw();

	// Render stuff here
	window->draw(bgSprite);
	window->draw(*ui->getWaveText());
	window->draw(*ui->getCountdownText());

	// Draw all the asteroids
	for (int i = 0; i < asteroids.size(); i++)
	{
		window->draw(*asteroids[i]->getAsteroidSprite());
	}

	window->draw(*player1->getPlayerSprite());

	// If player 2 exists render them
	if (player2)
	{
		window->draw(*player2->getPlayerSprite());
	}

	// Draw all the debug magenta collision boxes
	if (isDebugMode)
	{
		window->draw(player1ColBox);
		window->draw(player2ColBox);

		for (int i = 0; i < asteroidColBoxes.size(); ++i)
		{
			window->draw(asteroidColBoxes[i]);
		}
	}

	endDraw();
}

void Level::beginDraw()
{
	window->clear(sf::Color::Black);
}

void Level::endDraw()
{
	window->display();
}

void Level::loadTexture()
{
	if (!bgTexture.loadFromFile("res/gfx/bg/blue.png"))
	{
		std::cout << "Error loading background texture.\n";
	}
}

void Level::checkCollisions()
{
	// If player collides with an asteroid, set GAME_OVER state
	for (int i = 0; i < asteroids.size(); ++i)
	{
		if (player1->getCollisionBox().intersects(asteroids[i]->getCollisionBox()))
		{
			gameState->setCurrentState(State::GAMEOVER);
		}
	}
}

void Level::createNewAsteroidColBox()
{
	// Create a new asteroid collision box
	asteroidColBoxes.push_back(sf::RectangleShape());

	// Access the last element in the vector, i.e. the one we just pushed back
	int index = asteroidColBoxes.size() - 1;

	asteroidColBoxes[index].setFillColor(sf::Color(0, 0, 0, 0));
	asteroidColBoxes[index].setOutlineColor(sf::Color::Magenta);
	asteroidColBoxes[index].setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	asteroidColBoxes[index].setPosition(sf::Vector2f(asteroids[index]->getCollisionBox().left, asteroids[index]->getCollisionBox().top));
	asteroidColBoxes[index].setSize(sf::Vector2f(asteroids[index]->getCollisionBox().width, asteroids[index]->getCollisionBox().height));
}