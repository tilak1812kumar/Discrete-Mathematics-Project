#include <bits/stdc++.h>
using namespace std;

int N;

// grid numbers (islands)
int grid[7][7];

// bridges
int bh[7][7]; // horizontal (to right)
int bv[7][7]; // vertical (down)

// ---------------- DISPLAY ----------------
void display() {
    cout << "\n";
    for (int i = 0; i < N; i++) {

        // numbers + horizontal bridges
        for (int j = 0; j < N; j++) {
            if (grid[i][j] > 0)
                cout << " " << grid[i][j] << " ";
            else
                cout << " . ";

            if (bh[i][j] == 1) cout << "-";
            else if (bh[i][j] == 2) cout << "=";
            else cout << " ";
        }
        cout << "\n";

        // vertical bridges
        for (int j = 0; j < N; j++) {
            if (bv[i][j] == 1) cout << " |  ";
            else if (bv[i][j] == 2) cout << " || ";
            else cout << "    ";
        }
        cout << "\n";
    }
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

// ---------------- MAIN ----------------
int main() {
    cout << "=========== HASHI (BRIDGES) PUZZLE ===========\n";

    while (true) {
        cout << "Choose grid size (5 / 6 / 7): ";
        cin >> N;
        if (N == 5 || N == 6 || N == 7) break;
        cout << "Invalid size!\n";
    }

    loadPuzzle(N);

    cout << "\nCommands:\n";
    cout << " h r c  -> toggle horizontal bridge\n";
    cout << " v r c  -> toggle vertical bridge\n";
    cout << " s      -> check solution\n";
    cout << " r      -> reset\n";
    cout << " q      -> quit\n";

    string cmd;
    while (true) {
        display();
        cout << "Command: ";
        cin >> cmd;

        if (cmd == "q") break;

        else if (cmd == "r") {
            memset(bh, 0, sizeof(bh));
            memset(bv, 0, sizeof(bv));
        }

        else if (cmd == "h") {
            int r,c; cin >> r >> c;
            if (r>=0 && r<N && c>=0 && c<N)
                bh[r][c] = (bh[r][c] + 1) % 3;
        }

        else if (cmd == "v") {
            int r,c; cin >> r >> c;
            if (r>=0 && r<N && c>=0 && c<N)
                bv[r][c] = (bv[r][c] + 1) % 3;
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
