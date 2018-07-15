//���� �ʱ�ȭ -> ���ϻ��� -> bind->listen->accept-> ��������->��������
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include<Windows.h>
#include<process.h>
#include<stdlib.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 300

char dictionary_list[1000000][30];
int dictionary_count = 0;
char word_list[1000000][30];
int word_count = 0;

void ReadWordList();

typedef struct _packet {
	int type; // 1�̸� �ܾ��Է�, 2�̸� ä��
	char buffer[100];
}Packet;

unsigned WINAPI HandleClient(void* arg); //������ �Լ� 
//WINAPI : __stdcall
//�Լ� ��ȯ���� �ڷ���, �Լ�ȣ��Ծ�, �Ű������� �ڷ���,���� -> �Լ��� ����
void SendMsg(Packet* packet, int len);//�޼��� ������ �Լ�

int clientCount = 0;
SOCKET clientSocks[30]; //Ŭ���̾�Ʈ ���� ������ �迭


int main(int argc, char* argv[]) {
	//1. �������α׷����� �Լ��� ����Ϸ��� ����ü ����(SOCKET)
	//2. �ʱ�ȭ �Լ��� ����Ͽ� �������α׷��� �ʱ�ȭ(WSADATA wsa;)
	//3. ������ �����ϱ� ���ؼ� SOCKET sock; ����ü�� ����
	//4. ���� �ּ� ����ü�� Ȱ���Ͽ� �ּ� ü�� Ȯ��(struct sockaddr_in sockinfo, bind)
	//5. ������ ���(listen,accept)
	//5-1. listen : ���� �̳��� �������� Ŭ���̾�Ʈ �ϴ¸� ��� �����·� �־�
	//5-2. accept : listen�� ��û�� �����ͼ� ���� ���α׷� �ȿ� Ŭ���̾�Ʈ ���� ���� �����
	//6. send �Լ��� �̿��� ���� �ϳ��� �����ϰ� ����(send,closesocket)

	SOCKET sock,clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo, clientinfo;
	int clientsize;
	char message[] = "sucess";
	HANDLE hThread;

	ReadWordList();

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

	clientsize = sizeof(clientinfo);
	printf("Ŭ���̾�Ʈ�κ��� ������ ��ٸ��� �ֽ��ϴ�...\n");

	while (1)
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
		
		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClient, (void*)&sock, 0, NULL);
		//HandleClient ������ ����
	}
	closesocket(clientsock);
	closesocket(sock);
	WSACleanup();
	return 0;
}


void ReadWordList()
{
	FILE *file_dictionary;
	char buffer[30];
	file_dictionary = fopen("dictionary.txt", "rt");
	if (file_dictionary == NULL)
	{
		printf("File error");
	}
	//////////
	//fscanf�� ��ȯ���� ���������� ���� ������ ����
	//������ ���� ���̴� Ʈ����!
	while (fscanf(file_dictionary, "%s", buffer) == 1) {
		strcpy(word_list[word_count++], buffer);
	}
	//file_dictionary���� �о���� ��� word_list�� ����
	//////////
	fclose(file_dictionary);
}

unsigned WINAPI HandleClient(void* arg) {
	SOCKET clientSock = *((SOCKET*)arg); //�Ű������� ���� Ŭ���̾�Ʈ ��������
	int strLen = 0, i=0,j=0;
	char word[30],sameword[30];
	Packet packet;
	
	while ((strLen = recv(clientSock,(char*)&packet.buffer, sizeof(packet), 0)) != -1) {
		//recv�Լ�:��ȯ���� ���� ������ ũ�� , �����ϸ� -1
		//���� ���� �ܾ�� packet�� ����
		if (packet.type == 1) {
			// �ܾ ��ġ�ϴ� �˻� -> �ܾ��忡 �ִ��� <����1>
			// �ܾ� ���ڸ��� �� ���� �Դ� ���ڸ��� ������ <����2>
			// ������?
			//		�� �ܾ ��ȯ
			// �ٸ���?
			//		0
			// �ߺ��Ǹ�?
			//		1
			//1.���ϱ��̱��ϱ�
			int isIn = 0;
			for (int i = 0; i < dictionary_count; i++)
			{
				if (strcmp(dictionary_list[i], packet.buffer) == 0)
				{
					// ������ �־����� ? -> 1
					// ������ -> �ܾ��ȯ
					isIn = 1;
					break;
				}
			}

			if (isIn)
			{
				//word �˻��ؼ� ������ 1
				// ������ �ܾ�
				isIn = 0;
				for (int i = 0; i < word_count; i++) {
					if (strcmp(word_list[i], packet.buffer) == 0) {
						isIn = 1;
						break;
					}
				}
				if (isIn)
				{
					strcpy(packet.buffer, "1");
				}
				else
				{
					strcpy(word_list[word_count], packet.buffer);
					word_count++;
				}
			}
			else
			{
				strcpy(packet.buffer, "0");
				// 0
			}

			// ������ �ܾ��忡 ���� �ܾ�� -> 0

		}
		else if (packet.type == 2) {
			SendMsg(&packet, strLen);
		}
		else {
			strcpy(packet.buffer, "error");
			SendMsg(&packet, strLen);
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
		send(clientSocks[i],packet->buffer, len, 0);

}