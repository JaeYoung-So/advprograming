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
void kill_tail(int i, int nx, int ny);
void kill_false(int i);
bool getlife(int i);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기
int diedplayer[PLAYER_MAX];
//char diedmesseage[100];

void say_mugung(int time) {

	if (time == 500) {
		gotoxy(17, 0);
		printf("무                                                        ");
	}
	if (time == 600) {
		gotoxy(17, 0);
		printf("무 궁                                                     ");
	}
	if (time == 750) {
		gotoxy(17, 0);
		printf("무 궁 화                                                  ");
	}
	if (time == 950) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃                                               ");
	}
	if (time == 1200) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이                                            ");
	}
	if (time == 1500) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피                                        ");
	}
	if (time == 1800) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피 었                                     ");
	}
	if (time == 2050) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피 었 습                                  ");
	}
	if (time == 2250) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피 었 습 니                               ");
	}
	if (time == 2450) {
		gotoxy(17, 0);
		printf("무 궁 화 꽃 이  피 었 습 니 다!                           ");
		back_buf[(N_ROW / 2) - 1][1] = '#';
		back_buf[(N_ROW / 2)][1] = '#'; 
		back_buf[(N_ROW / 2) + 1][1] = '#';
	}
	if (time == 4000) {
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

bool getlife(int p) {
	if (player[p] == true) {
		return true;
	}
	else {
		return false;
	}
}
// 0 <= dir < 4가 아니면 랜덤
void move_random(int player, int dir) {
	int p = player;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리
	bool life;
	life = getlife(p);

	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)	
	if (life == true) {
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
	
}

//10프로 확률로만 움직임
/*
void move_stop(int player, int dir) {
	int p = player;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리
	int kill = 0;
	bool life = true;
	life = getlife(p);
	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)	
	if (life == true) {
		do {
			int random_num = randint(1, 10);
			if (random_num == 1) {
				int random_num2 = randint(1, 10);
				if (random_num2 == 1) {
					// 10% 확률로 위로 이동
					nx = px[p] - 1;
					ny = py[p];
				}
				else if (random_num2 == 2) {
					// 10% 확률로 아래로 이동
					nx = px[p] + 1;
					ny = py[p];
				}
				else {
					// 나머지 경우에는 왼쪽으로 이동
					nx = px[p];
					ny = py[p] - 1;
				}
				kill = 1;
			}
			else {
				// 나머지 경우에는 정지
				nx = px[p];
				ny = py[p];
			}
		} while (!placable(nx, ny));

		if (kill == 1) {
			kill_tail(p, nx, ny);
			kill  = 0;
		}
		else {
			move_tail(p, nx, ny);
			kill = 0;
		}
	}
}
*/
void move_stop(int player, int dir) {
	int p = player;
	int nx, ny;
	int kill = 0;
	bool life = true;
	life = getlife(p);

	if (life == true) {
		int random_num = randint(1, 10);
		if (random_num == 1) {
			int random_num2 = randint(1, 10);
			if (random_num2 == 1) {
				// 10% 확률로 위로 이동
				nx = px[p] - 1;
				ny = py[p];
			}
			else if (random_num2 == 2) {
				// 10% 확률로 아래로 이동
				nx = px[p] + 1;
				ny = py[p];
			}
			else {
				// 나머지 경우에는 왼쪽으로 이동
				nx = px[p];
				ny = py[p] - 1;
			}
			kill = 1;
		}
		else {
			// 나머지 경우에는 정지
			nx = px[p];
			ny = py[p];
		}

		if (placable(nx, ny)) {
			if (kill == 1) {
				kill_tail(p, nx, ny);
			}
			else {
				move_tail(p, nx, ny);
			}
		}
	}
}

// back_buf[][]에 죽은기록
void kill_tail(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = ' ';
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
	kill_false(p);
}

// back_buf[][]에 기록
void move_tail(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}
//죽으면 false로 바꿔주는 함수
void kill_false(int p) {
	player[p] = false;
	n_alive--;
	diedplayer[p] = p;
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
			if (tick < 2450 && tick >= 500) {
				for (int i = 1; i < n_player; i++) {
					if (tick % period[i] == 0) {
						move_random(i, -1);
					}
				}
			}
			
			
			else if (tick >= 2460) {

				char deadPlayersMessage[100] = "";
				int numDeadPlayers = 0;

				for (int i = 1; i < n_player; i++) {
					if (diedplayer[i] != 0) {
						if (numDeadPlayers > 0) {
							strcat_s(deadPlayersMessage, sizeof(deadPlayersMessage), ", ");
						}

						char playerNumber[10];
						if (_itoa_s(diedplayer[i], playerNumber, sizeof(playerNumber), 10) != 0) {
							// 에러 처리 (변환 실패)
						}

						strcat_s(deadPlayersMessage, sizeof(deadPlayersMessage), playerNumber);
						numDeadPlayers++;

						diedplayer[i] = 0; // 출력했으니 초기화
					}
				}

				if (numDeadPlayers > 0) {
					char fullMessage[100] = "";
					sprintf_s(fullMessage, sizeof(fullMessage), "%d player, %s dead! ", n_alive, deadPlayersMessage);
					killdialog(fullMessage);
				}
			}



			else if (tick == 2450) {
				for (int i = 1; i < n_player; i++) {
					move_stop(i, 0);
				}
			}

			//killdialog("playerdied");
			
			say_mugung(tick);
			display();
			Sleep(10);
			tick += 10;

			if (tick > 4000) {
				tick = 500;
			}

			printf("%d", tick);
		}
}






