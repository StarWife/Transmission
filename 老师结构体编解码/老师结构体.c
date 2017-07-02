#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "itcast_asn1_der.h"
#include "itcastderlog.h"

typedef struct _Teacher
{
	char name[64];
	int age;
	char *p;
	int plen;
}Teacher;

// 释放空间
void freeTeacher(Teacher **t){
	
	if (t == NULL || *t == NULL)
		return;

	if ((*t)->p != NULL){
		free((*t)->p);
		(*t)->p = NULL;
	}

	free(*t);
	*t = NULL;
}
// Teacher编码
int EncodeTeacher(Teacher *t, unsigned  char **pbuf, int *buflen)
{
	// 编码
	ITCAST_ANYBUF *tmp = NULL;

	// 编码 name
	ITCAST_ANYBUF *buf = NULL;

	int ret = EncodeChar(t->name, strlen(t->name), &buf);
	if (ret != 0){
		DER_ITCAST_FreeQueue(buf);
		return ret;
	}
	
	// 编码 age
	tmp = buf;
	ret = DER_ItAsn1_WriteInteger(t->age, &(tmp->next));
	if (ret != 0){
		DER_ITCAST_FreeQueue(buf);
		printf("WriteInteger  err!!!");
		return ret;
	}

	// 编码 p
	tmp = tmp->next;
	ret = EncodeChar(t->p, t->plen, &tmp->next);
	if (ret != 0){
		DER_ITCAST_FreeQueue(buf);
		return ret;
	}

	// 编码 plen
	tmp = tmp->next;
	ret = DER_ItAsn1_WriteInteger(t->plen, &tmp->next);
	if (ret != 0){
		DER_ITCAST_FreeQueue(buf);
		return ret;
	}

	// 编码整个 buf
	tmp = NULL;
	ret = DER_ItAsn1_WriteSequence(buf, &tmp);
	if (ret != 0){
		DER_ITCAST_FreeQueue(buf);
		return ret;
	}

	*pbuf = tmp->pData;
	*buflen = tmp->dataLen;
	
	return 0;
}

// Teacher解码
int DecodeTeacher(char *src, int len, Teacher **t)
{
	if (t == NULL || src == NULL){
		printf("Teacher or src err !!\n");
		return -1;
	}

	// 开辟空间
	if (*t == NULL){
		*t = (Teacher *)malloc(sizeof(Teacher));
		memset(*t, 0x0000, sizeof(Teacher));
	}

	ITCAST_ANYBUF *squ = NULL;
	ITCAST_ANYBUF *tmp = NULL;
	int ret = 0;

	// 将 src 转成 anybuf
	ret = DER_ITCAST_String_To_AnyBuf(&squ, src, len);
	if (ret != 0){
		freeTeacher(t);
		DER_ITCAST_FreeQueue(squ);
		return ret;
	}

	// 解码 大结构体
	ret = DER_ItAsn1_ReadSequence(squ, &tmp);
	if(ret != 0){
		freeTeacher(t);
		DER_ITCAST_FreeQueue(squ);
		return ret;
	}

	// 解码name
	ITCAST_ANYBUF *p = NULL;
	ret = DER_ItAsn1_ReadPrintableString(tmp, &p);
	if (ret != 0){
		freeTeacher(t);
		DER_ITCAST_FreeQueue(squ);
		DER_ITCAST_FreeQueue(tmp);
		return ret;
	}
	strcpy((*t)->name, p->pData);
	((*t)->name)[p->dataLen] = '\0';

	// 解码 age
	tmp = tmp->next;
	ret = DER_ItAsn1_ReadInteger(tmp, &((*t)->age));
	if (ret != 0){
		freeTeacher(t);
		DER_ITCAST_FreeQueue(squ);
		DER_ITCAST_FreeQueue(tmp);
		return ret;
	}

	// 解码 p
	p = NULL;
	tmp = tmp->next;
	ret = DER_ItAsn1_ReadPrintableString(tmp, &p);
	if (ret != 0){
		freeTeacher(t);
		DER_ITCAST_FreeQueue(squ);
		DER_ITCAST_FreeQueue(tmp);
		return ret;
	}

	//if ((*t)->p == NULL){
		(*t)->p = (char *)malloc(sizeof(char)* (p->dataLen + 1));
	//}
	strcpy((*t)->p, p->pData);
	((*t)->p)[p->dataLen] = '\0';

	// 解码 plen
	tmp = tmp->next;
	ret = DER_ItAsn1_ReadInteger(tmp, &((*t)->plen));
	if (ret != 0){
		freeTeacher(t);
		DER_ITCAST_FreeQueue(squ);
		DER_ITCAST_FreeQueue(tmp);
		return ret;
	}
	// 释放空间
	DER_ITCAST_FreeQueue(squ);
	DER_ITCAST_FreeQueue(tmp);


	return 0;
}

// 写入文件
int mywritefile(unsigned char *buf, int len)
{
	FILE *fp = NULL;
	fp = fopen("./teacher.ber", "wb+");
	if (fp == NULL)
	{
		printf("fopen file error \n");
		return -1;
	}
	fwrite(buf, 1, len, fp);
	fclose(fp);
	return 0;
}


int main()
{
	Teacher t = { "kangna", 6, NULL, 0 };
	Teacher * tea = NULL;

	t.p = (char *)malloc(sizeof(char)* 100);
	strcpy(t.p, "hello  world !!");
	t.plen = strlen(t.p);

	char  *tbuf = NULL;
	int len = 0;

	if (EncodeTeacher(&t, &tbuf, &len) == 0){

		if (DecodeTeacher(tbuf, len, &tea) == 0){

			printf("name\t%s\nage\t%d\np\t%s\nplen\t%d\n", tea->name, tea->age, tea->p, tea->plen);
			
			freeTeacher(&tea);
		}
	}

	system("pause");
	return 0;
}