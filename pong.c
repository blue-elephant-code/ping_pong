#include <stdio.h>

void fill_field(int field[32][82]);
void draw_field(int field[32][82], int rocket_1_y, int rocket_2_y, int *ball_x, int *ball_y, int *score_p1,
                int *score_p2, int *ball_to_sky, int *ball_to_right);
void control_ball(int rocket_1_y, int rocket_2_y, int *ball_x, int *ball_y, int *ball_to_sky,
                  int *ball_to_right, int *score_p1, int *score_p2);
void paint_rocket(int field[32][82], int rocket_1_y, int rocket_2_y);
void paint_score(int field[32][82], int score_p1, int score_p2);

int main() {
    int field[32][82];
    int rocket_1_y = 10;
    int rocket_2_y = 10;
    int ball_x = 2;
    int ball_y = 11;
    int ball_to_sky = 1;
    int ball_to_right = 1;
    int score_p1 = 0, score_p2 = 0;

    while (1) {
        draw_field(field, rocket_1_y, rocket_2_y, &ball_x, &ball_y, &score_p1, &score_p2, &ball_to_sky,
                   &ball_to_right);

        // Ввод символа
        char ch;
        if (scanf("%c", &ch) == 1) {
            while (getchar() != '\n');
        }

        if (score_p1 == 21) {
            printf("PLAYER1 победил! Поздравляем! Спасибо за прекрасную игру");
            break;
        } else if (score_p2 == 21) {
            printf("PLAYER2 победил! Поздравляем! Спасибо за прекрасную игру");
            break;
        } else if (ch == ' ') {
            continue;
        } else if (ch == 'q') {
            break;
        } else if (ch == 'a' && rocket_1_y > 5) {
            rocket_1_y -= 1;
        } else if (ch == 'z' && rocket_1_y < 27) {
            rocket_1_y += 1;
        } else if (ch == 'k' && rocket_2_y > 5) {
            rocket_2_y -= 1;
        } else if (ch == 'm' && rocket_2_y < 27) {
            rocket_2_y += 1;
        }
    }
}

// Отрисовка поля: обнуление -> создание мяча -> отрисовка ракеток -> вывод
void draw_field(int field[32][82], int rocket_1_y, int rocket_2_y, int *ball_x, int *ball_y, int *score_p1,
                int *score_p2, int *ball_to_sky, int *ball_to_right) {
    fill_field(field);
    control_ball(rocket_1_y, rocket_2_y, ball_x, ball_y, ball_to_sky, ball_to_right, score_p1, score_p2);
    field[(*ball_y)][(*ball_x)] = 79;
    paint_score(field, *score_p1, *score_p2);
    paint_rocket(field, rocket_1_y, rocket_2_y);
    for (int y = 0; y <= 30; y++) {
        for (int x = 0; x <= 81; x++) {
            if (x == 81) {
                printf("%c\n", field[y][x]);
            } else if (y == 3 && (x == 20 || x == 60)) {
                printf("%d", field[y][x]);
            } else {
                printf("%c", field[y][x]);
            }
        }
    }
}

// Обнуление поля - отрисовка границ
void fill_field(int field[32][82]) {
    for (int y = 0; y <= 30; y++) {
        for (int x = 0; x <= 81; x++) {
            if (y == 4 || y == 30) {
                field[y][x] = 126;
            } else if (y >= 4 && (x == 0 || x == 39 || x == 40 || x == 81)) {
                field[y][x] = 124;
            } else {
                field[y][x] = 32;
            }
        }
    }
}

// Отрисовка очков
void paint_score(int field[32][82], int score_p1, int score_p2) {
    for (int y = 2; y <= 3; y++) {
        for (int x = 0; x <= 81; x++) {
            if (y == 2 && x == 20) {
                field[y][x] = 80;
            } else if (y == 2 && x == 21) {
                field[y][x] = 49;
            } else if (y == 2 && x == 60) {
                field[y][x] = 80;
            } else if (y == 2 && x == 61) {
                field[y][x] = 50;
            } else if (y == 3 && x == 20) {
                field[y][x] = score_p1;
            } else if (y == 3 && x == 60) {
                field[y][x] = score_p2;
            }
        }
    }
}

// Отрисовка ракеток
void paint_rocket(int field[32][82], int rocket_1_y, int rocket_2_y) {
    for (int y = 5; y <= 29; y++) {
        field[y][1] = 32;
        field[y][80] = 32;
    }

    for (int i = rocket_1_y; i <= rocket_1_y + 2; i++) {
        field[i][1] = 125;
    }

    for (int i = rocket_2_y; i <= rocket_2_y + 2; i++) {
        field[i][80] = 123;
    }
}

// Физика мяча
void control_ball(int rocket_1_y, int rocket_2_y, int *ball_x, int *ball_y, int *ball_to_sky,
                  int *ball_to_right, int *score_p1, int *score_p2) {
    // Очко за пропуск мимо ракетки
    if (*ball_x == 1 && ((*ball_y - rocket_1_y) > 2 || (*ball_y - rocket_1_y) < 0)) {
        (*score_p2) += 1;
        *ball_x = 2;
        *ball_y = rocket_1_y + 1;
        *ball_to_right = 1;
    } else if (*ball_x == 80 && ((*ball_y - rocket_2_y) > 2 || (*ball_y - rocket_2_y) < 0)) {
        (*score_p1) += 1;
        *ball_x = 79;
        *ball_y = rocket_2_y + 1;
        *ball_to_right = 0;
    }

    // Отскок от ракетки
    if (*ball_x == 2 && (*ball_y >= rocket_1_y) && (*ball_y - rocket_1_y) <= 2) {
        (*ball_to_right) = 1;
        if (*ball_y - rocket_1_y <= 1) {
            (*ball_to_sky) = 1;
        } else {
            (*ball_to_sky) = 0;
        }
    } else if (*ball_x == 79 && (*ball_y >= rocket_2_y) && (*ball_y - rocket_2_y) <= 2) {
        (*ball_to_right) = 0;
        if (*ball_y - rocket_2_y <= 1) {
            (*ball_to_sky) = 1;
        } else {
            (*ball_to_sky) = 0;
        }
    }

    // Отскок от верхней и нижней стенки
    if (*ball_y <= 5) {
        (*ball_to_sky) = 1;
    } else if (*ball_y >= 29) {
        (*ball_to_sky) = 0;
    }

    // Движение
    if (*ball_to_sky == 1) {
        if (*ball_to_right == 1) {
            (*ball_x) += 1;
        } else {
            (*ball_x) -= 1;
        }
        (*ball_y) += 1;
    } else if (*ball_to_sky == 0) {
        if (*ball_to_right == 1) {
            (*ball_x) += 1;
        } else {
            (*ball_x) -= 1;
        }
        (*ball_y) -= 1;
    }
}
