﻿// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	printf("플레이어 수: ");
	scanf_s("%d", &n_player);

	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		player[i] = true;
	}
	return 0;
}

void intro() {
	printf("             ___\n");
	printf("                     .-'   `'.\n");
	printf("                    /         \\\n");
	printf("                    |         ;\n");
	printf("                    |         |           ___.--,\n");
	printf("           _.._     |0) ~ (0) |    _.---'`__.-( (_.\n");
	printf("    __.--'`_.. '.__.\\    '--. \\_.-' ,.--'`     `\"\"\"\n");
	printf("   ( ,.--'`   ',__ /./;  ;, '.__.'`    __\n");
	printf("  _`) )  .---.__.' / |  |\\   \\__..--\"\"  \"\"\"--.,_\n");
	printf("  `---' .'.''-._.-'`_./  /\\ '.  \\ _.-~~~````~~~-._`-.__.'\n");
	printf("        | |  .' _.-' |  |  \\  \\  '.               `~---`\n");
	printf("         \\ \\/ .'     \\  \\   '. '-._)\n");
	printf("          \\/ /        \\  \\    `=__`~-.\n");
	printf("     jgs  / /\\         `) )    / / `\"\".`\\\n");
	printf("    , _.-'.'\\ \\        / /    ( (     / /\n");
	printf("     `--~`   ) )    .-'.'      '.'.  | (\n");
	printf("            (/`    ( (`          ) )  '-;\n");
	printf("             `      '-;         (-'\n");

	
	Sleep(2000); 
	system("cls");
	
	printf("*********************************\n");
	printf("쭈꾸미 게임에 오신 걸 환영합니다.\n");
	printf("*********************************\n");
	Sleep(3000);

	system("cls");
	return 0;
}
void ending() {

}

int main(void) {
	jjuggumi_init();
	intro();
	//sample(1);
	mugunghwa();
	//nightgame();
	//juldarigi();
	//jebi();
	ending();
	return 0;
}
