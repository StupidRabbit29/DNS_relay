#include"main.h"

bool Get_Header(DNSheader& header, const char* src)
{
	unsigned short tmp;			// 

}

/*
//
// src�е�header���ֹ����������֣���12���ֽ�: 
// 1��ID: src[0]-src[1]: 2bytes
// 2) Flags: src[2]-src[3]: 2bytes
// 3) Questions: src[4]-src[5]: 2bytes
// 4) Answer RRs: src[6]-src[7]: 2bytes
// 5) Authority RRs: src[8]-src[9]: 2bytes
// 6) Additional RRs: src[10]-src[11]: 2bytes
*/
}


//������src�е�query�ֶ��еĲ�ѯ����������dest�У�ͬʱ����QTYPE
QUERY_KIND Get_Query(char* dest, char* src)
{
	char* query = src + 12;

}