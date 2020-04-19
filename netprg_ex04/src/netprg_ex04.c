/*
 ============================================================================
 Name        : ex04.c
 Author      : rimi kawasaki
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "chat.h"
#include "mynet.h"
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_LEN 256     /* サーバ名格納用バッファサイズ */
#define DEFAULT_PORT 50000 /* ポート番号既定値 */
#define DEFAULT_NCLIENT 3  /* 省略時のクライアント数 */
#define DEFAULT_MODE 'S'   /* 省略時はサーバー */
#define S_BUFSIZE 100   /* 送信用バッファサイズ */
#define R_BUFSIZE 100   /* 受信用バッファサイズ */
#define SUBWIN_LINES 5 /* サブウィンドウの行数 */

extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char *argv[])
{
	int port_number=DEFAULT_PORT;
	int num_client =DEFAULT_NCLIENT;
	char servername[SERVER_LEN] = "localhost";
	char mode = DEFAULT_MODE;
	int c;

	/* オプション文字列の取得 */
	opterr = 0;
	while( 1 ){
		c = getopt(argc, argv, "SCs:p:c:h");
		if( c == -1 ) break;
		switch( c ){
		case 'S' :  /* サーバモードにする */
			mode = 'S';
			break;
		case 'C':   /* クライアントモードにする */
			mode = 'C';
			break;
		case 's' :  /* サーバ名の指定 */
			snprintf(servername, SERVER_LEN, "%s", optarg);
			break;
		case 'p':  /* ポート番号の指定 */
			port_number = atoi(optarg);
			break;
		case 'c':  /* クライアントの数 */
			num_client = atoi(optarg);
			break;
		case '?' :
			fprintf(stderr,"Unknown option '%c'\n", optopt );
		case 'h' :
			fprintf(stderr,"Usage(Server): %s -S -p port_number -c num_client\n", argv[0]);
			fprintf(stderr,"Usage(Client): %s -C -s server_name -p port_number\n", argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}

	switch(mode){
	case 'S':
		chat_server(port_number, num_client);	/* サーバ部分ができたらコメントを外す */
		break;
	case 'C':
		chat_client(servername, port_number);
		break;
	}
	exit(EXIT_SUCCESS);
}

void chat_server(int port_number, int n_client)
{
	int sock_listen;

	/* サーバの初期化 */
	sock_listen = init_tcpserver(port_number, 5);

	/* クライアントの接続 */
	init_client(sock_listen, n_client);
	close(sock_listen);

	/* メインループ */
	chat_loop();
}

void chat_client(char* servername, int port_number)
{
	int sock;
	char s_buf[S_BUFSIZE], r_buf[R_BUFSIZE];
	int strsize;
	fd_set mask, readfds;
	WINDOW *win_main, *win_sub;

	/* サーバに接続する */
	sock = init_tcpclient(servername, port_number);
	printf("Connected.\n");

	/* 画面の初期化 */
	initscr();

	/* Windowを作る */
	win_main = newwin(LINES-SUBWIN_LINES, COLS, 0, 0);
	win_sub  = newwin(SUBWIN_LINES, COLS, LINES-SUBWIN_LINES, 0);

	/* スクロールを許可する */
	scrollok(win_main, TRUE);
	scrollok(win_sub, TRUE);

	wmove(win_main, 0,10);  /* カーソルを動かす */
	wprintw(win_main, "Chat Program\t Send 'logout' to eixt\n"); /* 文字列を出力 */

	/* 画面を更新 */
	wrefresh(win_main);
	wrefresh(win_sub);

	/* ビットマスクの準備 */
	FD_ZERO(&mask);
	FD_SET(0, &mask);
	FD_SET(sock, &mask);

	for(;;){
		/* 受信データの有無をチェック */
		readfds = mask;
		select( sock+1, &readfds, NULL, NULL, NULL );

		if( FD_ISSET(0, &readfds) ){
			/* サブウィンドウでキーボードから文字列を入力する */
			wgetnstr(win_sub, s_buf, S_BUFSIZE);
			strsize = strlen(s_buf);
			s_buf[strsize] = '\n';	/* 文字列の最後に改行コードを入れておく */
			Send(sock, s_buf, strsize+1, 0);
		}
		if(FD_ISSET(sock, &readfds)){
			/* サーバから文字列を受信する */
			strsize = Recv(sock, r_buf, R_BUFSIZE-1, 0);
			if(strsize==0){
				endwin();	/* 画面の設定を元に戻す */
				printf("Server is down.\n");
				exit(1);
			}
			r_buf[strsize] = '\0';
			if(strcmp(r_buf,"logout")==0){
				endwin(); /* 画面の設定を元に戻す */
				printf("logout.\n");
				exit(0);
			}
			wprintw(win_main, "%s", r_buf);	/* メインウィンドウに出力 */
			wrefresh(win_main);
			wclear(win_sub);	/* サブウィンドウをクリア */
		}
	}
}


