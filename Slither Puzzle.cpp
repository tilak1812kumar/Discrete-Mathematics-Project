#include <bits/stdc++.h>
using namespace std;

int n, m;
int grid[5][5];       // cell numbers, -1 = empty (max 5x5)
int h[6][5];          // horizontal edges (n+1 x m)
int v[5][6];          // vertical edges (n x m+1)

// ---------------- DISPLAY ----------------
void displayPuzzle() {
    cout << "\n";
    for (int i = 0; i <= n; i++) {
        // Top edges of row
        for (int j = 0; j < m; j++) {
            cout << "+";
            if (h[i][j]) {
                cout << "---";
            } else {
                cout << "   ";
            }
        }
        cout << "+\n";

        if (i < n) {
            // Vertical edges and numbers
            for (int j = 0; j <= m; j++) {
                if (v[i][j]) {
                    cout << "|";
                } else {
                    cout << " ";
                }
                
                if (j < m) {
                    // Print cell number
                    if (grid[i][j] == -1) {
                        cout << "   ";
                    } else {
                        // Center the number (0-3)
                        if (grid[i][j] == 0) {
                            cout << " 0 ";
                        } else if (grid[i][j] == 1) {
                            cout << " 1 ";
                        } else if (grid[i][j] == 2) {
                            cout << " 2 ";
                        } else if (grid[i][j] == 3) {
                            cout << " 3 ";
                        } else {
                            cout << " ? ";
                        }
                    }
                }
            }
            cout << "\n";
        }
    }
    cout << endl;
}

// ---------------- CELL CHECK ----------------
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

// ---------------- VERTEX CHECK ----------------
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

// ---------------- SINGLE LOOP CHECK ----------------
bool singleLoop() {
    vector<vector<bool>> visited(n+1, vector<bool>(m+1, false));
    int sx = -1, sy = -1;
    int total_vertices = 0;

    // find start vertex and count vertices in loop
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

    // No loop at all
    if (sx == -1) return false;

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

        // Check all possible moves
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

    // Check if all vertices are visited
    return visited_count == total_vertices;
}

// ---------------- WIN CHECK ----------------
bool checkWin() {
    return checkCells() && checkVertices() && singleLoop();
}

// ---------------- HELP ----------------
void showHelp() {
    cout << "______________________________________________________________\n";
    cout<<"                                                                 \n";
    cout << "SLITHERLINK COMMANDS:\n";
    cout << "______________________________________________________________\n";
    cout << "h r c      Toggle horizontal line between rows r and r+1 at column c\n";
    cout << "v r c      Toggle vertical line between columns c and c+1 at row r\n";
    cout << "s          Check solution\n";
    cout << "r          Reset all lines\n";
    cout << "d          Display puzzle\n";
    cout << "help       Show this help\n";
    cout << "q          Quit game\n";
    cout << "______________________________________________________________\n";
    cout<<"                                                                 \n";
    cout << "Note: Coordinates are 0-based. For a 3x3 grid:\n";
    cout << "  - h lines: r=0..3, c=0..2\n";
    cout << "  - v lines: r=0..2, c=0..3\n";
    cout << "______________________________________________________________\n";
}

// ---------------- LOAD DEFAULT PUZZLES ----------------
void loadPuzzle(int size) {
    memset(grid, -1, sizeof(grid));
    
    if (size == 3) {
        // 3x3 puzzle
        int temp[3][3] = {
            {2, -1, 1},
            {-1, 2, -1},
            {1, -1, 2}
        };
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                grid[i][j] = temp[i][j];
        cout << "\n3x3 puzzle loaded!\n";
    } 
    else if (size == 4) {
        // 4x4 puzzle
        int temp[4][4] = {
            {2, 1, 3, -1},
            {-1, 2, -1, 1},
            {1, -1, 2, -1},
            {-1, 3, -1, 2}
        };
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                grid[i][j] = temp[i][j];
        cout << "\n4x4 puzzle loaded!\n";
    }
    else if (size == 5) {
        // 5x5 puzzle
        int temp[5][5] = {
            {2, 1, 3, 3, 3},
            {2, 0, 3, 1, -1},
            {3, 0, -1, 2, 3},
            {0, -1, 0, -1, 1},
            {2, 1, 3, 3, -1}
        };
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                grid[i][j] = temp[i][j];
        cout << "\n5x5 puzzle loaded!\n";
    }
}

// ---------------- MAIN ----------------
int main() {
    cout << "______________________________________________________________\n";
    cout<<"                                                                 \n";
    cout << "                     SLITHERLINK PUZZLE GAME\n";
    cout << "______________________________________________________________\n";
    
    // Choose grid size
    int size;
    while (true) {
        cout << "\nChoose grid size (3, 4, or 5): ";
        cin >> size;
        
        if (size == 3 || size == 4 || size == 5) {
            n = size;
            m = size;
            break;
        } else {
            cout << "Invalid size! Please enter 3, 4, or 5.\n";
        }
    }

    // Initialize arrays
    memset(h, 0, sizeof(h));
    memset(v, 0, sizeof(v));
    memset(grid, -1, sizeof(grid));
    
    // Load puzzle based on size
    loadPuzzle(size);
    
    showHelp();
    
    string cmd;
    while (true) {
        displayPuzzle();
        cout << "Command: ";
        cin >> cmd;

        if (cmd == "q") {
            cout << "\nThanks for playing! Goodbye!\n";
            break;
        } else if (cmd == "help") {
            showHelp();
        } else if (cmd == "r") {
            memset(h, 0, sizeof(h));
            memset(v, 0, sizeof(v));
            cout << "\n✓ All lines reset!\n";
        } else if (cmd == "s") {
            if (checkWin()) {
                cout << "______________________________________________________________\n";
                cout << "               PUZZLE SOLVED CORRECTLY! \n";
                cout << "______________________________________________________________\n";
            } else {
                cout << "______________________________________________________________\n";
                cout << "                      NOT A VALID SOLUTION\n";
                cout << "______________________________________________________________\n";
                cout << "Issues found:\n";
                if (!checkCells()) cout << "   Some cells don't match their numbers\n";
                if (!checkVertices()) cout << "   Some vertices have wrong degree (not 0 or 2)\n";
                if (!singleLoop()) cout << "   Not a single continuous loop\n";
                cout << "______________________________________________________________\n";
            }
        } else if (cmd == "d") {
            // Just display (already shown)
            continue;
        } else if (cmd == "h") {
            int r, c;
            cin >> r >> c;
            if (r >= 0 && r <= n && c >= 0 && c < m) {
                h[r][c] ^= 1;
                cout << " Horizontal line at (" << r << "," << c << ") toggled\n";
            } else {
                cout << " Invalid coordinates!\n";
            }
        } else if (cmd == "v") {
            int r, c;
            cin >> r >> c;
            if (r >= 0 && r < n && c >= 0 && c <= m) {
                v[r][c] ^= 1;
                cout << " Vertical line at (" << r << "," << c << ") toggled\n";
            } else {
                cout << " Invalid coordinates!\n";
            }
        } else {
            cout << " Unknown command. Type 'help' for available commands.\n";
            // Clear input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    return 0;
}