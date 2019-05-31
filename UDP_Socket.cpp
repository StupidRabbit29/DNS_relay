#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "main.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

extern char* Upper_DNS;
const char* Local_Host = "127.0.0.1";
vector<struct Waiting>Buffer;
extern int debug_level;
SOCKET sServer;
SOCKET UpperDNS;
SOCKADDR_IN local;
SOCKADDR_IN UP_DNS;

bool Llocks = false;
bool Llockr = false;
bool Rlocks = true;
bool Rlockr = false;

unsigned short ID = 1;

void Init()
{
	//windows socket
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		return ;
	}

	//�����׽���
	sServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	UpperDNS = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sServer == INVALID_SOCKET)
	{
		cout << "socket error!" << endl;
		return ;
	}

	//�����м�DNS��IP�Ͷ˿�
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = inet_addr(Local_Host);
	//serAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

	//����ԭDNS��IP�Ͷ˿�
	UP_DNS.sin_family = AF_INET;
	UP_DNS.sin_port = htons(PORT);
	UP_DNS.sin_addr.s_addr = inet_addr(Upper_DNS);

	if (bind(sServer, (struct sockaddr*) & local, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "bind error!" << endl;
	}

	HANDLE handle1 = (HANDLE)_beginthreadex(NULL, 0, DNSServer, NULL, 0, NULL);
	HANDLE handle2 = (HANDLE)_beginthreadex(NULL, 0, remote, NULL, 0, NULL);

	WaitForSingleObject(handle1, INFINITE);
	CloseHandle(handle1);
	WaitForSingleObject(handle2, INFINITE);
	CloseHandle(handle2);

	WSACleanup();
}
unsigned __stdcall DNSServer(void* pAruguments)
{
	SOCKADDR_IN client;
	int len = sizeof(SOCKADDR_IN);

	while (true)
	{
		char recvData[MSGSIZE] = { '\0' };
		int LEN;

		while (true)
		{
			if (!Llocks)
			{
				Llockr = true;
				LEN = recvfrom(sServer, recvData, sizeof(recvData), 0, (sockaddr*)& client, &len);
				Llockr = false;
				break;
			}
			Sleep(20);
		}

		if (LEN == -1)
			continue;

		if (debug_level == 2)
			cout << "���յ�:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "����Ϣ" << endl;
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
				cout << "���յ�:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "����Ϣ" << endl;
				// ��ӡ����
				if (QR == 0) cout << "��ѯ���� ";
				if (QR == 1) cout << "��Ӧ���� ";
				cout << "DNS: " << endl;
				for (int i = 0; i < LEN; i++)
				{
					printf("%02x ", (unsigned  char)recvData[i]);
					if ((i + 1) % 8 == 0)
						printf("\n");
				}
				// ��ӡ����
				cout << endl;
				cout << "*ID: " << header.ID << endl;
				cout << "*QR: " << QR << "  *OPCODE: " << OPCODE << "  *AA: " << AA << "  *TC: " << TC << "  *RD: " << RD << "  *RA: " << RA << "  *Z: " << ZZ << "  *RCODE: " << RCODE << endl;
				cout << "*QDCOUNT: " << header.QDCOUNT << "  *ANCOUNT: " << header.ANCOUNT << "  *NSCOUNT: " << header.NSCOUNT << "  *ARCOUNT: " << header.ARCOUNT << endl;
				cout << "----------------------------------------------------------------------" << endl;
			}
			

		}

		DNSheader header;
		char DomainName[100] = { '\0' };

		////�������ݱ�����Դ
		//if (client.sin_addr.s_addr == UP_DNS.sin_addr.s_addr)//????????
		//{
		//	cout << "Packet from Upper DNS!" << endl;
		//	//ȡheader
		//	if (Get_Header(header, recvData) == false)
		//		continue;

		//	for (auto it = Buffer.begin(); it != Buffer.end(); it++)
		//		if ((*it).ID == header.ID && (*it).clientaddr.sin_addr.s_addr == client.sin_addr.s_addr)
		//		{
		//			cout << "���͸�:" << inet_ntoa((*it).clientaddr.sin_addr) << ":" << ntohs((*it).clientaddr.sin_port) << endl;

		//			sendto(sServer, recvData, sizeof(recvData), 0, (sockaddr*)&((*it).clientaddr), len);
		//			Buffer.erase(it);
		//			break;
		//		}
		//}
		//else// if (client.sin_addr == local.sin_addr)//????????
		//{
			cout << "Packet from client!" << endl;
			//ȡheader
			if (Get_Header(header, recvData) == false)
				continue;

			//ȡ����
			QUERY_KIND query_kind = Get_Query(DomainName, recvData);
			cout << query_kind << endl;

			//��������
			if (query_kind == Ipv4)
				//����IPV4��IP
			{
				cout << "IPV4" << endl;

				char IPaddr[20] = { '\0' };
				//���
				SEARCH_RESULT result = Search(DomainName, IPaddr);

				if (result == Find)
					//�ҵ���Ӧ����IP
				{
					cout << "IPV4 & Find!!!!!!!!!!" << endl;
					int index, index0 = 12;
					string recv, send;
					string	Header, Query, Answer;
					// ��Ӧ��Header
					for (int i = 0; i < LEN; i++)
						recv.push_back(recvData[i]);
					Header = recv.substr(0, 12);
					Header[2] = (char)0x85; Header[3] = (char)0x80; Header[7] = (char)0x01;
					//index = recv.find_last_of((char)0x01);	// ����recvData��Query���ֽ�β��index
					//Query = recv.substr(index0, index - 11); 	// ��Ӧ��Query
					//Answer = recv.substr(index0, index - 11);
					index = recv.find_first_of((char)0x00, 12);
					Query = recv.substr(12, index - 7);
					Answer = recv.substr(12, index - 7);
					string TTL_L = {0x00,0x00,0x00,0x78,0x00,0x04};
					send = Header + Query + Answer + TTL_L + get_ip(IPaddr);
					//char sendData[1024];
					char* sendData = const_cast<char*>(send.c_str());

					cout << "���͸�:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << endl;
					sendto(sServer, sendData, send.size(), 0, (sockaddr*)& client, len);
									   					 

					cout << "----------------------------------------------------------------------" << endl;
					// ��ӡ����
					
					cout << "DNS: " << endl;
					for (int i = 0; i < send.size(); i++)
					{
						printf("%02x ", (unsigned  char)send[i]);
						if ((i + 1) % 8 == 0)
							printf("\n");
					}
					
					cout << "----------------------------------------------------------------------" << endl;

					continue;
				}
				else if (result == Block)
					//Ӧ����
				{
					cout << "IPV4 & Block!!!!!!!!!" << endl;

					// FlagλӦ������Ϊ8583
					recvData[2] = (char)0x85;
					recvData[3] = (char)0x83;

					cout << "���͸�:" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << endl;
					sendto(sServer, recvData, LEN, 0, (sockaddr*)& client, len);
					continue;
				}
				else
					cout << "IPV4 & NFind!!!!!!!!!" << endl;
			}
			
			unsigned short temp = htons(ID);
			memcpy(recvData, &temp, sizeof(unsigned short));
			ID = (ID + 1) % 2333 + 1;

			//�������ͷ�IPV4���ڶ��ձ���δ�ҵ���Ӧ��ԭDNS�м�
			Waiting user;
			//user.client = sServer;
			user.clientaddr = client;
			user.ID = header.ID;
			user.tempID = ID;
			//strcpy(user.query, DomainName);

			Buffer.push_back(user);
			
			cout << "���͸�:" << inet_ntoa(UP_DNS.sin_addr) << ":" << ntohs(UP_DNS.sin_port) << endl;
			
			Rlocks = true;
			while (true)
			{
				if (!Rlockr)
				{//��ԭ���ݰ�ֱ�ӷ��͸�ԭDNS
					sendto(UpperDNS, recvData, LEN, 0, (sockaddr*)& UP_DNS, len);
					break;
				}
				Sleep(20);
			}
			Rlocks = false;
	
	}

	closesocket(sServer);

	_endthreadex(0);
	return 0;
}