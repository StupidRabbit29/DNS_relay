#pragma once
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>


#define PORT 53							//端口号
#define MSGSIZE 1024					//最大数据长度

//查询种类	QTYPE
enum QUERY_KIND{Ipv4, NIpv4};
//查找DNSrelay.txt的查找结果
enum SEARCH_RESULT{Find, NFind, Block};

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

typedef struct Waiting
{
	struct sockaddr_in clientaddr;
	unsigned int ID;
	char query[100];
}Waiting;

//bool MyDEBUG = true;

//将报文src中的header字段拷贝到dest中，返回函数是否执行成功
bool Get_Header(char* dest, const char* src);
//将报文src中的query字段中的查询域名拷贝到dest中，同时返回QTYPE
QUERY_KIND Get_Query(char* dest, const char* src);
//在表中查找域名name， 将找到的IP地址存入IP，返回查找结果
SEARCH_RESULT Serach(const char* name, char* IP);