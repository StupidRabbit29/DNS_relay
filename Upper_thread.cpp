#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include "main.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

extern char* Upper_DNS;
extern const char* Local_Host;
extern vector<struct Waiting>Buffer;
extern int debug_level;

extern SOCKET sServer;
extern SOCKET UpperDNS;
extern SOCKADDR_IN local;
extern SOCKADDR_IN UP_DNS;

extern bool Llocks;
extern bool Llockr;
extern bool Rlocks;
extern bool Rlockr;

unsigned __stdcall remote(void* pArguments)
{
	SOCKADDR_IN client;
	int len = sizeof(SOCKADDR_IN);

	while (true)
	{
		char recvData[MSGSIZE] = { '\0' };
		int LEN;

		while (true)
		{
			if (!Rlocks)
			{
				Rlockr = true;
				LEN = recvfrom(UpperDNS, recvData, sizeof(recvData), 0, (sockaddr*)& client, &len);
				Rlockr = false;
				break;
			}
			Sleep(20);
		}

		if (LEN == -1)
			continue;

		DNSheader header;
		char DomainName[100] = { '\0' };

		cout << "Packet from Upper DNS!" << endl;

		//È¡header
		if (Get_Header(header, recvData) == false)
			continue;

		unsigned short temp = ntohs(header.ID);

		for (auto it = Buffer.begin(); it != Buffer.end(); it++)
			if ((*it).tempID == temp)
			{
				cout << "·¢ËÍ¸ø:" << inet_ntoa((*it).clientaddr.sin_addr) << ":" << ntohs((*it).clientaddr.sin_port) << endl;
				
				memcpy(recvData, &(*it).ID, sizeof(unsigned short));
				
				Llocks = true;
				while (true)
				{
					if (!Llockr)
					{
						sendto(sServer, recvData, LEN, 0, (sockaddr*) & ((*it).clientaddr), len);
						break;
					}
					Sleep(20);
				}
				Llocks = false;
				
				Buffer.erase(it);
				break;
			}

	}

	closesocket(UpperDNS);

	_endthreadex(0);
	return 0;
}










