#include <iostream>
#include <cstring>
#include <vector>
#include <windows.h>
#include <conio.h>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Windows Console Colors
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#define COLOR_WHITE 7
#define COLOR_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_LIGHT_YELLOW 14
#define COLOR_BRIGHT_WHITE 15

int N;

// grid numbers (islands)
int grid[7][7];

// bridges
int bh[7][7]; // horizontal (to right)
int bv[7][7]; // vertical (down)

// ---------------- COLOR FUNCTIONS ----------------
void setColor(int textColor, int bgColor = COLOR_BLACK) {
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void resetColor() {
    SetConsoleTextAttribute(hConsole, COLOR_WHITE);
}

// ---------------- CLEAR SCREEN ----------------
void clearScreen() {
    system("cls");
}

// ---------------- CURSOR POSITION ----------------
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

// ---------------- DISPLAY HEADER ----------------
void displayHeader() {
    setColor(COLOR_LIGHT_CYAN);
    cout << "========================================================\n";
    setColor(COLOR_LIGHT_YELLOW);
    cout << "           HASHI (BRIDGES) PUZZLE GAME\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "========================================================\n";
    resetColor();
}

// ---------------- DISPLAY GRID ----------------
void display() {
    clearScreen();
    displayHeader();
    
    setColor(COLOR_BRIGHT_WHITE);
    cout << "\n     ";
    for (int j = 0; j < N; j++) {
        setColor(COLOR_LIGHT_CYAN);
        cout << j << "   ";
    }
    resetColor();
    cout << "\n";
    
    setColor(COLOR_CYAN);
    cout << "   +";
    for (int j = 0; j < N; j++) {
        cout << "----";
    }
    cout << "+\n";
    resetColor();

    for (int i = 0; i < N; i++) {
        // Row number
        setColor(COLOR_LIGHT_CYAN);
        cout << " " << i << " ";
        setColor(COLOR_CYAN);
        cout << "|";
        resetColor();

        // numbers + horizontal bridges
        for (int j = 0; j < N; j++) {
            if (grid[i][j] > 0) {
                setColor(COLOR_YELLOW, COLOR_BLUE);
                cout << " " << grid[i][j] << " ";
                resetColor();
            } else {
                setColor(COLOR_GRAY);
                cout << " . ";
                resetColor();
            }

            if (bh[i][j] == 1) {
                setColor(COLOR_LIGHT_GREEN);
                cout << "-";
            } else if (bh[i][j] == 2) {
                setColor(COLOR_LIGHT_GREEN);
                cout << "=";
            } else {
                cout << " ";
            }
            resetColor();
        }
        
        setColor(COLOR_CYAN);
        cout << "|\n";
        resetColor();

        // vertical bridges
        if (i < N - 1) {
            setColor(COLOR_CYAN);
            cout << "   |";
            resetColor();
            
            for (int j = 0; j < N; j++) {
                if (bv[i][j] == 1) {
                    setColor(COLOR_LIGHT_GREEN);
                    cout << " | ";
                    resetColor();
                } else if (bv[i][j] == 2) {
                    setColor(COLOR_LIGHT_GREEN);
                    cout << " H ";
                    resetColor();
                } else {
                    cout << "   ";
                }
                cout << " ";
            }
            
            setColor(COLOR_CYAN);
            cout << "|\n";
            resetColor();
        }
    }

    setColor(COLOR_CYAN);
    cout << "   +";
    for (int j = 0; j < N; j++) {
        cout << "----";
    }
    cout << "+\n";
    resetColor();
}

// ---------------- DEGREE CHECK ----------------
bool checkDegrees() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] > 0) {
                int deg = 0;

                if (j > 0) deg += bh[i][j-1];
                deg += bh[i][j];

                if (i > 0) deg += bv[i-1][j];
                deg += bv[i][j];

                if (deg != grid[i][j])
                    return false;
            }
        }
    }
    return true;
}

