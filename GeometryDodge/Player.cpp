#include "Player.h"
#include <iostream>

Player::Player(int playerNum, sf::RenderWindow* hwnd, Input* in) : playerNum(playerNum), window(hwnd), input(in)
{
	position = sf::Vector2f(0.0f, 0.0f);
	velocity = sf::Vector2f(0.0f, 0.0f);
	speed = 700.0f;

	initPlayer();
}

Player::~Player()
{

}

void Player::handleInput(float dt)
{
	// Handle player input
}

void Player::networkUpdate(float dt, Connection* conn)
{
	//conn->receive();
}

void Player::update(float dt, PlayerDataMsg* msg)
{
	// update player
	//playerSprite.move(input->getLeftStick() * dt * speed);
	playerSprite.move(sf::Vector2f(msg->x * dt * speed, msg->y * dt * speed));
	collisionBox = sf::FloatRect(playerSprite.getPosition().x - size.x * 0.4f, playerSprite.getPosition().y - size.y * 0.4f, size.x * 0.8f, size.y * 0.8f);
	checkScreenBounds();
}

void Player::initPlayer()
{
	loadTexture();
	playerTexture.setSmooth(true);
	playerSprite.setTexture(playerTexture);
	size = playerTexture.getSize();
	playerSprite.setOrigin(size.x * 0.5f, size.y * 0.5f);
	//playerSprite.setOrigin(getTextureRect().width * 0.5f, getTextureRect().height * 0.5f);
	playerSprite.setPosition(sf::Vector2f(200.0f, 200.0f));
	playerSprite.setScale(0.75f, 0.75f);
	collisionBox = sf::FloatRect(playerSprite.getPosition().x - size.x * 0.4f, playerSprite.getPosition().y - size.y * 0.4f, size.x * 0.8f, size.y * 0.8f);
}

void Player::loadTexture()
{
	if (!playerTexture.loadFromFile("res/gfx/ships/altShip_" + std::to_string(playerNum) + ".png"))
	{
		std::cout << "Error loading ship texture " << playerNum << '\n';
	}
}

void Player::checkScreenBounds()
{
	// Check left edge of screen
	if (playerSprite.getPosition().x - (size.x * 0.5f) < 0)
	{
		playerSprite.setPosition(sf::Vector2f(size.x * 0.5f, playerSprite.getPosition().y));
	}

	// Check right edge of screen
	if (playerSprite.getPosition().x + (size.x * 0.5f) > window->getSize().x)
	{
		playerSprite.setPosition(sf::Vector2f(window->getSize().x - size.x * 0.5f, playerSprite.getPosition().y));
	}

	// Check top edge of screen
	if (playerSprite.getPosition().y - (size.y * 0.5f) < 0)
	{
		playerSprite.setPosition(sf::Vector2f(playerSprite.getPosition().x, size.y * 0.5f));
	}

	// Check btm edge of screen
	if (playerSprite.getPosition().y + (size.y * 0.5f) > window->getSize().y)
	{
		playerSprite.setPosition(sf::Vector2f(playerSprite.getPosition().x, window->getSize().y - size.y * 0.5f));
	}
}

void Player::sendData()
{

}

sf::Sprite* Player::getPlayerSprite()
{
	return &playerSprite;
}

sf::Vector2u* Player::getPlayerSize()
{
	return &size;
}

sf::FloatRect Player::getCollisionBox()
{
	return collisionBox;
}

void Player::setCollisionBox(float x, float y, float width, float height)
{
	collisionBox = sf::FloatRect(x, y, width, height);
}