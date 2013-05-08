#include "Net_Server.h"
#include "Act_Server.h"
#include "Act.h"
#include "State.h"
#include "Disp.h"
#include "Net.h"
/* 片方は関数内に内包できる。初期化用のがこっち */
static fd_set  readfds;
static SOCKET socks[NUM_OF_PLAYERS], sock_accept[NUM_OF_PLAYERS];

int IN_InitNetS()
{	
	printf("IN_InitNetS start\n");
	struct sockaddr_in addr_lst[NUM_OF_PLAYERS];
	WSADATA wsaData;
	int result;          //戻り値
	WSAStartup(MAKEWORD(2,0), &wsaData);

	// 受信ソケットを2つ作ります
	for(int i=0;i<NUM_OF_PLAYERS;i++)
	{
		socks[i] = socket(AF_INET, SOCK_STREAM, 0);
		addr_lst[i].sin_family = AF_INET;
		//addr_lst[i].sin_addr.S_un.S_addr = INADDR_ANY;
		//addr_lst[i].sin_addr.S_un.S_addr = inet_addr(server_ip_dest);
		addr_lst[i].sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		// 2つの別々のポートで待つために別のポート番号をそれぞれ設定します
		addr_lst[i].sin_port = htons(port+i);

		// 2つの別々のポートで待つようにbindします
		result = bind(socks[i], (struct sockaddr *)&(addr_lst[i]), sizeof(addr_lst[i]));
		if (result < 0){
			printf("%d\n", GetLastError());
			printf("バインドエラー:%d\n",i);
			return SOCKET_BIND_ERROR;
		}
		//接続の許可
		result = listen(socks[i], 1);
		if (result < 0){
			printf("接続許可エラー\n");
			return SOCKET_LISTEN_ERROR;
		}
	}

	/* acceptまで済ませる */
	for(int i=0;i<NUM_OF_PLAYERS;i++)
	{
		sock_accept[i]=accept(socks[i], NULL,NULL);
		if (sock_accept[i] < 0){
			printf("待機エラー\n");
			return SOCKET_ACCEPT_ERROR;
		}
		printf("client %d accepted\n",i);
	}

	/* Let's Catan Message */
		
	for(int i=0;i<NUM_OF_PLAYERS;i++)
	{
		//same process as 
		//Net_ProcessStartMessagePacket()
		if(Net_SendStartMessagePacket(&sock_accept[i])==SOCKET_SEND_ERROR)
			return SOCKET_SEND_ERROR;
		//NetS_SendSstartMessagePacket()
	}
	CatanInput input={0};
	St_Event(input);
	
	// fd_setを初期化します
	FD_ZERO(&readfds);

	// selectで待つ読み込みソケットとしてsock1を登録します
	for(int i=0;i<NUM_OF_PLAYERS;i++)
	{
		FD_SET(sock_accept[i], &readfds);
	}
	printf("IN_InitNetS end\n");

	return SOCKET_NO_ERROR;
}
//int NetS_SendSstartMessagePacket(SOCKET *s)
//{
//
//}
int NetS_RecieveLoop(void)
{
	static fd_set fds;
	char buf[MAX_BUF_SIZE];
	char ans[] = "succeed 12345678901234567890\n";
	char ans2[]="other client success\n";
	// 無限ループです
	// このサンプルでは、この無限ループを抜けません
	while (1) {
		// 読み込み用fd_setの初期化
		// selectが毎回内容を上書きしてしまうので、毎回初期化します
		memcpy(&fds, &readfds, sizeof(fd_set));

		// fdsに設定されたソケットが読み込み可能になるまで待ちます
		select(0, &fds, NULL, NULL, NULL);
		for(int i=0;i<NUM_OF_PLAYERS;i++)
		{
			// sock1に読み込み可能データがある場合
			//if (FD_ISSET(socks[i], &fds)) {
			if (FD_ISSET(sock_accept[i], &fds)) {
				memset(buf, 0, sizeof(buf));
				int n=recv(sock_accept[i], buf, sizeof(buf), 0);
				PacketType result;
				printf("received data\n");
			for(int i=0;i<n;i++)
			{
				DPRINTF("%d ",buf[i]);
			}
			DPRINTF("\n");
			printf("data size:%d\n", n);
				result=Net_ProcessPacket(buf,n);
				switch(result)
				{
				case PACKET_TYPE_VALID:
					//send(sock_accept[i], buf, n, 0);
					//for(int other_i=0;other_i<NUM_OF_PLAYERS;other_i++)
					//{
					//	if(other_i==i)continue;
					//	send(sock_accept[other_i],buf,n,0);
					//}
					/* 全員に通知 */
					for(int j=0;j<NUM_OF_PLAYERS;j++)
					{
						send(sock_accept[j], buf, n, 0);
					}
					break;
				case PACKET_TYPE_ERROR:
					exit(-1);
					break;
				case PACKET_GRAMMER_ERROR:
				case PACKET_TYPE_UNKNOWN:
					Net_SendEmptyPacket(&(sock_accept[i]));
					break;
				}
				printf("%s\n", buf);
				Act_UpdateButtonLst();
				St_DrawMode();
				Dsp_UpdateMainWindow();
				
			}
		}
	}
	// このサンプルでは、ここへは到達しません
	//closesocket(socks[0]);
	//closesocket(socks[1]);

	WSACleanup();

	return 0;


}