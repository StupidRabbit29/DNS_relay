#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "main.h"

#pragma comment(lib, "ws2_32.lib")

extern char* Upper_DNS;
const char* Local_Host = "127.0.0.1";
vector<Waiting>Buffer;


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
	local.sin_addr.s_addr = inet_addr(Local_Host);
	//serAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

	//����ԭDNS��IP�Ͷ˿�
	SOCKADDR_IN UP_DNS;
	UP_DNS.sin_family = AF_INET;
	UP_DNS.sin_port = htons(PORT);
	UP_DNS.sin_addr.s_addr = inet_addr(Upper_DNS);

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
		//cout << recvData << endl;

		//�������ݱ�����Դ
		if (client.sin_addr.s_addr == UP_DNS.sin_addr.s_addr)//????????
		{
			//ȡheader


		}
		else// if (client.sin_addr == local.sin_addr)//????????
		{
			//ȡheader

			//ȡ����


			//��������
			if (== Ipv4)
				//����IPV4��IP
			{
				if (== Find)
					//�ҵ���Ӧ����IP
				{

					continue;
				}
				else if (== Block)
					//Ӧ����
				{

					continue;
				}
			}
			

			//�������ͷ�IPV4���ڶ��ձ���δ�ҵ���Ӧ��ԭDNS�м�



		}

	}

	closesocket(sServer);
	WSACleanup();
	return ;
}