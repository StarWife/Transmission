#define _CRT_SECURE_NO_WARNINGS
#include "keymng_msg.h"
#include "itcast_asn1_der.h"
#include "itcastderlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
pstruct :	输入的报文数据 ; (指向相应结构体的指针)
type :		输入的类型标识(函数内部通过type 得到 pstruct 所指向的报文类型)
poutData:	输出的编码后的报文 ;
outlen :	输出的数据长度;
*/

// 释放空间
/*
释放 MsgEncode( )函数中的outData; 方法：MsgMemFree((void **)outData, 0);
释放MsgDecode( )函数中的pstruct结构体，MsgMemFree((void **)outData, type);
type : 输入参数,便于函数判断调用哪个结构体的free函数
*/
void free_MsgKey_Req(MsgKey_Req **pStruct)
{
	if (pStruct == NULL || *pStruct == NULL){
		return;
	}

	free(*pStruct);

	*pStruct = NULL;

	return;
}
void free_MsgKey_Res(MsgKey_Res **pStruct)
{
	if (pStruct == NULL || *pStruct == NULL){
		return;
	}

	free(*pStruct);

	*pStruct = NULL;

	return;
}
__declspec(dllexport)int MsgMemFree(void **point, int type)
{
	if (point == NULL || *point == NULL){
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

void free_AnyBuf(ITCAST_ANYBUF **buf)
{
	if (buf == NULL || *buf == NULL){
		return;
	}

	DER_ITCAST_FreeQueue(*buf);

	*buf = NULL;
}

// MsKey_Req 的编码
int Encode_MsgKey_Req(MsgKey_Req *req, ITCAST_ANYBUF **buf)
{
	if (req == NULL || buf == NULL){
		printf(" func %s 参 数 错 误 !\n", __FUNCTION__);
		return -1;
	}

	int ret = 0;
	ITCAST_ANYBUF *tmp = NULL;

	// 先编码 cmdType;
	ret = DER_ItAsn1_WriteInteger(req->cmdType, buf);
	if (ret != 0){
		printf("DER_ItAsn1_WriteInteger cmdType error !\n");
		return ret;
	}
	// 编码 clientId
	tmp  = *buf;
	ret = EncodeChar(req->clientId, strlen(req->clientId), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar  clientId  error !\n");
		return ret;
	}
	// 编码 AuthCode
	tmp = tmp->next;
	ret = EncodeChar(req->AuthCode, strlen(req->AuthCode), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar AuthCode error !\n");
		return ret;
	}
	// 编码 serverId
	tmp = tmp->next;
	ret = EncodeChar(req->serverId, strlen(req->serverId), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar serverId error !\n");
		return ret;
	}
	// 编码 serverId
	tmp = tmp->next;
	ret = EncodeChar(req->r1, strlen(req->r1), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar r1 error !\n");
		return ret;
	}

	return  ret;
}

// MsgKey_Res 的编码
int Encode_MsgKey_Res(MsgKey_Res *res, ITCAST_ANYBUF **buf)
{
	int ret = 0;
	ITCAST_ANYBUF *tmp = NULL;

	// 参数判断
	if (res == NULL || buf == NULL){
		printf(" func %s 参 数 错 误 !\n", __FUNCTION__);
		return -1;
	}

	// 先编码 rv;
	ret = DER_ItAsn1_WriteInteger(res->rv, buf);
	if (ret != 0){
		printf("DER_ItAsn1_WriteInteger rv error !\n");
		return ret;
	}
	// 编码 clientId
	tmp = *buf;
	ret = EncodeChar(res->clientId, strlen(res->clientId), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar clientId error !\n");
		return ret;
	}
	// 编码 serverId
	tmp = tmp->next;
	ret = EncodeChar(res->serverId, strlen(res->serverId), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar serverId error !\n");
		return ret;
	}
	// 编码 r2
	tmp = tmp->next;
	ret = EncodeChar(res->r2, strlen(res->r2), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar r2 error !\n");
		return ret;
	}
	// 编码 seckeyid
	tmp = tmp->next;
	ret = DER_ItAsn1_WriteInteger(res->seckeyid, &(tmp->next));
	if (ret != 0){
		printf("EncodeChar seckeyid error !\n");
		return ret;
	}
	return ret;
}

// 大结构体 的编码
int structEncode(ITCAST_ANYBUF *buf, unsigned char	**outData, int *outlen)
{
	if (buf == NULL || outData == NULL){

		return -1;
	}

	ITCAST_ANYBUF *tmp = NULL;
	int ret = DER_ItAsn1_WriteSequence(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_WriteSequence  outData  error !\n");
		return ret;
	}

	// 将参数传出
	*outData = tmp->pData;
	*outlen = tmp->dataLen;

	buf->pData = NULL;

	free_AnyBuf(&buf);

	return 0;
}
// 释放outData
void freeData(unsigned char ** outData)
{
	if (outData == NULL || *outData == NULL){
		return;
	}

	free(*outData);
	outData = NULL;

	return;
}
// 总编码
__declspec(dllexport)int MsgEncode(void	*pStruct, int type, unsigned char **outData, int *outLen)
{
	// 参数判断
	if (pStruct == NULL || outData == NULL || outLen == NULL){
		printf(" func %s 参 数 错 误 !\n", __FUNCTION__);
		return -1;
	}

	int ret = 0;
	ITCAST_ANYBUF *buf = NULL;

	do 
	{
		// 编码 type
		ret = DER_ItAsn1_WriteInteger(type, &buf);
		if (ret != 0){
			printf("DER_ItAsn1_WriteInteger type error !!\n");
			break;
		}

		switch (type)
		{
		case ID_MsgKey_Req: // 请求报文
			ret = Encode_MsgKey_Req((MsgKey_Req *)pStruct, &(buf->next));
			break;
		case ID_MsgKey_Res: // 应答报文
			ret = Encode_MsgKey_Res((MsgKey_Res *)pStruct, &(buf->next));
			break;
		default:
			ret = -1;
			printf(" func %s 类 型 错 误!\n", __FUNCTION__);
			break;
		}
		if (ret != 0){
			printf("Encode_MsgKey pStruct error !\n");
			break;
		}
		// 编码大结构体
		ret = structEncode(buf, outData, outLen);

		if (ret != 0){
			printf("structEncode outData error !\n");
			freeData(outData);
			break;
		}

	} while (0);

	// 错误释放空间
	if (ret != 0){
		free_AnyBuf(&buf);
		return ret;
	}

	return ret;
}

/*
inData		: 输入的编码后的数据;
inLen		: 输入的数据长度 ;
pstruct	: 输出的解码后的数据; (其空间是在内部开辟的，也需要用内部定义的free函数进行释放)
type		: 结构的类型标识(返回类型标识，使得调用者通过flag进行判断，将pstruct 转换为相应的结构)
*/
// 解码 请求消息
int MsgKey_ReqDecode(MsgKey_Req **pStruct, ITCAST_ANYBUF *buf)
{
	// 判断参数
	if (pStruct == NULL || buf == NULL){
		return -1;
	}

	// 开辟空间
	if (*pStruct == NULL){

		*pStruct = (MsgKey_Req *)malloc(sizeof(MsgKey_Req));

		// 如果开辟空间失败 !
		if (pStruct == NULL){

			return -1;
		}
	}
	ITCAST_ANYBUF *tmp = NULL;
	int ret = 0;
	// 解码 cmdType
	ret = DER_ItAsn1_ReadInteger(buf, &((*pStruct)->cmdType));
	if (ret != 0){

		printf("DER_ItAsn1_ReadInteger cmdType  error !\n");
		return ret;
	}
	// 解码 clientID
	
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString clientID  error !\n");
		return ret;
	}
	strcpy((*pStruct)->clientId, tmp->pData);
	(*pStruct)->clientId[tmp->dataLen] = '\0';

	// 解码 AuthCode
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString AuthCode  error !\n");
		return ret;
	}
	strcpy((*pStruct)->AuthCode, tmp->pData);
	(*pStruct)->AuthCode[tmp->dataLen] = '\0';

	// 解码 serverId
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString serverId  error !\n");
		return ret;
	}
	strcpy((*pStruct)->serverId, tmp->pData);
	(*pStruct)->serverId[tmp->dataLen] = '\0';

	// 解码 r1
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString r1  error !\n");
		return ret;
	}
	strcpy((*pStruct)->r1, tmp->pData);
	(*pStruct)->r1[tmp->dataLen] = '\0';

	return 0;
}
// 解码应答消息
int MsgKey_ResDecode(MsgKey_Res **pStruct, ITCAST_ANYBUF *buf)
{
	// 判断参数
	if (pStruct == NULL || buf == NULL){
		return -1;
	}

	// 开辟空间
	if (*pStruct == NULL){

		*pStruct = (MsgKey_Res *)malloc(sizeof(MsgKey_Res));

		if (*pStruct == NULL){
			printf("空 间 开 辟 失 败 !\n");
			return -1;
		}
	}

	ITCAST_ANYBUF * tmp = NULL;
	int ret = 0;

	// 先解码 rv
	ret = DER_ItAsn1_ReadInteger(buf, &((*pStruct)->rv));
	if (ret != 0){

		printf("DER_ItAsn1_ReadInteger rv  error !\n");
		return ret;
	}
	// 解码 clientID
	
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString clientID  error !\n");
		return ret;
	}
	strcpy((*pStruct)->clientId, tmp->pData);
	(*pStruct)->clientId[tmp->dataLen] = '\0';

	// 解码 serverID
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString serverId  error !\n");
		return ret;
	}
	strcpy((*pStruct)->serverId, tmp->pData);
	(*pStruct)->serverId[tmp->dataLen] = '\0';

	// 解码 r2
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString r2  error !\n");
		return ret;
	}
	strcpy((*pStruct)->r2, tmp->pData);
	(*pStruct)->r2[tmp->dataLen] = '\0';

	// 解码 seckeyId
	ret = DER_ItAsn1_ReadInteger(buf, &((*pStruct)->seckeyid));
	if (ret != 0){

		printf("DER_ItAsn1_ReadInteger seckeyId  error !\n");
		return ret;
	}
	return 0;
}

