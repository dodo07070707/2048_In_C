#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<conio.h>
int arr4[4][4];
int arr5[5][5];
int checkarr4[4][4];
int checkarr5[5][5];
int checkifchanged = 0;
int highscore4x4 = 0;
int score4x4 = 0;
int highscore5x5 = 0;
int score5x5 = 0;
char filenames4x4[16][10];
char filenames5x5[25][10];
int retry = 0;

void gotoxy(int x, int y) {
    COORD Pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void Nocursor()
{
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
    cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int random16() {
    return rand() % 16;
}

int random2or4() {
    int randint = rand() % 10;
    if (randint == 0) {
        return 4;
    }
    else return 2;
}

void _printtitle() {
    gotoxy(50, 5);
    printf("_______________      _____   ______   .__         _________  \n");
    gotoxy(50, 6);
    printf("\\_____  \\   _  \\    /  |  | /  __  \\  |__| ____   \\_   ___ \\ \n");
    gotoxy(50, 7);
    printf(" /  ____/  /_\\  \\  /   |  |_>      <  |  |/    \\  /    \\  \\/ \n");
    gotoxy(50, 8);
    printf("/       \\  \\_/   \\/    ^   /   --   \\ |  |   |  \\ \\     \\____\n");
    gotoxy(50, 9);
    printf("\\_______ \\_____  /\\____   |\\______  / |__|___|  /  \\______  /\n");
    gotoxy(50, 10);
    printf("        \\/     \\/      |__|       \\/          \\/          \\/ \n");
    gotoxy(50, 11);
    printf("                                                             \n");
    Sleep(1000);
    system("cls");
}

void _printrules() {
    gotoxy(52, 5);
    printf(".______       __    __   __       _______     _______.\n");
    gotoxy(52, 6);
    printf("|   _  \\     |  |  |  | |  |     |   ____|   /       |\n");
    gotoxy(52, 7);
    printf("|  |_)  |    |  |  |  | |  |     |  |__     |   (----`\n");
    gotoxy(52, 8);
    printf("|      /     |  |  |  | |  |     |   __|     \\   \\    \n");
    gotoxy(52, 9);
    printf("|  |\\  \\----.|  `--'  | |  `----.|  |____.----)   |   \n");
    gotoxy(52, 10);
    printf("| _| `._____| \\______/  |_______||_______|_______/    \n");
    Sleep(1000);
    gotoxy(50, 17);
    printf("1. 이 게임은 play2048.co의 2048을 토대로 만들어졌습니다.");
    Sleep(800);
    gotoxy(52, 19);
    printf("2. 수가 2048인 블록이 만들어지면 게임이 종료됩니다.");
    Sleep(800);
    gotoxy(63, 21);
    printf("3. WASD를 이용해 조작합니다.");
    Sleep(800);
    gotoxy(67, 23);
    printf("4. 게임을 즐기세요 !");
    Sleep(2000);
    system("cls");
}



void _saveimagenames4x4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            sprintf(filenames4x4[i*4+j], "%d.bmp", arr4[i][j]);
        }
    }
}

void _saveimagenames5x5() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            sprintf(filenames5x5[i * 5 + j], "%d.bmp", arr5[i][j]);
        }
    }
}

void DisplayImage(const char* filename, HDC hdc, int x, int y, int width, int height) {
    // Convert ASCII filename to wide string
    int wfilename_length = MultiByteToWideChar(CP_ACP, 0, filename, -1, NULL, 0);
    wchar_t* wfilename = malloc(wfilename_length * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, filename, -1, wfilename, wfilename_length);
    HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, wfilename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    free(wfilename);

    if (hBitmap) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);

        BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY);
        //printf("1");

        DeleteDC(hdcMem);
        DeleteObject(hBitmap);
    }
    else {
        DWORD error = GetLastError();
        //printf("LoadImage failed with error code %lu\n", error);
    }
}

void _displayimages4x4() {
    HDC hdc = GetDC(GetConsoleWindow());
    int cnt = 0;
    int imageWidth = 150;
    int imageHeight = 150;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x = j * imageWidth;
            int y = i * imageHeight;

            DisplayImage(filenames4x4[cnt], hdc, x, y, imageWidth, imageHeight);

            cnt++;
        }
    }

    ReleaseDC(GetConsoleWindow(), hdc);
}

void _displayimages5x5() {
    HDC hdc = GetDC(GetConsoleWindow());
    int cnt = 0;
    int imageWidth = 150;
    int imageHeight = 150;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int x = j * imageWidth;
            int y = i * imageHeight;

            DisplayImage(filenames5x5[cnt], hdc, x, y, imageWidth, imageHeight);

            cnt++;
        }
    }

    ReleaseDC(GetConsoleWindow(), hdc);
}

