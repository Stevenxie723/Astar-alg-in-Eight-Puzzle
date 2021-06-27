#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <array>
#include <fstream>
#include <ctime>
double rate = 0.05;

const int goalRow[9] = {-1, 0, 0, 0, 1, 1, 1, 2, 2};
const int goalCol[9] = {-1, 0, 1, 2, 0, 1, 2, 0, 1};
const int dx[4] = {0, 0, 1, -1};
const int dy[4] = {1, -1, 0, 0};
using namespace std;
map<array<array<int, 3>, 3>, int> database1;

char int_to_dir(int dir) {
    switch(dir) {
        case 0:
            return 'r';
        case 1:
            return 'l';
        case 2:
            return 'd';
        case 3:
            return 'u';
        default:
            return '\0';
    }
}

class State{
    public:
        array<array<int, 3>, 3> board;
        int ex, ey;
        double heuristic;
        int currentMove;
        int lastMove;
        string path;
        State(int arr[3][3]) : currentMove(0), lastMove(-1){
            for(int i=0;i<3;i++)
                for(int j=0;j<3;j++) {
                    if(arr[i][j] == 0) {
                        ex = i; ey = j;
                    }
                    board[i][j] = arr[i][j];
                }
            path.clear();
            setHeuristic();
        }
        void setHeuristic(void) {
            heuristic = database1[board] * rate;
            return;
        }
        bool operator<(const State& rhs) const {
            return currentMove + heuristic > rhs.currentMove + rhs.heuristic;
        }
        void Move(int dir, int newx, int newy) {
            path.push_back(int_to_dir(dir));
            lastMove = dir;
            swap(board[ex][ey], board[newx][newy]);
            ex = newx; ey = newy;
            setHeuristic();
            currentMove++;
            return;
        }
};

bool solvable(const State& now) {
    int arr[9], cnt = 0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            arr[3*i + j] = now.board[i][j];
    for(int i=0;i<9;i++) {
        if(arr[i] == 0) continue;
        for(int j=i+1;j<9;j++) {
            if(arr[j] == 0) continue;
            if(arr[i] > arr[j]) cnt++;
        }
    }
    if(cnt % 2) return false;
    else return true;
}
int main(int argc, char *argv[]) {
    freopen("input.txt", "r", stdin);
    int arr[3][3];
    int t;
    char temp;
    array<array<int, 3>, 3> datain;
    fstream data("data.txt", ios::in);
    for(int i=0;i<362880;i++) {
        for(int j=0;j<3;j++)
        for(int k=0;k<3;k++) {
            data >> datain[j][k];
        }
        int ans;
        data >> ans;
        database1[datain] = ans;
    }
    data.close();
    for(rate = 0.05; rate <= 1.05 && cin >> t; rate += 0.05)
    while(t--) {
        for(int i=0;i<3;i++) {
            for(int j=0;j<3;j++) {
                cin >> temp;
                if(temp >= '0' && temp <= '9') arr[i][j] = temp - '0';
                else arr[i][j] = 0;
            }
        }
        State initial(arr);
        priority_queue<State> candidate;
        bool solve = false;
        candidate.push(initial);
        if(!solvable(initial)) {
            cout << "unsolvable\n";
            continue;
        }
        int cnt = 0;
        while(!candidate.empty()) {
            cnt++;
            State now = candidate.top();
            candidate.pop();
            if(now.heuristic == 0) {
                cout << now.currentMove << endl;
                solve = true;
                break;
            }
            if(now.currentMove + now.heuristic > 31) continue;
            for(int i=0;i<4;i++) {
                int newx = now.ex + dx[i], newy = now.ey + dy[i];
                if(newx >= 3 || newx < 0 || newy >= 3 || newy < 0) continue;
                if(now.lastMove == (i^1)) continue;

                State next = now;
                next.Move(i, newx, newy);
                candidate.push(next);
            }
        }
        if(!solve) { 
            cout << "unsolvable\n"; 
        }
        cout << "Accuracy = " << rate << ", The number of passing loops = " << cnt << endl;
        rewind(stdin);
    }
    return 0;
}