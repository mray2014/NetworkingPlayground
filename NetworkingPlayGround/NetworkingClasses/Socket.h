#pragma once

#pragma comment( lib, "wsock32.lib" )

#include <WinSock2.h>
#include <thread>
#include "Address.h"

typedef int socklen_t;

enum SocketType 
{
	TCP,
	UDP
};

class Socket
{
public:
	
	static bool InitSocket();
	static void ShutDownSockets();

	Socket();
	Socket(SocketType type);
	~Socket();

	void SendThread();
	void RecieveThread();

	bool OpenBind(int portNum, bool useLocal = false);
	bool OpenConnect(int portNum, bool useLocal = false);
	void Close();

	bool IsOpen();

	bool SetNonBlockingMode();

	sockaddr_in GetSocketAddressIn();
	int GetSocketHandle();
	int GetPortNumber();

	void JoinThreads();

	std::thread sendingThread;
	std::thread recievingThread;

private:
	bool Send(const char data[], unsigned int packetSize, Address newAddress);
	int Recieve(unsigned char buffer[], unsigned int bufferSize);

	SocketType socketType;
	sockaddr_in addressIN;
	int socketHandle;
	int port;
};