int _printmain() {
    gotoxy(51, 5);
    printf("                                                                        \n");
    gotoxy(51, 6);
    printf("  _________      .__                 __      _________.__               \n");
    gotoxy(51, 7);
    printf(" /   _____/ ____ |  |   ____   _____/  |_   /   _____/|__|_______ ____  \n");
    gotoxy(51, 8);
    printf(" \\_____  \\_/ __ \\|  | _/ __ \\_/ ___\\   __\\  \\_____  \\ |  \\___   // __ \\ \n");
    gotoxy(51, 9);
    printf(" /        \\  ___/|  |_\\  ___/\\  \\___|  |    /        \\|  |/    /\\  ___/ \n");
    gotoxy(51, 10);
    printf("/_______  /\\___  >____/\\___  >\\___  >__|   /_______  /|__/_____ \\___  >\n");
    gotoxy(51, 11);
    printf("        \\/     \\/          \\/     \\/               \\/          \\/    \\/ \n");
    gotoxy(51, 12);
    printf("                                                                        \n");
    Sleep(500);

    gotoxy(38, 17);
    printf("   _____           _____  \n");
    gotoxy(110, 17);
    printf(" .________        .________\n");
    Sleep(150);

    gotoxy(38, 18);
    printf("  /  |  |___  ___ /  |  | \n");
    gotoxy(110, 18);
    printf(" |   ____/__  ___ |   ____/\n");
    Sleep(150);

    gotoxy(38, 19);
    printf(" /   |  |\\  \\/  //   |  |_\n");
    gotoxy(110, 19);
    printf(" |____  \\\\  \\/  / |____  \\ \n");
    Sleep(150);

    gotoxy(38, 20);
    printf("/    ^   />    </    ^   /\n");
    gotoxy(110, 20);
    printf(" /       \\>    <  /       \\ \n");
    Sleep(150);

    gotoxy(38, 21);
    printf("\\____   |/__/\\_ \\____   | \n");
    gotoxy(110, 21);
    printf("/______  /__/\\_ \\/______  /\n");
    Sleep(150);

    gotoxy(38, 22);
    printf("     |__|      \\/    |__| \n");
    gotoxy(110, 22);
    printf("       \\/      \\/       \\/ \n");
    Sleep(150);

    gotoxy(23, 30);
    printf("__________                                   _____                   .________   __             _________ __                 __   \n");
    gotoxy(23, 31);
    printf("\\______   \\_______   ____   ______ ______   /  |  |    ___________   |   ____/ _/  |_  ____    /   _____//  |______ ________/  |_ \n");
    gotoxy(23, 32);
    printf(" |     ___/\\_  __ \\_/ __ \\ /  ___//  ___/  /   |  |_  /  _ \\_  __ \\  |____  \\  \\   __\\/  _ \\   \\_____  \\\\   __\\__  \\\\_  __ \\   __\\ \n");
    gotoxy(23, 33);
    printf(" |    |     |  | \\/\\  ___/ \\___ \\ \\___ \\  /    ^   / (  <_> )  | \\/  /       \\  |  | (  <_> )  /        \\|  |  / __ \\|  | \\/|  |  \n");
    gotoxy(23, 34);
    printf(" |____|     |__|    \\___  >____  >____  > \\____   |   \\____/|__|    /______  /  |__|  \\____/  /_______  /|__| (____  /__|   |__|  \n");
    gotoxy(23, 35);
    printf("                        \\/     \\/     \\/       |__|                        \\/                         \\/           \\/             \n");
    char c = _getch();
    int n = c - '0';

    system("cls");
    return n;
}


void _resetcheckarr4x4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            checkarr4[i][j] = 0;
        }
    }
}

void _resetcheckarr5x5() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            checkarr5[i][j] = 0;
        }
    }
}

void _scoreadd4x4(int n) {
    score4x4 = score4x4 + n;
}

void _scoreadd5x5(int n) {
    score5x5 = score5x5 + n;
}

void _logicw4x4() {
    while (1) {
        for (int i = 1; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (arr4[i - 1][j] == 0) {
                    arr4[i - 1][j] = arr4[i][j];
                    arr4[i][j] = 0;
                    checkarr4[i - 1][j] = checkarr4[i][j];
                    checkarr4[i][j] = 0;
                    checkifchanged++;
                }
                else if (arr4[i - 1][j] != 0) {
                    if (arr4[i - 1][j] == arr4[i][j]) {
                        arr4[i - 1][j] = arr4[i][j] * 2;
                        arr4[i][j] = 0;
                        checkarr4[i - 1][j] = 1;
                        checkarr4[i][j] = 0;
                        checkifchanged++;
                        _scoreadd4x4(arr4[i - 1][j]);
                    }
                }
            }
        }
        int check = 0;
        for (int i = 1; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if ((arr4[i][j] != 0 && arr4[i - 1][j] == 0) || (((arr4[i][j] == arr4[i - 1][j]) && arr4[i][j] != 0) && (checkarr4[i][j] == 0 && checkarr4[i - 1][j] == 0))) {
                    check++;
                }
            }
        }
        if (check == 0) {
            _resetcheckarr4x4();
            break;
        }
    }
}

