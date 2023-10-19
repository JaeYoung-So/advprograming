#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void mugung_init(void);
void move_manual(key_t key);
void move_random(int i, int dir);
void move_tail(int i, int nx, int ny);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

// "무궁화꽃이피었습니다"를 저장한 문자열
char mugung_text[] = "무궁화꽃이피었습니다";
int mugung_index = 0;  // 현재 출력할 문자의 인덱스
int delay = 300;  // 출력 간격(ms), 초기 값: 300ms

void say_mugung(int time) {
	
	if (time == 500) {
		gotoxy(17, 0);
		printf("무                                                        ");
	}
	if (time == 650) {
		gotoxy(17, 0);
		printf("무 궁                                                     ");
	}
	if (time == 850) {
		gotoxy(17, 0);
		printf("무 궁 화                                                  ");
	}
	if (time == 1100) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃                                               ");
	}
	if (time == 1500) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이                                            ");
	}
	if (time == 1800) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피                                        ");
	}
	if (time == 1950) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피 었                                     ");
	}
	if (time == 2170) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피 었 습 니                               ");
	}
	if (time == 2270) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피 었 습 니 다                           ");
		back_buf[(N_ROW / 2) - 1][1] = '#';
		back_buf[(N_ROW / 2)][1] = '#';
		back_buf[(N_ROW / 2) + 1][1] = '#';
		for (int i = 1; i < n_player; i++) {
			// 무작위로 1부터 10까지 숫자 생성
			int randdom_num = randint(1, 10);
			int randdom_move = randint(1, 10);
			//10% 확률로 왼쪽 위 아래로 움직임
			if (randdom_num == 1) {
				if (randdom_move >= 1 && randdom_move <= 3) {
					move_random(i, DIR_LEFT);
					
				}
				else if (randdom_move >= 4 && randdom_move <= 7) {
					move_random(i, DIR_UP);
					
				}
				else {
					move_random(i, DIR_DOWN);
					
				}
			}

			else {
				// 나머지 경우에는 가만히 있음
				move_random(i, -1); // -1은 가만히 있는 것을 나타냅니다.
			}
		}
	}
	if (time == 5270) {
		back_buf[(N_ROW / 2) - 1][1] = '@';
		back_buf[(N_ROW / 2)][1] = '@';
		back_buf[(N_ROW / 2) + 1][1] = '@';
		gotoxy(17, 0);
		printf("                                                          ");
	}
}

void mugung_init(void) {
	map_init(15, 50);
	int x, y;

	px[0] = (N_ROW / 2) - 1;
	py[0] = 1;
	back_buf[(N_ROW / 2) - 1][1] = '@';
	back_buf[(N_ROW / 2)][1] = '@';
	back_buf[(N_ROW / 2) + 1][1] = '@';



	for (int i = 0; i < n_player; i++) {
		// 같은 자리가 나오면 다시 생성
		do {
			x = randint(1, N_ROW - 2);
			y = 48;
		} while (!placable(x, y));
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 500);

		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
		x++;
	}

	tick = 0;

}

void mugung_set(void) {
	map_init(15, 50);
	int x, y;

	px[0] = (N_ROW / 2) - 1;
	py[0] = 1;
	back_buf[(N_ROW / 2) - 1][1] = '@';
	back_buf[(N_ROW / 2)][1] = '@';
	back_buf[(N_ROW / 2) + 1][1] = '@';
}

void move_manual(key_t key) {
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 움직여서 놓일 자리
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}

	move_tail(0, nx, ny);
}

// 0 <= dir < 4가 아니면 랜덤
void move_random(int player, int dir) {
	int p = player;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리

	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)	

	do {
		int random_num = randint(1, 10);

		if (random_num == 1) {
			// 10% 확률로 가만히 있음
			nx = px[p];
			ny = py[p];
		}
		else if (random_num == 2) {
			// 10% 확률로 위로 이동
			nx = px[p] - 1;
			ny = py[p];
		}
		else if (random_num == 3) {
			// 10% 확률로 아래로 이동
			nx = px[p] + 1;
			ny = py[p];
		}
		else {
			// 나머지 경우에는 왼쪽으로 이동
			nx = px[p];
			ny = py[p] - 1;
		}
	} while (!placable(nx, ny));

	move_tail(p, nx, ny);
}

// back_buf[][]에 기록
void move_tail(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void mugunghwa() {

	mugung_set();
	display();
	dialog("무궁화꽃이 피었습니다를 시작합니다.");
	system("cls");
	mugung_init();
	display();
	//test 
	while (1) {

		// player 0만 손으로 움직임(4방향)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual(key);
		}


		// player 1 부터는 랜덤으로 움직임(8방향)
		if (tick >= 500) {
			for (int i = 1; i < n_player; i++) {
				if (tick % period[i] == 0) {
					move_random(i, -1);
				}
			}
		}

		say_mugung(tick);
		display();
		Sleep(25);
		tick += 10;
		if (tick >= 4000) {
			tick = 500;
		}
		//printf("%d", tick);
	}
}