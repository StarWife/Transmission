#include "poolsocket.h"


//�ͻ��� ��ʼ��
int sckClient_init()
{

}

//�ͻ��� ���ӷ�����
int sckClient_connect(char *ip, int port, int connecttime, int *connfd)
{

}

//�ͻ��� �رպͷ���˵�����
int sckClient_closeconn(int connfd)
{

}

//�ͻ��� ���ͱ���
int sckClient_send(int connfd, int sendtime, unsigned char *data, int datalen)
{

}

//�ͻ��� ���ܱ���
int sckClient_rev(int connfd, int revtime, unsigned char **out, int *outlen) //1
{

}

//�ͻ��� �ͷ�
int sckClient_destroy()
{

}




// //�ͷ��ڴ�
// int sck_FreeMem(void **buf);
// 
// 
// typedef struct _SCKClitPoolParam
// {
// 	char 	serverip[64];
// 	int 	serverport;
// 	int 	bounds; //������
// 	int 	connecttime;
// 	int 	sendtime;
// 	int 	revtime;
// }SCKClitPoolParam;


//�ͻ��� socket�س�ʼ��
int sckCltPool_init(void **handle, SCKClitPoolParam *param)
{

}


//�ͻ��� socket�� ��ȡһ������ 
int sckCltPool_getConnet(void *handle, int *connfd)
{

}

//�ͻ��� socket�� �������� 
int sckCltPool_send(void *handle, int  connfd, unsigned char *data, int datalen)
{

}

//�ͻ��� socket�� ��������
int sckCltPool_rev(void *handle, int  connfd, unsigned char **out, int *outlen) //1
{

}

//�ͻ��� socket�� �����ӷŻ� socket���� 
int sckCltPool_putConnet(void *handle, int connfd, int validFlag) //0���� 1
{

}


//�ͻ��� socket�� ��������
int sckCltPool_destroy(void *handle)
{

}




//�������˳�ʼ��
int sckServer_init(int port, int *listenfd)
{

}

int sckServer_accept(int listenfd, int timeout, int *connfd)
{

}

//�������˷��ͱ���
int sckServer_send(int connfd, int timeout, unsigned char *data, int datalen)
{

}

//�������˶˽��ܱ���
int sckServer_rev(int  connfd, int timeout, unsigned char **out, int *outlen) //1
{

}

int sckServer_close(int connfd)
{

}

//�������˻����ͷ� 
int sckServer_destroy()
{

}