void _logicw5x5() {
    while (1) {
        for (int i = 1; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (arr5[i - 1][j] == 0) {
                    arr5[i - 1][j] = arr5[i][j];
                    arr5[i][j] = 0;
                    checkarr5[i - 1][j] = checkarr5[i][j];
                    checkarr5[i][j] = 0;
                    checkifchanged++;
                }
                else if (arr5[i - 1][j] != 0) {
                    if (arr5[i - 1][j] == arr5[i][j]) {
                        arr5[i - 1][j] = arr5[i][j] * 2;
                        arr5[i][j] = 0;
                        checkarr5[i - 1][j] = 1;
                        checkarr5[i][j] = 0;
                        checkifchanged++;
                        _scoreadd5x5(arr5[i - 1][j]);
                    }
                }
            }
        }
        int check = 0;
        for (int i = 1; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if ((arr5[i][j] != 0 && arr5[i - 1][j] == 0) || (((arr5[i][j] == arr5[i - 1][j]) && arr5[i][j] != 0) && (checkarr5[i][j] == 0 && checkarr5[i - 1][j] == 0))) {
                    check++;
                }
            }
        }
        if (check == 0) {
            _resetcheckarr5x5();
            break;
        }
    }
}

void _logica4x4() {
    while (1) {
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j < 4; j++) {
                if (arr4[i][j - 1] == 0) {
                    arr4[i][j - 1] = arr4[i][j];
                    arr4[i][j] = 0;
                    checkarr4[i][j - 1] = checkarr4[i][j];
                    checkarr4[i][j] = 0;
                    checkifchanged++;
                }
                else if (arr4[i][j - 1] != 0) {
                    if (arr4[i][j - 1] == arr4[i][j]) {
                        arr4[i][j - 1] = arr4[i][j] * 2;
                        arr4[i][j] = 0;
                        checkarr4[i][j - 1] = 1;
                        checkarr4[i][j] = 0;
                        checkifchanged++;
                        _scoreadd4x4(arr4[i][j - 1]);
                    }
                }
            }
        }
        int check = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j < 4; j++) {
                if ((arr4[i][j] != 0 && arr4[i][j - 1] == 0) || (((arr4[i][j] == arr4[i][j - 1]) && arr4[i][j] != 0) && (checkarr4[i][j] == 0 && checkarr4[i][j - 1] == 0))) {
                    check++;
                }
            }
        }
        if (check == 0) {
            _resetcheckarr4x4();
            break;
        }
    }
}

void _logica5x5() {
    while (1) {
        for (int i = 0; i < 5; i++) {
            for (int j = 1; j < 5; j++) {
                if (arr5[i][j - 1] == 0) {
                    arr5[i][j - 1] = arr5[i][j];
                    arr5[i][j] = 0;
                    checkarr5[i][j - 1] = checkarr5[i][j];
                    checkarr5[i][j] = 0;
                    checkifchanged++;
                }
                else if (arr5[i][j - 1] != 0) {
                    if (arr5[i][j - 1] == arr5[i][j]) {
                        arr5[i][j - 1] = arr5[i][j] * 2;
                        arr5[i][j] = 0;
                        checkarr5[i][j - 1] = 1;
                        checkarr5[i][j] = 0;
                        checkifchanged++;
                        _scoreadd4x4(arr5[i][j - 1]);
                    }
                }
            }
        }
        int check = 0;
        for (int i = 0; i < 5; i++) {
            for (int j = 1; j < 5; j++) {
                if ((arr5[i][j] != 0 && arr5[i][j - 1] == 0) || (((arr5[i][j] == arr5[i][j - 1]) && arr5[i][j] != 0) && (checkarr5[i][j] == 0 && checkarr5[i][j - 1] == 0))) {
                    check++;
                }
            }
        }
        if (check == 0) {
            _resetcheckarr5x5();
            break;
        }
    }
}