// ---------------- DFS ----------------
void dfs(int x, int y, vector<vector<bool>>& vis) {
    vis[x][y] = true;

    if (y+1 < N && bh[x][y] && !vis[x][y+1])
        dfs(x, y+1, vis);
    if (y > 0 && bh[x][y-1] && !vis[x][y-1])
        dfs(x, y-1, vis);
    if (x+1 < N && bv[x][y] && !vis[x+1][y])
        dfs(x+1, y, vis);
    if (x > 0 && bv[x-1][y] && !vis[x-1][y])
        dfs(x-1, y, vis);
}

// ---------------- CONNECTIVITY CHECK ----------------
bool connected() {
    vector<vector<bool>> vis(N, vector<bool>(N, false));
    int sx = -1, sy = -1;

    for (int i = 0; i < N && sx == -1; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] > 0) {
                sx = i;
                sy = j;
                break;
            }
        }
    }

    if (sx == -1) return false;

    dfs(sx, sy, vis);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (grid[i][j] > 0 && !vis[i][j])
                return false;

    return true;
}

// ---------------- WIN CHECK ----------------
bool checkWin() {
    return checkDegrees() && connected();
}

// ---------------- LOAD PUZZLES ----------------
void loadPuzzle(int size) {
    memset(grid, 0, sizeof(grid));
    memset(bh, 0, sizeof(bh));
    memset(bv, 0, sizeof(bv));

    if (size == 5) {
        int t[5][5] = {
            {0,2,0,0,3},
            {0,0,0,0,0},
            {1,0,4,0,2},
            {0,0,0,0,0},
            {3,0,0,1,0}
        };
        for (int i=0;i<5;i++)
            for (int j=0;j<5;j++)
                grid[i][j] = t[i][j];
    }

    else if (size == 6) {
        int t[6][6] = {
            {0,3,0,0,2,0},
            {0,0,0,0,0,0},
            {2,0,4,0,0,1},
            {0,0,0,0,0,0},
            {1,0,0,3,0,2},
            {0,2,0,0,1,0}
        };
        for (int i=0;i<6;i++)
            for (int j=0;j<6;j++)
                grid[i][j] = t[i][j];
    }

    else if (size == 7) {
        int t[7][7] = {
            {0,3,0,0,2,0,1},
            {0,0,0,0,0,0,0},
            {2,0,4,0,0,3,0},
            {0,0,0,0,0,0,0},
            {1,0,0,3,0,0,2},
            {0,0,0,0,0,0,0},
            {3,0,1,0,2,0,0}
        };
        for (int i=0;i<7;i++)
            for (int j=0;j<7;j++)
                grid[i][j] = t[i][j];
    }
}

// ---------------- DISPLAY COMMANDS ----------------
void displayCommands() {
    cout << "\n";
    setColor(COLOR_LIGHT_MAGENTA);
    cout << "=========================================\n";
    cout << "              COMMANDS                   \n";
    cout << "=========================================\n";
    resetColor();
    
    setColor(COLOR_LIGHT_CYAN);
    cout << "  h <row> <col>";
    setColor(COLOR_WHITE);
    cout << "  -> Toggle horizontal bridge\n";
    
    setColor(COLOR_LIGHT_CYAN);
    cout << "  v <row> <col>";
    setColor(COLOR_WHITE);
    cout << "  -> Toggle vertical bridge\n";
    
    setColor(COLOR_LIGHT_CYAN);
    cout << "  s          ";
    setColor(COLOR_WHITE);
    cout << "  -> Check solution\n";
    
    setColor(COLOR_LIGHT_CYAN);
    cout << "  r          ";
    setColor(COLOR_WHITE);
    cout << "  -> Reset puzzle\n";
    
    setColor(COLOR_LIGHT_CYAN);
    cout << "  q          ";
    setColor(COLOR_WHITE);
    cout << "  -> Quit game\n";
    
    resetColor();
}

