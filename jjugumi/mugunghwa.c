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

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�

// "����ȭ�����Ǿ����ϴ�"�� ������ ���ڿ�
char mugung_text[] = "����ȭ�����Ǿ����ϴ�";
int mugung_index = 0;  // ���� ����� ������ �ε���
int delay = 300;  // ��� ����(ms), �ʱ� ��: 300ms

void say_mugung(int time) {
	
	if (time == 500) {
		gotoxy(17, 0);
		printf("��                                                        ");
	}
	if (time == 650) {
		gotoxy(17, 0);
		printf("�� ��                                                     ");
	}
	if (time == 850) {
		gotoxy(17, 0);
		printf("�� �� ȭ                                                  ");
	}
	if (time == 1100) {
		gotoxy(17, 0);
		printf("�� �� ȭ ��                                               ");
	}
	if (time == 1500) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��                                            ");
	}
	if (time == 1800) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  ��                                        ");
	}
	if (time == 1950) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  �� ��                                     ");
	}
	if (time == 2170) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  �� �� �� ��                               ");
	}
	if (time == 2270) {
		gotoxy(17, 0);
		printf("�� �� ȭ �� ��  �� �� �� �� ��                           ");
		back_buf[(N_ROW / 2) - 1][1] = '#';
		back_buf[(N_ROW / 2)][1] = '#';
		back_buf[(N_ROW / 2) + 1][1] = '#';
		for (int i = 1; i < n_player; i++) {
			// �������� 1���� 10���� ���� ����
			int randdom_num = randint(1, 10);
			int randdom_move = randint(1, 10);
			//10% Ȯ���� ���� �� �Ʒ��� ������
			if (randdom_num == 1) {
				if (randdom_move >= 1 && randdom_move <= 3) {
					move_random(i, DIR_LEFT);
				}
				else if (randdom_move >= 4 && randdom_move <= 7) {
					move_random(i, DIR_RIGHT);
				}
				else {
					move_random(i, DIR_DOWN);
				}
			}

			else {
				// ������ ��쿡�� ������ ����
				move_random(i, -1); // -1�� ������ �ִ� ���� ��Ÿ���ϴ�.
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
	int x, y;

	px[0] = (N_ROW / 2) - 1;
	py[0] = 1;
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

// 0 <= dir < 4�� �ƴϸ� ����
void move_random(int player, int dir) {
	int p = player;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�

	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	

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

// back_buf[][]�� ���
void move_tail(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
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