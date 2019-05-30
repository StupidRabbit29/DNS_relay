#include "main.h"
using namespace std;


char* Upper_DNS/* = "222.222.222.222"*/;		//原DNS IP地址
char* List_Name = "dnsrelay.txt";		//默认列表名

map<int, LocalRecord> LocalList;
int debug_level;

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
	int i = 0;
	ifstream inList(List_Name, ifstream::in);
	for (; inList >> temp.IP_Addr;i++) {
		inList >> temp.Domain_Name;
		LocalList.insert(pair<int, LocalRecord>(i, temp));
	}
	inList.close();
}



void printList() {
	for (int i = 0; i < LocalList.size(); i++)
		cout << LocalList[i].IP_Addr << LocalList[i].Domain_Name << endl;
}


int main(int argc, char* argv[]) {

	initProgram(argc, argv[]);
	readList();
	printList();

	return 0;
}