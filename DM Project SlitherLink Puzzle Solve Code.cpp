#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <windows.h>
#include <vector>
#include <stack>
#include <limits>
using namespace std;

const int MAX_SIZE = 7;
int n, m;
int grid[MAX_SIZE][MAX_SIZE];  // cell numbers, -1 = empty
int h[MAX_SIZE+1][MAX_SIZE];   // horizontal edges
int v[MAX_SIZE][MAX_SIZE+1];   // vertical edges
int solution_h[MAX_SIZE+1][MAX_SIZE];
int solution_v[MAX_SIZE][MAX_SIZE+1];

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum Color {
    BLACK = 0, DARK_BLUE = 1, DARK_GREEN = 2, DARK_CYAN = 3,
    DARK_RED = 4, DARK_MAGENTA = 5, DARK_YELLOW = 6, GRAY = 7,
    DARK_GRAY = 8, BLUE = 9, GREEN = 10, CYAN = 11,
    RED = 12, MAGENTA = 13, YELLOW = 14, WHITE = 15
};

void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void clearScreen() {
    system("cls");
}

void printTitle() {
    setColor(CYAN);
    cout << "\n";
    cout << "  ==========================================\n";
    cout << "      SLITHERLINK - LOOP THE LOOP PUZZLE   \n";
    cout << "  ==========================================\n";
    setColor(GRAY);
}

void displayPuzzle() {
    cout << "\n";
    
    for (int i = 0; i <= n; i++) {
        // Top edges of row
        for (int j = 0; j < m; j++) {
            setColor(YELLOW);
            cout << "+";
            setColor(GRAY);
            
            if (h[i][j]) {
                setColor(GREEN);
                cout << "---";
                setColor(GRAY);
            } else {
                setColor(DARK_GRAY);
                cout << " . ";
                setColor(GRAY);
            }
        }
        setColor(YELLOW);
        cout << "+";
        setColor(GRAY);
        cout << "\n";

        if (i < n) {
            // Vertical edges and numbers
            for (int j = 0; j <= m; j++) {
                if (v[i][j]) {
                    setColor(GREEN);
                    cout << "|";
                    setColor(GRAY);
                } else {
                    setColor(DARK_GRAY);
                    cout << ".";
                    setColor(GRAY);
                }
                
                if (j < m) {
                    // Print cell number
                    if (grid[i][j] == -1) {
                        cout << "   ";
                    } else {
                        setColor(MAGENTA);
                        cout << " " << grid[i][j] << " ";
                        setColor(GRAY);
                    }
                }
            }
            cout << "\n";
        }
    }
    cout << endl;
}

bool checkCells() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] != -1) {
                int cnt = h[i][j] + h[i+1][j] + v[i][j] + v[i][j+1];
                if (cnt != grid[i][j]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool checkVertices() {
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            int deg = 0;
            if (i > 0 && j < m && h[i][j]) deg++;
            if (i < n && j < m && h[i+1][j]) deg++;
            if (j > 0 && i < n && v[i][j]) deg++;
            if (j < m && i < n && v[i][j+1]) deg++;
            if (deg != 0 && deg != 2) return false;
        }
    }
    return true;
}

bool singleLoop() {
    vector<vector<bool>> visited(n+1, vector<bool>(m+1, false));
    int sx = -1, sy = -1;
    int total_vertices = 0;

    // Find start vertex and count vertices in loop
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            int deg = 0;
            if (i > 0 && j < m && h[i][j]) deg++;
            if (i < n && j < m && h[i+1][j]) deg++;
            if (j > 0 && i < n && v[i][j]) deg++;
            if (j < m && i < n && v[i][j+1]) deg++;
            
            if (deg > 0) {
                total_vertices++;
                if (deg == 2 && sx == -1) {
                    sx = i;
                    sy = j;
                }
            }
        }
    }

    if (sx == -1 || total_vertices == 0) return false;

    // Perform DFS from start vertex
    int visited_count = 0;
    stack<pair<int, int>> stk;
    stk.push(make_pair(sx, sy));
    visited[sx][sy] = true;
    visited_count++;

    while (!stk.empty()) {
        int x = stk.top().first;
        int y = stk.top().second;
        stk.pop();

        if (x > 0 && y < m && h[x][y] && !visited[x-1][y]) {
            visited[x-1][y] = true;
            stk.push(make_pair(x-1, y));
            visited_count++;
        }
        if (x < n && y < m && h[x+1][y] && !visited[x+1][y]) {
            visited[x+1][y] = true;
            stk.push(make_pair(x+1, y));
            visited_count++;
        }
        if (y > 0 && x < n && v[x][y] && !visited[x][y-1]) {
            visited[x][y-1] = true;
            stk.push(make_pair(x, y-1));
            visited_count++;
        }
        if (y < m && x < n && v[x][y+1] && !visited[x][y+1]) {
            visited[x][y+1] = true;
            stk.push(make_pair(x, y+1));
            visited_count++;
        }
    }

    return visited_count == total_vertices;
}

