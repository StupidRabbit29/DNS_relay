#pragma once
#include <iostream>

#define Upper_DNS 222.222.222.222		//ԭDNS IP��ַ
#define PORT 53							//�˿ں�
#define MSGSIZE 1024					//������ݳ���

enum DGRAM_KIND{};

//DNS���ĵ�header
typedef struct DNSheader
{
	unsigned short ID;				//ID�ֶ�
	unsigned short CtrlCode;		//��16λ�����ֶ�
	unsigned short QDCOUNT;			//Number of entries in the question section
	unsigned short ANCOUNT;			//Number of RRs in the answer section
	unsigned short NSCOUNT;			//Number of name server RRs in authority records section
	unsigned short ARCOUNT;			//Number of RRs in additional records section
}DNSheader, *PtrDNSH;

//bool MyDEBUG = true;