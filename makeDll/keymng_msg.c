#define _CRT_SECURE_NO_WARNINGS
#include "keymng_msg.h"
#include "itcast_asn1_der.h"
#include "itcastderlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
pstruct :	����ı������� ; (ָ����Ӧ�ṹ���ָ��)
type :		��������ͱ�ʶ(�����ڲ�ͨ��type �õ� pstruct ��ָ��ı�������)
poutData:	����ı����ı��� ;
outlen :	��������ݳ���;
*/

// �ͷſռ�
/*
�ͷ� MsgEncode( )�����е�outData; ������MsgMemFree((void **)outData, 0);
�ͷ�MsgDecode( )�����е�pstruct�ṹ�壬MsgMemFree((void **)outData, type);
type : �������,���ں����жϵ����ĸ��ṹ���free����
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

// MsKey_Req �ı���
int Encode_MsgKey_Req(MsgKey_Req *req, ITCAST_ANYBUF **buf)
{
	if (req == NULL || buf == NULL){
		printf(" func %s �� �� �� �� !\n", __FUNCTION__);
		return -1;
	}

	int ret = 0;
	ITCAST_ANYBUF *tmp = NULL;

	// �ȱ��� cmdType;
	ret = DER_ItAsn1_WriteInteger(req->cmdType, buf);
	if (ret != 0){
		printf("DER_ItAsn1_WriteInteger cmdType error !\n");
		return ret;
	}
	// ���� clientId
	tmp  = *buf;
	ret = EncodeChar(req->clientId, strlen(req->clientId), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar  clientId  error !\n");
		return ret;
	}
	// ���� AuthCode
	tmp = tmp->next;
	ret = EncodeChar(req->AuthCode, strlen(req->AuthCode), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar AuthCode error !\n");
		return ret;
	}
	// ���� serverId
	tmp = tmp->next;
	ret = EncodeChar(req->serverId, strlen(req->serverId), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar serverId error !\n");
		return ret;
	}
	// ���� serverId
	tmp = tmp->next;
	ret = EncodeChar(req->r1, strlen(req->r1), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar r1 error !\n");
		return ret;
	}

	return  ret;
}

// MsgKey_Res �ı���
int Encode_MsgKey_Res(MsgKey_Res *res, ITCAST_ANYBUF **buf)
{
	int ret = 0;
	ITCAST_ANYBUF *tmp = NULL;

	// �����ж�
	if (res == NULL || buf == NULL){
		printf(" func %s �� �� �� �� !\n", __FUNCTION__);
		return -1;
	}

	// �ȱ��� rv;
	ret = DER_ItAsn1_WriteInteger(res->rv, buf);
	if (ret != 0){
		printf("DER_ItAsn1_WriteInteger rv error !\n");
		return ret;
	}
	// ���� clientId
	tmp = *buf;
	ret = EncodeChar(res->clientId, strlen(res->clientId), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar clientId error !\n");
		return ret;
	}
	// ���� serverId
	tmp = tmp->next;
	ret = EncodeChar(res->serverId, strlen(res->serverId), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar serverId error !\n");
		return ret;
	}
	// ���� r2
	tmp = tmp->next;
	ret = EncodeChar(res->r2, strlen(res->r2), &(tmp->next));
	if (ret != 0){
		printf("EncodeChar r2 error !\n");
		return ret;
	}
	// ���� seckeyid
	tmp = tmp->next;
	ret = DER_ItAsn1_WriteInteger(res->seckeyid, &(tmp->next));
	if (ret != 0){
		printf("EncodeChar seckeyid error !\n");
		return ret;
	}
	return ret;
}

// ��ṹ�� �ı���
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

	// ����������
	*outData = tmp->pData;
	*outlen = tmp->dataLen;

	buf->pData = NULL;

	free_AnyBuf(&buf);

	return 0;
}
// �ͷ�outData
void freeData(unsigned char ** outData)
{
	if (outData == NULL || *outData == NULL){
		return;
	}

	free(*outData);
	outData = NULL;

	return;
}
// �ܱ���
__declspec(dllexport)int MsgEncode(void	*pStruct, int type, unsigned char **outData, int *outLen)
{
	// �����ж�
	if (pStruct == NULL || outData == NULL || outLen == NULL){
		printf(" func %s �� �� �� �� !\n", __FUNCTION__);
		return -1;
	}

	int ret = 0;
	ITCAST_ANYBUF *buf = NULL;

	do 
	{
		// ���� type
		ret = DER_ItAsn1_WriteInteger(type, &buf);
		if (ret != 0){
			printf("DER_ItAsn1_WriteInteger type error !!\n");
			break;
		}

		switch (type)
		{
		case ID_MsgKey_Req: // ������
			ret = Encode_MsgKey_Req((MsgKey_Req *)pStruct, &(buf->next));
			break;
		case ID_MsgKey_Res: // Ӧ����
			ret = Encode_MsgKey_Res((MsgKey_Res *)pStruct, &(buf->next));
			break;
		default:
			ret = -1;
			printf(" func %s �� �� �� ��!\n", __FUNCTION__);
			break;
		}
		if (ret != 0){
			printf("Encode_MsgKey pStruct error !\n");
			break;
		}
		// �����ṹ��
		ret = structEncode(buf, outData, outLen);

		if (ret != 0){
			printf("structEncode outData error !\n");
			freeData(outData);
			break;
		}

	} while (0);

	// �����ͷſռ�
	if (ret != 0){
		free_AnyBuf(&buf);
		return ret;
	}

	return ret;
}

/*
inData		: ����ı���������;
inLen		: ��������ݳ��� ;
pstruct	: ����Ľ���������; (��ռ������ڲ����ٵģ�Ҳ��Ҫ���ڲ������free���������ͷ�)
type		: �ṹ�����ͱ�ʶ(�������ͱ�ʶ��ʹ�õ�����ͨ��flag�����жϣ���pstruct ת��Ϊ��Ӧ�Ľṹ)
*/
// ���� ������Ϣ
int MsgKey_ReqDecode(MsgKey_Req **pStruct, ITCAST_ANYBUF *buf)
{
	// �жϲ���
	if (pStruct == NULL || buf == NULL){
		return -1;
	}

	// ���ٿռ�
	if (*pStruct == NULL){

		*pStruct = (MsgKey_Req *)malloc(sizeof(MsgKey_Req));

		// ������ٿռ�ʧ�� !
		if (pStruct == NULL){

			return -1;
		}
	}
	ITCAST_ANYBUF *tmp = NULL;
	int ret = 0;
	// ���� cmdType
	ret = DER_ItAsn1_ReadInteger(buf, &((*pStruct)->cmdType));
	if (ret != 0){

		printf("DER_ItAsn1_ReadInteger cmdType  error !\n");
		return ret;
	}
	// ���� clientID
	
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString clientID  error !\n");
		return ret;
	}
	strcpy((*pStruct)->clientId, tmp->pData);
	(*pStruct)->clientId[tmp->dataLen] = '\0';

	// ���� AuthCode
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString AuthCode  error !\n");
		return ret;
	}
	strcpy((*pStruct)->AuthCode, tmp->pData);
	(*pStruct)->AuthCode[tmp->dataLen] = '\0';

	// ���� serverId
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString serverId  error !\n");
		return ret;
	}
	strcpy((*pStruct)->serverId, tmp->pData);
	(*pStruct)->serverId[tmp->dataLen] = '\0';

	// ���� r1
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
// ����Ӧ����Ϣ
int MsgKey_ResDecode(MsgKey_Res **pStruct, ITCAST_ANYBUF *buf)
{
	// �жϲ���
	if (pStruct == NULL || buf == NULL){
		return -1;
	}

	// ���ٿռ�
	if (*pStruct == NULL){

		*pStruct = (MsgKey_Res *)malloc(sizeof(MsgKey_Res));

		if (*pStruct == NULL){
			printf("�� �� �� �� ʧ �� !\n");
			return -1;
		}
	}

	ITCAST_ANYBUF * tmp = NULL;
	int ret = 0;

	// �Ƚ��� rv
	ret = DER_ItAsn1_ReadInteger(buf, &((*pStruct)->rv));
	if (ret != 0){

		printf("DER_ItAsn1_ReadInteger rv  error !\n");
		return ret;
	}
	// ���� clientID
	
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString clientID  error !\n");
		return ret;
	}
	strcpy((*pStruct)->clientId, tmp->pData);
	(*pStruct)->clientId[tmp->dataLen] = '\0';

	// ���� serverID
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString serverId  error !\n");
		return ret;
	}
	strcpy((*pStruct)->serverId, tmp->pData);
	(*pStruct)->serverId[tmp->dataLen] = '\0';

	// ���� r2
	tmp = NULL;
	buf = buf->next;
	ret = DER_ItAsn1_ReadPrintableString(buf, &tmp);
	if (ret != 0){
		printf("DER_ItAsn1_ReadPrintableString r2  error !\n");
		return ret;
	}
	strcpy((*pStruct)->r2, tmp->pData);
	(*pStruct)->r2[tmp->dataLen] = '\0';

	// ���� seckeyId
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
		printf(" func %s �� �� �� �� !\n", __FUNCTION__);
		return -1;
	}
	ITCAST_ANYBUF *buf;
	int ret = 0;

	// ��ת�� AnyBuf����
	ret = DER_ITCAST_String_To_AnyBuf(&buf, inData, inLen);
	if (ret != 0){
		free_AnyBuf(&buf);
		printf("DER_ITCAST_String_To_AnyBuf  inData  error !\n");
		return ret;
	}

	// ����
	ret = DER_ItAsn1_ReadSequence(buf, out);
	if (ret != 0){
		free_AnyBuf(&buf);
		printf("DER_ItAsn1_ReadSequence  strcut  error !\n");
		return ret;
	}

	return ret;
}

