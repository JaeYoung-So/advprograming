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

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�
int diedplayer[PLAYER_MAX];
//char diedmesseage[100];

void say_mugung(int time) {

	if (time == 500) {
		gotoxy(17, 0);
		printf("��                                                        ");
	}
	if (time == 600) {
		gotoxy(17, 0);
		printf("�� ��                                                     ");
	}
	if (time == 750) {
		gotoxy(17, 0);
		printf("�� �� ȭ                                                  ");
	}
	if (time == 950) {
		gotoxy(17, 0);
		printf("�� �� ȭ ��                                               ");
	}
	if (time == 1200) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��                                            ");
	}
	if (time == 1500) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  ��                                        ");
	}
	if (time == 1800) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  �� ��                                     ");
	}
	if (time == 2050) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  �� �� ��                                  ");
	}
	if (time == 2250) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  �� �� �� ��                               ");
	}
	if (time == 2450) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  �� �� �� �� ��!                           ");
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
		// ���� �ڸ��� ������ �ٽ� ����
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
	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // ������ ����(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// �������� ���� �ڸ�
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
// 0 <= dir < 4�� �ƴϸ� ����
void move_random(int player, int dir) {
	int p = player;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�
	bool life;
	life = getlife(p);

	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	
	if (life == true) {
		do {
			int random_num = randint(1, 10);
			
			if (random_num == 1) {
				// 10% Ȯ���� ������ ����
				nx = px[p];
				ny = py[p];
			}
			else if (random_num == 2) {
				// 10% Ȯ���� ���� �̵�
				nx = px[p] - 1;
				ny = py[p];
			}
			else if (random_num == 3) {
				// 10% Ȯ���� �Ʒ��� �̵�
				nx = px[p] + 1;
				ny = py[p];
			}
			else {
				// ������ ��쿡�� �������� �̵�
				nx = px[p];
				ny = py[p] - 1;
			}
		} while (!placable(nx, ny));

		move_tail(p, nx, ny);
	}
	
}

//10���� Ȯ���θ� ������
/*
void move_stop(int player, int dir) {
	int p = player;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�
	int kill = 0;
	bool life = true;
	life = getlife(p);
	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	
	if (life == true) {
		do {
			int random_num = randint(1, 10);
			if (random_num == 1) {
				int random_num2 = randint(1, 10);
				if (random_num2 == 1) {
					// 10% Ȯ���� ���� �̵�
					nx = px[p] - 1;
					ny = py[p];
				}
				else if (random_num2 == 2) {
					// 10% Ȯ���� �Ʒ��� �̵�
					nx = px[p] + 1;
					ny = py[p];
				}
				else {
					// ������ ��쿡�� �������� �̵�
					nx = px[p];
					ny = py[p] - 1;
				}
				kill = 1;
			}
			else {
				// ������ ��쿡�� ����
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
				// 10% Ȯ���� ���� �̵�
				nx = px[p] - 1;
				ny = py[p];
			}
			else if (random_num2 == 2) {
				// 10% Ȯ���� �Ʒ��� �̵�
				nx = px[p] + 1;
				ny = py[p];
			}
			else {
				// ������ ��쿡�� �������� �̵�
				nx = px[p];
				ny = py[p] - 1;
			}
			kill = 1;
		}
		else {
			// ������ ��쿡�� ����
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

// back_buf[][]�� �������
void kill_tail(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = ' ';
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
	kill_false(p);
}

// back_buf[][]�� ���
void move_tail(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}
//������ false�� �ٲ��ִ� �Լ�
void kill_false(int p) {
	player[p] = false;
	n_alive--;
	diedplayer[p] = p;
}

void mugunghwa() {

	mugung_set();
	display();
	dialog("����ȭ���� �Ǿ����ϴٸ� �����մϴ�.");
	system("cls");
	mugung_init();
	display();
	//test 
		while (1) {

			// player 0�� ������ ������(4����)
			key_t key = get_key();
			if (key == K_QUIT) {
				break;
			}
			else if (key != K_UNDEFINED) {
				move_manual(key);
			}

			
			// player 1 ���ʹ� �������� ������(8����)
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
							// ���� ó�� (��ȯ ����)
						}

						strcat_s(deadPlayersMessage, sizeof(deadPlayersMessage), playerNumber);
						numDeadPlayers++;

						diedplayer[i] = 0; // ��������� �ʱ�ȭ
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






