#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "main.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

extern char* Upper_DNS;
const char* Local_Host = "127.0.0.1";
vector<struct Waiting>Buffer;
extern int debug_level;

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
		int LEN = recvfrom(sServer, recvData, sizeof(recvData), 0, (sockaddr*)& client, &len);

		if (LEN == -1)
			continue;

		if (debug_level == 2)
			cout << "接收到:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "的消息" << endl;
		for (int i = 0; i < LEN; i++)
			printf("%02x ", (unsigned  char)recvData[i]);

		cout << endl;

		if (debug_level == 1)
		{
			
		}
		else if (debug_level == 2)
		{
			DNSheader header;
			if (Get_Header(header, recvData) == true)
			{
				char _QR = 0x80, _AA = 0x04, _TC = 0x02, _RD = 0x01, _RA = 0x80;
				int QR, OPCODE, AA, TC, RD, RA, ZZ, RCODE;
				if (recvData[2] & _QR) QR = 1; else QR = 0;			// QR
				if (recvData[2] & _AA) AA = 1; else AA = 0;			// AA
				if (recvData[2] & _TC) TC = 1; else TC = 0;			// TC
				if (recvData[2] & _RD) RD = 1; else RD = 0;			// RD
				if (recvData[3] & _RA) RA = 1; else RA = 0;			// RA
				// RCODE
				if ((recvData[3] & (char)0x0f) == (char)0x03) RCODE = 3;
				else if ((recvData[3] & (char)0x0f) == (char)0x00) RCODE = 0;
				else RCODE = -1;
				// ZZ
				if ((recvData[3] & (char)0x70) == (char)0x00) ZZ = 0;
				else ZZ = -1;
				// OPCODE
				if ((recvData[2] & (char)0x78) == (char)0x00) OPCODE = 0;
				else if ((recvData[2] & (char)0x78) == (char)0x08) OPCODE = 1;
				else if ((recvData[2] & (char)0x78) == (char)0x10) OPCODE = 2;
				else OPCODE = -1;

				cout << endl;
				cout << "----------------------------------------------------------------------" << endl;
				// 打印报文
				if (QR == 0) cout << "查询报文 ";
				if (QR == 1) cout << "响应报文 ";
				cout << "DNS: " << endl;
				for (int i = 0; i < LEN; i++)
				{
					printf("%02x ", recvData[i]);
					if ((i + 1) % 8 == 0)
						printf("\n");
				}
				// 打印参数
				cout << endl;
				cout << "*ID: " << header.ID << endl;
				cout << "*QR: " << QR << "  *OPCODE: " << OPCODE << "  *AA: " << AA << "  *TC: " << TC << "  *RD: " << RD << "  *RA: " << RA << "  *Z: " << ZZ << "  *RCODE: " << RCODE << endl;
				cout << "*QDCOUNT: " << header.QDCOUNT << "  *ANCOUNT: " << header.ANCOUNT << "  *NSCOUNT: " << header.NSCOUNT << "  *ARCOUNT: " << header.ARCOUNT << endl;
				cout << "----------------------------------------------------------------------" << endl;
			}
			

		}

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
					cout << "发送给:" << inet_ntoa((*it).clientaddr.sin_addr) << ":" << ntohs((*it).clientaddr.sin_port) << endl;

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
				SEARCH_RESULT result = Search(DomainName, IPaddr);

				if (result == Find)
					//找到，应发送IP
				{
					int index, index0 = 12;
					string recv, send;
					string	Header, Query, Answer;
					// 响应报Header
					for (int i = 0; i < LEN; i++)
						recv.push_back(recvData[i]);
					Header = recv.substr(0, 12);
					Header[2] = (char)0x85; Header[3] = (char)0x80; Header[7] = (char)0x01;
					index = recv.find_last_of(0x01);	// 查找recvData中Query部分结尾的index
					Query = recv.substr(index0, index - 11); 	// 响应报Query
					Answer = recv.substr(index0, index - 11);
					string TTL_L = {0x00,0x00,0x00,0x78,0x00,0x04};
					send = Header + Query + Answer + TTL_L + get_ip(IPaddr);
					//char sendData[1024];
					char* sendData = const_cast<char*>(send.c_str());

					cout << "发送给:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << endl;
					sendto(sServer, sendData, send.size(), 0, (sockaddr*)& client, len);
					continue;
				}
				else if (result == Block)
					//应屏蔽
				{
					// Flag位应该设置为8583
					recvData[2] = (char)0x85;
					recvData[3] = (char)0x83;

					cout << "发送给:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << endl;
					sendto(sServer, recvData, LEN, 0, (sockaddr*)& client, len);
					continue;
				}
			}
			
			//查找类型非IPV4或在对照表中未找到，应向原DNS中继
			Waiting user;
			user.clientaddr = client;
			user.ID = header.ID;
			//strcpy(user.query, DomainName);

			Buffer.push_back(user);

			cout << "发送给:" << inet_ntoa(UP_DNS.sin_addr) << ":" << ntohs(UP_DNS.sin_port) << endl;
			//将原数据包直接发送给原DNS
			sendto(sServer, recvData, LEN, 0, (sockaddr*)& UP_DNS, len);
		}

	}

	closesocket(sServer);
	WSACleanup();
	return ;
}