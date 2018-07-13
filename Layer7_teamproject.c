#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

DWORD WINAPI ThreadFunc(LPVOID);
// DWORD�� ������� == long
// WINAPI �����쿡���� API
//LPVOID ms���� ����ϴ� void������

int global = 1;
int end_flag = 0;

typedef struct player
{
	char name[21];
	int life;
	int score;
}Player_Information;

//�Լ� ���� ����
void Word_Chain(LPVOID n);
int Word_Check(char *input);


int main(int argc, char **argv)
{
	HANDLE hThrd_Word,hThrd_Test;
	DWORD threadId_Word, threadId_Test;
	int i;

	hThrd_Word = CreateThread(NULL, 0, (DWORD WINAPI)Word_Chain,0, 0, &threadId_Word);

	Sleep(10000000000000000);
}

void Word_Chain(LPVOID n)
{
	Player_Information *Players;
	int player_count, i, j, k, flag = 0, word_flag = 0, input_flag = 0, Chain = 0;
	char dump, end_word, first_word;
	char Player_input[101], Prev_input[101];

	//�÷��̾��� �� �Է�
	printf("���Բ� �����ձ� - Beta\n\n");
	printf("�÷��̾� ���� �Է��� �ּ��� : ");
	scanf("%d", &player_count);
	scanf("%c", &dump);

	//�÷��̾� �� ��ŭ �迭 �Ҵ�
	Players = (Player_Information*)malloc(player_count * sizeof(Player_Information));

	//�÷��̾� ������ 3���� �ʱ�ȭ
	for (i = 0; i < player_count; i++)
		Players[i].life = 3;

	//�÷��̾� �̸� �Է�
	for (i = 0; i < player_count; i++)
	{
		printf("�÷��̾� %d�� �̸��� �Է��� �ּ��� : ", i + 1);
		gets_s(Players[i].name, 20);
	}

	//�����ձ� �κ�
	while (1)
	{

		//�÷��̾� �ܾ� �Է�
		for (i = 0; i < player_count; i++)
		{
			input_flag = 0;

			//�÷��̾� �������� 0 �ϰ�� �Է� �ܰ� ��ŵ
			if (!Players[i].life)
				continue;

			//�÷��̾� �ܾ� �Է�
			printf("\n�÷��̾� %s�� �Է����ּ��� : ", Players[i].name);

			fgets(Player_input, 100, stdin); // �������� \n�� ������ �ҷ����� ������ fgets���

			if (Chain != 0) // ù ������ ��� �ƹ� �ܾ �Է� ����
			{
				//�� �Է� �� �ܾ�� �� �Է� ù �ܾ� ��
				if (Player_input[0] != Prev_input[strlen(Prev_input) - 2])
				{
					printf("\n�� �ܾ��� �� �ܾ�� �� �ܾ��� ù �ܾ �ٸ��ϴ�.\n");
					Players[i].life--;
					input_flag = 1;
					continue;
				}
			}

			//�÷��̾� �ܾ� üũ
			word_flag = Word_Check(Player_input);

			if (word_flag == 1) // �ܾ ������ ���� ��� 
			{
				Players[i].life--;
				input_flag = 1;
			}
			else if (word_flag == -1) // ������ ���� ���
				return 0;

			if (input_flag == 0)
			{
				strcpy(Prev_input, Player_input);
				Chain = 1;
			}
		}

		//�÷��̾���� ������ ���� üũ
		for (i = 0, flag = 0; i < player_count; i++)
		{
			if (Players[i].life == 0)
				flag++;
		}

		if (flag == player_count - 1) // ��� �÷��̾��� �������� 0�� ���
			break;

	}

	//life�� 0�� �ƴ� �÷��̾� Ž��
	for (i = 0; i < player_count; i++)
	{
		if (Players[i].life != 0)
		{
			printf("\n�÷��̾� %s���� �¸� �Ͽ����ϴ�!\n", Players[i].name);
			break;
		}
	}

	exit(EXIT_SUCCESS);
}

//�ܾ� üũ �Լ�
int Word_Check(char *input)
{
	int i = 0, flag = 0;
	char file_load[100];
	FILE *file_pointer;
	file_pointer = fopen("Word.txt", "rt");

	//���� ������ �������
	if (file_pointer == NULL)
	{
		printf("\n���� ������ �����ϴ�. Word������ ������ �̿��� �ּ���.\n");
		return -1;
	}

	//������ �˻� �� �ܾ ������ 0�� ����
	while (!feof(file_pointer))
	{
		fgets(file_load, 99, file_pointer);
		if (!strcmp(file_load, input))
		{
			printf("\n�ܾ �����մϴ�.\n");
			return 0;
		}
	}
	
	//������ 1�� ����
	printf("\n�ܾ �����ϴ�\n");
	return 1;
}

