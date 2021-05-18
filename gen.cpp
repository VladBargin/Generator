#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <array>

const char nl = '\n';

using namespace std;

mt19937 rng;

int n, m, max_dir;

bool gen_seg(int lleft, vector<int> &ans, vector<int> &shv) {
    if (lleft == 0) {
        return true;
    }
    
    shuffle(shv.begin(), shv.end(), rng);
    for (int x: shv) {
        if (x <= lleft) {
            ans.push_back(x);
            if (gen_seg(lleft - x, ans, shv)) {
                return true;
            }
            ans.pop_back();
        }
    } 
    
    return false;
} 

int dir_diff;
string generate_path(int pathl, int mi_segl, int ma_segl, vector<pair<int, int>> &vpath) {
    vector<int> shv;
    for (int i = mi_segl; i <= ma_segl; i++) shv.push_back(i);
    
    vector<int> ans;
    if (!gen_seg(pathl, ans, shv)) return "";
    
    shuffle(ans.begin(), ans.end(), rng);
    
    int cdir = rng() % 4;
    string s = "";
    for (int len: ans) {
        char c = 'N';
        if (cdir == 1) c = 'E';
        else if (cdir == 2) c = 'S';
        else if (cdir == 3) c = 'W';
        
        s += c;
        s += to_string(len);
        
        vpath.emplace_back(cdir, len);
        
        int ndir = cdir;
        while (cdir == ndir || abs(cdir - ndir) == dir_diff) ndir = rng() % 4;
        cdir = ndir;
    }
    
    return s;
}

bool legal(int i, int j) {
    return i >= 0 && j >= 0 && i < n && j < m;
}

pair<int, int> check(int i, int j, int dr, vector<pair<int, int>> &path, vector<vector<bool>> &board) {
    if (board[i][j]) return {-1, -1};
    int ci = i, cj = j;
    
    const vector<pair<int, int>> td = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (auto p: path) {
        int k = (p.first + dr) % 4;
        for (int _ = 0; _ < p.second; _++) {
            ci += td[k].first; cj += td[k].second;
            if (!legal(ci, cj) || board[ci][cj]) return {-1, -1};
        }
    }
    return {ci, cj};
}

bool generate(vector<vector<bool>> &board, vector<pair<int, int>> &path, set<array<int, 5>> &solutions, int cnt_sol) {
    vector<pair<int, int>> pos;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            pos.emplace_back(i, j);
            for (int dr = 0; dr < max_dir; dr++) {
                auto r = check(i, j, dr, path, board);                
                if (r.first >= 0) {
                    solutions.insert({i, j, r.first, r.second, dr});
                }
            }
        }
    }
    
    shuffle(pos.begin(), pos.end(), rng);
    
    for (auto p: pos) {
        if ((int)(solutions.size()) <= cnt_sol) break;
        board[p.first][p.second] = true;
        
        
        for (auto it = solutions.begin(); it != solutions.end(); ) {
            if (check((*it)[0], (*it)[1], (*it)[4], path, board).first < 0) {
                it = solutions.erase(it);
            }
            else ++it;
        }

    }
    return (int)(solutions.size()) == cnt_sol;
}

int main() {
    rng = mt19937(chrono::steady_clock::now().time_since_epoch().count());
//    rng = mt19937(0);
    ios::sync_with_stdio(0); cin.tie(0);
    
    int pathl, mi_segl, ma_segl, cnt_sol, allow_turn, num_sets;
    cin >> n >> m >> pathl >> mi_segl >> ma_segl >> cnt_sol >> max_dir >> allow_turn >> num_sets;
    if (allow_turn) dir_diff = -1;
    else dir_diff = 2;
    
    
    mi_segl = max(mi_segl, 1);
    ma_segl = min(ma_segl, max(n, m));
    ma_segl = max(ma_segl, mi_segl);
    
        
    while (num_sets--) {
        // Try to generate something 1000 times
        for (int _ = 0; _ < 10000; _++) {
            vector<vector<bool>> board(n, vector<bool> (m));   
            
            // A solution is just the start (i; j), end (i, j) and the direction
            set<array<int, 5>> solutions;
            vector<pair<int, int>> vpath;
            string path = generate_path(pathl, mi_segl, ma_segl, vpath);

            // This means that it is impossible to generate any path that satisfies the contraints
            if ((int)(path.size()) == 0) {
                cout << "-1" << nl;
                return 0;
            }

            if (generate(board, vpath, solutions, cnt_sol)) {
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < m; j++) {
                        //if (board[i][j]) cout << "*";
                        //else cout << ".";
                        cout << board[i][j] << ' ';
                    }
                    cout << nl;
                }
                cout << path << nl;
                for (auto sol: solutions) {
                    for (int c: sol) cout << c << ' ';
                    cout << nl;
                }
                break;
            }
        }
    }
    cout << "-1" << nl;
}
