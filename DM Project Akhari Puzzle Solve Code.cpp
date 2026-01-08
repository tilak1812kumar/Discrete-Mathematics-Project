#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

const int MAX_SIZE = 7;
int n;
char grid[MAX_SIZE][MAX_SIZE];
char userGrid[MAX_SIZE][MAX_SIZE];

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Windows console colors
enum Color {
    BLACK = 0, DARK_BLUE = 1, DARK_GREEN = 2, DARK_CYAN = 3,
    DARK_RED = 4, DARK_MAGENTA = 5, DARK_YELLOW = 6, GRAY = 7,
    DARK_GRAY = 8, BLUE = 9, GREEN = 10, CYAN = 11,
    RED = 12, MAGENTA = 13, YELLOW = 14, WHITE = 15
};

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void clearScreen() {
    system("cls");
}

bool inBounds(int x, int y) {
    return x >= 0 && y >= 0 && x < n && y < n;
}

void printTitle() {
    setColor(MAGENTA);
    cout << "\n";
    cout << "  ====================================\n";
    cout << "     AKARI - LIGHT UP PUZZLE GAME    \n";
    cout << "  ====================================\n";
    setColor(GRAY);
}

void drawGrid() {
    // Column numbers
    cout << "     ";
    setColor(CYAN);
    for (int j = 0; j < n; j++) {
        cout << " " << j << "  ";
    }
    setColor(GRAY);
    cout << "\n";
    
    // Top border
    cout << "   +";
    for (int j = 0; j < n; j++) {
        cout << "---+";
    }
    cout << "\n";
    
    // Grid rows
    for (int i = 0; i < n; i++) {
        setColor(CYAN);
        cout << " " << i << " ";
        setColor(GRAY);
        cout << "|";
        
        for (int j = 0; j < n; j++) {
            cout << " ";
            
            if (userGrid[i][j] == 'B') {
                setColor(YELLOW);
                cout << "O";
                setColor(GRAY);
            } else if (userGrid[i][j] >= '0' && userGrid[i][j] <= '4') {
                setColor(GREEN);
                cout << userGrid[i][j];
                setColor(GRAY);
            } else if (userGrid[i][j] == '#') {
                setColor(RED);
                cout << "#";
                setColor(GRAY);
            } else {
                // Check if cell is lit
                bool lit = false;
                for (int ii = 0; ii < n; ii++) {
                    for (int jj = 0; jj < n; jj++) {
                        if (userGrid[ii][jj] == 'B') {
                            if (ii == i) {
                                bool blocked = false;
                                int start = min(jj, j);
                                int end = max(jj, j);
                                for (int k = start + 1; k < end; k++) {
                                    if (userGrid[i][k] == '#' || (userGrid[i][k] >= '0' && userGrid[i][k] <= '4')) {
                                        blocked = true;
                                        break;
                                    }
                                }
                                if (!blocked && jj != j) lit = true;
                            } else if (jj == j) {
                                bool blocked = false;
                                int start = min(ii, i);
                                int end = max(ii, i);
                                for (int k = start + 1; k < end; k++) {
                                    if (userGrid[k][j] == '#' || (userGrid[k][j] >= '0' && userGrid[k][j] <= '4')) {
                                        blocked = true;
                                        break;
                                    }
                                }
                                if (!blocked && ii != i) lit = true;
                            }
                        }
                    }
                }
                
                if (lit) {
                    setColor(DARK_GRAY);
                    cout << ".";
                    setColor(GRAY);
                } else {
                    cout << ".";
                }
            }
            
            cout << " |";
        }
        cout << "\n";
        
        // Row separator
        cout << "   +";
        for (int j = 0; j < n; j++) {
            cout << "---+";
        }
        cout << "\n";
    }
}