void _logics4x4() {
    while (1) {
        for (int i = 2; i >= 0; i--) {
            for (int j = 0; j < 4; j++) {
                if (arr4[i + 1][j] == 0) {
                    arr4[i + 1][j] = arr4[i][j];
                    arr4[i][j] = 0;
                    checkarr4[i + 1][j] = checkarr4[i][j];
                    checkarr4[i][j] = 0;
                    checkifchanged++;
                }
                else if (arr4[i + 1][j] != 0) {
                    if (arr4[i + 1][j] == arr4[i][j]) {
                        arr4[i + 1][j] = arr4[i][j] * 2;
                        arr4[i][j] = 0;
                        checkarr4[i + 1][j] = 1;
                        checkarr4[i][j] = 0;
                        checkifchanged++;
                        _scoreadd4x4(arr4[i + 1][j]);
                    }
                }
            }
        }
        int check = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                if ((arr4[i][j] != 0 && arr4[i + 1][j] == 0) || (((arr4[i][j] == arr4[i + 1][j]) && arr4[i][j] != 0) && (checkarr4[i][j] == 0 && checkarr4[i + 1][j] == 0))) {
                    check++;
                }
            }
        }
        if (check == 0) {
            _resetcheckarr4x4();
            break;
        }
    }
}

void _logics5x5() {
    while (1) {
        for (int i = 3; i >= 0; i--) {
            for (int j = 0; j < 5; j++) {
                if (arr5[i + 1][j] == 0) {
                    arr5[i + 1][j] = arr5[i][j];
                    arr5[i][j] = 0;
                    checkarr5[i + 1][j] = checkarr5[i][j];
                    checkarr5[i][j] = 0;
                    checkifchanged++;
                }
                else if (arr5[i + 1][j] != 0) {
                    if (arr5[i + 1][j] == arr5[i][j]) {
                        arr5[i + 1][j] = arr5[i][j] * 2;
                        arr5[i][j] = 0;
                        checkarr5[i + 1][j] = 1;
                        checkarr5[i][j] = 0;
                        checkifchanged++;
                        _scoreadd5x5(arr5[i + 1][j]);
                    }
                }
            }
        }
        int check = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 5; j++) {
                if ((arr5[i][j] != 0 && arr5[i + 1][j] == 0) || (((arr5[i][j] == arr5[i + 1][j]) && arr5[i][j] != 0) && (checkarr5[i][j] == 0 && checkarr5[i + 1][j] == 0))) {
                    check++;
                }
            }
        }
        if (check == 0) {
            _resetcheckarr5x5();
            break;
        }
    }
}

void _logicd4x4() {
    while (1) {
        for (int i = 0; i < 4; i++) {
            for (int j = 2; j >= 0; j--) {
                if (arr4[i][j + 1] == 0) {
                    arr4[i][j + 1] = arr4[i][j];
                    arr4[i][j] = 0;
                    checkarr4[i][j + 1] = checkarr4[i][j];
                    checkarr4[i][j] = 0;
                    checkifchanged++;
                }
                else if (arr4[i][j + 1] != 0) {
                    if (arr4[i][j + 1] == arr4[i][j]) {
                        arr4[i][j + 1] = arr4[i][j] * 2;
                        arr4[i][j] = 0;
                        checkarr4[i][j + 1] = 1;
                        checkarr4[i][j] = 0;
                        checkifchanged++;
                        _scoreadd4x4(arr4[i][j + 1]);
                    }
                }
            }
        }
        int check = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                if ((arr4[i][j] != 0 && arr4[i][j + 1] == 0) || (((arr4[i][j] == arr4[i][j + 1]) && arr4[i][j] != 0) && (checkarr4[i][j] == 0 && checkarr4[i][j + 1] == 0))) {
                    check++;
                }
            }
        }
        if (check == 0) {
            _resetcheckarr4x4();
            break;
        }
    }
}

void _logicd5x5() {
    while (1) {
        for (int i = 0; i < 5; i++) {
            for (int j = 3; j >= 0; j--) {
                if (arr5[i][j + 1] == 0) {
                    arr5[i][j + 1] = arr5[i][j];
                    arr5[i][j] = 0;
                    checkarr5[i][j + 1] = checkarr5[i][j];
                    checkarr5[i][j] = 0;
                    checkifchanged++;
                }
                else if (arr5[i][j + 1] != 0) {
                    if (arr5[i][j + 1] == arr5[i][j]) {
                        arr5[i][j + 1] = arr5[i][j] * 2;
                        arr5[i][j] = 0;
                        checkarr5[i][j + 1] = 1;
                        checkarr5[i][j] = 0;
                        checkifchanged++;
                        _scoreadd5x5(arr5[i][j + 1]);
                    }
                }
            }
        }
        int check = 0;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 4; j++) {
                if ((arr5[i][j] != 0 && arr5[i][j + 1] == 0) || (((arr5[i][j] == arr5[i][j + 1]) && arr5[i][j] != 0) && (checkarr5[i][j] == 0 && checkarr5[i][j + 1] == 0))) {
                    check++;
                }
            }
        }
        if (check == 0) {
            _resetcheckarr5x5();
            break;
        }
    }
}

