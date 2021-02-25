#include <iostream>
#include <thread>
#include "TCPSocket.hpp"

void SendFile(std::string Filename, char* addrToConnect) {
	char temp[1024] = "\0";
	//std::string temp;
	TCPSocket my_socket;
	my_socket.CreateTCPSocket();

	sockaddr_in dest_addr = my_socket.ConvertToAddr(addrToConnect);
	FILE* inFile;
	if (my_socket.Connect(dest_addr) == 1) {
		errno_t err = fopen_s(&inFile, Filename.c_str(), "rb");
		std::cout << Filename;
		if (my_socket.Send(Filename.c_str(), strlen(Filename.c_str()))) {

			fseek(inFile, 0, SEEK_END);
			unsigned long fileSize = ftell(inFile);
			rewind(inFile);

			char sizeInChar[10] = { 0 };
			err = _itoa_s(fileSize, sizeInChar, 10);
			unsigned long alreadySent = 0;

			my_socket.Send(sizeInChar, sizeof(sizeInChar));

			while (alreadySent < fileSize) {
				int realBytes = fread(temp, 1, sizeof(temp), inFile);
				alreadySent += realBytes;
				my_socket.Send(temp, sizeof(temp));
			}
		}
		fclose(inFile);
	}
	my_socket.CloseTCPSocket(my_socket.my_socket);
}

int AcceptFile() {
	TCPSocket my_socket;
	my_socket.CreateTCPSocket();

	my_socket.Bind();
	my_socket.Listen(32);
	char temp[1024] = "\0";
	char filename[40] = "\0";
	SOCKET* sock = my_socket.Accept(my_socket.client_addr);
	FILE* outFile;
	if (WSAGetLastError() == 0) {		
		printf("Connect accepted");
		if (my_socket.Receive(filename, sizeof(filename))) {
			unsigned long fileSize = 0;
			int bytesReceived;
			char sizeInChar[10] = { 0 };

			if (my_socket.Receive(sizeInChar, sizeof(sizeInChar))) {
				fileSize = _atoi64(sizeInChar);
				errno_t err = fopen_s(&outFile, filename, "wb");
				unsigned long bytesCount = 0;
				while (bytesReceived = my_socket.Receive(temp, sizeof(temp))) {					
					fwrite(temp, 1, bytesReceived, outFile);
					bytesCount += bytesReceived;
				}
				fclose(outFile);
				my_socket.CloseTCPSocket(my_socket.my_socket);
				return 0;
			}
		}
	}
	return -1;
}


std::string Filename;


int Choose_Menu() {
	int choice;
	std::cout << "\n[1] SetFilename,\n[2] SendFile,\n[3] AcceptFile,\n[4]Exit program\n";
	std::cin >> choice;

	switch (choice) {
	case 1: {
		std::cin >> Filename;
		break;
	}
	case 2: {
		//char addr[14] = "192.168.43.";
		/*std::string LastBytes;
		std::cin >> LastBytes;*/
		//strcat_s(addr, LastBytes.c_str());
		char addr[14] = "127.0.0.1";
		SendFile(Filename, addr);
		break; }
	case 3: {
		AcceptFile();
		break;
	}
	case 4: return 0;
	case 5: {

		/*char temp[40] = "\0";
		std::cout << Filename;
		while (input.getline(temp, sizeof(temp))) {
			std::cout << temp;
		}*/
		break;
	}
	default: break;
	}
	//system("cls");
	return 1;
}

int main()
{
	//Filename = "1.png";
	Filename = "filenameToSend.txt";

	while (Choose_Menu());
	return 0;
}