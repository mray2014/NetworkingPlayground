// ClientApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Socket.h"
#include <string>
#include <iostream>

int main()
{
	bool init = Socket::InitSocket();

	if (!init)
	{
		printf("Socket initializing fucked up");
	}


	Socket* severSocket = new Socket(SocketType::UDP);
	severSocket->OpenConnect(8080);

	bool quit = false;

	while (!quit) {

	}
}

