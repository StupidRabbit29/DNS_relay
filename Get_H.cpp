#include"main.h"

bool Get_Header(DNSheader& header, const char* src)
{

}


//将报文src中的query字段中的查询域名拷贝到dest中，同时返回QTYPE
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