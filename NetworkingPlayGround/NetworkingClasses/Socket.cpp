#include "Socket.h"
#include <iostream>
#include <string>

bool quit = false;

bool Socket::InitSocket()
{
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
}

void Socket::ShutDownSockets()
{
	quit = true;
	WSACleanup();
}

Socket::Socket()
{

}

Socket::Socket(SocketType type)
{
	if (type == SocketType::UDP) 
	{
		socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


		if (socketHandle <= 0)
		{
			printf("We fucked up making a socket my guy");
		}
	}
	else if(type == SocketType::TCP)
	{

	}
	sendingThread = std::thread(&Socket::SendThread, this);
	recievingThread = std::thread(&Socket::RecieveThread, this);
}


Socket::~Socket()
{
}

void Socket::SendThread()
{
	while (!quit)
	{
		std::string input;

		std::getline(std::cin, input);
		//std::cin >> input;

		if (input[0] == 'q')
		{
			quit = true;
		}
		else
		{
			Send(&input[0], sizeof(input), Address(127, 0, 0, 1, GetPortNumber()));
		}
	}
}

void Socket::RecieveThread()
{
	while (!quit)
	{
		unsigned char buffer[256];
		unsigned int bufferSize = sizeof(buffer);
		int result = Recieve(buffer, bufferSize);
	}
}

bool Socket::OpenBind(int portNum, bool useLocal)
{
	Address newAdd = Address(127, 0, 0, 1, portNum);
	port = portNum;

	// Setup the description of the address
	addressIN;
	addressIN.sin_family = AF_INET;
	addressIN.sin_addr.S_un.S_addr = (ULONG)newAdd.GetAddress();
	addressIN.sin_port = htons((unsigned short)portNum);

	if (useLocal) 
	{
		addressIN.sin_addr.S_un.S_addr = INADDR_ANY;
	}

	// Bind address/port to socket
	int finishBind = bind(socketHandle, (const sockaddr*)&addressIN, sizeof(sockaddr_in));

	if (finishBind < 0)
	{
		printf("We done fucked up the binding bro");
		return false;
	}
	return true;
}

bool Socket::OpenConnect(int portNum, bool useLocal)
{
	Address newAdd = Address(127, 0, 0, 1, portNum);
	port = portNum;

	// Setup the description of the address
	addressIN;
	addressIN.sin_family = AF_INET;
	addressIN.sin_addr.S_un.S_addr = (ULONG)newAdd.GetAddress();
	addressIN.sin_port = htons((unsigned short)portNum);
	if (useLocal)
	{
		addressIN.sin_addr.S_un.S_addr = INADDR_ANY;
	}

	// Bind address/port to socket
	int finishConnect = connect(socketHandle, (const sockaddr*)&addressIN, sizeof(sockaddr_in));

	if (finishConnect < 0)
	{
		printf("We done fucked up the binding bro");
		return false;
	}
	return true;
}

void Socket::Close()
{
}

bool Socket::IsOpen()
{
	return false;
}

bool Socket::Send(const char data[], unsigned int packetSize, Address newAddress)
{
//	printf("packetSize: %i", packetSize);

	sockaddr_in newAddr;
	newAddr.sin_family = AF_INET;
	newAddr.sin_addr.S_un.S_addr = htonl(newAddress.GetAddress());
	newAddr.sin_port = htons(newAddress.GetPort());

	// Sending data
	int sent_bytes = sendto(socketHandle, data, packetSize, 0, (const sockaddr*)&newAddr, sizeof(sockaddr_in));

	if (sent_bytes != packetSize)
	{
		//printf("This fuck shit did not send boiiii");
		return false;
	}

	return true;
}

int Socket::Recieve(unsigned char buffer[], unsigned int bufferSize)
{
	sockaddr_in fromAddress;
	socklen_t length = sizeof(fromAddress);

	int bytes = recvfrom(socketHandle, (char*)buffer, bufferSize, 0, (sockaddr*)&fromAddress, &length);

	if (bytes <= 0)
	{
		//printf("We aint recieve shit");
		return 0;
	}

	unsigned int fromAddr = ntohl(fromAddress.sin_addr.S_un.S_addr);
	unsigned short port = ntohs(fromAddress.sin_port);

	//Address fromAddress = Address(fromAddr, port);

	printf("Recieved Message: %s\n", &buffer[0]);

	return 1;
}

bool Socket::SetNonBlockingMode()
{
	DWORD nonBlocking = 1;

	int blockingChange = ioctlsocket(socketHandle, FIONBIO, &nonBlocking);

	if (blockingChange != 0)
	{
		printf("RIP, BLOCK CHANGING FUCKED UP SOMEHOW");
		return false;
	}

	return true;
}

sockaddr_in Socket::GetSocketAddressIn()
{
	return addressIN;
}

int Socket::GetSocketHandle()
{
	return socketHandle;
}

int Socket::GetPortNumber()
{
	return port;
}

void Socket::JoinThreads()
{
	sendingThread.join();
	recievingThread.join();
}
