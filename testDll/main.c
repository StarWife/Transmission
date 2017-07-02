#include <stdio.h>
#include <stdlib.h>
#include "keymng_msg.h"

void test01()
{
	MsgKey_Req req = {
		10, "client_ID", "AuthCode", "server_ID", "MsgKey_Req"
	};
	MsgKey_Res res = {
		12, "client_ID", "server_ID", "MsgKey_Res", 1
	};
	

	char *p = NULL;
	int len = 0;
	void *ptmp = NULL;
	int type = 0;

	// ±‡¬Î
	if (MsgEncode(&req, ID_MsgKey_Req, &p, &len) == 0 ){

		// Ω‚¬Î
		if (MsgDecode(p, len, &ptmp, &type) == 0){

			if (type == ID_MsgKey_Req && ptmp != NULL){
				MsgKey_Req * tmp = (MsgKey_Req *)ptmp;
				printf("cmdType\t\t%d\nclientid\t%s\nAuthCode\t%s\nserverid\t%s\nr1\t\t%s\n",
					tmp->cmdType, tmp->clientId, tmp->AuthCode, tmp->serverId, tmp->r1);

				//  Õ∑≈
				if (MsgMemFree(&ptmp, ID_MsgKey_Req) != 0){

				}
			}
		}

	}
	else {
		printf("-================-\n");
	}
	
	printf("--=============================================--\n");
	// ±‡¬Î
	p = NULL;
	if (MsgEncode(&res, ID_MsgKey_Res, &p, &len) == 0){

		// Ω‚¬Î
		ptmp = NULL;
		if (MsgDecode(p, len, &ptmp, &type ) == 0){

			if (type == ID_MsgKey_Res && ptmp != NULL){
				MsgKey_Res * tmp = (MsgKey_Res *)ptmp;
				printf("rv\t\t%d\nclientid\t%s\nserverId\t%s\nr2\t\t%s\nseckeyid\t%s\n",
					tmp->rv, tmp->clientId, tmp->serverId, tmp->r2, tmp->serverId);

				//  Õ∑≈
				MsgMemFree(&ptmp, ID_MsgKey_Res);
			}
		}
	}
	else {
		printf("-================-\n");
	}

}
int main()
{
	test01();


	system("pause");
 	return 0;
}