void _logicaddnewnum4x4() {
    int n = rand() % 10;
    int howmanyadd;
    if (0 <= n && n <= 6) howmanyadd = 1;
    else howmanyadd = 0;
    int newpos1;
    int newnum1 = random2or4();
    while (1) {
        newpos1 = random16();
        if (arr4[newpos1 / 4][newpos1 % 4] == 0) {
            break;
        }
    }
    arr4[newpos1 / 4][newpos1 % 4] = newnum1;
}

void _logicaddnewnum5x5() {
    int n = rand() % 10;
    int howmanyadd;
    if (0 <= n && n <= 6) howmanyadd = 1;
    else howmanyadd = 0;
    int newpos1;
    int newnum1 = random2or4();
    while (1) {
        newpos1 = random16();
        if (arr5[newpos1 / 5][newpos1 % 5] == 0) {
            break;
        }
    }
    arr5[newpos1 / 5][newpos1 % 5] = newnum1;
}

int _checkifclear4x4() {
    int check = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (arr4[i][j] == 2048) {
                check = 1;
            }
        }
    }
    if (check == 1) return 1;
    else return 0;
}

int _checkifclear5x5() {
    int check = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (arr5[i][j] == 2048) {
                check = 1;
            }
        }
    }
    if (check == 1) return 1;
    else return 0;
}

int _checkifdead4x4() {
    int checkarr[5][5];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            checkarr[i][j] = -1;
        }
    }
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            checkarr[i][j] = arr4[i - 1][j - 1];
        }
    }
    int check = 0;
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            if (checkarr[i][j]!=0 && (((checkarr[i][j] != checkarr[i - 1][j]) && (checkarr[i][j] != checkarr[i + 1][j])) && ((checkarr[i][j] != checkarr[i][j - 1]) && (checkarr[i][j] != checkarr[i][j + 1])))) {
                check++;
            }
        }
    }
    if (check == 16) {
        if (score4x4 > highscore4x4) highscore4x4 = score4x4;
        score4x4 = 0;
        for (int i = 0; i < 4; i++) {
            gotoxy(27, 33);
            printf("저런!");
            Sleep(500);
            gotoxy(27, 33);
            printf("        ");
            Sleep(500);
        }
        return 1;
    }
    else return 0;
}

int _checkifdead5x5() {
    int checkarr[6][6];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            checkarr[i][j] = -1;
        }
    }
    for (int i = 1; i < 6; i++) {
        for (int j = 1; j < 6; j++) {
            checkarr[i][j] = arr5[i - 1][j - 1];
        }
    }
    int check = 0;
    for (int i = 1; i < 6; i++) {
        for (int j = 1; j < 6; j++) {
            if (checkarr[i][j] != 0 && (((checkarr[i][j] != checkarr[i - 1][j]) && (checkarr[i][j] != checkarr[i + 1][j])) && ((checkarr[i][j] != checkarr[i][j - 1]) && (checkarr[i][j] != checkarr[i][j + 1])))) {
                check++;
            }
        }
    }
    if (check == 25) {
        if (score5x5 > highscore5x5) highscore5x5 = score5x5;
        score5x5 = 0;
        for (int i = 0; i < 4; i++) {
            gotoxy(27, 33);
            printf("저런!");
            Sleep(500);
            gotoxy(27, 33);
            printf("        ");
            Sleep(500);
        }
        return 1;
    }
    else return 0;
}

void _printclearscreen4x4(int n) {
    system("mode con: cols=180 lines=60");
    system("cls");
    gotoxy(45, 5);
    printf("_________                                     __        .__          __  .__           ");
    gotoxy(45, 6);
    printf("\\_   ___ \\  ____   ____    ________________ _/  |_ __ __|  | _____ _/  |_|__| ____   ____  ");
    gotoxy(45, 7);
    printf("/    \\  \\/ /  _ \\ /    \\  / ___\\_  __ \\__  \\\\   __\\  |  \\  | \\__  \\\\   __\\  |/  _ \\ /    \\ ");
    gotoxy(45, 8);
    printf("\\     \\___(  <_> )   |  \\/ /_/  >  | \\// __ \\|  | |  |  /  |__/ __ \\|  | |  (  <_> )   |  \\");
    gotoxy(45, 9);
    printf(" \\______  /\\____/|___|  /\\___  /|__|  (____  /__| |____/|____(____  /__| |__|\\____/|___|  /");
    gotoxy(45, 10);
    printf("        \\/            \\//_____/            \\/                     \\/                    \\/ ");
    gotoxy(60, 15);
    printf("┌──────────────────────────────────────────────────────────┐\n");
    gotoxy(60, 16);
    printf("│                                                          │\n");
    gotoxy(60, 17);
    printf("│                                                          │\n");
    gotoxy(60, 18);
    printf("│                                                          │\n");
    gotoxy(60, 19);
    printf("│                                                          │\n");
    gotoxy(60, 20);
    printf("│                                                          │\n");
    gotoxy(60, 21);
    printf("│                                                          │\n");
    gotoxy(60, 22);
    printf("│                                                          │\n");
    gotoxy(60, 23);
    printf("└──────────────────────────────────────────────────────────┘\n");
    gotoxy(72, 19);
    printf("Your Score : %05d | Best Score : %05d", score4x4, highscore4x4);
    Sleep(10000);
}

