#pragma once

#pragma comment( lib, "ws2_32.lib" )
#include < Windows.h>
#include < conio.h>
//#include <WinSock2.h>


#define PORT 50004

class TCPSocket
{
public:
	sockaddr_in local_addr;
	int local_addr_size;
	sockaddr_in dest_addr;

	SOCKET my_socket;

	SOCKET client_socket;    // сокет для клиента
	sockaddr_in client_addr;
	int client_addr_size;
	HOSTENT* hst;
	int isBinded;

	int Connect(const sockaddr_in inAddress);
	int Bind();
	int Listen(int inBackLog = 32);
	SOCKET* Accept(sockaddr_in client_addr);
	int Send(const void* inData, int inLen);
	int Receive(void* inBuffer, int inLen);
	sockaddr_in ConvertToAddr(char* DestAddr);
	SOCKET* CreateTCPSocket();
	int CloseTCPSocket(SOCKET socket);

	TCPSocket();
	~TCPSocket();
private:

};