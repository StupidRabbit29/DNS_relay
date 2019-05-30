#include"main.h"

bool Get_Header(DNSheader& header, const char* src)
{

}


//将报文src中的query字段中的查询域名拷贝到dest中，同时返回QTYPE
QUERY_KIND Get_Query(char* dest, char* src)
{
	char* query = src + 12;

}