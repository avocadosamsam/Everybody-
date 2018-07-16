//���� �ʱ�ȭ -> ���ϻ��� -> bind->listen->accept-> ��������->��������
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#pragma comment(lib,"ws2_32.lib")


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

	clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize);
	//accept(���� ����,((SOCKADDR*)&Ŭ���̾�Ʈ�ּҰ�,&Ŭ���̾�Ʈ �ּҰ� ������))
	//Ŭ���̾�Ʈ�� ���� �� ������ ����� �־�� �ϹǷ� clientsock���� ����

	if (clientsock == INVALID_SOCKET)
	{
		printf("Ŭ���̾�Ʈ ���� ���� ����");
		return 0;
	}

	send(clientsock, message, sizeof(message), 0);

	closesocket(clientsock);
	closesocket(sock);

	printf("����\n");

	WSACleanup();

	return 0;
}