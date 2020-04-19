# TCP-Chat
大学3回後期の課題  
TCPによるチャットプログラム
# 使用言語
C言語
# コマンドライン引数
* サーバー  
  -S -p "ポート番号" -n "クライアント数"
* クライアント  
  -C -s "サーバー名" -p "ポート番号"
# 仕様
* サーバー側立ち上げ後クライアント側がログインしていく  
* ログイン後サーバーが指定した人数がそろわないと対話が開始しない
* 人数がそろう前に送信されたメッセージは対話開始後に送信される
* 全員がログアウトした時、もしくはサーバー側が終了した時、通信が終了する