bool canPlaceBulb(int x, int y, char g[MAX_SIZE][MAX_SIZE]) {
    if (g[x][y] != '.') return false;
    
    // Check all four directions for bulb conflicts
    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        
        while (inBounds(nx, ny)) {
            if (g[nx][ny] == '#' || (g[nx][ny] >= '0' && g[nx][ny] <= '4')) break;
            if (g[nx][ny] == 'B') return false;
            nx += dx[d];
            ny += dy[d];
        }
    }
    return true;
}

bool checkNumberedCells(char g[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] >= '0' && g[i][j] <= '4') {
                int required = g[i][j] - '0';
                int count = 0;
                
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (inBounds(ni, nj) && g[ni][nj] == 'B') count++;
                }
                
                if (count != required) return false;
            }
        }
    }
    return true;
}

bool allLit(char g[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] == '.') {
                bool lit = false;
                
                // Check if any bulb lights this cell
                for (int ii = 0; ii < n; ii++) {
                    for (int jj = 0; jj < n; jj++) {
                        if (g[ii][jj] == 'B') {
                            if (ii == i) {
                                bool blocked = false;
                                int start = min(jj, j);
                                int end = max(jj, j);
                                for (int k = start + 1; k < end; k++) {
                                    if (g[i][k] == '#' || (g[i][k] >= '0' && g[i][k] <= '4')) {
                                        blocked = true;
                                        break;
                                    }
                                }
                                if (!blocked) lit = true;
                            } else if (jj == j) {
                                bool blocked = false;
                                int start = min(ii, i);
                                int end = max(ii, i);
                                for (int k = start + 1; k < end; k++) {
                                    if (g[k][j] == '#' || (g[k][j] >= '0' && g[k][j] <= '4')) {
                                        blocked = true;
                                        break;
                                    }
                                }
                                if (!blocked) lit = true;
                            }
                        }
                    }
                }
                
                if (!lit) return false;
            }
        }
    }
    return true;
}

void generatePuzzle() {
    // Initialize empty grid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = '.';
        }
    }
    
    // Place some walls and numbered cells based on grid size
    if (n == 3) {
        grid[1][1] = '2';
        grid[0][2] = '#';
    } else if (n == 4) {
        grid[1][1] = '2';
        grid[2][2] = '1';
        grid[0][3] = '#';
        grid[3][0] = '#';
    } else if (n == 5) {
        grid[0][2] = '1';
        grid[2][0] = '2';
        grid[2][4] = '2';
        grid[4][2] = '1';
        grid[1][1] = '#';
        grid[3][3] = '#';
    } else if (n == 6) {
        grid[1][1] = '2';
        grid[1][4] = '1';
        grid[4][1] = '1';
        grid[4][4] = '2';
        grid[0][0] = '#';
        grid[0][5] = '#';
        grid[5][0] = '#';
        grid[5][5] = '#';
    } else if (n == 7) {
        grid[1][3] = '2';
        grid[3][1] = '2';
        grid[3][5] = '2';
        grid[5][3] = '2';
        grid[0][0] = '#';
        grid[0][6] = '#';
        grid[6][0] = '#';
        grid[6][6] = '#';
        grid[3][3] = '3';
    }
}

void printRules() {
    setColor(YELLOW);
    cout << "\n  RULES:\n";
    setColor(GRAY);
    cout << "  * Place bulbs (O) to light all cells\n";
    cout << "  * Numbers show required adjacent bulbs\n";
    cout << "  * Bulbs cannot see each other\n";
    cout << "  * # = Wall, O = Bulb, . = Empty cell\n";
    cout << "  * Lit cells appear darker\n\n";
}

