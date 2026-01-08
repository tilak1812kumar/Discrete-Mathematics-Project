#include <iostream>
#include <cstring>
#include <queue>
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

// true = BLACK, false = WHITE
bool cell[10][10];

// -1 = no number
int grid[10][10];

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

// ---------------- DISPLAY HEADER ----------------
void displayHeader() {
    setColor(COLOR_LIGHT_CYAN);
    cout << "========================================================\n";
    setColor(COLOR_LIGHT_YELLOW);
    cout << "             NURIKABE PUZZLE GAME\n";
    setColor(COLOR_LIGHT_CYAN);
    cout << "========================================================\n";
    resetColor();
}

// ---------------- DRAW GRID ----------------
void displayGrid() {
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

        // Cell content
        for (int j = 0; j < N; j++) {
            if (cell[i][j]) {
                // Black cell
                setColor(COLOR_WHITE, COLOR_BLACK);
                cout << " # ";
                resetColor();
            } else if (grid[i][j] > 0) {
                // Numbered island (white)
                setColor(COLOR_YELLOW, COLOR_BLUE);
                cout << " " << grid[i][j] << " ";
                resetColor();
            } else {
                // Empty white cell
                setColor(COLOR_GRAY, COLOR_WHITE);
                cout << "   ";
                resetColor();
            }
            
            setColor(COLOR_CYAN);
            cout << "|";
            resetColor();
        }
        cout << "\n";

        // Bottom border of row
        setColor(COLOR_CYAN);
        cout << "   +";
        for (int j = 0; j < N; j++) {
            cout << "----";
        }
        cout << "+\n";
        resetColor();
    }
    cout << "\n";
}

// ---------------- NO 2x2 BLACK ----------------
bool no2x2Black() {
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < N - 1; j++)
            if (cell[i][j] && cell[i+1][j] &&
                cell[i][j+1] && cell[i+1][j+1])
                return false;
    return true;
}

// ---------------- BLACK CONNECTED ----------------
bool blackConnected() {
    bool vis[10][10] = {false};
    queue<pair<int,int> > q;
    int total = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (cell[i][j]) total++;

    if (total == 0) return false;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (cell[i][j]) {
                q.push(make_pair(i,j));
                vis[i][j] = true;
                goto bfs;
            }

bfs:
    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};
    int cnt = 0;

    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        cnt++;

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx>=0 && nx<N && ny>=0 && ny<N &&
                cell[nx][ny] && !vis[nx][ny]) {
                vis[nx][ny] = true;
                q.push(make_pair(nx,ny));
            }
        }
    }
    return cnt == total;
}

// ---------------- ISLAND CHECK ----------------
bool checkIslands() {
    bool vis[10][10] = {false};
    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            if (grid[i][j] > 0) {
                int need = grid[i][j];
                int cnt = 0;
                queue<pair<int,int> > q;

                q.push(make_pair(i,j));
                vis[i][j] = true;

                while (!q.empty()) {
                    int x = q.front().first;
                    int y = q.front().second;
                    q.pop();
                    cnt++;

                    for (int d = 0; d < 4; d++) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];

                        if (nx>=0 && nx<N && ny>=0 && ny<N &&
                            !cell[nx][ny] && !vis[nx][ny]) {

                            if (grid[nx][ny] > 0 && !(nx==i && ny==j))
                                return false;

                            vis[nx][ny] = true;
                            q.push(make_pair(nx,ny));
                        }
                    }
                }
                if (cnt != need) return false;
            }
        }
    }
    return true;
}

// ---------------- WIN CHECK ----------------
bool checkWin() {
    return no2x2Black() && blackConnected() && checkIslands();
}

// ---------------- LOAD SAMPLE PUZZLES ----------------
void loadPuzzle(int size) {
    memset(cell, false, sizeof(cell));
    memset(grid, -1, sizeof(grid));

    if (size == 5) {
        // 5x5 puzzle
        grid[0][0] = 3;
        grid[0][3] = 2;
        grid[1][4] = 2;
        grid[2][1] = 1;
        grid[3][0] = 2;
        grid[3][3] = 3;
        grid[4][4] = 1;
    }
    else if (size == 6) {
        // 6x6 puzzle
        grid[0][0] = 4;
        grid[0][3] = 2;
        grid[1][5] = 2;
        grid[2][1] = 1;
        grid[3][3] = 3;
        grid[4][0] = 2;
        grid[4][4] = 2;
        grid[5][2] = 3;
        grid[5][5] = 1;
    }
    else if (size == 7) {
        // 7x7 puzzle
        grid[0][0] = 4;
        grid[0][4] = 3;
        grid[1][2] = 2;
        grid[1][6] = 2;
        grid[2][5] = 1;
        grid[3][1] = 3;
        grid[3][3] = 2;
        grid[4][6] = 3;
        grid[5][0] = 2;
        grid[5][4] = 2;
        grid[6][2] = 3;
        grid[6][6] = 1;
    }
    else if (size == 8) {
        // 8x8 puzzle
        grid[0][1] = 4;
        grid[0][5] = 3;
        grid[1][3] = 2;
        grid[1][7] = 2;
        grid[2][0] = 3;
        grid[2][6] = 1;
        grid[3][2] = 2;
        grid[3][4] = 3;
        grid[4][1] = 3;
        grid[4][6] = 2;
        grid[5][4] = 1;
        grid[6][0] = 2;
        grid[6][3] = 4;
        grid[7][2] = 2;
        grid[7][7] = 3;
    }
}