// �ܽ���
__declspec(dllexport)int MsgDecode(unsigned char *inData, int inLen, void **pStruct, int *type)
{

	if (inData == NULL || pStruct == NULL ||  type == NULL){
		printf(" func %s �� �� �� �� ( >__< ) !\n", __FUNCTION__);
		return -1;
	}

	int ret = 0;
	ITCAST_ANYBUF *buf = NULL;

	do 
	{
		// �����ṹ��
		ret = decodeStruct(inData, inLen, &buf);
		if (ret != 0){
			break;
		}

		// ���� type
		ret = DER_ItAsn1_ReadInteger(buf, type);
		if (ret != 0){
			printf("DER_ItAsn1_ReadInteger  type  error !\n");
			break;
		}

		switch (*type)
		{
		case ID_MsgKey_Req: // ������
			ret = MsgKey_ReqDecode((MsgKey_Req **)pStruct, buf->next);

			// ʧ���ͷſռ�
			if (ret != 0){
				printf("MsgKey_ReqDecode pStruct error !\n");
				free_MsgKey_Req((MsgKey_Req **)pStruct);
				break;
			}
			break;
		case ID_MsgKey_Res: // Ӧ����
			ret = MsgKey_ResDecode((MsgKey_Res **)pStruct, buf->next);
			
			// ʧ���ͷſռ�
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

	// �жϲ���
	if (ret != 0){
		free_AnyBuf(&buf);
		return ret;
	}

	return ret;
}