int decodeStruct(unsigned char *inData, int inLen, ITCAST_ANYBUF **out)
{
	if (inLen <= 0 || inData == NULL || out == NULL){
		printf(" func %s 参 数 错 误 !\n", __FUNCTION__);
		return -1;
	}
	ITCAST_ANYBUF *buf;
	int ret = 0;

	// 先转成 AnyBuf类型
	ret = DER_ITCAST_String_To_AnyBuf(&buf, inData, inLen);
	if (ret != 0){
		free_AnyBuf(&buf);
		printf("DER_ITCAST_String_To_AnyBuf  inData  error !\n");
		return ret;
	}

	// 解码
	ret = DER_ItAsn1_ReadSequence(buf, out);
	if (ret != 0){
		free_AnyBuf(&buf);
		printf("DER_ItAsn1_ReadSequence  strcut  error !\n");
		return ret;
	}

	return ret;
}

// 总解码
__declspec(dllexport)int MsgDecode(unsigned char *inData, int inLen, void **pStruct, int *type)
{

	if (inData == NULL || pStruct == NULL ||  type == NULL){
		printf(" func %s 参 数 错 误 ( >__< ) !\n", __FUNCTION__);
		return -1;
	}

	int ret = 0;
	ITCAST_ANYBUF *buf = NULL;

	do 
	{
		// 解码大结构体
		ret = decodeStruct(inData, inLen, &buf);
		if (ret != 0){
			break;
		}

		// 解码 type
		ret = DER_ItAsn1_ReadInteger(buf, type);
		if (ret != 0){
			printf("DER_ItAsn1_ReadInteger  type  error !\n");
			break;
		}

		switch (*type)
		{
		case ID_MsgKey_Req: // 请求报文
			ret = MsgKey_ReqDecode((MsgKey_Req **)pStruct, buf->next);

			// 失败释放空间
			if (ret != 0){
				printf("MsgKey_ReqDecode pStruct error !\n");
				free_MsgKey_Req((MsgKey_Req **)pStruct);
				break;
			}
			break;
		case ID_MsgKey_Res: // 应答报文
			ret = MsgKey_ResDecode((MsgKey_Res **)pStruct, buf->next);
			
			// 失败释放空间
			if (ret != 0){
				printf("MsgKey_ResDecode pStruct error !\n");
				free_MsgKey_Res((MsgKey_Res **)pStruct);
				break;
			}
			break;
		default:
			ret = type == 0 ? -1 : ret;
			printf(" func %s type error !\n");
			break;
		}

	} while (0);

	// 判断参数
	if (ret != 0){
		free_AnyBuf(&buf);
		return ret;
	}

	return ret;
}
