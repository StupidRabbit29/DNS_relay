#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include "main.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;
void DNSServer()
{
	//windows socket
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		return ;
	}

	//�����׽���
	SOCKET sServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sServer == INVALID_SOCKET)
	{
		cout << "socket error!" << endl;
		return ;
	}

	//��IP�Ͷ˿�
	SOCKADDR_IN local;
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sServer, (struct sockaddr*) & local, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "bind error!" << endl;
	}

	SOCKADDR_IN client;
	int len = sizeof(SOCKADDR_IN);

	while (true)
	{
		char recvData[MSGSIZE] = { '\0' };
		recvfrom(sServer, recvData, sizeof(recvData), 0, (sockaddr*)& client, &len);
		cout << recvData << endl;

		//if (MyDEBUG)
			//cout << "���յ�:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "����Ϣ" << endl;

		//test
		//Sleep(10000);

		char sendData[MSGSIZE] = { '\0' };
		strcpy_s(sendData, "Hello Client!");
		sendto(sServer, sendData, sizeof(sendData), 0, (sockaddr*)& client, len);
	}

	closesocket(sServer);
	WSACleanup();
	return ;
}