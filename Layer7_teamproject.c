#include <stdio.h>
#include <stdlib.h>

typedef struct player
{
	char name[21];
	int life;
	int score;
}Player_Information;

//함수 원형 선언
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

	//플레이어의 수 입력
	printf("다함께 끝말잇기 - Beta\n\n");
	printf("플레이어 수를 입력해 주세요 : ");
	scanf("%d", &player_count);
	scanf("%c", &dump);

	//플레이어 수 만큼 배열 할당
	Players = (Player_Information*)malloc(player_count * sizeof(Player_Information));

	//플레이어 라이프 3으로 초기화
	for (i = 0; i < player_count; i++)
		Players[i].life = 3;

	//플레이어 이름 입력
	for (i = 0; i < player_count; i++)
	{
		system("cls");
		printf("플레이어 %d의 이름을 입력해 주세요 : ", i + 1);
		gets_s(Players[i].name, 20);
	}

	//끝말잇기 부분
	while (1)
	{
		//플레이어 단어 입력
		for (i = 0; i < player_count; i++)
		{
			//플레이어 라이프가 0 일경우 입력 단계 스킵
			if (!Players[i].life)
				continue;
			system("cls");

			//플레이어 단어 입력
			printf("플레이어 %s님 입력해주세요 : ", Players[i].name);
			gets_s(Player_input, 100);

			//플레이어 단어 체크
			word_flag = Word_Check(Player_input);
			if (word_flag == -1)
				return;
			else if (word_flag == 0)
				Players[i].life--;
		}

		//플레이어들의 라이프 상태 체크
		for (i = 0; i < player_count; i++)
		{
			if (Players[i].life == 0)
				flag++;
		}

		if (flag!=0)
			break;
	}
}

//단어 체크 함수
int Word_Check(char *input)
{
	int i = 0 , flag = 0;
	char file_load[100];
	FILE *file_pointer;
	file_pointer = fopen("Word.txt", "rt");
	if (file_pointer == NULL)
	{
		printf("사전 파일이 없습니다. Word파일을 설정후 이용해 주세요\n");
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

