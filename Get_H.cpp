#include"main.h"

bool Get_Header(DNSheader& header, const char* src)
{
	// src�е�header���ֹ����������֣���12���ֽ�: 
	unsigned short tmp;			
	char* ptr = (char*)&tmp;	// �Ƚ�����ת����unsigned short������ntohs����ת��Ϊ���صĸ�ʽ(��˷�->С�˷�)
	// 1��ID: src[0]-src[1]: 2bytes
	*ptr = src[0];
	*(ptr + 1) = src[1];
	header.ID = ntohs(tmp);
	// 2) CtrlCode: src[2]-src[3]: 2bytes
	*ptr = src[0];
	*(ptr + 1) = src[1];
	header.CtrlCode = ntohs(tmp);
	// 3) Questions: src[4]-src[5]: 2bytes
	*ptr = src[0];
	*(ptr + 1) = src[1];
	header.QDCOUNT = ntohs(tmp);
	// 4) Answer RRs: src[6]-src[7]: 2bytes
	*ptr = src[0];
	*(ptr + 1) = src[1];
	header.ANCOUNT = ntohs(tmp);
	// 5) Authority RRs: src[8]-src[9]: 2bytes
	*ptr = src[0];
	*(ptr + 1) = src[1];
	header.NSCOUNT = ntohs(tmp);
	// 6) Additional RRs: src[10]-src[11]: 2bytes
	*ptr = src[0];
	*(ptr + 1) = src[1];
	header.ARCOUNT = ntohs(tmp);

	return true;
}


//������src�е�query�ֶ��еĲ�ѯ����������dest�У�ͬʱ����QTYPE
QUERY_KIND Get_Query(char* dest, char* src)
{
	char* query = src + 12;
	while (*query != '\0')
	{
		int number = *query;
		query++;
		for (int i = 0; i < number; i++)
		{
			*dest = *query;
			dest++;
			query++;
		}
		*dest = '.';
		dest++;
	}
	dest--;
	*dest = '\0';

	unsigned short temp;
	char* ptr = (char*)&temp;
	*ptr = *query;
	*(ptr + 1) = *(query + 1);

	unsigned short query_kind = ntohs(temp);
	if (query_kind == 1)
		return Ipv4;
	else
		return NIpv4;  
}