#include "main.h"
using namespace std;

const char a[] = "10.3.9.5";	//原IP
char* Upper_DNS = a/* = "222.222.222.222"*/;		//原DNS IP地址
string List_Name = "dnsrelay.txt";		//默认列表名


int debug_level;	//调试信息输出等级
vector <LocalRecord> LocalList;


//判断debug类型
void initProgram(int count, char* value[]) {
	//判断参数个数
	switch (count)
	{
	//无参数
	case 1:
		debug_level = 0;
		break;
	//完整参数
	case 4:
		if (strcmp(value[1], "-d") == 0) {
			debug_level = 1;
			Upper_DNS = value[2];
			List_Name = value[3];
		}
		else
			if (strcmp(value[1], "-dd") == 0) {
				debug_level = 2;
				Upper_DNS = value[2];
				List_Name = value[3];
			}
			else
				debug_level = -1;
		break;
	default:
		debug_level = -1;
		break;
	}
}

//打印本地列表
void printList() {
	for (unsigned int i = 0; i < LocalList.size(); i++) {
		cout << "\t" << i << ":" << LocalList[i].IP_Addr << "\t" << LocalList[i].Domain_Name << endl;
	}
}

//读取本地列表，打印相关信息
void readList() {
	cout << "Try to load table \"" << List_Name << "\" ... ";
	LocalRecord temp;
	ifstream inList(List_Name, ifstream::in);

	//判断读取是否成功
	if (inList.good())
		cout << "OK!" << endl;
	else
		cout << "Ignored!" << endl;
	
	//读取文件并加入容器中
	for (int i = 0; inList >> temp.IP_Addr; i++) {
		inList >> temp.Domain_Name;
		LocalList.push_back(temp);
	}
	inList.close();

	//打印列表
	if (debug_level == 2)
		printList();
	cout << LocalList.size() << " names" << endl;
}

//打印初始信息
void printMsg() {
	cout << "DNSRELAY, Version 1.0beta, Build: May 31 2019, By 223" << endl;
	cout << "Usage: dnsrelay[-d | -dd][<dns - server>][<db - file>]" << endl;
	cout << "Name Server " << Upper_DNS << endl;
	cout << "\tDebug Level " << debug_level << endl;
	cout << "\tBind UDP port " << PORT << " ...OK!" << endl;
}

//在本地列表中按域名查找
SEARCH_RESULT Search(const char* name, char* IP) {
	SEARCH_RESULT result = NFind;
	string NAME = name;
	for (unsigned int i = 0; i < LocalList.size(); i++) {
		if (NAME == LocalList[i].Domain_Name)
			if (LocalList[i].IP_Addr == "0.0.0.0")
				result = Block;
			else {
				result = Find;
				strcpy_s(IP, LocalList[i].IP_Addr.size() + 1, LocalList[i].IP_Addr.c_str());
			}
	}
	return result;
}



//测试用函数
void testSearch() {
	char ininin[100] = { '\0' };
	char IPaddr[20] = { '\0' };
	while (true) {
		cin >> ininin;
		SEARCH_RESULT res = Search(ininin, IPaddr);

		if (res == Find)
			cout << IPaddr << endl;
		if (res == NFind)
			cout << "Upper" << endl;
		if (res == Block)
			cout << "Block" << endl;
		cout << endl;
		//system("pause");
	}
}



int main(int argc, char* argv[]) {

	initProgram(argc, argv);
	printMsg();
	readList();
	
//	printList();
//	testSearch();

	DNSServer();

	return 0;
}