bool checkWin() {
    return checkCells() && checkVertices() && singleLoop();
}

void showHelp() {
    setColor(YELLOW);
    cout << "\n  ==========================================\n";
    cout << "              GAME COMMANDS\n";
    cout << "  ==========================================\n";
    setColor(GRAY);
    cout << "  h <row> <col>  - Toggle horizontal line\n";
    cout << "  v <row> <col>  - Toggle vertical line\n";
    cout << "  s              - Check solution\n";
    cout << "  r              - Reset all lines\n";
    cout << "  d              - Display puzzle\n";
    cout << "  show           - Show solution\n";
    cout << "  help           - Show this help\n";
    cout << "  q              - Quit game\n";
    setColor(YELLOW);
    cout << "  ==========================================\n";
    setColor(CYAN);
    cout << "\n  RULES:\n";
    setColor(GRAY);
    cout << "  - Draw a single loop through the grid\n";
    cout << "  - Numbers show how many edges surround them\n";
    cout << "  - Loop cannot cross or branch\n";
    cout << "  - All vertices must have degree 0 or 2\n";
    setColor(YELLOW);
    cout << "  ==========================================\n";
    setColor(GRAY);
}

void generateSimplePuzzle() {
    // Create a simple valid loop first
    memset(solution_h, 0, sizeof(solution_h));
    memset(solution_v, 0, sizeof(solution_v));
    
    // Generate a simple rectangular loop as solution
    for (int j = 0; j < m; j++) {
        solution_h[0][j] = 1;      // Top edge
        solution_h[n][j] = 1;      // Bottom edge
    }
    for (int i = 0; i < n; i++) {
        solution_v[i][0] = 1;      // Left edge
        solution_v[i][m] = 1;      // Right edge
    }
    
    // Generate clues based on solution
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int count = solution_h[i][j] + solution_h[i+1][j] + 
                       solution_v[i][j] + solution_v[i][j+1];
            
            // Place clue with 50% probability
            if (rand() % 100 < 50) {
                grid[i][j] = count;
            } else {
                grid[i][j] = -1;
            }
        }
    }
    
    // Ensure minimum clues (at least 30% of cells)
    int clue_count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] != -1) clue_count++;
        }
    }
    
    int min_clues = (n * m * 3) / 10;
    while (clue_count < min_clues) {
        int i = rand() % n;
        int j = rand() % m;
        if (grid[i][j] == -1) {
            int count = solution_h[i][j] + solution_h[i+1][j] + 
                       solution_v[i][j] + solution_v[i][j+1];
            grid[i][j] = count;
            clue_count++;
        }
    }
}

void loadPredefinedPuzzle(int size) {
    // Initialize solution arrays for predefined puzzles
    memset(solution_h, 0, sizeof(solution_h));
    memset(solution_v, 0, sizeof(solution_v));
    
    if (size == 3) {
        int temp[3][3] = {
            {2, -1, 1},
            {-1, 2, -1},
            {1, -1, 2}
        };
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                grid[i][j] = temp[i][j];
        
        // Create solution for 3x3
        solution_h[0][0]=1; solution_h[0][1]=1; solution_h[0][2]=1;
        solution_h[3][0]=1; solution_h[3][1]=1; solution_h[3][2]=1;
        solution_v[0][0]=1; solution_v[1][0]=1; solution_v[2][0]=1;
        solution_v[0][3]=1; solution_v[1][3]=1; solution_v[2][3]=1;
    } 
    else if (size == 4) {
        int temp[4][4] = {
            {2, 1, -1, 2},
            {-1, 2, 2, -1},
            {2, -1, 2, 2},
            {-1, 2, 1, -1}
        };
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                grid[i][j] = temp[i][j];
        
        // Create solution for 4x4
        for (int j = 0; j < 4; j++) {
            solution_h[0][j] = 1;
            solution_h[4][j] = 1;
        }
        for (int i = 0; i < 4; i++) {
            solution_v[i][0] = 1;
            solution_v[i][4] = 1;
        }
    }
    else if (size == 5) {
        int temp[5][5] = {
            {2, 1, -1, 1, 2},
            {-1, -1, 2, -1, -1},
            {2, 2, -1, 2, 2},
            {-1, -1, 2, -1, -1},
            {2, 1, -1, 1, 2}
        };
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                grid[i][j] = temp[i][j];
        
        // Create solution for 5x5
        for (int j = 0; j < 5; j++) {
            solution_h[0][j] = 1;
            solution_h[5][j] = 1;
        }
        for (int i = 0; i < 5; i++) {
            solution_v[i][0] = 1;
            solution_v[i][5] = 1;
        }
    }
}

