#include "main.h"
using namespace std;


char* Upper_DNS/* = "222.222.222.222"*/;		//原DNS IP地址
string List_Name = "dnsrelay.txt";		//默认列表名

//map<int, LocalRecord> LocalList;
//map <char*, char*> LocalList;
int debug_level;
vector <LocalRecord> LocalList;


void initProgram(int count, char* value[]) {
	switch (count)
	{
	case 1:
		debug_level = 0;
		break;
	case 4:
		if (strcmp(value[2], "-d") == 0) {
			debug_level = 1;
			Upper_DNS = value[3];
			List_Name = value[4];
		}
		else
			if (strcmp(value[2], "-dd") == 0) {
				debug_level = 2;
				Upper_DNS = value[3];
				List_Name = value[4];
			}
			else
				debug_level = -1;
		break;
	default:
		debug_level = -1;
		break;
	}
}
void readList() {
	LocalRecord temp;
	ifstream inList(List_Name, ifstream::in);
	for (int i = 0; inList >> temp.IP_Addr; i++) {
		inList >> temp.Domain_Name;
		LocalList.push_back(temp);
//		cout << LocalList[i].IP_Addr << ":" << LocalList[i].Domain_Name << endl;
	}
	inList.close();
}
void printList() {
	for (unsigned int i = 0; i < LocalList.size(); i++) {
		cout << LocalList[i].IP_Addr << ":" << LocalList[i].Domain_Name << endl;
	}
}

SEARCH_RESULT Serach(const char* name, char* IP) {
	SEARCH_RESULT result = NFind;
	string NAME = name;
	for (unsigned int i = 0; i < LocalList.size(); i++) {
		if (NAME == LocalList[i].Domain_Name)
			if (LocalList[i].IP_Addr == "0.0.0.0")
				result = Block;
			else {
				result = Find;
				IP = (char*)LocalList[i].IP_Addr.data();
			}
	}
	return result;
}





int main(int argc, char* argv[]) {

	initProgram(argc, argv);
	readList();
//	printList();

	return 0;
}