void _printclearscreen5x5(int n) {
    system("mode con: cols=180 lines=60");
    system("cls");
    gotoxy(45, 5);
    printf("_________                                     __        .__          __  .__           ");
    gotoxy(45, 6);
    printf("\\_   ___ \\  ____   ____    ________________ _/  |_ __ __|  | _____ _/  |_|__| ____   ____  ");
    gotoxy(45, 7);
    printf("/    \\  \\/ /  _ \\ /    \\  / ___\\_  __ \\__  \\\\   __\\  |  \\  | \\__  \\\\   __\\  |/  _ \\ /    \\ ");
    gotoxy(45, 8);
    printf("\\     \\___(  <_> )   |  \\/ /_/  >  | \\// __ \\|  | |  |  /  |__/ __ \\|  | |  (  <_> )   |  \\");
    gotoxy(45, 9);
    printf(" \\______  /\\____/|___|  /\\___  /|__|  (____  /__| |____/|____(____  /__| |__|\\____/|___|  /");
    gotoxy(45, 10);
    printf("        \\/            \\//_____/            \\/                     \\/                    \\/ ");
    gotoxy(60, 15);
    printf("┌──────────────────────────────────────────────────────────┐\n");
    gotoxy(60, 16);
    printf("│                                                          │\n");
    gotoxy(60, 17);
    printf("│                                                          │\n");
    gotoxy(60, 18);
    printf("│                                                          │\n");
    gotoxy(60, 19);
    printf("│                                                          │\n");
    gotoxy(60, 20);
    printf("│                                                          │\n");
    gotoxy(60, 21);
    printf("│                                                          │\n");
    gotoxy(60, 22);
    printf("│                                                          │\n");
    gotoxy(60, 23);
    printf("└──────────────────────────────────────────────────────────┘\n");
    gotoxy(72, 19);
    printf("Your Score : %05d | Best Score : %05d", score5x5, highscore5x5);
    Sleep(10000);
}

void _printdeadscreen4x4(int n) {
    system("mode con: cols=180 lines=60");
    system("cls");
    gotoxy(45, 5);
    printf(" d888b   .d8b.  .88b  d88. d88888b       .d88b.  db    db d88888b d8888b. ");
    Sleep(300);
    gotoxy(45, 6);
    printf("88' Y8b d8' `8b 88'YbdP`88 88'          .8P  Y8. 88    88 88'     88  `8D ");
    Sleep(300);
    gotoxy(45, 7);
    printf("88      88ooo88 88  88  88 88ooooo      88    88 Y8    8P 88ooooo 88oobY' ");
    Sleep(300);
    gotoxy(45, 8);
    printf("88  ooo 88~~~88 88  88  88 88           88    88 `8b  d8' 88      88`8b   ");
    Sleep(300);
    gotoxy(45, 9);
    printf("88. ~8~ 88   88 88  88  88 88.          `8b  d8'  `8bd8'  88.     88 `88. ");
    Sleep(300);
    gotoxy(45, 10);
    printf(" Y888P  YP   YP YP  YP  YP Y88888P       `Y88P'     YP    Y88888P 88   YD ");
    Sleep(300);
    gotoxy(50, 15);
    printf("┌──────────────────────────────────────────────────────────┐\n");
    gotoxy(50, 16);
    printf("│                                                          │\n");
    gotoxy(50, 17);
    printf("│                                                          │\n");
    gotoxy(50, 18);
    printf("│                                                          │\n");
    gotoxy(50, 19);
    printf("│                                                          │\n");
    gotoxy(50, 20);
    printf("│                                                          │\n");
    gotoxy(50, 21);
    printf("│                                                          │\n");
    gotoxy(50, 22);
    printf("│                                                          │\n");
    gotoxy(50, 23);
    printf("└──────────────────────────────────────────────────────────┘\n");
    gotoxy(62, 19);
    printf("Your Score : %05d | Best Score : %05d", score4x4, highscore4x4);
    gotoxy(52, 25);
    printf("    d8888b. d88888b d888888b d8888b. db    db. d888b.");
    Sleep(300);
    gotoxy(52, 26);
    printf("    88  `8D 88'     `~~88~~' 88  `8D `8b  d8' VP `8D ");
    Sleep(300);
    gotoxy(52, 27);
    printf("    88oobY' 88ooooo    88    88oobY'  `8bd8'     odD' ");
    Sleep(300);
    gotoxy(52, 28);
    printf("    88`8b   88~~~~~88  88   `8b 88      8P'     odT");
    Sleep(300);
    gotoxy(52, 29);
    printf("    88 `88. 88.        88    88 `88.    88      ");
    Sleep(300);
    gotoxy(52, 30);
    printf("    88   YD Y88888P    YP    88   YD    YP      VP");
    Sleep(300);
    char inputkey;
    gotoxy(60, 32);
    printf("Press 'y' to retry, 'n' to End : ");
    while (1) {
        inputkey = _getch();
        if (inputkey == 'y') {
            retry = 1;
            break;
        }
        else if (inputkey == 'n') {
            retry = 0;
            break;
        }
        else continue;
    }
    system("cls");
}   

