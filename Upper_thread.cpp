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

		/*while (true)
		{
			if (!Rlocks)
			{
				Rlockr = true;*/
				LEN = recvfrom(UpperDNS, recvData, sizeof(recvData), 0, (sockaddr*)& client, &len);
		//		Rlockr = false;
		//		break;
		//	}
		//	//Sleep(20);
		//}
		
		if (LEN == -1)
			continue;

		DNSheader header;
		char DomainName[100] = { '\0' };





		cout << "Packet from Upper DNS!" << endl;
		DNSheader headerhh;
		if (Get_Header(headerhh, recvData) == true)
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
			cout << "接收到:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "的消息" << endl;
			// 打印报文
			if (QR == 0) cout << "查询报文 ";
			if (QR == 1) cout << "响应报文 ";
			cout << "DNS: " << endl;
			for (int i = 0; i < LEN; i++)
			{
				printf("%02x ", (unsigned  char)recvData[i]);
				if ((i + 1) % 8 == 0)
					printf("\n");
			}
			// 打印参数
			cout << endl;
			cout << "*ID: " << headerhh.ID << endl;
			cout << "*QR: " << QR << "  *OPCODE: " << OPCODE << "  *AA: " << AA << "  *TC: " << TC << "  *RD: " << RD << "  *RA: " << RA << "  *Z: " << ZZ << "  *RCODE: " << RCODE << endl;
			cout << "*QDCOUNT: " << headerhh.QDCOUNT << "  *ANCOUNT: " << headerhh.ANCOUNT << "  *NSCOUNT: " << headerhh.NSCOUNT << "  *ARCOUNT: " << headerhh.ARCOUNT << endl;
			cout << "----------------------------------------------------------------------" << endl;
		}















		

		//取header
		if (Get_Header(header, recvData) == false)
			continue;

		unsigned short temp = ntohs(header.ID);

		for (auto it = Buffer.begin(); it != Buffer.end(); it++)
			if ((*it).tempID == temp)
			{
				cout << "发送给:" << inet_ntoa((*it).clientaddr.sin_addr) << ":" << ntohs((*it).clientaddr.sin_port) << endl;
				
				memcpy(recvData, &(*it).ID, sizeof(unsigned short));
				
				/*Llocks = true;
				while (true)
				{
					if (!Llockr)
					{*/
						sendto(sServer, recvData, LEN, 0, (sockaddr*) & ((*it).clientaddr), len);
				//		break;
				//	}
				//	//Sleep(20);
				//}
				//Llocks = false;
				
				Buffer.erase(it);
				break;
			}

	}

	closesocket(UpperDNS);

	_endthreadex(0);
	return 0;
}










