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
 * @brief Client�p�̃l�b�g���[�N���������BIP�A�h���X����͂�����("xx.xx.xx.xx"->"yes")
 * @param [] 
 */
void IN_InitNetC(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	int me_player=ActC_GetMePlayer();
	dest.sin_family = AF_INET;
	char server_ip_dest[255]="192.168.4.138";//���X��Net.h���ɂ�����
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

	//�\�P�b�g�̐���
	sock = socket(AF_INET, SOCK_STREAM, 0);

	//�T�[�o�ւ̐ڑ�
	if(connect(sock, (struct sockaddr *) &dest, sizeof(dest))){
		printf("%s�ɐڑ��ł��܂���ł���\n", server_ip_dest);
		//return -1;
		exit(-1);
	}
	printf("%s�ɐڑ����܂���\n", server_ip_dest);

	// �����ŁA�m���u���b�L���O�ɐݒ肵�Ă��܂��B
	// val = 0�Ńu���b�L���O���[�h�ɐݒ�ł��܂��B
	// �\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�ł��B
	u_long val=1;
	ioctlsocket(sock, FIONBIO, &val);

}
/*!
* @brief 1�p�P�b�g���M�@�\�P�b�g��n�����߂������o�R
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
 * @brief ����mainloop�B�����̑������p�P�b�g�ɑ΂��郊�v���C��҂B
          �Q�[���i�s�����͎�M�����p�P�b�g�����ɂ����Ȃ��B
 * @param [] 
 */
int NetC_RecieveLoop(void)
{
	char buf[MAX_BUF_SIZE];
	int n;
	int me_player = ActC_GetMePlayer();
	while(1)
	{
		//�T�[�o�Ƀf�[�^�𑗐M
		n = recv(sock, buf, MAX_BUF_SIZE, 0);
		//�T�[�o����f�[�^����M
		if (n < 1) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				// �܂����Ȃ��B
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
			/* �}�E�X�N���b�N����ړ] 2013.02.17 */
			Act_UpdateButtonLst(me_player);
			St_DrawMode();
			Dsp_UpdateMainWindow();
			//u_long val=1;
			//ioctlsocket(sock, FIONBIO, &val);
		}
		/* �����Ń}�E�X�N���b�N�����o�B���M���� */
		waitKey(100);
	}
	closesocket(sock);

	WSACleanup();

	return 0;

}