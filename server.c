//���� �ʱ�ȭ -> ���ϻ��� -> bind->listen->accept-> ��������->��������

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include<Windows.h>
#include<process.h>
#include<stdlib.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 300

unsigned WINAPI HandleClient(void* arg); //������ �Լ� 
										 //WINAPI : __stdcall
										 //�Լ� ��ȯ���� �ڷ���, �Լ�ȣ��Ծ�, �Ű������� �ڷ���,���� -> �Լ��� ����
void SendMsg(char* msg, int len);//�޼��� ������ �Լ�
int Word_Check(char *input, char *prev_word);

int clientCount = 0;
SOCKET clientSocks[30]; //Ŭ���̾�Ʈ ���� ������ �迭

typedef struct _packet {
	int type; // 1�̸� �ܾ��Է�, 2�̸� ä��
	char buffer[101];
	char re[20];
	char name[21];
}Packet;

int main(int argc, char* argv[]) {
	//1. �������α׷����� �Լ��� ����Ϸ��� ����ü ����(SOCKET)
	//2. �ʱ�ȭ �Լ��� ����Ͽ� �������α׷��� �ʱ�ȭ(WSADATA wsa;)
	//3. ������ �����ϱ� ���ؼ� SOCKET sock; ����ü�� ����
	//4. ���� �ּ� ����ü�� Ȱ���Ͽ� �ּ� ü�� Ȯ��(struct sockaddr_in sockinfo, bind)
	//5. ������ ���(listen,accept)
	//5-1. listen : ���� �̳��� �������� Ŭ���̾�Ʈ �ϴ¸� ��� �����·� �־�
	//5-2. accept : listen�� ��û�� �����ͼ� ���� ���α׷� �ȿ� Ŭ���̾�Ʈ ���� ���� �����
	//6. send �Լ��� �̿��� ���� �ϳ��� �����ϰ� ����(send,closesocket)

	SOCKET sock, clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo, clientinfo;
	int clientsize;
	char message[] = "sucess";
	HANDLE hThread;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) //�ʱ�ȭ
	{
		printf("�ʱ�ȭ ����\n");
		return 0;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//sock=socket(af,type,protocol);
	//af=�ּ�ü�� , type=����Ÿ�� , protocol=��������
	//���ͳ� ������ ����ϴ� TCP����� ��������
	if (sock == INVALID_SOCKET)
	{
		printf("���� ���� ����\n");
		return 0;
	}

	memset(&sockinfo, 0x00, sizeof(sockinfo));
	sockinfo.sin_family = AF_INET; //sockinfo ����ü�� sin_family�� ���ͳ� �ּ� ���
	sockinfo.sin_port = htons(5959); //��Ʈ ����
	sockinfo.sin_addr.s_addr = htonl(INADDR_ANY); //0.0.0.0�ּҸ� ����Ͽ� 
												  //��� ��밡���� �ּҷκ��� ���

	if (bind(sock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
	{
		//int bind (���� ��,(SOCKADDR*)&�����ּұ���ü ���尪,sizeof(���ϱ���ü ���尪))
		//bind �ּҸ� ����Ͽ� ���� ����ü�� ������ ������ ���Ͽ� ����
		printf("bind ����");
		return 0;
	}


	if (listen(sock, 5) == SOCKET_ERROR)
	{
		printf("��⿭ ����\n");
		return 0;
	}
	//int listen(SOCKET,backlog)
	//SOCKET :���� ������ ���� �����ߴ� ���� �ڵ� ��
	//BAcklog : ��ΰ�

	// only ����
	clientsize = sizeof(clientinfo);
	printf("Ŭ���̾�Ʈ�κ��� ������ ��ٸ��� �ֽ��ϴ�...\n");

	for (i = 0; i<4; i++)
	{
		clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize);
		//accept(���� ����,((SOCKADDR*)&Ŭ���̾�Ʈ�ּҰ�,&Ŭ���̾�Ʈ �ּҰ� ������))
		//Ŭ���̾�Ʈ�� ���� �� ������ ����� �־�� �ϹǷ� clientsock���� ����

		if (clientsock == INVALID_SOCKET)
		{
			printf("Ŭ���̾�Ʈ ���� ���� ����");
			return 0;
		}
		clientSocks[clientCount++] = sock;
		//Ŭ���̾�Ʈ ���Ϲ迭����� ������ ���� �ּҸ� ����
		_beginthreadex_proc_type

			hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClient, (void*)&sock, 0, NULL);
		//HandleClient ������ ����
	}

	closesocket(clientsock);
	closesocket(sock);
	WSACleanup();
	return 0;
}


