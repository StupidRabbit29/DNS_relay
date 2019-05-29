#pragma once
#include <iostream>

#define Upper_DNS 222.222.222.222		//原DNS IP地址
#define PORT 53							//端口号
#define MSGSIZE 1024					//最大数据长度

enum DGRAM_KIND{};

//DNS报文的header
typedef struct DNSheader
{
	unsigned short ID;				//ID字段
	unsigned short CtrlCode;		//共16位控制字段
	unsigned short QDCOUNT;			//Number of entries in the question section
	unsigned short ANCOUNT;			//Number of RRs in the answer section
	unsigned short NSCOUNT;			//Number of name server RRs in authority records section
	unsigned short ARCOUNT;			//Number of RRs in additional records section
}DNSheader, *PtrDNSH;

//bool MyDEBUG = true;