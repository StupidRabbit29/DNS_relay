#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "main.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

extern char* Upper_DNS;
const char* Local_Host = "127.0.0.1";
vector<struct Waiting>Buffer;

void DNSServer()
{
	//windows socket
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		return ;
	}

	//创建套接字
	SOCKET sServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sServer == INVALID_SOCKET)
	{
		cout << "socket error!" << endl;
		return ;
	}

	//设置中继DNS的IP和端口
	SOCKADDR_IN local;
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = inet_addr(Local_Host);
	//serAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

	//设置原DNS的IP和端口
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

		DNSheader header;
		char DomainName[100] = { '\0' };
		//分析数据报的来源
		if (client.sin_addr.s_addr == UP_DNS.sin_addr.s_addr)//????????
		{
			//取header
			if (Get_Header(header, recvData) == false)
				continue;

			for (auto it = Buffer.begin(); it != Buffer.end(); it++)
				if ((*it).ID == header.ID && (*it).clientaddr.sin_addr.s_addr == client.sin_addr.s_addr)
				{
					sendto(sServer, recvData, sizeof(recvData), 0, (sockaddr*)&((*it).clientaddr), len);
					Buffer.erase(it);
					break;
				}
		}
		else// if (client.sin_addr == local.sin_addr)//????????
		{
			//取header
			if (Get_Header(header, recvData) == false)
				continue;

			//取问题
			QUERY_KIND query_kind = Get_Query(DomainName, recvData);

			//问题类型
			if (query_kind == Ipv4)
				//查找IPV4的IP
			{
				char IPaddr[20] = { '\0' };
				//查表
				SEARCH_RESULT result = Serach(DomainName, IPaddr);

				if (result == Find)
					//找到，应发送IP
				{

					continue;
				}
				else if (result == Block)
					//应屏蔽
				{

					continue;
				}
			}
			
			//查找类型非IPV4或在对照表中未找到，应向原DNS中继
			Waiting user;
			user.clientaddr = client;
			user.ID = header.ID;
			//strcpy(user.query, DomainName);

			Buffer.push_back(user);

			//将原数据包直接发送给原DNS
			sendto(sServer, recvData, sizeof(recvData), 0, (sockaddr*)& client, len);
		}

	}

	closesocket(sServer);
	WSACleanup();
	return ;
}