unsigned WINAPI HandleClient(void* arg) {
	SOCKET clientSock = *((SOCKET*)arg); //�Ű������� ���� Ŭ���̾�Ʈ ��������
	int strLen = 0, i;
	char name[21];
	Packet packet;
	FILE *file_dictionary, *file_note;
	file_dictionary = fopen("dictionary.txt", "rt");
	file_note = fopen("note.txt", "wt");

	if ((strlen = recv(clientSocks, (char*)&packet, sizeof(packet), 0)) != -1)
		strcpy(name, packet.buffer);

	while ((strLen = recv(clientSock, (char*)&packet, sizeof(packet), 0)) != -1) {

		//recv�Լ�:��ȯ���� ���� ������ ũ�� , �����ϸ� -1
		if (packet.type == 1) {
			Word_Check(char *input, char *prev_word);
			// �ܾ ��ġ�ϴ� �˻� -> �ܾ��忡 �ִ��� <����1>
			// �ܾ� ���ڸ��� �� ���� �Դ� ���ڸ��� ������ <����2>
			// ������?
			//      �� �ܾ ��ȯ
			// �ٸ���?
			//      0
			// �ߺ��Ǹ�?
			//      1
		}
		else if (packet.type == 2) {
			SendMsg((char*)&packet, strLen);
		}
		else {
			strcpy(packet.buffer, "error");
			SendMsg((char*)&packet, strLen);
		}
	}
	//�� ���� �����Ѵٴ� ���� �ش� Ŭ���̾�Ʈ�� �����ٴ� ���
	//���� �ش� Ŭ���̾�Ʈ�� �迭���� ������Ѵ�
	for (i = 0; i < clientCount; i++) { //�迭�� ������ŭ
		if (clientSock == clientSocks[i]) { //���� ���� clientSocks���� �迭�� ���� ���ٸ�
			while (i++ < clientCount - 1)//Ŭ���̾�Ʈ ������ŭ
				clientSocks[i] = clientSocks[i + 1]; //������ �����
			break;
		}
	}
	clientCount--;
	closesocket(clientSock);//������ �����Ѵ�
	return 0;
}

void SendMsg(Packet* packet, int len) { //�޼����� ��� Ŭ���̾�Ʈ�� ������
	int i;
	for (i = 0; i < clientCount; i++)
		send(clientSocks[i], (char*)&packet, sizeof(Packet), 0);
}

int Word_Check(char *input, char *prev_word)
{
	int i = 0, flag = 0;
	char file_load[100];
	char Player_input[32];
	FILE *file_pointer;
	file_pointer = fopen("Word.txt", "rt");

	//���� ������ �������
	if (file_pointer == NULL)
	{
		Clear_line(PRINT_WORD_CHAIN_X, 9);
		gotoxy(PRINT_WORD_CHAIN_X, 9);
		printf("���� ������ �����ϴ�. Word������ ������ �̿��� �ּ���.");
		return -1;
	}

	//�Էµ� ���ڿ����� !�� ����
	for (i = 1; i < 32; i++)
		Player_input[i - 1] = input[i];

	//������ �˻� �� �ܾ ������ 0�� ����
	while (!feof(file_pointer))
	{
		fgets(file_load, 99, file_pointer); //���Ͽ��� ���پ� �Է�
		if (!strcmp(file_load, Player_input)) //���Ͽ��ִ� ���ڿ��� �ϳ���� Player_input�� ���� ���� ������
		{
			Clear_line(PRINT_WORD_CHAIN_X, 9);
			gotoxy(PRINT_WORD_CHAIN_X, 9);
			printf("�ܾ �����մϴ�.");
			return 0;
		}
	}

	//������ 1�� ����
	Clear_line(PRINT_WORD_CHAIN_X, 9);
	gotoxy(PRINT_WORD_CHAIN_X, 9);
	printf("�ܾ �����ϴ�");
	return 1;
}