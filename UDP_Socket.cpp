#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include "main.h"

#pragma comment(lib, "ws2_32.lib")

extern string Upper_DNS;
string Local_Host = "127.0.0.1";

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

	//�����м�DNS��IP�Ͷ˿�
	SOCKADDR_IN local;
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = inet_addr(Local_Host.c_str());
	//serAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

	//����ԭDNS��IP�Ͷ˿�
	SOCKADDR_IN UP_DNS;
	UP_DNS.sin_family = AF_INET;
	UP_DNS.sin_port = htons(PORT);
	UP_DNS.sin_addr.s_addr = inet_addr(Upper_DNS.c_str());

	if (bind(sServer, (struct sockaddr*) & local, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "bind error!" << endl;
	}

	SOCKADDR_IN client;
	int len = sizeof(SOCKADDR_IN);

	while (true)
	{
		char recvData[MSGSIZE] = { '\0' };
		memset(client, 0, sizeof(SOCKADDR_IN));
		recvfrom(sServer, recvData, sizeof(recvData), 0, (sockaddr*)& client, &len);
		//cout << recvData << endl;

		//�������ݱ�����Դ
		if (client.sin_addr == UP_DNS.sin_addr)
		{
			//ȡheader


		}
		else if (client.sin_addr == local.sin_addr)
		{
			//ȡheader

			//ȡ����


		}

	}

	closesocket(sServer);
	WSACleanup();
	return ;
}