void _printdeadscreen5x5(int n) {
    system("mode con: cols=180 lines=60");
    system("cls");
    gotoxy(45, 5);
    printf(" d888b   .d8b.  .88b  d88. d88888b       .d88b.  db    db d88888b d8888b. ");
    Sleep(300);
    gotoxy(45, 6);
    printf("88' Y8b d8' `8b 88'YbdP`88 88'          .8P  Y8. 88    88 88'     88  `8D ");
    Sleep(300);
    gotoxy(45, 7);
    printf("88      88ooo88 88  88  88 88ooooo      88    88 Y8    8P 88ooooo 88oobY' ");
    Sleep(300);
    gotoxy(45, 8);
    printf("88  ooo 88~~~88 88  88  88 88           88    88 `8b  d8' 88      88`8b   ");
    Sleep(300);
    gotoxy(45, 9);
    printf("88. ~8~ 88   88 88  88  88 88.          `8b  d8'  `8bd8'  88.     88 `88. ");
    Sleep(300);
    gotoxy(45, 10);
    printf(" Y888P  YP   YP YP  YP  YP Y88888P       `Y88P'     YP    Y88888P 88   YD ");
    Sleep(300);
    gotoxy(50, 15);
    printf("┌──────────────────────────────────────────────────────────┐\n");
    gotoxy(50, 16);
    printf("│                                                          │\n");
    gotoxy(50, 17);
    printf("│                                                          │\n");
    gotoxy(50, 18);
    printf("│                                                          │\n");
    gotoxy(50, 19);
    printf("│                                                          │\n");
    gotoxy(50, 20);
    printf("│                                                          │\n");
    gotoxy(50, 21);
    printf("│                                                          │\n");
    gotoxy(50, 22);
    printf("│                                                          │\n");
    gotoxy(50, 23);
    printf("└──────────────────────────────────────────────────────────┘\n");
    gotoxy(62, 19);
    printf("Your Score : %05d | Best Score : %05d", score5x5, highscore5x5);
    gotoxy(52, 25);
    printf("    d8888b. d88888b d888888b d8888b. db    db. d888b.");
    Sleep(300);
    gotoxy(52, 26);
    printf("    88  `8D 88'     `~~88~~' 88  `8D `8b  d8' VP `8D ");
    Sleep(300);
    gotoxy(52, 27);
    printf("    88oobY' 88ooooo    88    88oobY'  `8bd8'     odD' ");
    Sleep(300);
    gotoxy(52, 28);
    printf("    88`8b   88~~~~~88  88   `8b 88      8P'     odT");
    Sleep(300);
    gotoxy(52, 29);
    printf("    88 `88. 88.        88    88 `88.    88      ");
    Sleep(300);
    gotoxy(52, 30);
    printf("    88   YD Y88888P    YP    88   YD    YP      VP");
    Sleep(300);
    char inputkey;
    gotoxy(60, 32);
    printf("Press 'y' to retry, 'n' to End : ");
    while (1) {
        inputkey = _getch();
        if (inputkey == 'y') {
            retry = 1;
            break;
        }
        else if (inputkey == 'n') {
            retry = 0;
            break;
        }
        else continue;
    }
    system("cls");
}

void _printdescription4x4() {
    gotoxy(0, 30);
    printf("┌──────────────────────────────────────────────────────────┐\n");
    gotoxy(0, 31);
    printf("│                                                          │\n");
    gotoxy(0, 32);
    printf("│                                                          │\n");
    gotoxy(0, 33);  
    printf("│                                                          │\n");
    gotoxy(0, 34);
    printf("│                                                          │\n");
    gotoxy(0, 35);
    printf("│                                                          │\n");
    gotoxy(0, 36);
    printf("│                                                          │\n");
    gotoxy(0, 37);
    printf("│                                                          │\n");
    gotoxy(0, 38);
    printf("└──────────────────────────────────────────────────────────┘\n");
    gotoxy(10, 34);
    printf("Your Score : %05d | Best Score : %05d", score4x4,highscore4x4);
}

