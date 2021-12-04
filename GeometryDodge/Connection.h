#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "PlayerDataMsg.h"
#include <queue>
#include <vector>

class Connection
{
public:
	Connection();
	~Connection();

	void createTCPListner();
	PlayerDataMsg* checkConnections();
	//void acceptConnection(sf::TcpListener* tcpListener);
	void send(PlayerDataMsg* msg, sf::TcpSocket* clientSock);
	/*void receive(sf::TcpSocket* clientSock);
	void updatePlayers();*/

private:
	//sf::IpAddress SERVERIP = "127.0.0.1";
	unsigned short SERVERPORT = 5555;
	std::queue<PlayerDataMsg> playerData;
	std::size_t received;
	//sf::TcpSocket clientSocket;
	sf::TcpListener tcpListenerSocket;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clientSockets;
	PlayerDataMsg* msgRecv = new PlayerDataMsg;
};