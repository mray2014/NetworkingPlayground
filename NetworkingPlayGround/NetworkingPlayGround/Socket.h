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

	bool Open(int portNum);
	void Close();

	bool IsOpen();

	bool SetNonBlockingMode();

	int GetSocketHandle();
	int GetPortNumber();

	void JoinThreads();

	std::thread sendingThread;
	std::thread recievingThread;

private:
	bool Send(const char data[], unsigned int packetSize, Address newAddress);
	int Recieve(unsigned char buffer[], unsigned int bufferSize);

	SocketType socketType;
	int socketHandle;
	int port;
};

