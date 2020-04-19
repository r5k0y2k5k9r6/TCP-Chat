/*
 * chat_util.c
 *
 *  Created on: 2019/06/23
 *      Author: rimi kawasaki
 */
#include "mynet.h"
#include <stdlib.h>
#include <sys/select.h>
#include "chat.h"
#include <stdio.h>

#define NAMELENGTH 20 /* ログイン名の長さ制限 */
#define BUFLEN 500    /* 通信バッファサイズ */

/* 各クライアントのユーザ情報を格納する構造体の定義 */
typedef struct{
	int  sock;
	char name[NAMELENGTH];
} client_info;

/* プライベート変数 */
static int N_client;         /* クライアントの数 */
static client_info *Client;  /* クライアントの情報 */
static int Max_sd;               /* ディスクリプタ最大値 */
static char R_Buf[BUFLEN];     /* 受信用バッファ */
static char S_Buf[BUFLEN];     /* 送信用バッファ */

/* プライベート関数 */
static int client_login(int sock_listen);
static char *chop_nl(char *s);

void init_client(int sock_listen, int n_client)
{
	N_client = n_client;

	/* クライアント情報の保存用構造体の初期化 */
	if( (Client=(client_info *)malloc(N_client*sizeof(client_info)))==NULL ){
		exit_errmesg("malloc()");
	}

	/* クライアントのログイン処理 */
	Max_sd = client_login(sock_listen);
}
void chat_loop()
{
	fd_set mask, readfds;
	int client_id;
	int out_flag[N_client];
	int flagn=0;
	int len;
	int strsize;

	/* ビットマスク,out_flagの準備 */
	FD_ZERO(&mask);
	for(client_id=0; client_id<N_client; client_id++){
		FD_SET(Client[client_id].sock, &mask);
		out_flag[client_id]=0;
	}
	for(;;){
		/* 受信データの有無をチェック */
		readfds = mask;
		select( Max_sd+1, &readfds, NULL, NULL, NULL );

		for( client_id=0; client_id<N_client; client_id++ ){
			if( FD_ISSET(Client[client_id].sock, &readfds) ){
				strsize = Recv(Client[client_id].sock, R_Buf, BUFLEN-1,0);
				R_Buf[strsize]='\0';

				/* 送信する文字列を作成 */
				if(strcmp(R_Buf,"logout\n")==0||strsize==0){
					len=snprintf(S_Buf, BUFLEN, "---- %s logout----\n",Client[client_id].name);
					Send(Client[client_id].sock, "logout", 6, 0);
					close(Client[client_id].sock);
					out_flag[client_id]=1;
					flagn++;

					if(flagn==N_client){
						printf("all Client logout.\n");
						exit(0);
					}

					printf("Client[%d]logout.\n",client_id);
					FD_CLR(Client[client_id].sock,&mask);
				}else{
					len=snprintf(S_Buf, BUFLEN, "[%s]%s",Client[client_id].name,R_Buf);
				}

				/* 文字列を送信する */
				for(client_id=0; client_id<N_client; client_id++){
					if(out_flag[client_id]==0){
						Send(Client[client_id].sock, S_Buf, len+1, 0);
					}
				}
			}
		}
	}
}
static int client_login(int sock_listen)
{
	int client_id,sock_accepted;
	static char prompt[]="Input your name:";
	char loginname[NAMELENGTH];
	int strsize;
	for( client_id=0; client_id<N_client; client_id++){

		/* クライアントの接続を受け付ける */
		sock_accepted = accept(sock_listen, NULL, NULL);
		printf("Client[%d] connected.\n",client_id);

		/* ログインプロンプトを送信 */
		Send(sock_accepted, prompt, strlen(prompt), 0);

		/* ログイン名を受信 */
		strsize = recv(sock_accepted, loginname, NAMELENGTH-1, 0);
		loginname[strsize] = '\0';
		chop_nl(loginname);
		strsize=snprintf(S_Buf, BUFLEN, "%s\n",loginname);
		Send(sock_accepted,S_Buf,strsize+1,0);

		/* ユーザ情報を保存 */
		Client[client_id].sock = sock_accepted;
		strncpy(Client[client_id].name, loginname, NAMELENGTH);
	}
	return(sock_accepted);
}
static char *chop_nl(char *s)
{
	int len;
	len = strlen(s);

	if( s[len-1] == '\n' ){
		s[len-1] = '\0';
	}
	return(s);
}