void drawAsciiArt(bool solved) {
    if (solved) {
        setColor(GREEN);
        cout << "\n";
        cout << "        ___________\n";
        cout << "       |  SUCCESS! |\n";
        cout << "       |___________|\n";
        cout << "           |  |\n";
        cout << "          _|  |_\n";
        cout << "         |      |\n";
        cout << "         | BULB |\n";
        cout << "         |______|\n";
        cout << "           \\  /\n";
        cout << "            \\/\n";
        cout << "         ** LIGHT **\n";
        setColor(GRAY);
    } else {
        setColor(RED);
        cout << "\n";
        cout << "         __________\n";
        cout << "        |   NOT    |\n";
        cout << "        | COMPLETE |\n";
        cout << "        |__________|\n";
        cout << "            ||\n";
        cout << "           _||_\n";
        cout << "          |    |\n";
        cout << "          | ?? |\n";
        cout << "          |____|\n";
        cout << "        TRY AGAIN!\n";
        setColor(GRAY);
    }
}

void userPlay() {
    int moves = 0;
    
    while (true) {
        clearScreen();
        printTitle();
        printRules();
        drawGrid();
        
        setColor(CYAN);
        cout << "\n  Moves: " << moves << "\n";
        setColor(GRAY);
        cout << "  Enter row col (or -1 -1 to check): ";
        
        int x, y;
        cin >> x >> y;
        
        if (x == -1 && y == -1) break;
        
        if (!inBounds(x, y)) {
            setColor(RED);
            cout << "  Invalid coordinates! Press Enter...";
            setColor(GRAY);
            cin.ignore();
            cin.get();
            continue;
        }
        
        moves++;
        
        if (userGrid[x][y] == '.') {
            if (canPlaceBulb(x, y, userGrid)) {
                userGrid[x][y] = 'B';
            } else {
                setColor(RED);
                cout << "  Cannot place bulb here! Press Enter...";
                setColor(GRAY);
                cin.ignore();
                cin.get();
                moves--;
            }
        } else if (userGrid[x][y] == 'B') {
            userGrid[x][y] = '.';
        } else {
            setColor(RED);
            cout << "  Cannot place bulb on wall/number! Press Enter...";
            setColor(GRAY);
            cin.ignore();
            cin.get();
            moves--;
        }
    }
    
    // Final check
    clearScreen();
    setColor(MAGENTA);
    cout << "\n";
    cout << "  ====================================\n";
    cout << "           FINAL RESULT              \n";
    cout << "  ====================================\n";
    setColor(GRAY);
    
    cout << "\n  Your Solution (Moves: " << moves << "):\n\n";
    drawGrid();
    
    bool numbersOk = checkNumberedCells(userGrid);
    bool allLitOk = allLit(userGrid);
    
    cout << "\n";
    setColor(YELLOW);
    cout << "  RESULTS:\n";
    setColor(GRAY);
    
    if (numbersOk) {
        setColor(GREEN);
        cout << "  [OK] ";
        setColor(GRAY);
    } else {
        setColor(RED);
        cout << "  [X]  ";
        setColor(GRAY);
    }
    cout << "Numbered cells satisfied\n";
    
    if (allLitOk) {
        setColor(GREEN);
        cout << "  [OK] ";
        setColor(GRAY);
    } else {
        setColor(RED);
        cout << "  [X]  ";
        setColor(GRAY);
    }
    cout << "All cells lit\n";
    
    drawAsciiArt(numbersOk && allLitOk);
    
    cout << "\n  Press Enter to exit...";
    cin.ignore();
    cin.get();
}

int main() {
    srand(time(0));
    
    clearScreen();
    printTitle();
    
    setColor(YELLOW);
    cout << "\n  Welcome to AKARI Puzzle!\n";
    setColor(GRAY);
    cout << "  Enter grid size (3-7): ";
    cin >> n;
    
    if (n < 3 || n > 7) {
        setColor(RED);
        cout << "\n  Invalid size! Use 3-7 only.\n";
        setColor(GRAY);
        cout << "  Press Enter to exit...";
        cin.ignore();
        cin.get();
        return 0;
    }
    
    generatePuzzle();
    
    // Copy to user grid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            userGrid[i][j] = grid[i][j];
        }
    }
    
    userPlay();
    return 0;
}