void _printdescription5x5() {
    gotoxy(0, 38);
    printf("┌────────────────────────────────────────────────────────────────────────┐\n");
    gotoxy(0, 39);
    printf("│                                                                        │\n");
    gotoxy(0, 40); 
    printf("│                                                                        │\n");
    gotoxy(0, 41);
    printf("│                                                                        │\n");
    gotoxy(0, 42);
    printf("│                                                                        │\n");
    gotoxy(0, 43);
    printf("│                                                                        │\n");
    gotoxy(0, 44);
    printf("│                                                                        │\n");
    gotoxy(0, 45); 
    printf("│                                                                        │\n");
    gotoxy(0, 46);
    printf("└────────────────────────────────────────────────────────────────────────┘\n");
    gotoxy(10,42);
    printf("Your Score : %05d | Best Score : %05d", score5x5, highscore5x5);
}

void _print4x4() {
    _saveimagenames4x4();
    _printdescription4x4();
    _displayimages4x4();
}

void _print5x5() {
    _saveimagenames5x5();
    _printdescription5x5();
    _displayimages5x5();
    //flag
}
void _play4x4() {
    system("mode con: cols=61 lines=40");
    int rowstart = 30;
    int rowend = 112;
    int columnstart = 2;
    int columnend = 44;
    //_write4x4standard(rowstart,rowend,columnstart,columnend);
    int startingpos1 = random16();
    int startingpos2 = random16();
    int startingnum1 = random2or4();
    int startingnum2 = random2or4();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            arr4[i][j] = 0;
        }
    }
    arr4[startingpos1 / 4][startingnum1 % 4] = startingnum1;
    arr4[startingpos2 / 4][startingnum2 % 4] = startingnum2;
    _print4x4();
    while (1) {
        char inputkey;
        inputkey = _getch();
        if (inputkey == 'w') {
            _logicw4x4();
        }
        else if (inputkey == 'a') {
            _logica4x4();
        }
        else if (inputkey == 's') {
            _logics4x4();
        }
        else if (inputkey == 'd') {
            _logicd4x4();
        }
        if (checkifchanged != 0) _logicaddnewnum4x4();
        checkifchanged = 0;
        _print4x4();
        int ischecked = _checkifclear4x4();
        int isdead = _checkifdead4x4();
        if (ischecked == 1) {
            _printclearscreen4x4(4);
            break;
        }
        if (isdead == 1) {
            _printdeadscreen4x4(4);
            break;
        }
    }
}
void _play5x5() {
    system("mode con: cols=76 lines=48");
    int rowstart = 30;
    int rowend = 112;
    int columnstart = 2;
    int columnend = 44;
    //_write4x4standard(rowstart,rowend,columnstart,columnend);
    int startingpos1 = random16();
    int startingpos2 = random16();
    int startingnum1 = random2or4();
    int startingnum2 = random2or4();
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            arr5[i][j] = 0;
        }
    }
    arr5[startingpos1 / 5][startingnum1 % 5] = startingnum1;
    arr5[startingpos2 / 5][startingnum2 % 5] = startingnum2;
    _print5x5();
    while (1) {
        char inputkey;
        inputkey = _getch();
        if (inputkey == 'w') {
            _logicw5x5();
        }
        else if (inputkey == 'a') {
            _logica5x5();
        }
        else if (inputkey == 's') {
            _logics5x5();
        }
        else if (inputkey == 'd') {
            _logicd5x5();
        }
        if (checkifchanged != 0) _logicaddnewnum5x5();
        checkifchanged = 0;
        _print5x5();
        int ischecked = _checkifclear5x5();
        int isdead = _checkifdead5x5();
        if (ischecked == 1) {
            _printclearscreen5x5(5);
            break;
        }
        if (isdead == 1) {
            _printdeadscreen5x5(5);
            break;
        }
    }
}

int main() {
    srand(time(NULL));
    system("title 2048_in_C"); // 프로세스 이름
    system("mode con: cols=180 lines=60"); // !콘솔창 크기
    system("COLOR 0F"); //!배경색, 글자색
    HDC hdc = GetDC(GetConsoleWindow());
    Nocursor();
    while (1) {
        int selected_round;
        selected_round = _printmain();
        if (selected_round == 4) {
            _play4x4();
        }
        else {
            _play5x5();
        }
        if (retry == 0) break;
    }
}