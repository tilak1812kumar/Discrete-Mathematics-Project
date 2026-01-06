#include <bits/stdc++.h>
using namespace std;

int N;

// true = BLACK, false = WHITE
bool cell[10][10];

// -1 = no number
int grid[10][10];

// ---------------- DRAW FULL BOX GRID ----------------
void displayGrid() {
    cout << "\n";

    for (int i = 0; i < N; i++) {

        // top border of row
        for (int j = 0; j < N; j++)
            cout << "+---";
        cout << "+\n";

        // cell content
        for (int j = 0; j < N; j++) {
            cout << "|";
            if (cell[i][j])
                cout << " # ";
            else if (grid[i][j] > 0)
                cout << " " << grid[i][j] << " ";
            else
                cout << " . ";
        }
        cout << "|\n";
    }

    // bottom border
    for (int j = 0; j < N; j++)
        cout << "+---";
    cout << "+\n\n";
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

// ---------------- WIN ----------------
bool checkWin() {
    return no2x2Black() && blackConnected() && checkIslands();
}

// ---------------- LOAD SAMPLE PUZZLE ----------------
void loadPuzzle() {
    memset(cell, false, sizeof(cell));
    memset(grid, -1, sizeof(grid));

    // simple demo puzzle (works for any N >= 5)
    if (N >= 5) {
        grid[0][0] = 3;
        grid[0][3] = 2;
        grid[2][1] = 1;
        grid[2][4] = 3;
        grid[4][0] = 2;
        grid[4][4] = 1;
    }
}

// ---------------- MAIN ----------------
int main() {
    cout << "=========== NURIKABE PUZZLE ===========\n";
    cout << "Enter grid size: ";
    cin >> N;

    if (N < 3 || N > 10) {
        cout << "Grid size must be between 3 and 10\n";
        return 0;
    }

    loadPuzzle();

    cout << "\nCommands:\n";
    cout << " b r c  -> mark BLACK cell\n";
    cout << " w r c  -> mark WHITE cell\n";
    cout << " s      -> check solution\n";
    cout << " r      -> reset\n";
    cout << " q      -> quit\n";

    string cmd;
    while (true) {
        displayGrid();
        cout << "Command: ";
        cin >> cmd;

        if (cmd == "q") break;

        else if (cmd == "r")
            memset(cell, false, sizeof(cell));

        else if (cmd == "b") {
            int r,c; cin >> r >> c;
            if (r>=0 && r<N && c>=0 && c<N)
                cell[r][c] = true;
        }

        else if (cmd == "w") {
            int r,c; cin >> r >> c;
            if (r>=0 && r<N && c>=0 && c<N)
                cell[r][c] = false;
        }

        else if (cmd == "s") {
            if (checkWin())
                cout << "\n PUZZLE SOLVED!\n";
            else
                cout << "\n NOT SOLVED\n";
        }
    }
    return 0;
}
