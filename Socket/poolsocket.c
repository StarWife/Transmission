#include "poolsocket.h"


//客户端 初始化
int sckClient_init()
{

}

//客户端 连接服务器
int sckClient_connect(char *ip, int port, int connecttime, int *connfd)
{

}

//客户端 关闭和服务端的连接
int sckClient_closeconn(int connfd)
{

}

//客户端 发送报文
int sckClient_send(int connfd, int sendtime, unsigned char *data, int datalen)
{

}

//客户端 接受报文
int sckClient_rev(int connfd, int revtime, unsigned char **out, int *outlen) //1
{

}

//客户端 释放
int sckClient_destroy()
{

}




// //释放内存
// int sck_FreeMem(void **buf);
// 
// 
// typedef struct _SCKClitPoolParam
// {
// 	char 	serverip[64];
// 	int 	serverport;
// 	int 	bounds; //池容量
// 	int 	connecttime;
// 	int 	sendtime;
// 	int 	revtime;
// }SCKClitPoolParam;


//客户端 socket池初始化
int sckCltPool_init(void **handle, SCKClitPoolParam *param)
{

}


//客户端 socket池 获取一条连接 
int sckCltPool_getConnet(void *handle, int *connfd)
{

}

//客户端 socket池 发送数据 
int sckCltPool_send(void *handle, int  connfd, unsigned char *data, int datalen)
{

}

//客户端 socket池 接受数据
int sckCltPool_rev(void *handle, int  connfd, unsigned char **out, int *outlen) //1
{

}

//客户端 socket池 把连接放回 socket池中 
int sckCltPool_putConnet(void *handle, int connfd, int validFlag) //0正常 1
{

}


//客户端 socket池 销毁连接
int sckCltPool_destroy(void *handle)
{

}




//服务器端初始化
int sckServer_init(int port, int *listenfd)
{

}

int sckServer_accept(int listenfd, int timeout, int *connfd)
{

}

//服务器端发送报文
int sckServer_send(int connfd, int timeout, unsigned char *data, int datalen)
{

}

//服务器端端接受报文
int sckServer_rev(int  connfd, int timeout, unsigned char **out, int *outlen) //1
{

}

int sckServer_close(int connfd)
{

}

//服务器端环境释放 
int sckServer_destroy()
{

}
