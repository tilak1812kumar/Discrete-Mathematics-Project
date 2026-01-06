#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

int n;
char grid[5][5];
char solution[5][5];

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

// Function declarations
bool inBounds(int x, int y);
void drawGrid();
bool canPlaceBulb(int x, int y, char g[5][5]);
bool checkNumberedCells(char g[5][5]);
bool allLit(char g[5][5]);
bool noBulbConflict(char g[5][5]);
bool solve(char g[5][5], int idx = 0);
void generatePuzzle();
void userPlay();

int main() {
    srand(time(0));
    cout << "Enter grid size (3, 4, 5 only): ";
    cin >> n;
    if (n != 3 && n != 4 && n != 5) {
        cout << "Invalid input! Only 3, 4, or 5 allowed.\n";
        return 0;
    }

    generatePuzzle();
    cout << "\nGenerated Puzzle:\n";
    drawGrid();
    cout << "\nNow place your bulbs!\n";
    userPlay();
    return 0;
}

// Check if inside grid
bool inBounds(int x, int y) {
    return x >= 0 && y >= 0 && x < n && y < n;
}

// Draw grid
void drawGrid() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << "+---";
        cout << "+\n";
        for (int j = 0; j < n; j++)
            cout << "| " << grid[i][j] << " ";
        cout << "|\n";
    }
    for (int j = 0; j < n; j++)
        cout << "+---";
    cout << "+\n";
}

// Can place bulb at x,y
bool canPlaceBulb(int x, int y, char g[5][5]) {
    if (g[x][y] != '.')
        return false;

    for (int d = 0; d < 4; d++) {
        int nx = x, ny = y;
        while (true) {
            nx += dx[d];
            ny += dy[d];
            if (!inBounds(nx, ny)) break;
            if (isdigit(g[nx][ny]) || g[nx][ny] == '#') break;
            if (g[nx][ny] == 'B') return false;
        }
    }
    return true;
}

// Check numbered cells constraints
bool checkNumberedCells(char g[5][5]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (isdigit(g[i][j])) {
                int required = g[i][j] - '0';
                int count = 0;
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (inBounds(ni, nj) && g[ni][nj] == 'B') count++;
                }
                if (count > required) return false;
            }
    return true;
}

// Check if all empty cells are lit
bool allLit(char g[5][5]) {
    bool lit[5][5] = {false};
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j] == 'B') {
                lit[i][j] = true;
                for (int d = 0; d < 4; d++) {
                    int x = i, y = j;
                    while (true) {
                        x += dx[d]; y += dy[d];
                        if (!inBounds(x, y)) break;
                        if (isdigit(g[x][y]) || g[x][y] == '#') break;
                        lit[x][y] = true;
                    }
                }
            }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j] == '.' && !lit[i][j])
                return false;
    return true;
}

// No two bulbs in line of sight
bool noBulbConflict(char g[5][5]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j] == 'B') {
                for (int y = j-1; y>=0; y--) { if (isdigit(g[i][y])||g[i][y]=='#') break; if(g[i][y]=='B') return false; }
                for (int y = j+1; y<n; y++) { if (isdigit(g[i][y])||g[i][y]=='#') break; if(g[i][y]=='B') return false; }
                for (int x = i-1; x>=0; x--) { if (isdigit(g[x][j])||g[x][j]=='#') break; if(g[x][j]=='B') return false; }
                for (int x = i+1; x<n; x++) { if (isdigit(g[x][j])||g[x][j]=='#') break; if(g[x][j]=='B') return false; }
            }
    return true;
}

// Solve recursively
bool solve(char g[5][5], int idx) {
    if (idx == n*n) return checkNumberedCells(g) && allLit(g);

    int x = idx / n, y = idx % n;
    if (g[x][y] == '.') {
        if (canPlaceBulb(x,y,g)) {
            g[x][y]='B';
            if (checkNumberedCells(g) && solve(g, idx+1)) return true;
            g[x][y]='.';
        }
        if (solve(g, idx+1)) return true;
    } else return solve(g, idx+1);

    return false;
}

// Generate puzzle with numbered and blocked cells
void generatePuzzle() {
    int attempts=0;
    while (attempts<50) {
        attempts++;
        for (int i=0;i<n;i++) for(int j=0;j<n;j++) grid[i][j]='.';

        // Place at least 2 blocked cells
        int blockedCount = 2 + rand()%((n==3?2:(n==4?3:4)));
        int placed=0;
        while(placed<blockedCount){
            int x=rand()%n, y=rand()%n;
            if(grid[x][y]=='.'){ grid[x][y]='#'; placed++; }
        }

        // Copy to solution
        for (int i=0;i<n;i++) for(int j=0;j<n;j++) solution[i][j]=grid[i][j];

        if(!solve(solution)) continue;

        // Convert some blocked cells to numbered cells
        int numberedCount=0;
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if(grid[i][j]=='#') {
                    int count=0;
                    for(int d=0;d<4;d++){
                        int ni=i+dx[d], nj=j+dy[d];
                        if(inBounds(ni,nj) && solution[ni][nj]=='B') count++;
                    }
                    if(count>0){ grid[i][j]='0'+count; numberedCount++; }
                }

        int pureBlocked=0;
        for(int i=0;i<n;i++) for(int j=0;j<n;j++) if(grid[i][j]=='#') pureBlocked++;

        if(pureBlocked>=2 && numberedCount>=2) return;
    }

    // Default pattern if generation fails
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) grid[i][j]='.';
    if(n==3){ grid[0][0]='#'; grid[0][2]='#'; grid[1][1]='2'; }
    if(n==4){ grid[0][0]='#'; grid[0][3]='#'; grid[1][1]='2'; grid[2][2]='1'; }
    if(n==5){ grid[0][0]='#'; grid[0][4]='#'; grid[1][1]='2'; grid[3][3]='3'; grid[4][2]='#'; }

    for(int i=0;i<n;i++) for(int j=0;j<n;j++) solution[i][j]=grid[i][j];
    solve(solution);
}

// User interaction
void userPlay() {
    int x,y;
    while(true){
        drawGrid();
        cout << "Enter bulb coordinates (row col) or -1 -1 to finish: ";
        cin >> x >> y;
        if(x==-1 && y==-1) break;
        if(!inBounds(x,y)){ cout<<"Invalid coordinates!\n"; continue; }
        if(grid[x][y]!='.'){ cout<<"Cannot place bulb here!\n"; continue; }
        grid[x][y]='B';
    }
    drawGrid();
    if(checkNumberedCells(grid) && allLit(grid) && noBulbConflict(grid))
        cout<<"Congratulations! Puzzle solved correctly!\n";
    else
        cout<<"Puzzle not solved correctly. Check bulbs row/column conflicts or illumination!\n";
}
