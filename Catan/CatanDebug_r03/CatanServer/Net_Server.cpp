#include "Net_Server.h"
#include "Act_Server.h"
#include "Act.h"
#include "State.h"
#include "Disp.h"
#include "Net.h"
/* �Е��͊֐����ɓ���ł���B�������p�̂������� */
static fd_set  readfds;
static SOCKET socks[NUM_OF_PLAYERS], sock_accept[NUM_OF_PLAYERS];

int IN_InitNetS()
{	
	printf("IN_InitNetS start\n");
	struct sockaddr_in addr_lst[NUM_OF_PLAYERS];
	WSADATA wsaData;
	int result;          //�߂�l
	WSAStartup(MAKEWORD(2,0), &wsaData);

	// ��M�\�P�b�g��2���܂�
	for(int i=0;i<NUM_OF_PLAYERS;i++)
	{
		socks[i] = socket(AF_INET, SOCK_STREAM, 0);
		addr_lst[i].sin_family = AF_INET;
		//addr_lst[i].sin_addr.S_un.S_addr = INADDR_ANY;
		//addr_lst[i].sin_addr.S_un.S_addr = inet_addr(server_ip_dest);
		addr_lst[i].sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		// 2�̕ʁX�̃|�[�g�ő҂��߂ɕʂ̃|�[�g�ԍ������ꂼ��ݒ肵�܂�
		addr_lst[i].sin_port = htons(port+i);

		// 2�̕ʁX�̃|�[�g�ő҂悤��bind���܂�
		result = bind(socks[i], (struct sockaddr *)&(addr_lst[i]), sizeof(addr_lst[i]));
		if (result < 0){
			printf("%d\n", GetLastError());
			printf("�o�C���h�G���[:%d\n",i);
			return SOCKET_BIND_ERROR;
		}
		//�ڑ��̋���
		result = listen(socks[i], 1);
		if (result < 0){
			printf("�ڑ����G���[\n");
			return SOCKET_LISTEN_ERROR;
		}
	}

	/* accept�܂ōς܂��� */
	for(int i=0;i<NUM_OF_PLAYERS;i++)
	{
		sock_accept[i]=accept(socks[i], NULL,NULL);
		if (sock_accept[i] < 0){
			printf("�ҋ@�G���[\n");
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
	
	// fd_set�����������܂�
	FD_ZERO(&readfds);

	// select�ő҂ǂݍ��݃\�P�b�g�Ƃ���sock1��o�^���܂�
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
	// �������[�v�ł�
	// ���̃T���v���ł́A���̖������[�v�𔲂��܂���
	while (1) {
		// �ǂݍ��ݗpfd_set�̏�����
		// select��������e���㏑�����Ă��܂��̂ŁA���񏉊������܂�
		memcpy(&fds, &readfds, sizeof(fd_set));

		// fds�ɐݒ肳�ꂽ�\�P�b�g���ǂݍ��݉\�ɂȂ�܂ő҂��܂�
		select(0, &fds, NULL, NULL, NULL);
		for(int i=0;i<NUM_OF_PLAYERS;i++)
		{
			// sock1�ɓǂݍ��݉\�f�[�^������ꍇ
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
					/* �S���ɒʒm */
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
	// ���̃T���v���ł́A�����ւ͓��B���܂���
	//closesocket(socks[0]);
	//closesocket(socks[1]);

	WSACleanup();

	return 0;


}