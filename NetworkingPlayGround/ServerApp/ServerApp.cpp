// ServerApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Socket.h"
#include <string>
#include <iostream>

int main()
{
	bool init = Socket::InitSocket();
	
	int person1 = -1;
	int person2 = -1;

	if (!init)
	{
		printf("Socket initializing fucked up");
	}

	Socket* severSocket = new Socket(SocketType::UDP);
	severSocket->OpenBind(8080, true);

	bool quit = false;

	while (!quit)
	{
		if (listen(severSocket->GetSocketHandle(), 2) >= 0)
		{
			sockaddr_in severAddressIN = severSocket->GetSocketAddressIn();
			int newSocket = accept(severSocket->GetSocketHandle(), (sockaddr*)&severAddressIN, (socklen_t *)sizeof(severAddressIN));
			std::string question = "Joining Room 1";
			send(newSocket, &question[0], sizeof(question), 0);

			if (person1 == -1)
			{
				person1 = newSocket;
			}
			else if(person2 == -1)
			{
				person2 = newSocket;
			}
			else {
				std::string question = "Can't join Room 1, to many people!";
				send(newSocket, &question[0], sizeof(question), 0);
			}

			/*std::string question = "What room do you want to join?\n";
			send(newSocket, &question[0], sizeof(question), 0);

			std::string input;
			std::getline(std::cin, input);*/
		}

		if (person1 != 1 && person2 != 1)
		{
			sockaddr_in fromAddress;
			socklen_t length = sizeof(fromAddress);
			char buffer[1024] = { 0 };
			int bytes = recvfrom(person1, (char*)buffer, 1024, 0, (sockaddr*)&fromAddress, &length);

			if (bytes > 0)
			{
				send(person2, &buffer[0], sizeof(buffer), 0);
			}

			sockaddr_in fromAddress2;
			socklen_t length2 = sizeof(fromAddress2);
			char buffer2[1024] = { 0 };
			int bytes2 = recvfrom(person2, (char*)buffer2, 1024, 0, (sockaddr*)&fromAddress2, &length2);

			if (bytes2 > 0)
			{
				send(person1, &buffer2[0], sizeof(buffer2), 0);
			}
		}
	}

}


