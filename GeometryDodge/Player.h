#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include "Connection.h"

class Player : sf::Sprite
{
public:
	Player() {}
	Player(int playerNum, sf::RenderWindow* hwnd, Input* in);
	~Player();

	void handleInput(float dt);
	void networkUpdate(float dt, Connection* conn);
	void update(float dt, PlayerDataMsg* msg);
	sf::Sprite* getPlayerSprite();
	sf::Vector2u* getPlayerSize();
	sf::FloatRect getCollisionBox();
	void setCollisionBox(float x, float y, float width, float height);
	

private:
	void initPlayer();
	void loadTexture();
	void checkScreenBounds();
	void sendData();

	Input* input;
	sf::RenderWindow* window;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2u size;
	sf::FloatRect collisionBox;

	int playerNum;
	float speed;
};