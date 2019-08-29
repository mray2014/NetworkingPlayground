#include "stdafx.h"
#include "Socket.h"

bool Socket::InitSocket()
{
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
}

void Socket::ShutDownSockets()
{
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

}


Socket::~Socket()
{
}

bool Socket::Open(int portNum)
{
	port = portNum;

	// Setup the description of the address
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.S_un.S_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short)portNum);

	// Bind address/port to socket
	int finishBind = bind(socketHandle, (const sockaddr*)&address, sizeof(sockaddr_in));

	if (finishBind < 0)
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

int Socket::GetSocketHandle()
{
	return 0;
}

int Socket::GetPortNumber()
{
	return port;
}