void drawAsciiArt(bool solved) {
    if (solved) {
        setColor(GREEN);
        cout << "\n";
        cout << "       _______________\n";
        cout << "      /               \\\n";
        cout << "     |  PUZZLE SOLVED! |\n";
        cout << "      \\_______________/\n";
        cout << "            |   |\n";
        cout << "         ___/   \\___\n";
        cout << "        /           \\\n";
        cout << "       |    LOOP     |\n";
        cout << "       |   COMPLETE  |\n";
        cout << "        \\___________/\n";
        cout << "             ***\n";
        cout << "          EXCELLENT!\n";
        setColor(GRAY);
    } else {
        setColor(RED);
        cout << "\n";
        cout << "         ____________\n";
        cout << "        |            |\n";
        cout << "        |  NOT YET!  |\n";
        cout << "        |____________|\n";
        cout << "             ||\n";
        cout << "            _||_\n";
        cout << "           /    \\\n";
        cout << "          | ???? |\n";
        cout << "           \\____/\n";
        cout << "        KEEP TRYING!\n";
        setColor(GRAY);
    }
}

void showSolution() {
    clearScreen();
    printTitle();
    setColor(YELLOW);
    cout << "\n  ========== SOLUTION ==========\n";
    setColor(GRAY);
    cout << "\n";
    
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < m; j++) {
            setColor(YELLOW);
            cout << "+";
            setColor(GRAY);
            
            if (solution_h[i][j]) {
                setColor(CYAN);
                cout << "---";
                setColor(GRAY);
            } else {
                cout << "   ";
            }
        }
        setColor(YELLOW);
        cout << "+";
        setColor(GRAY);
        cout << "\n";

        if (i < n) {
            for (int j = 0; j <= m; j++) {
                if (solution_v[i][j]) {
                    setColor(CYAN);
                    cout << "|";
                    setColor(GRAY);
                } else {
                    cout << " ";
                }
                
                if (j < m) {
                    if (grid[i][j] == -1) {
                        cout << "   ";
                    } else {
                        setColor(MAGENTA);
                        cout << " " << grid[i][j] << " ";
                        setColor(GRAY);
                    }
                }
            }
            cout << "\n";
        }
    }
    cout << "\n";
}

