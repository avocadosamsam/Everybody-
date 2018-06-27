#include <stdio.h>
#include <stdlib.h>

typedef struct player
{
	char name[21];
	int life;
	int score;
}Player_Information;

//�Լ� ���� ����
void Word_Chain();
int Word_Check(char *input);


int main(int argc, char **argv)
{
	Word_Chain();
}

void Word_Chain()
{
	Player_Information *Players;
	int player_count, i, j, k, flag = 0, word_flag;
	char dump;
	char Player_input[101];

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
		system("cls");
		printf("�÷��̾� %d�� �̸��� �Է��� �ּ��� : ", i + 1);
		gets_s(Players[i].name, 20);
	}

	//�����ձ� �κ�
	while (1)
	{
		//�÷��̾� �ܾ� �Է�
		for (i = 0; i < player_count; i++)
		{
			//�÷��̾� �������� 0 �ϰ�� �Է� �ܰ� ��ŵ
			if (!Players[i].life)
				continue;
			system("cls");

			//�÷��̾� �ܾ� �Է�
			printf("�÷��̾� %s�� �Է����ּ��� : ", Players[i].name);
			gets_s(Player_input, 100);

			//�÷��̾� �ܾ� üũ
			word_flag = Word_Check(Player_input);
			if (word_flag == -1)
				return;
			else if (word_flag == 0)
				Players[i].life--;
		}

		//�÷��̾���� ������ ���� üũ
		for (i = 0; i < player_count; i++)
		{
			if (Players[i].life == 0)
				flag++;
		}

		if (flag!=0)
			break;
	}
}

//�ܾ� üũ �Լ�
int Word_Check(char *input)
{
	int i = 0 , flag = 0;
	char file_load[100];
	FILE *file_pointer;
	file_pointer = fopen("Word.txt", "rt");
	if (file_pointer == NULL)
	{
		printf("���� ������ �����ϴ�. Word������ ������ �̿��� �ּ���\n");
		return -1;
	}

	while (fgets(file_load, 99, file_pointer) != EOF)
	{
		if (file_load[i] == input[i])
			i++;
		if (input[i] == '\0'&&file_load == '0')
		{
			printf("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
			_sleep(5000);
			return 1;
		}
		if (input[i] == '\0' || file_load == '\0')
			break;
		memset(file_load, 0, 99);
	}
	printf("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	_sleep(5000);
	return 0;
}
