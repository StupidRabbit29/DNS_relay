#include"main.h"

bool Get_Header(DNSheader& header, const char* src)
{
	// src中的header部分共有六个部分，共12个字节: 
	unsigned short tmp;			
	char* ptr = (char*)&tmp;	// 先将报文转化成unsigned short，再用ntohs将其转化为本地的格式(大端法->小端法)
	// 1）ID: src[0]-src[1]: 2bytes
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

string get_ip(const char* IPaddr)
{
	unsigned char tmp = 0;
	char num[5];
	string result;
	int i = 0, j = 0;
	while (IPaddr[i] != '\0')
	{
		if (IPaddr[i] != '.')
		{
			tmp = tmp * 16 + IPaddr[i] - '0';
		}
		else
		{
			num[j] = tmp;
			result.push_back(num[j]);
			tmp = 0;
			j++;
		}
		i++;
	}
	num[3] = tmp;
	result.push_back(num[3]);
	//num[4] = '\0';
	//cout << num << endl;
	return result;
}


//string get_ip(string IPaddr)
//{
//	int index1, index2, index3;
//	string field, ip;
//	index1 = IPaddr.find_first_of(".", 0);
//	//cout << index1 << endl;
//	index2 = IPaddr.find_first_of(".", index1 + 1);
//	//cout << index2 << endl;
//	index3 = IPaddr.find_first_of(".", index2 + 1);
//	//cout << index3 << endl;
//	field = dec2hex(stoi(IPaddr.substr(0, index1)));
//	ip += field;
//	field = dec2hex(stoi(IPaddr.substr(index1 + 1, index2 - index1 - 1)));
//	ip += field;
//	field = dec2hex(stoi(IPaddr.substr(index2 + 1, index3 - index2 - 1)));
//	ip += field;
//	field = dec2hex(stoi(IPaddr.substr(index3 + 1, IPaddr.size() - index3 - 1)));
//	ip += field;
//	return ip;
//}
//
//string dec2hex(int i) //将int转成16进制字符串
//{
//	stringstream ioss; //定义字符串流
//	string s_temp; //存放转化后字符
//	ioss << setiosflags(ios::uppercase) << hex << i; //以十六制(大写)形式输出
//	ioss >> s_temp;
//	return s_temp;
//}