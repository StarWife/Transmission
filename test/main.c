#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"MesEncode.h"
void test()
{
	int					ret = 0;
	Teacher				t1;

	unsigned char		*myOut = NULL;
	int					myOutlen;

	Teacher				*myStruct = NULL;
	int						iType = 0;
	memset(&t1, 0, sizeof(Teacher));
	strcpy(t1.name, "name1");
	t1.age = 100;
	t1.p = (char *)malloc(100);
	strcpy(t1.p, "aaaaabbbbb"); //
	t1.plen = strlen(t1.p);

	ret = MsgEncode(&t1, ID_MsgKey_Teacher, &myOut, &myOutlen);
	if (ret != 0)
	{
		printf("func MsgEncode() err:%d \n", ret);
		return ret;
	}
	printf("myOut : %s\n",myOut);
	ret = MsgDecode(myOut, myOutlen, &myStruct, &iType);
	printf("name:%s,age:%d", myStruct->name, myStruct->age);
	if (ret != 0)
	{
		printf("func MsgEncode() err:%d \n", ret);
		return ret;
	}

	//ÊÍ·ÅÄÚ´æ¿é
	MsgMemFree((void **)&myOut, 0);
	MsgMemFree((void **)&myStruct, iType);

	printf("hello...\n");
}

int main()
{
	test();
	system("pause");
}