// ---------------- MAIN ----------------
int main() {
    clearScreen();
    displayHeader();
    
    setColor(COLOR_LIGHT_YELLOW);
    cout << "\n*** Welcome to Hashi Puzzle Game! ***\n";
    resetColor();
    
    setColor(COLOR_WHITE);
    cout << "Connect all islands with bridges!\n";
    cout << "Each island number shows how many bridges it needs.\n\n";
    resetColor();

    while (true) {
        setColor(COLOR_LIGHT_CYAN);
        cout << "Choose grid size (";
        setColor(COLOR_YELLOW);
        cout << "5";
        setColor(COLOR_LIGHT_CYAN);
        cout << " / ";
        setColor(COLOR_YELLOW);
        cout << "6";
        setColor(COLOR_LIGHT_CYAN);
        cout << " / ";
        setColor(COLOR_YELLOW);
        cout << "7";
        setColor(COLOR_LIGHT_CYAN);
        cout << "): ";
        resetColor();
        
        cin >> N;
        if (N == 5 || N == 6 || N == 7) break;
        
        setColor(COLOR_LIGHT_RED);
        cout << "Invalid size! Please choose 5, 6, or 7.\n";
        resetColor();
    }

    loadPuzzle(N);
    
    setColor(COLOR_LIGHT_GREEN);
    cout << "\nPress any key to start...";
    resetColor();
    _getch();

    string cmd;
    while (true) {
        display();
        displayCommands();
        
        cout << "\n";
        setColor(COLOR_BRIGHT_WHITE);
        cout << ">> Command: ";
        resetColor();
        cin >> cmd;

        if (cmd == "q" || cmd == "Q") {
            setColor(COLOR_LIGHT_YELLOW);
            cout << "\nThanks for playing! Goodbye!\n";
            resetColor();
            break;
        }

        else if (cmd == "r" || cmd == "R") {
            memset(bh, 0, sizeof(bh));
            memset(bv, 0, sizeof(bv));
            setColor(COLOR_LIGHT_GREEN);
            cout << "\nPuzzle reset!\n";
            resetColor();
            Sleep(800);
        }

        else if (cmd == "h" || cmd == "H") {
            int r, c;
            cin >> r >> c;
            if (r >= 0 && r < N && c >= 0 && c < N) {
                bh[r][c] = (bh[r][c] + 1) % 3;
                setColor(COLOR_LIGHT_GREEN);
                cout << "Horizontal bridge toggled at (" << r << "," << c << ")\n";
                resetColor();
            } else {
                setColor(COLOR_LIGHT_RED);
                cout << "Invalid coordinates!\n";
                resetColor();
            }
            Sleep(500);
        }

        else if (cmd == "v" || cmd == "V") {
            int r, c;
            cin >> r >> c;
            if (r >= 0 && r < N && c >= 0 && c < N) {
                bv[r][c] = (bv[r][c] + 1) % 3;
                setColor(COLOR_LIGHT_GREEN);
                cout << "Vertical bridge toggled at (" << r << "," << c << ")\n";
                resetColor();
            } else {
                setColor(COLOR_LIGHT_RED);
                cout << "Invalid coordinates!\n";
                resetColor();
            }
            Sleep(500);
        }

        else if (cmd == "s" || cmd == "S") {
            if (checkWin()) {
                setColor(COLOR_BLACK, COLOR_LIGHT_GREEN);
                cout << "\n";
                cout << "========================================\n";
                cout << "    *** CONGRATULATIONS! ***\n";
                cout << "        PUZZLE SOLVED!\n";
                cout << "========================================\n";
                resetColor();
            } else {
                setColor(COLOR_BLACK, COLOR_LIGHT_YELLOW);
                cout << "\n";
                cout << "========================================\n";
                cout << "        NOT SOLVED YET!\n";
                resetColor();
                
                if (!checkDegrees()) {
                    setColor(COLOR_LIGHT_RED);
                    cout << "  Bridge counts don't match!\n";
                }
                if (!connected()) {
                    setColor(COLOR_LIGHT_RED);
                    cout << "  Islands not all connected!\n";
                }
                
                setColor(COLOR_BLACK, COLOR_LIGHT_YELLOW);
                cout << "========================================\n";
                resetColor();
            }
            
            setColor(COLOR_LIGHT_CYAN);
            cout << "\nPress any key to continue...";
            resetColor();
            _getch();
        }

        else {
            setColor(COLOR_LIGHT_RED);
            cout << "Unknown command! Use h, v, s, r, or q\n";
            resetColor();
            Sleep(800);
        }
    }

    return 0;
}