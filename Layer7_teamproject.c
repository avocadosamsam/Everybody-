#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#define PRINT_WORD_CHAIN_X 40
DWORD WINAPI ThreadFunc(LPVOID);
// DWORD�� ������� == long
// WINAPI �����쿡���� API
//LPVOID ms���� ����ϴ� void������

char Chat[101];
char Chatingchang[20][32];
HANDLE hThrd_Word, hThrd_Chating;
HANDLE WORD_EVENT, CHATING_EVENT;
DWORD threadId_Word, threadId_Chating;

typedef struct player
{
	char name[21];
	int life;
	int score;
}Player_Information;

//�Լ� ���� ����
void Word_Chain(LPVOID n);
int Word_Check(char *input);
void Chating(LPVOID n);
void gotoxy(int x, int y);
void Clear_line(x, y);


int main(int argc, char **argv)
{
	int i;

	hThrd_Word = CreateThread(NULL, 0, (DWORD WINAPI)Word_Chain, 0, 0, &threadId_Word);
	hThrd_Chating = CreateThread(NULL, 0, (DWORD WINAPI)Chating, 0, 0, &threadId_Chating);

	WORD_EVENT = CreateEvent(NULL, TRUE, FALSE, NULL);
	CHATING_EVENT = CreateEvent(NULL, TRUE, FALSE, NULL);

	WaitForSingleObject(hThrd_Word, INFINITE);
}

void Word_Chain(LPVOID n)
{
	Player_Information *Players;
	int player_count, i, j, k, flag = 0, word_flag = 0, input_flag = 0, Chain = 0;
	char dump, end_word, first_word;
	char Player_input[101], Prev_input[101];

	// ���� ���� //

	//�÷��̾��� �� �Է�
	gotoxy(PRINT_WORD_CHAIN_X, 4);
	printf("���Բ� �����ձ� - Beta");
	gotoxy(PRINT_WORD_CHAIN_X, 5);
	printf("�÷��̾� ���� �Է��� �ּ��� : ");
	scanf("%d", &player_count);
	scanf("%c", &dump);

	Clear_line(PRINT_WORD_CHAIN_X, 4);
	Clear_line(PRINT_WORD_CHAIN_X, 5);

	//�÷��̾� �� ��ŭ �迭 �Ҵ�
	Players = (Player_Information*)malloc(player_count * sizeof(Player_Information));

	//�÷��̾� ������ 3���� �ʱ�ȭ
	for (i = 0; i < player_count; i++)
		Players[i].life = 3;

	//�÷��̾� �̸� �Է�
	for (i = 0; i < player_count; i++)
	{
		Clear_line(PRINT_WORD_CHAIN_X, 15);
		gotoxy(PRINT_WORD_CHAIN_X, 15);
		printf("�÷��̾� %d�� �̸��� �Է��� �ּ��� : ", i + 1);
		gets_s(Players[i].name, 20);
	}

	////

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

			//�� �Է� �ܾ� ���
			if (Chain != 0)
			{
				Clear_line(PRINT_WORD_CHAIN_X, 12);
				gotoxy(PRINT_WORD_CHAIN_X, 12);
				printf("�� �Է� �ܾ� : %s", Prev_input);
			}
			//�÷��̾� �ܾ� �Է�
			Clear_line(PRINT_WORD_CHAIN_X, 15);
			gotoxy(PRINT_WORD_CHAIN_X, 15);

			printf("�÷��̾� %s�� �Է����ּ��� : ", Players[i].name);

			SetEvent(WORD_EVENT);
			ResetEvent(CHATING_EVENT);
			WaitForSingleObject(CHATING_EVENT, INFINITE);

			if (Chat[0] == '!')
			{
				if (Chain != 0) // ù ������ ��� �ƹ� �ܾ �Է� ����
				{
					//�� �Է� �� �ܾ�� �� �Է� ù �ܾ� ��
					if (Player_input[0] != Prev_input[strlen(Prev_input) - 2])
					{
						Clear_line(PRINT_WORD_CHAIN_X, 9);
						gotoxy(PRINT_WORD_CHAIN_X, 9);
						printf("�� �ܾ��� �� �ܾ�� �� �ܾ��� ù �ܾ �ٸ��ϴ�.");
						Players[i].life--;
						input_flag = 1;
						continue;
					}
				}

				//�÷��̾� �ܾ� üũ
				word_flag = Word_Check(Chat, Prev_input);

				if (word_flag == 1) // �ܾ ������ ���� ��� 
				{
					Players[i].life--;
					input_flag = 1;
				}
				else if (word_flag == -1) // ������ ���� ���
					SetEvent(hThrd_Word);

				if (input_flag == 0) // �ܾ ������� �� �ܾ� �迭�� �Է´ܾ� ����
				{
					for (k = 1; k < 32; k++)
					{
						Prev_input[k-1] = Chat[k];
					}
					Chain = 1;
				}

				//�÷��̾���� ������ ���� üũ
				for (j = 0, flag = 0; j < player_count; j++)
				{
					if (Players[j].life == 0)
						flag++;
				}

				if (flag == player_count - 1) // �Ѹ��� ������ ��� �÷��̾��� �������� 0�� ���
					break;
			}

			else
			{
				i--;
				Clear_line(1, 1);
				gotoxy(1, 1);
				printf("%s", Chat);
			}
		}

		//�÷��̾���� ������ ���� üũ
		for (j = 0, flag = 0; j < player_count; j++)
		{
			if (Players[j].life == 0)
				flag++;
		}

		if (flag == player_count - 1) // �Ѹ��� ������ ��� �÷��̾��� �������� 0�� ���
			break;

	}

	//life�� 0�� �ƴ� �÷��̾� Ž��
	for (i = 0; i < player_count; i++)
	{
		if (Players[i].life != 0)
		{
			Clear_line(PRINT_WORD_CHAIN_X, 15);
			gotoxy(PRINT_WORD_CHAIN_X, 15);
			printf("�÷��̾� %s���� �¸� �Ͽ����ϴ�!", Players[i].name);
			break;
		}
	}

	SetEvent(hThrd_Word);
}

//�ܾ� üũ �Լ�
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

	for (i = 1; i < 32; i++)
		Player_input[i - 1] = input[i];

	//������ �˻� �� �ܾ ������ 0�� ����
	while (!feof(file_pointer))
	{
		fgets(file_load, 99, file_pointer);
		if (!strcmp(file_load, Player_input))
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

void gotoxy(int x, int y)
{
	COORD pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Clear_line(x, y)
{
	gotoxy(x, y);
	printf("                                                            ");
}

void Chating(LPVOID n)
{
	FILE *file_pointer;

	if (file_pointer = fopen("Chating_Log.txt", "a") == NULL)
	{
		printf("���� �б� ����");
		return;
	}

	while (1)
	{
		ResetEvent(WORD_EVENT);
		WaitForSingleObject(WORD_EVENT, INFINITE);
		gotoxy(PRINT_WORD_CHAIN_X + 28, 15);
		fgets(Chat, 100, stdin);
		SetEvent(CHATING_EVENT);
	}

	fclose(file_pointer);
	SetEvent(hThrd_Chating);
}