#include "Connection.h"

Connection::Connection()
{
	
}

Connection::~Connection()
{

}

void Connection::createTCPListner()
{
	// Bind the listener to a port
	if (tcpListenerSocket.listen(SERVERPORT) != sf::Socket::Done)
	{
		std::cout << "Bind failed!\n";
	}
	else
	{
		std::cout << "Bind worked\n";
		std::cout << "Listening for connections on TCP, port number " << SERVERPORT << "\n";
	}
	
	// Add the listener to the selector
	selector.add(tcpListenerSocket);

	//acceptConnection(&tcpListener);
}

PlayerDataMsg* Connection::checkConnections()
{
	// Make the selector wait for data on any socket
	if (selector.wait())
	{
		// Test the listener
		if (selector.isReady(tcpListenerSocket))
		{
			// The listener is ready: there is a pending connection
			sf::TcpSocket* client = new sf::TcpSocket;

			if (tcpListenerSocket.accept(*client) == sf::Socket::Done)
			{
				// Add the new client to the clients list
				clientSockets.push_back(client);

				// Add the new client to the selector so that we will
				// be notified when they send something
				selector.add(*client);

				std::cout << "A new player has joined the game!\n";
			}
			else
			{
				// Error, we won't get a new connection, delete the socket
				delete client;
			}
		}
		else
		{
			// The listener socket is not ready, test all other sockets (the clients)
			for (auto it = clientSockets.begin(); it != clientSockets.end(); ++it)
			{
				sf::TcpSocket& client = **it;

				if (selector.isReady(client))
				{
					sf::Socket::Status status = client.receive(msgRecv, sizeof(PlayerDataMsg), received);

					if (status != sf::Socket::Done)
					{
						std::cout << "Server side error receiving using TCP!, error: " << status << "\n";
						break;
					}
					else
					{
						std::cout << "Server side received: " << received << " bytes\n";

						std::cout << "Player ID: " << msgRecv->playerID << '\n';
						std::cout << "Msg time sent: " << msgRecv->timeSent << '\n';
						std::cout << "Player X: " << msgRecv->x << '\n';
						std::cout << "Player Y: " << msgRecv->y << '\n';
					}

					// If we received data from player 1, relay that to player 2 and vice versa
					/*if (msgRecv->playerID == 1)
					{
						send(msgRecv, clientSockets[1]);
					}
					else if (msgRecv.playerID == 2)
					{
						send(&msgRecv, clientSockets[0]);
					}*/
				}
			}
		}
	}

	return msgRecv;
}

//void Connection::acceptConnection(sf::TcpListener* tcpListener)
//{
//	// Accept a new connection
//	if (tcpListener->accept(clientSocket) != sf::Socket::Done)
//	{
//		std::cout << "Accept failed!\n";
//	}
//	else
//	{
//		std::cout << "Accept worked!\n";
//	}
//}

void Connection::send(PlayerDataMsg* msg, sf::TcpSocket* clientSock)
{
	// Send data
	if (clientSock->send(msg, sizeof(PlayerDataMsg)) != sf::Socket::Done)
	{
		std::cout << "Error sending data by TCP!\n";
		return;
	}
	else
	{
		std::cout << "Player " << msg->playerID << " data sent from server to client " << clientSock->getRemotePort() << "\n";
	}
}

//void Connection::receive(sf::TcpSocket* clientSock)
//{
//	//playerData.push(msg);
//
//	PlayerDataMsg msgRecv;
//
//	if (clientSock->receive(&msgRecv, sizeof(PlayerDataMsg), received) != sf::Socket::Done)
//	{
//		std::cout << "Server side error receiving using TCP!\n";
//		return;
//	}
//
//	std::cout << "Server side received: " << received << " bytes\n";
//
//	std::cout << "Player ID: " << msgRecv.playerID << '\n';
//	std::cout << "Msg time sent: " << msgRecv.timeSent << '\n';
//	std::cout << "Player X: " << msgRecv.x << '\n';
//	std::cout << "Player Y: " << msgRecv.y << '\n';
//
//	// Alter data slightly to ensure this works
//	/*msgRecv.playerID = 2;
//	msgRecv.timeSent += 10;
//	msgRecv.x += 20;
//	msgRecv.y += 20;*/
//
//	// If we received data from player 1, relay that to player 2 and vice versa
//	if (msgRecv.playerID == 1)
//	{
//		send(&msgRecv, clientSockets[1]);
//	}
//	else if (msgRecv.playerID == 2)
//	{
//		send(&msgRecv, clientSockets[0]);
//	}
//}

//void Connection::updatePlayers()
//{
//	// Check for incoming data
//	// Send player data (1->2) (2->1) use id's
//	// Send asteroid data to all players
//}