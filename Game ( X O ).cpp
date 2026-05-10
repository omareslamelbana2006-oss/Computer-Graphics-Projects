#include <windows.h>
#include <string>

// تعريف الزراير وحالة اللعبة
HWND buttons[3][3];
bool playerX = true;
int board[3][3] = {0}; // 0: فاضي, 1: X, 2: O
int movesCount = 0;    // لحساب عدد الحركات ومعرفة التعادل

// دالة لإعادة تشغيل اللعبة
void ResetGame() {
    playerX = true;
    movesCount = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = 0;
            SetWindowTextA(buttons[i][j], ""); // مسح النص من على الزراير
        }
    }
}

void CheckGameState() {
    int winner = 0;
    // فحص الصفوف والأعمدة والأقطار
    for(int i=0; i<3; i++) {
        if(board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) winner = board[i][0];
        if(board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) winner = board[0][i];
    }
    if(board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) winner = board[0][0];
    if(board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) winner = board[0][2];

    if(winner != 0) {
        std::string msg = (winner == 1 ? "Player X Wins!" : "Player O Wins!");
        MessageBoxA(NULL, msg.c_str(), "Game Over", MB_OK);
        ResetGame(); // إعادة اللعبة بدل الخروج
    } 
    else if (movesCount == 9) { // لو وصلنا لـ 9 حركات ومافيش فائز
        MessageBoxA(NULL, "It's a Draw! No winner this time.", "Game Over", MB_OK);
        ResetGame(); // إعادة اللعبة تلقائياً
    }
}

// دالة التحكم في الضغط على الزراير
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    buttons[i][j] = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD,
                                              10 + j * 100, 10 + i * 100, 90, 90,
                                              hwnd, (HMENU)(intptr_t)(i * 3 + j), NULL, NULL);
                }
            }
            break;
        case WM_COMMAND: {
            int id = LOWORD(wp);
            int r = id / 3;
            int c = id % 3;
            if (board[r][c] == 0) {
                movesCount++; // زيادة عدد الحركات
                board[r][c] = playerX ? 1 : 2;
                SetWindowTextA(buttons[r][c], playerX ? "X" : "O");
                CheckGameState();
                playerX = !playerX;
            }
            break;
        }
        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProcA(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nCmdShow) {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "TicTacToeClass";
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    RegisterClassA(&wc);
    HWND hwnd = CreateWindowA("TicTacToeClass", "Tic Tac Toe - C++ Native GUI",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 330, 380,
                             NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 0;
}