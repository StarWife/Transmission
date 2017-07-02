#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"MesEncode.h"
#include"itcast_asn1_der.h"
#include"itcastderlog.h"
void free_MsgKey_Req(MsgKey_Req **pStruct)
{
	if (pStruct == NULL || *pStruct == NULL) {
		return;
	}

	free(*pStruct);

	*pStruct = NULL;

	return;
}
void free_MsgKey_Res(MsgKey_Res **pStruct)
{
	if (pStruct == NULL || *pStruct == NULL) {
		return;
	}

	free(*pStruct);

	*pStruct = NULL;

	return;
}
int TeacherEncode(void *tStuct, ITCAST_ANYBUF **out)
{
	int             ret = 0;
	ITCAST_ANYBUF * pOriginbuf = NULL;
	ITCAST_ANYBUF * pHeadbuf = NULL;
	ITCAST_ANYBUF * pTemp = NULL;
	ITCAST_ANYBUF * outData = NULL;
	//name 转码
	Teacher * pStuct = (Teacher *)tStuct;
	ret = DER_ITCAST_String_To_AnyBuf(&pOriginbuf, pStuct->name, strlen(pStuct->name));
	if (ret != 0)
	{
		printf("Encode DER_ITCAST_String_To_AnyBuf is error,the number is : %d\n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pOriginbuf, &pHeadbuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pOriginbuf);
		printf("Encode DER_ItAsn1_WritePrintableString is error,the number is : %d\n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pOriginbuf);
	//保存头节点
	pTemp = pHeadbuf;

	//age
	ret = DER_ItAsn1_WriteInteger(pStuct->age, &pTemp->next);
	if (ret != 0)
	{
		printf("Encode DER_ItAsn1_WriteInteger is error,the number is : %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//p
	ret = EncodeChar(pStuct->p, strlen(pStuct->p), &pTemp->next);
	if (ret != 0)
	{
		printf("Encode EncodeChar is error,the number is : %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//plength
	ret = DER_ItAsn1_WriteInteger(pStuct->plen, &pTemp->next);
	if (ret != 0)
	{
		printf("Encode DER_ItAsn1_WriteInteger is error,the number is : %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//最后一个总结点
	ret = DER_ItAsn1_WriteSequence(pHeadbuf, &outData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("Encode DER_ItAsn1_WriteSequence is error,the number is : %d\n", ret);
		return ret;
	}
	*out = outData;
}
void freeTeacher(Teacher ** tea)
{
	if (tea == NULL)
	{
		return;
	}
	if (NULL == *tea)
	{
		return;
	}
	if ((*tea)->p != NULL)
	{
		free((*tea)->p);
		(*tea)->p = NULL;
	}
	free(*tea);
	*tea = NULL;
}
int TeacherDecode(ITCAST_ANYBUF * inData, void **pStruct)
{
	int             ret = NULL;
	ITCAST_ANYBUF * pHeadbuf = NULL;
	ITCAST_ANYBUF * pTemp = NULL;
	ITASN1_PRINTABLESTRING * outName = NULL;
	ITCAST_UINT32 age;
	char * p = NULL;
	int plen = 0;

	Teacher *temp = malloc(sizeof(Teacher));
	if (temp == NULL)
	{
		printf("decode:new teacher address is error\n");
		return -1;
	}
	memset(temp, 0, sizeof(temp));

	//取最外面的节点
	ret = DER_ItAsn1_ReadSequence(inData, &pHeadbuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(inData);
		freeTeacher(&temp);
		printf("Decode DER_ItAsn1_ReadSequence is error,the number is : %d\n", ret);
		return ret;
	}
	pTemp = pHeadbuf;

	//取name节点	
	ret = DER_ItAsn1_ReadPrintableString(pTemp, &outName);
	if (ret != 0)
	{
		freeTeacher(&temp);
		DER_ITCAST_FreeQueue(inData);
		printf("Decode DER_ItAsn1_ReadPrintableString is error,the number is : %d\n", ret);
		return ret;
	}
	strcpy(temp->name, outName->pData);
	temp->name[outName->dataLen] = '\0';
	pTemp = pTemp->next;

	//取age节点
	ret = DER_ItAsn1_ReadInteger(pTemp, &age);
	if (ret != 0)
	{
		freeTeacher(&temp);
		DER_ITCAST_FreeQueue(inData);
		printf("Decode DER_ItAsn1_ReadInteger is error,the number is : %d\n", ret);
		return ret;
	}
	temp->age = age;
	pTemp = pTemp->next;

	//取p节点和plen节点
	ret = DER_ItAsn1_ReadPrintableString(pTemp, &outName);
	if (ret != 0)
	{
		freeTeacher(&temp);
		DER_ITCAST_FreeQueue(inData);
		printf("Decode DER_ItAsn1_ReadPrintableString is error,the number is : %d\n", ret);
		return ret;
	}
	if (ret != 0)
	{
		freeTeacher(&temp);
		DER_ITCAST_FreeQueue(inData);
		printf("Decode DecodeChar is error,the number is : %d\n", ret);
		return ret;
	}
	temp->p = malloc(plen + 1);
	memset(temp->p, 0, sizeof(temp->p));
	strcpy(temp->p, p);
	temp->plen = plen;
	//传出参数temp
	*pStruct = temp;
	DER_ITCAST_FreeQueue(inData);
	return 0;
}
int MesRequestEncode(MsgKey_Req * MesReq, ITCAST_ANYBUF ** outData)
{
	//准备工作
	int             ret = 0;
	ITCAST_ANYBUF * pHeadbuf = NULL;
	ITCAST_ANYBUF * pTemp = NULL;
	ITCAST_ANYBUF *pDSeq = NULL;

	if (MesReq == NULL)
	{
		printf("MesRequestEncode error : unuserful argument of MesReq\n");
		return -1;
	}

	if (outData == NULL)
	{
		printf("MesRequestEncode error : unuserful argument of outData\n");
		return -1;
	}

	//cmdType
	ret = DER_ItAsn1_WriteInteger(MesReq->cmdType,&pHeadbuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesRequestEncode error:DER_ItAsn1_WriteInteger ,the cmdType number is %d\n",ret);
		return ret;
	}
	pTemp = pHeadbuf;

	//clientId
	ret = EncodeChar(MesReq->clientId,strlen(MesReq->clientId),&pTemp->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesRequestEncode error:EncodeChar ,the clientId error number is %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//AuthCode
	ret = EncodeChar(MesReq->AuthCode, strlen(MesReq->AuthCode), &pTemp->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesRequestEncode error:EncodeChar ,the AuthCode error number is %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//serverId
	ret = EncodeChar(MesReq->serverId, strlen(MesReq->serverId), &pTemp->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesRequestEncode error:EncodeChar ,the serverId error number is %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//r1
	ret = EncodeChar(MesReq->r1, strlen(MesReq->r1), &pTemp->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesRequestEncode error:EncodeChar ,the r1 error number is %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//创建最大的节点
	ret = DER_ItAsn1_WriteSequence(pHeadbuf,&pDSeq);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesRequestEncode error:DER_ItAsn1_WriteSequence ,the pHeadbuf error number is %d\n", ret);
		return ret;
	}
	*outData = pDSeq;
	//释放空间
	DER_ITCAST_FreeQueue(pHeadbuf);
}
/*
pstruct :	输入的报文数据 ; (MsgKey_Res)
outData :	输出参数（ITCAST_ANYBUF）;
*/
int MesCallEncode(MsgKey_Res * MesRes, ITCAST_ANYBUF ** outData)
{
	int				ret = 0;
	ITCAST_ANYBUF * pHeadbuf = NULL;
	ITCAST_ANYBUF * pTemp = NULL;
	ITCAST_ANYBUF * out = NULL;

	if (MesRes == NULL)
	{
		printf("MesCallEncode error : unuserful argument of MesReq\n");
		return -1;
	}

	if (outData == NULL)
	{
		printf("MesCallEncode error : unuserful argument of outData\n");
		return -1;
	}
	//rv
	ret = DER_ItAsn1_WriteInteger(MesRes->rv,&pHeadbuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallEncode error:DER_ItAsn1_WriteInteger ,the rv error number is %d\n", ret);
		return ret;
	}
	pTemp = pHeadbuf;

	//cliendid
	ret = EncodeChar(MesRes->clientId,strlen(MesRes->clientId),&pTemp->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallEncode error:EncodeChar ,the rv error number is %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//serverid
	ret = EncodeChar(MesRes->serverId,strlen(MesRes->serverId),&pTemp->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallEncode error:EncodeChar ,the serverId error number is %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//r2
	ret = EncodeUnsignedChar(MesRes->r2, strlen(MesRes->r2), &pTemp->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallEncode error:EncodeUnsignedChar ,the r2 error number is %d\n", ret);
		return ret;
	}
	pTemp = pTemp->next;

	//seckeyid
	ret = DER_ItAsn1_WriteInteger(MesRes->serverId,&pTemp->next);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallEncode error:DER_ItAsn1_WriteInteger ,the seckeyid error number is %d\n", ret);
		return ret;
	}

	//最后封装
	ret = DER_ItAsn1_WriteSequence(pHeadbuf, &out);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallEncode error:DER_ItAsn1_WriteSequence ,the pHeadbuf error number is %d\n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pHeadbuf);
	*outData = out;

	//释放空间
}
/*
pstruct :	输入的报文数据 ; (指向相应结构体的指针)
type :		输入的类型标识(函数内部通过type 得到 pstruct 所指向的报文类型)
poutData:	输出的编码后的报文 ;
outlen :	输出的数据长度;
*/
int MsgEncode(
	void			*pStruct, /*in*/
	int				type,
	unsigned char	**outData, /*out*/
	int				*outLen)
{
	ITCAST_ANYBUF * inData = NULL;
	ITCAST_ANYBUF * pHeadbuf = NULL;
	ITCAST_ANYBUF * pTemp = NULL;
	ITCAST_ANYBUF * out;
	switch (type)
	{
	case ID_MsgKey_Teacher:
		//教师数据编码
		TeacherEncode((Teacher *)pStruct,&inData);
		break;
	case ID_MsgKey_Req:
		//发送请求数据
		MesRequestEncode((MsgKey_Req *)pStruct, &inData);
		break;
	case ID_MsgKey_Res:
		//应答数据
		MesCallEncode((MsgKey_Res *)pStruct, &inData);
		break;
	default:
		break;
	}
	DER_ItAsn1_WriteInteger(type, &pHeadbuf);
	pTemp = pHeadbuf;
	pTemp->next = inData;
	DER_ItAsn1_WriteSequence(pHeadbuf,&out);
	*outData = out->pData;
	*outLen = out->dataLen;

	return 0;
}


int MesRequestDecode(ITCAST_ANYBUF * inData, MsgKey_Req **Stuct)
{
	int				ret = 0;
	int		        tempBufLen = 0;
	char		  * tempBuf = NULL;
	ITCAST_ANYBUF * pHeadbuf = NULL;
	ITCAST_ANYBUF * pTemp = NULL;
	ITCAST_UINT32   cmdType = 0;

	if (inData == NULL)
	{
		printf("MesRequestDecode error : unuserful argument of inData\n");
		return -1;
	}

	if (Stuct == NULL)
	{
		printf("MesRequestDecode error : unuserful argument of Stuct\n");
		return -1;
	}
	ret = DER_ItAsn1_ReadSequence(inData,&pHeadbuf);
	if (ret != 0)
	{
		printf("MesRequestDecode error:DER_ItAsn1_ReadSequence ,the inData error number is %d\n", ret);
		return ret;
	}
	pTemp = pHeadbuf;

	(*Stuct) = malloc(sizeof(MsgKey_Req));
	if ((*Stuct) == NULL)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesRequestDecode error:malloc ,the malloc is failed\n");
		return -1;
	}
	memset((*Stuct),0,sizeof((*Stuct)));


	ret = DER_ItAsn1_ReadInteger(pTemp,&cmdType);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		free_MsgKey_Req(Stuct);
		printf("MesRequestDecode error:DER_ItAsn1_ReadSequence ,the inData error number is %d\n", ret);
		return ret;
	}
	(*Stuct)->cmdType = cmdType;
	pTemp = pTemp->next;

	//clientid
	ret = DecodeChar(pTemp,&tempBuf,&tempBufLen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		free_MsgKey_Req(Stuct);
		printf("MesRequestDecode error:DecodeChar ,the clientId error number is %d\n", ret);
		return ret;
	}
	strcpy((*Stuct)->clientId, tempBuf);
	(*Stuct)->clientId[tempBufLen] = '\0';
	tempBuf = NULL;
	tempBufLen = 0;
	pTemp = pTemp->next;

	//authcode
	ret = DecodeChar(pTemp, &tempBuf, &tempBufLen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		free_MsgKey_Req(Stuct);
		printf("MesRequestDecode error:DecodeChar ,the AuthCode error number is %d\n", ret);
		return ret;
	}
	strcpy((*Stuct)->AuthCode, tempBuf);
	(*Stuct)->AuthCode[tempBufLen] = '\0';
	tempBuf = NULL;
	tempBufLen = 0;
	pTemp = pTemp->next;

	ret = DecodeChar(pTemp, &tempBuf, &tempBufLen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		free_MsgKey_Req(Stuct);
		printf("MesRequestDecode error:DecodeChar ,the serverId error number is %d\n", ret);
		return ret;
	}
	strcpy((*Stuct)->serverId, tempBuf);
	(*Stuct)->serverId[tempBufLen] = '\0';
	tempBuf = NULL;
	tempBufLen = 0;
	pTemp = pTemp->next;

	ret = DecodeChar(pTemp, &tempBuf, &tempBufLen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		free_MsgKey_Req(Stuct);
		printf("MesRequestDecode error:DecodeChar ,the r1 error number is %d\n", ret);
		return ret;
	}
	strcpy((*Stuct)->r1, tempBuf);
	(*Stuct)->r1[tempBufLen] = '\0';

	//释放节点空间
	DER_ITCAST_FreeQueue(pHeadbuf);
	return 0;
}

int MesCallDecode(ITCAST_ANYBUF * inData, MsgKey_Res ** Stuct)
{
	int				ret = 0;
	ITCAST_ANYBUF * pHeadbuf = NULL;
	ITCAST_ANYBUF * pTemp = NULL;
	ITCAST_UINT32   cmdType = 0;
	char		  * tempBuf = NULL;
	int		        tempBufLen = 0;

	if (inData == NULL)
	{
		printf("MesCallDecode error : unuserful argument of inData\n");
		return -1;
	}

	if (Stuct == NULL)
	{
		printf("MesCallDecode error : unuserful argument of Stuct\n");
		return -1;
	}
	
	ret = DER_ItAsn1_ReadSequence(inData, &pHeadbuf);
	if (ret != 0)
	{
		printf("MesCallDecode error:DER_ItAsn1_ReadSequence ,the inData error number is %d\n", ret);
		return ret;
	}
	pTemp = pHeadbuf;

	(*Stuct) = malloc(sizeof(MsgKey_Res));
	if (*Stuct == NULL )
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallDecode error:malloc ,the malloc error number is %d\n", ret);
		return ret;
	}
	memset((*Stuct), 0, sizeof((*Stuct)));

	//cmdType
	ret = DER_ItAsn1_ReadInteger(pTemp, &cmdType);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);	
		printf("MesCallDecode error:DER_ItAsn1_ReadInteger ,the cmdType error number is %d\n", ret);
		return ret;
	}
	(*Stuct)->rv = cmdType;
	pTemp = pTemp->next;

	//clientid
	ret = DecodeChar(pTemp, &tempBuf, &tempBufLen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallDecode error:DecodeChar ,the clientId error number is %d\n", ret);
		return ret;
	}
	strcpy((*Stuct)->clientId, tempBuf);
	(*Stuct)->clientId[tempBufLen] = '\0';
	tempBuf = NULL;
	tempBufLen = 0;
	pTemp = pTemp->next;

	//serverId
	ret = DecodeChar(pTemp, &tempBuf, &tempBufLen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallDecode error:DecodeChar ,the serverId error number is %d\n", ret);
		return ret;
	}
	strcpy((*Stuct)->serverId, tempBuf);
	(*Stuct)->serverId[tempBufLen] = '\0';
	tempBuf = NULL;
	tempBufLen = 0;
	pTemp = pTemp->next;

	//r2
	ret = DecodeChar(pTemp, &tempBuf, &tempBufLen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallDecode error:DecodeChar ,the r2 error number is %d\n", ret);
		return ret;
	}
	strcpy((*Stuct)->r2, tempBuf);
	(*Stuct)->r2[tempBufLen] = '\0';
	tempBuf = NULL;
	tempBufLen = 0;
	pTemp = pTemp->next;

	cmdType = 0;
	ret = DER_ItAsn1_ReadInteger(pTemp, &cmdType);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("MesCallDecode error:DER_ItAsn1_ReadInteger ,the seckeyid error number is %d\n", ret);
		return ret;
	}
	(*Stuct)->seckeyid = cmdType;
	DER_ITCAST_FreeQueue(pHeadbuf);
	return 0;
}
/*
inData		: 输入的编码后的数据;
inLen		: 输入的数据长度 ;
pstruct	: 输出的解码后的数据; (其空间是在内部开辟的，也需要用内部定义的free函数进行释放)
type		: 结构的类型标识(返回类型标识，使得调用者通过flag进行判断，将pstruct 转换为相应的结构)
*/
int MsgDecode(
	unsigned char *inData,/*in*/
	int           inLen,
	void          **pStruct /*out*/,
	int           *type /*out*/)
{
	int				inType = 0;
	void          * Stuct = NULL;
	ITCAST_ANYBUF * pOriginBuf = NULL;
	ITCAST_ANYBUF * pHeadbuf = NULL;
	ITCAST_ANYBUF * pTemp = NULL;

	ITCAST_ANYBUF * inStuctData = NULL;
	DER_ITCAST_String_To_AnyBuf(&pOriginBuf, inData, inLen);
	DER_ItAsn1_ReadSequence(pOriginBuf,&pHeadbuf);
	pTemp = pHeadbuf;

	DER_ItAsn1_ReadInteger(pTemp,&inType);
	inStuctData = pTemp->next;

	switch (inType)
	{
	case ID_MsgKey_Teacher:
		TeacherDecode(inStuctData, (Teacher **)pStruct);
		break;
	case ID_MsgKey_Req:
		//发送请求数据
		MesRequestDecode(inStuctData, (MsgKey_Req **)pStruct);
		break;
	case ID_MsgKey_Res:
		MesCallDecode(inStuctData, (MsgKey_Res **)pStruct);
		break;
	default:
		break;
	}
	*pStruct = Stuct;
	*type = inType;
	return 0;

}


/*
释放 MsgEncode( )函数中的outData; 方法：MsgMemFree((void **)outData, 0);
释放MsgDecode( )函数中的pstruct结构体，MsgMemFree((void **)outData, type);
type : 输入参数,便于函数判断调用哪个结构体的free函数
*/


int MsgMemFree(void **point, int type)
{
	if (point == NULL || *point == NULL) {
		return 0;
	}

	switch (type)
	{
	case ID_MsgKey_Req:
		free_MsgKey_Req((MsgKey_Req **)point);
		break;
	case ID_MsgKey_Res:
		free_MsgKey_Res((MsgKey_Res **)point);
		break;
	default:
		free(*point);
		break;
	}
	point = NULL;
	return 0;
}