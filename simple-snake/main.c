#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>


void clear_screen() {
    system("cls");
}

void pause_screen() {
    system("pause > nul");
}

void gotoxy(int x, int y) {
    COORD in = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), in);
}

void ConsoleAnsiMode() {
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void ConsoleCursor() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(h, &structCursorInfo);
    structCursorInfo.bVisible = 0;
    SetConsoleCursorInfo(h, &structCursorInfo);
}

#define UseAnsi 0 //if ansi doesn't work write null
#if UseAnsi
void rgb_color(int r, int g, int b, int size_font, short fix_bug) {
    printf("\x1b[38;2;%i;%i;%i;%im", r, g, b, size_font);
    if ((r == 0 && g == 0 && b == 0) && (fix_bug == 1)) { printf("\033[0m"); }
}
void reset_color() {
    printf("\033[0m");
}
#else
void rgb_color(int r, int g, int b, int size_font, short fix_bug) {
    if ((r == 0 && g == 0 && b == 0) && (fix_bug == 1)) { r = 7; }
    if ((r == 204) && (fix_bug == 1)) { r = 112; }
    if ((r == 194) && (fix_bug == 1)) { r = 96; }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), r);
}
void reset_color() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
#endif

struct color_preset {
    const char* color_console;
    int color_inside[3];
    int color_border[3];
};

void border_print(char symbol, int x_pos, int y_pos, int height, int width, int sleep_print) {
    for (int i = 0; i <= height; i++) {
        gotoxy(x_pos, i + y_pos);
        putchar(symbol);
        gotoxy(width + x_pos, height - i + y_pos);
        putchar(symbol);
        Sleep(sleep_print);
    }
    for (int i = 0; i <= width; i++) {
        gotoxy(width - i + x_pos, y_pos);
        putchar(symbol);
        gotoxy(i + x_pos, height + y_pos);
        putchar(symbol);
        Sleep(sleep_print);
    }
}

void inside_print(char symbol, int x_pos, int y_pos, int height, int width, int sleep_print) {
    char* color = (char*)malloc(width + 2);
    for (int i = 0; i <= height; i++) {
        Sleep(sleep_print);
        gotoxy(x_pos, i + y_pos);
        for (int j = 0; j <= width; j++) {
            color[j] = symbol;
        }
        color[width + 1] = '\0';
        printf("%s", color);
    }
    free(color);
}

int buy_item(int buy, int money[], int price_item[], int count_item, int index_item, int add) {
    if (index_item >= count_item) { index_item = count_item - 1; }
    if (money[0] >= price_item[index_item] && buy == 1) {
        money[0] -= price_item[index_item];
        index_item += 1;
        if (index_item >= count_item) { index_item = count_item - 1; }
        price_item[0] = price_item[index_item];
        return add;
    }
    return 0;
}