int main() {
    srand(time(0));
    
    clearScreen();
    printTitle();
    
    int size;
    while (true) {
        setColor(CYAN);
        cout << "\n  Choose grid size (3, 4, or 5): ";
        setColor(GRAY);
        
        if (!(cin >> size)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            setColor(RED);
            cout << "  Invalid input! Please enter a number.\n";
            setColor(GRAY);
            continue;
        }
        
        if (size >= 3 && size <= 5) {
            n = size;
            m = size;
            break;
        } else {
            setColor(RED);
            cout << "  Invalid size! Please enter 3, 4, or 5.\n";
            setColor(GRAY);
        }
    }

    memset(h, 0, sizeof(h));
    memset(v, 0, sizeof(v));
    memset(grid, -1, sizeof(grid));
    
    // Choose puzzle type
    setColor(CYAN);
    cout << "\n  Generate random puzzle? (y/n): ";
    setColor(GRAY);
    char choice;
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        generateSimplePuzzle();
        setColor(GREEN);
        cout << "  Random puzzle generated!\n";
        setColor(GRAY);
    } else {
        loadPredefinedPuzzle(size);
        setColor(GREEN);
        cout << "  Predefined puzzle loaded!\n";
        setColor(GRAY);
    }
    
    showHelp();
    
    string cmd;
    int moves = 0;
    
    while (true) {
        clearScreen();
        printTitle();
        displayPuzzle();
        
        setColor(CYAN);
        cout << "  Moves: " << moves << " | ";
        setColor(GRAY);
        cout << "Command: ";
        
        if (!(cin >> cmd)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (cmd == "q") {
            setColor(YELLOW);
            cout << "\n  Thanks for playing! Goodbye!\n";
            setColor(GRAY);
            break;
        } 
        else if (cmd == "help") {
            showHelp();
            cout << "\n  Press Enter to continue...";
            cin.ignore();
            cin.get();
        } 
        else if (cmd == "r") {
            memset(h, 0, sizeof(h));
            memset(v, 0, sizeof(v));
            moves = 0;
            setColor(GREEN);
            cout << "\n  All lines reset!\n";
            setColor(GRAY);
            cout << "  Press Enter to continue...";
            cin.ignore();
            cin.get();
        } 
        else if (cmd == "s") {
            if (checkWin()) {
                setColor(GREEN);
                cout << "\n  ==========================================\n";
                cout << "       CONGRATULATIONS! PUZZLE SOLVED!\n";
                cout << "  ==========================================\n";
                cout << "  Total moves: " << moves << "\n";
                setColor(GRAY);
                drawAsciiArt(true);
                cout << "\n  Press Enter to continue...";
                cin.ignore();
                cin.get();
            } else {
                setColor(RED);
                cout << "\n  ==========================================\n";
                cout << "           NOT A VALID SOLUTION\n";
                cout << "  ==========================================\n";
                setColor(GRAY);
                cout << "  Issues found:\n";
                if (!checkCells()) {
                    setColor(RED);
                    cout << "  [X] ";
                    setColor(GRAY);
                    cout << "Some cells don't match their numbers\n";
                }
                if (!checkVertices()) {
                    setColor(RED);
                    cout << "  [X] ";
                    setColor(GRAY);
                    cout << "Some vertices have wrong degree\n";
                }
                if (!singleLoop()) {
                    setColor(RED);
                    cout << "  [X] ";
                    setColor(GRAY);
                    cout << "Not a single continuous loop\n";
                }
                setColor(RED);
                cout << "  ==========================================\n";
                setColor(GRAY);
                drawAsciiArt(false);
                cout << "\n  Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
        } 
        else if (cmd == "d") {
            // Display is already shown, just continue
            continue;
        }
        else if (cmd == "show") {
            showSolution();
            cout << "  Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
        else if (cmd == "h") {
            int r, c;
            if (!(cin >> r >> c)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                setColor(RED);
                cout << "\n  Invalid input! Use: h <row> <col>\n";
                setColor(GRAY);
                cout << "  Press Enter to continue...";
                cin.get();
                continue;
            }
            
            if (r >= 0 && r <= n && c >= 0 && c < m) {
                h[r][c] ^= 1;
                moves++;
                setColor(GREEN);
                cout << "\n  Horizontal line toggled at (" << r << "," << c << ")\n";
                setColor(GRAY);
            } else {
                setColor(RED);
                cout << "\n  Invalid coordinates! Row: 0-" << n << ", Col: 0-" << (m-1) << "\n";
                setColor(GRAY);
                cout << "  Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
        } 
        else if (cmd == "v") {
            int r, c;
            if (!(cin >> r >> c)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                setColor(RED);
                cout << "\n  Invalid input! Use: v <row> <col>\n";
                setColor(GRAY);
                cout << "  Press Enter to continue...";
                cin.get();
                continue;
            }
            
            if (r >= 0 && r < n && c >= 0 && c <= m) {
                v[r][c] ^= 1;
                moves++;
                setColor(GREEN);
                cout << "\n  Vertical line toggled at (" << r << "," << c << ")\n";
                setColor(GRAY);
            } else {
                setColor(RED);
                cout << "\n  Invalid coordinates! Row: 0-" << (n-1) << ", Col: 0-" << m << "\n";
                setColor(GRAY);
                cout << "  Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
        } 
        else {
            setColor(RED);
            cout << "\n  Unknown command! Type 'help' for available commands.\n";
            setColor(GRAY);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Press Enter to continue...";
            cin.get();
        }
    }
    
    return 0;
}