// ---------------- DISPLAY COMMANDS ----------------
void displayCommands() {
    setColor(COLOR_LIGHT_MAGENTA);
    cout << "=========================================\n";
    cout << "              COMMANDS                   \n";
    cout << "=========================================\n";
    resetColor();
    
    setColor(COLOR_LIGHT_CYAN);
    cout << "  b <row> <col>";
    setColor(COLOR_WHITE);
    cout << "  -> Mark cell as BLACK\n";
    
    setColor(COLOR_LIGHT_CYAN);
    cout << "  w <row> <col>";
    setColor(COLOR_WHITE);
    cout << "  -> Mark cell as WHITE\n";
    
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

// ---------------- DISPLAY RULES ----------------
void displayRules() {
    setColor(COLOR_LIGHT_YELLOW);
    cout << "\n*** How to Play Nurikabe ***\n";
    resetColor();
    
    setColor(COLOR_WHITE);
    cout << "1. Paint cells BLACK to create a continuous wall\n";
    cout << "2. Each number shows the size of its WHITE island\n";
    cout << "3. WHITE islands cannot touch each other\n";
    cout << "4. No 2x2 blocks of BLACK cells allowed\n";
    cout << "5. All BLACK cells must be connected\n\n";
    resetColor();
}

// ---------------- MAIN ----------------
int main() {
    clearScreen();
    displayHeader();
    displayRules();

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
        cout << " / ";
        setColor(COLOR_YELLOW);
        cout << "8";
        setColor(COLOR_LIGHT_CYAN);
        cout << "): ";
        resetColor();
        
        cin >> N;
        
        if (N >= 5 && N <= 8) break;
        
        setColor(COLOR_LIGHT_RED);
        cout << "Invalid size! Please choose 5, 6, 7, or 8.\n";
        resetColor();
    }

    loadPuzzle(N);
    
    setColor(COLOR_LIGHT_GREEN);
    cout << "\nPuzzle loaded! Press any key to start...";
    resetColor();
    _getch();

    string cmd;
    while (true) {
        displayGrid();
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
            memset(cell, false, sizeof(cell));
            setColor(COLOR_LIGHT_GREEN);
            cout << "\nPuzzle reset! All cells cleared.\n";
            resetColor();
            Sleep(800);
        }

        else if (cmd == "b" || cmd == "B") {
            int r, c;
            cin >> r >> c;
            if (r >= 0 && r < N && c >= 0 && c < N) {
                if (grid[r][c] > 0) {
                    setColor(COLOR_LIGHT_RED);
                    cout << "Cannot mark numbered cell as BLACK!\n";
                    resetColor();
                } else {
                    cell[r][c] = true;
                    setColor(COLOR_LIGHT_GREEN);
                    cout << "Cell (" << r << "," << c << ") marked BLACK\n";
                    resetColor();
                }
            } else {
                setColor(COLOR_LIGHT_RED);
                cout << "Invalid coordinates!\n";
                resetColor();
            }
            Sleep(500);
        }

        else if (cmd == "w" || cmd == "W") {
            int r, c;
            cin >> r >> c;
            if (r >= 0 && r < N && c >= 0 && c < N) {
                cell[r][c] = false;
                setColor(COLOR_LIGHT_GREEN);
                cout << "Cell (" << r << "," << c << ") marked WHITE\n";
                resetColor();
            } else {
                setColor(COLOR_LIGHT_RED);
                cout << "Invalid coordinates!\n";
                resetColor();
            }
            Sleep(500);
        }

        else if (cmd == "s" || cmd == "S") {
            bool win = checkWin();
            bool no2x2 = no2x2Black();
            bool connected = blackConnected();
            bool islands = checkIslands();
            
            if (win) {
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
                cout << "========================================\n";
                resetColor();
                
                if (!no2x2) {
                    setColor(COLOR_LIGHT_RED);
                    cout << "  X 2x2 BLACK blocks detected!\n";
                    resetColor();
                }
                if (!connected) {
                    setColor(COLOR_LIGHT_RED);
                    cout << "  X BLACK cells not all connected!\n";
                    resetColor();
                }
                if (!islands) {
                    setColor(COLOR_LIGHT_RED);
                    cout << "  X Island sizes don't match!\n";
                    resetColor();
                }
            }
            
            setColor(COLOR_LIGHT_CYAN);
            cout << "\nPress any key to continue...";
            resetColor();
            _getch();
        }

        else {
            setColor(COLOR_LIGHT_RED);
            cout << "Unknown command! Use b, w, s, r, or q\n";
            resetColor();
            Sleep(800);
        }
    }

    return 0;
}