int main() {
    ConsoleAnsiMode();
    ConsoleCursor();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    short first_game = 0;

    //Money
    int money[] = { 0 };
    int money_skin_snake[] = { 10, 10, 50,  100, 200, 0, 0 };
    int money_skin_level[] = { 30, 50, 100, 250, 500, 0, 0 };
    int money_casual_iter_mult[] = { 25, 50, 100, 200 };
    int money_size_snake[] = { 10 };
    int money_add_speed_snake[] = { 10 };
    int money_subtract_speed_snake[] = { 10 };
    int money_max_die_casual_bounce[] = { 10 };
    int money_open_box[] = { 50 };

    //item for snake
    float casual_iter_mult = 1;
    int max_die_casual_bounce = 0;
    int add_size_snake = 0;
    int speed_snake = 22;
    int skin_snake = 0;
    int skin_level = 0;
    int open_box = 0;

    srand(time(0));
    while (1) {
        while (first_game == 1) { //Shop
            reset_color(); clear_screen();
            printf("You have: %i$\n", money[0]);
            printf("Press the button (0-7) to buy:\n\n");
            printf("0. %i$\t- score multiplier (multiplier = %i)\n", money_casual_iter_mult[0], (int)casual_iter_mult);
            printf("1. %i$\t- bounce (bounce = %i)\n", money_max_die_casual_bounce[0], max_die_casual_bounce);
            printf("2. %i$\t- open loot box\n", money_open_box[0]);
            printf("3. %i$\t- increase speed (speed = %i)\n", money_add_speed_snake[0], (102 - speed_snake));
            printf("4. %i$\t- decrease speed (speed = %i)\n", money_subtract_speed_snake[0], (102 - speed_snake));
            printf("5. %i$\t- snake size (size = %i)\n", money_size_snake[0], add_size_snake + 1);
            printf("6. %i$\t- skin snake (skin = %i)\n", money_skin_snake[0], skin_snake);
            printf("7. %i$\t- level design (level design = %i)\n", money_skin_level[0], skin_level);
            pause_screen();
            char pay_symbol = _getch();
            int old_money = money[0];
            casual_iter_mult += buy_item(pay_symbol == '0', money, money_casual_iter_mult, 4, casual_iter_mult - 1, 1);
            max_die_casual_bounce += buy_item(pay_symbol == '1', money, money_max_die_casual_bounce, 1, 0, 1);
            open_box += buy_item(pay_symbol == '2', money, money_open_box, 1, 0, 1) - open_box;
            speed_snake += buy_item((pay_symbol == '3') * (speed_snake < 100), money, money_add_speed_snake, 1, 0, 1);
            speed_snake += buy_item((pay_symbol == '4') * (speed_snake > 2), money, money_subtract_speed_snake, 1, 0, -1);
            add_size_snake += buy_item(pay_symbol == '5', money, money_size_snake, 1, 0, 1);
            skin_snake += buy_item(pay_symbol == '6', money, money_skin_snake, 7, skin_snake, 1);
            skin_level += buy_item(pay_symbol == '7', money, money_skin_level, 7, skin_level, 1);
            if (open_box == 1) {
                const char* array_loot_add[3] = { "You're lucky! Plus %i$!", "You win! Plus %i$! Congratulations!", "Plus %i$! Congratulations!" };
                const char* array_loot_minus[3] = { "You're lucky! Minus %i$!", "You win! Minus %i$! Congratulations!", "Minus %i$! Congratulations!" };
                int casino_money = ((rand() % 9) * 10);
                money[0] += casino_money;
                if (money_open_box[0] > casino_money) { printf(array_loot_minus[rand() % 3], abs(casino_money - money_open_box[0])); }
                if (money_open_box[0] <= casino_money) { printf(array_loot_add[rand() % 3], abs(casino_money - money_open_box[0])); }
                Sleep(2000);
            }
            skin_snake %= 6;
            skin_level %= 6;
            if (old_money != money[0] || pay_symbol == '7' || pay_symbol == '6') { continue; }
            clear_screen(); break;
        }

        //Global
        int width = 60, height = 18;
        int x_global = 20, y_global = 5;
        short use_statistics = 1; //statistics
        int x_statistics = 0, y_statistics = 0; //statistics
        int* move_x = (int*)malloc(sizeof(int) * (width * height));
        int* move_y = (int*)malloc(sizeof(int) * (width * height));
        int x = 2, y = 2;
        short block_x = 0, block_y = 0;
        short die_snake = 0;
        int size_snake = 2 + add_size_snake; //snake with size 2 equals 1

        //Other
        int dir_x = 0, dir_y = 0;
        int old_x = 0, old_y = 0;
        first_game = 1;
        short print_apple_bool = 1;
        int x_apple = (rand() % (width - 1)) + 1;
        int y_apple = (rand() % (height - 1)) + 1;
        int win_count = (height - 1) * (width - 1) - 1;
        short* block_position = calloc(height * width, sizeof(short));
        int die_casual_bounce = 0; //count bounce
        int iter = 0;
        int size_snake_now = 0; //snake size now
        char border_symbol = ' ';
        if (skin_level == 3) { border_symbol = '@'; }
        for (int i = 0; i < width * height; i++) { move_x[i] = -1; move_y[i] = -1; }

        //Settings color
        struct color_preset preset[6] = {
            { {"color 00"}, {0,0,0},       {128,128,128}},
            { {"color 70"}, {128,128,128}, {204,204,204}},
            { {"color 70"}, {128,128,128}, {0,0,0}},
            { {"color 00"}, {0,0,0},       {0,0,0}},
            { {"color 60"}, {204,204,204}, {128,128,128}},
            { {"color 30"}, {128,128,128}, {0,0,0}},
        };
        struct color_preset skin = preset[skin_level];

        system(skin.color_console);
        rgb_color(skin.color_inside[0], skin.color_inside[1], skin.color_inside[2], 7, 1);
        inside_print(' ', x_global, y_global, height, width, 0);
        rgb_color(skin.color_border[0], skin.color_border[1], skin.color_border[2], 7, 1);
        border_print(border_symbol, x_global, y_global, height, width, 0);


        while (1) { //Algorithm
            //Detect Button
            if (size_snake_now != 0) { old_x = x; old_y = y; }
            int w = 0, a = 0, s = 0, d = 0;
            if (dir_y != -1) { s = GetAsyncKeyState('S'); }
            if (dir_y != 1) { w = GetAsyncKeyState('W'); }
            if (dir_x != -1) { d = GetAsyncKeyState('D'); }
            if (dir_x != 1) { a = GetAsyncKeyState('A'); }
            if (dir_y != -1 && s != -32767) { s = GetAsyncKeyState(VK_DOWN); }
            if (dir_y != 1 && w != -32767) { w = GetAsyncKeyState(VK_UP); }
            if (dir_x != -1 && d != -32767) { d = GetAsyncKeyState(VK_RIGHT); }
            if (dir_x != 1 && a != -32767) { a = GetAsyncKeyState(VK_LEFT); }
            if (a == -32767) { dir_x = -1; block_x = 1; }
            if (d == -32767) { dir_x = 1;  block_x = 1; }
            if (w == -32767) { dir_y = -1; block_y = 1; }
            if (s == -32767) { dir_y = 1;  block_y = 1; }
            if (block_x == 1 && dir_x != 0) { dir_y = 0; block_x = 0; }
            if (block_y == 1 && dir_y != 0) { dir_x = 0; block_y = 0; }
            if (dir_x != 0) { x += dir_x; }
            if (dir_y != 0) { y += dir_y; Sleep(6); }

            //Move Snake
            if (x != old_x || y != old_y) {
                move_x[size_snake_now] = x;
                move_y[size_snake_now] = y;
                size_snake_now += 1;
                //Die Snake
                for (int i = 0; i < size_snake_now - 1; i++) {
                    if (move_x[i] == x && move_y[i] == y) { die_snake = 1; break; }
                }
                if ((width == x && 0 <= y && height >= y) || (0 == x && 0 <= y && height >= y) ||
                    (height == y && 0 <= x && width >= x) || (0 == y && 0 <= x && width >= x))
                {
                    die_snake = 1;
                }
                //Save Life Snake
                if (die_snake == 0) { 
                    die_casual_bounce = 0; 
                }
                if (die_snake == 1) {
                    x = move_x[size_snake_now - 2];
                    y = move_y[size_snake_now - 2];
                    move_x[size_snake_now] = -1;
                    move_y[size_snake_now] = -1;
                    size_snake_now -= 1;
                    die_casual_bounce += 1;
                    die_snake = 0;
                }
                //Game Over
                if (die_snake == 1 || (max_die_casual_bounce < die_casual_bounce)) {
                    gotoxy(move_x[size_snake_now - 1] + x_global + dir_x, move_y[size_snake_now - 1] + y_global + dir_y);
                    rgb_color(192, 0, 0, 7, 1); putchar(' ');
                    break;
                }
            }

            //Position Apple calc
            if (x == x_apple && y == y_apple) {
                size_snake += 1 * casual_iter_mult;
                for (int i = 0; i < height * width; i++) {
                    block_position[i] = 0;
                }
                for (int i = 0; i < size_snake_now; i++) {
                    block_position[move_x[i] + (move_y[i] - 1) * width] = 1;
                }
                while (1) {
                    x_apple = 1 + (rand() % (width - 1));
                    y_apple = 1 + (rand() % (height - 1));
                    if (block_position[x_apple + (y_apple - 1) * width] == 0) { break; }
                }
                print_apple_bool = 1;
            }

            //Print apple
            if (print_apple_bool == 1) {
                if (skin_level != 3) { rgb_color(192, 0, 0, 7, 1); }
                gotoxy(x_apple + x_global, y_apple + y_global); putchar('$');
                print_apple_bool = 0;
            }

            //Print snake
            if (size_snake_now > 0) {
                Sleep(speed_snake + 1);
                gotoxy(move_x[size_snake_now - 1] + x_global, move_y[size_snake_now - 1] + y_global);
                int global_color = (rand() % 128) + 128;
                if (skin_snake == 0) { rgb_color(255, 255, 255, 7, 1); putchar(' '); }
                if (skin_snake == 1) { rgb_color(global_color * 0.35, global_color, global_color + rand() % 128, 7, 0); putchar(' '); }
                if (skin_snake == 2) { reset_color(); putchar('o'); }
                if (skin_snake == 3) { rgb_color(move_x[size_snake_now - 1] * 1.5 + 15, 40 + move_y[size_snake_now - 1] * 6, 40 + move_x[size_snake_now - 1] * 6, 7, 0); putchar(' '); }
                if (skin_snake == 4) { reset_color(); rgb_color(10, 196, 128, 2, 1); char matrix[3] = { "01" }; putchar(matrix[rand() % 2]); }
                if (skin_snake == 5) {
                    iter++;
                    for (int i = 0; i < size_snake_now; i++) {
                        gotoxy(move_x[i] + x_global, move_y[i] + y_global);
                        if (skin_snake == 5) {
                            if ((iter % 10 == 0) || (i == (size_snake_now - 1)) || size_snake_now < 10) {
                                rgb_color(sin(((float)y) / height) * 64 + 24, (cos(((float)x) / width) + sin(((float)y) / height)) * 255, cos(((float)y) / height) * 225, 7, 0); putchar(' ');
                            }
                        }
                    }
                }
            }

            //Print Delete step
            if (size_snake_now == size_snake) {
                rgb_color(skin.color_inside[0], skin.color_inside[1], skin.color_inside[2], 7, 1);
                gotoxy(move_x[0] + x_global, move_y[0] + y_global); putchar(' ');
                for (int i = 0; i < size_snake_now; i++) {
                    move_x[i] = move_x[i + 1];
                    move_y[i] = move_y[i + 1];
                }
                size_snake_now -= 1;
            }

            //Print other (statistics)
            if (use_statistics == 1) {
                if (skin_level == 0) { rgb_color(0, 0, 0, 7, 1); }
                if (skin_level == 1) { rgb_color(204, 204, 204, 7, 1); }
                if (skin_level == 2) { rgb_color(204, 204, 204, 7, 1); }
                if (skin_level == 3) { rgb_color(0, 0, 0, 7, 1); }
                if (skin_level == 4) { rgb_color(194, 156, 10, 7, 1); }
                if (skin_level == 5) { rgb_color(48, 156, 222, 7, 1); }
                gotoxy(x_statistics, 0 + y_statistics); printf("                 ");
                gotoxy(x_statistics, 0 + y_statistics);
                printf("x - %i", x);
                gotoxy(x_statistics, 1 + y_statistics); printf("                 ");
                gotoxy(x_statistics, 1 + y_statistics);
                printf("y - %i", y);
                gotoxy(x_statistics, 2 + y_statistics); printf("                 ");
                gotoxy(x_statistics, 2 + y_statistics);
                printf("size snake - %i", size_snake - 1);
                gotoxy(x_statistics, 3 + y_statistics);
                printf("win count - %i ", win_count);
                gotoxy(x + x_global, y + y_global);
            }
            if (win_count <= size_snake - 1) { break; }
        }
        money[0] += (size_snake - 2 - add_size_snake);

        //Win
        if (win_count <= size_snake - 1) {
            reset_color(); Sleep(350); clear_screen();
            gotoxy(((int)(width * 0.5)) - 5 + x_global, ((int)(height * 0.5)) + y_global);
            printf("You win!");
            Sleep(2500); clear_screen();
            gotoxy(((int)(width * 0.5)) - 11 + x_global, ((int)(height * 0.5)) + y_global);
            printf("Game creator for_example");
            gotoxy(((int)(width * 0.5)) - 11 + 13 + x_global, ((int)(height * 0.5)) + y_global);
            Sleep(1500);
            rgb_color(240, 240, 240, 7, 1);
            border_print(' ', x_global, y_global, height, width, 40);
            const char* gradient_words[4] = { "Thanks for playing!", "Hope you enjoyed!", "You the best!", "@for_example" };
            int gradient_color[4] = { 240, 204, 128, 0 };
            for (int k = 0; k < 4; k++) {
                rgb_color(gradient_color[k], gradient_color[k], gradient_color[k], 7, 1);
                inside_print(' ', x_global, y_global, height * 0.5, width, 60);
                gotoxy(((int)(width * 0.5)) - (strlen(gradient_words[k]) * 0.5) + x_global, ((int)height * 0.5) + y_global);
                printf("%s", gradient_words[k]);
                inside_print(' ', x_global, y_global + height * 0.5 + 1, height * 0.5 - 1, width, 60);
                Sleep(65);
            }
            money[0] += 20000;
            Sleep(5000); clear_screen();
        }
        pause_screen();
        free(block_position);
        free(move_x); free(move_y);
    }
}