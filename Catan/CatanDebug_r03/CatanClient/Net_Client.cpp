#include "common.h"
#include "Net_Client.h"
#include "Act_Client.h"
#include "Act.h"
#include "State.h"
#include "Disp.h"
#include "Net.h"
static SOCKET sock;
static struct sockaddr_in dest;
using namespace cv;
/*!
 * @brief Client用のネットワーク環境初期化。IPアドレスを入力させる("xx.xx.xx.xx"->"yes")
 * @param [] 
 */
void IN_InitNetC(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	int me_player=ActC_GetMePlayer();
	dest.sin_family = AF_INET;
	char server_ip_dest[255]="192.168.4.138";//元々はNet.h内にあった
	bool isSetIpAddress=false;
	while(!isSetIpAddress)
	{
		string line;
		cout << "IP Address:" << server_ip_dest << endl;
		cout << "enter IP address or type yes" << endl;
		getline(cin,line);
		if(line == "yes")
		{
			break;
		}else
		{
			strcpy(server_ip_dest,line.c_str());
		}
	}
	dest.sin_addr.s_addr = inet_addr(server_ip_dest);
	dest.sin_port = htons(port+me_player);

	//ソケットの生成
	sock = socket(AF_INET, SOCK_STREAM, 0);

	//サーバへの接続
	if(connect(sock, (struct sockaddr *) &dest, sizeof(dest))){
		printf("%sに接続できませんでした\n", server_ip_dest);
		//return -1;
		exit(-1);
	}
	printf("%sに接続しました\n", server_ip_dest);

	// ここで、ノンブロッキングに設定しています。
	// val = 0でブロッキングモードに設定できます。
	// ソケットの初期設定はブロッキングモードです。
	u_long val=1;
	ioctlsocket(sock, FIONBIO, &val);

}
/*!
* @brief 1パケット送信　ソケットを渡すためここを経由
* @param [] 
*/
int NetC_SendInputPacket(CatanInput input)
{
	if(Net_SendInputPacket(input,&sock))
	{
		return SOCKET_SEND_ERROR;
	}
	return SOCKET_NO_ERROR;
}

/*!
 * @brief 実質mainloop。自分の送ったパケットに対するリプライを待つ。
          ゲーム進行処理は受信したパケットを元におこなう。
 * @param [] 
 */
int NetC_RecieveLoop(void)
{
	char buf[MAX_BUF_SIZE];
	int n;
	int me_player = ActC_GetMePlayer();
	while(1)
	{
		//サーバにデータを送信
		n = recv(sock, buf, MAX_BUF_SIZE, 0);
		//サーバからデータを受信
		if (n < 1) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				// まだ来ない。
				//printf("MADA KONAI\n");
			} else {
				printf("error : 0x%x\n", WSAGetLastError());
				break;
			}
		} else {
			printf("received data\n");
			for(int i=0;i<n;i++)
			{
				DPRINTF("%d ",buf[i]);
			}
			DPRINTF("\n");
			printf("data size:%d\n", n);
			Net_ProcessPacket(buf,n);
			/* マウスクリックから移転 2013.02.17 */
			Act_UpdateButtonLst(me_player);
			St_DrawMode();
			Dsp_UpdateMainWindow();
			//u_long val=1;
			//ioctlsocket(sock, FIONBIO, &val);
		}
		/* ここでマウスクリックを検出。送信処理 */
		waitKey(100);
	}
	closesocket(sock);

	WSACleanup();

	return 0;

}