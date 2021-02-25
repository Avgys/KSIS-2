#include "TCPSocket.hpp"
#pragma once
#include <iostream>

TCPSocket::TCPSocket() {
	char* buff = new char[1024];
	if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
	{
		// Ошибка!
		printf("Error WSAStartup %d\n",
			WSAGetLastError());
	}
}

TCPSocket::~TCPSocket() {

}

SOCKET* TCPSocket::CreateTCPSocket() {
	isBinded = 0;	
	if ((my_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) // Шаг 2 - создание сокета
	{
		// Ошибка!
		printf("Error socket %d\n", WSAGetLastError());
		WSACleanup();
		// Деиницилизация библиотеки Winsock
		return &my_socket;
	}
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORT);
	local_addr.sin_addr.s_addr = 0;

	dest_addr.sin_addr.s_addr = 0;;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	return &my_socket;
}

int TCPSocket::Connect(sockaddr_in dest_addr)
{	
	int err = connect(my_socket, (sockaddr*)&dest_addr, sizeof(dest_addr));
	if (err < 0)
	{
		printf("Connect error %d\n", err = WSAGetLastError());
		return -1;
	}
	return 1;
}
int TCPSocket::Listen(int inBackLog)
{
	int err = listen(my_socket, inBackLog);
	if (err < 0)
	{
		printf("Error listen %d\n", err = WSAGetLastError());
		return -WSAGetLastError();
	}
	return NO_ERROR;
}

SOCKET* TCPSocket::Accept(sockaddr_in client_addr)
{
	client_addr_size = sizeof(client_addr);
	client_socket = accept(my_socket, (sockaddr*)&client_addr, &client_addr_size);
	if (client_socket != INVALID_SOCKET)
	{
		return (SOCKET*)&client_socket;
	}
	else
	{
		WSAGetLastError();
		return nullptr;
	}
}

int TCPSocket::Send(const void* inData, int inLen) {
	int bytesSentCount = send(client_socket, static_cast<const char*>(inData), inLen, 0);
	if (bytesSentCount < 0)
	{
		printf("TCPSocket::Send");
		return -WSAGetLastError();
	}
	return bytesSentCount;
}

int TCPSocket::Bind(){	
	if (isBinded) {
		printf("Allready binded");
		return NO_ERROR;
	}
	else {
		isBinded = 1;
		
		local_addr_size = sizeof(local_addr);
		int err = bind(my_socket, (sockaddr*)&local_addr, sizeof(local_addr));
		if (err < 0)
		{
			printf("TCPSocket::Bind");
			return -(err = WSAGetLastError());
		}
	}
	return NO_ERROR;
}

int TCPSocket::Receive(void* inData, int inLen)
{
	int bytesReceivedCount = recv(client_socket,
		static_cast<char*>(inData), inLen, 0);
	if (bytesReceivedCount < 0)
	{
		printf("TCPSocket::Receive");
		return -WSAGetLastError();
	}
	return bytesReceivedCount;
}

sockaddr_in TCPSocket::ConvertToAddr(char* DestAddr)
{
	HOSTENT* hst;
	if (inet_addr(DestAddr) != INADDR_NONE)
				dest_addr.sin_addr.s_addr = inet_addr(DestAddr);
		else
			// попытка получить IP адрес по доменному имени сервера
			if (hst = gethostbyname(DestAddr))
				// hst->h_addr_list массив указателей на адреса
				((unsigned long*)&dest_addr.sin_addr)[0] =
				((unsigned long**)hst->h_addr_list)[0][0];
			else
			{
				printf("Invalid address %s\n", DestAddr);
				*DestAddr = '\0';
				return dest_addr;
			}
	return dest_addr;
}

int TCPSocket::CloseTCPSocket(SOCKET socket)
{
	if (isBinded)
		isBinded = 0;
	if (closesocket(socket)) {
		std::cout << (" TCPSocket::CloseTCPSocket ", WSAGetLastError());
		return -WSAGetLastError();
	}
	return 0;
}
