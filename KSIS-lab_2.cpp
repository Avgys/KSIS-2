#include <iostream>
#include <fstream>
#include <thread>
#include "TCPSocket.hpp"


DWORD WINAPI WorkWithClient(SOCKET* socket);
DWORD __stdcall WorkWithServer(SOCKET* socket);

bool isUserver = 0;

std::thread* ClientThread;
std::thread* ServerThread;
std::string IpInput;
bool isUconnected = 0;

TCPSocket* initSocket(int type,TCPSocket &my_socket) {
	my_socket.CreateTCPSocket();	
	if (type) {

		my_socket.Bind();
		isUserver = 1;
		my_socket.Listen(32);
		SOCKET* sock = my_socket.Accept(my_socket.client_addr);
		if (WSAGetLastError() == 0) {
			return &my_socket;
		}
		
	}
	else {
		isUserver = 0;
		char addr[16] = "192.168.43.4";

		sockaddr_in dest_addr = my_socket.ConvertToAddr(addr);;
		if (IpInput.length() > 0 && IpInput.length() < 17) {
			strcpy_s(addr, IpInput.c_str());
			sockaddr_in temp_dest_addr = my_socket.ConvertToAddr(addr);
			if ((strlen(addr) > 0 && strlen(addr) < 17) && strlen(addr) > 0)
				dest_addr = temp_dest_addr;
			else {
				IpInput = "Incorrect";
			}
		}
		if (my_socket.Connect(dest_addr) == 1) {
			my_socket.client_socket = my_socket.my_socket;
			return &my_socket;
		}
	}
	isUconnected = 0;
	isUserver = 1;
}

DWORD WINAPI WorkWithClient(TCPSocket &socket)
{

	isUconnected = 1;
	char buff[1024];
	std::string str;

	int bytes_recv;
	// возвращение ее клиенту
	while (isUconnected)
	{		//Receive Part
		{	

			strcpy_s(buff, str.c_str());	//Send Part		
			if (socket.Send(&buff[0], str.length()) <= 0) {
				printf("-disconnect\n");
				break;
			}
		}
	}

	isUConnected = 0;
	return 0;
}
//
DWORD __stdcall WorkWithServer(SOCKET* socket)
{
//	char buf[1024];
//	std::string str;
//	while (isUConnected)
//	{
//		memset(buf, 0, sizeof(buf));
//		//Receive Part
//		GameSocket.Receive(buf, sizeof(buf));
//		str = buf;
//		if (str.length() >= 20) {
//			
//		}
//		memset(buf, 0, sizeof(buf));
//
//		str = to_string(MyPosition.y);
//		strcpy_s(buf, str.substr(0, 7).c_str());
//		Sleep(1);
//		if (GameSocket.Send(buf, (int)strlen(buf)) < 0) {
//			printf("-disconnect\n");
//			break;
//		}
//	}
//	isUConnected = 0;
	return 0;
}

void SendFile(std::string Filename) {	
	std::fstream input(Filename, std::ios_base::in | std::ios_base::binary);
	char temp;
	while (input.read(&temp, sizeof(char)))
		//std::cout << temp;
		socket.Send(&buff[0], str.length()) <= 0
}

int Choose_Menu() {
	int choice;
	std::cout << "[1] SendFile \n";
	std::cin >> choice;
	
	std::string Filename;
	switch (choice) {
	case 1: {
			//std::cin >> Filename;
		Filename = "filename.txt";
			break; 
			}
	case 2: break;
	case 9: return 0;
	default: break;
	}
	system("cls");
	return 1;
}

int main()
{	
	while(Choose_Menu());
}