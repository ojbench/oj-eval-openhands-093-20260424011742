#include <iostream>
#include <vector>
#include <set>
#include <string>
using namespace std;

struct State {
    int x, y, dir;  // position and direction
    vector<vector<int>> grid;  // current grid state
    int score;
    
    bool operator<(const State& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        if (dir != other.dir) return dir < other.dir;
        if (score != other.score) return score < other.score;
        return grid < other.grid;
    }
};

// Directions: 0=up, 1=right, 2=down, 3=left
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

// Turn directions: right, straight, left, back
int turn_priority[] = {1, 0, 3, 2};  // right, straight, left, back

bool isValid(int x, int y, const vector<vector<int>>& grid) {
    return x >= 0 && x < 10 && y >= 0 && y < 10 && grid[x][y] != 0;
}

int getNewDirection(int current_dir, int turn) {
    if (turn == 0) return current_dir;  // straight
    if (turn == 1) return (current_dir + 1) % 4;  // right
    if (turn == 2) return (current_dir + 2) % 4;  // back
    if (turn == 3) return (current_dir + 3) % 4;  // left
    return current_dir;
}

int main() {
    vector<vector<int>> grid(10, vector<int>(10));
    int start_x = -1, start_y = -1;
    
    // Read input
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 4) {
                start_x = i;
                start_y = j;
                grid[i][j] = 1;  // Convert start position to empty space
            }
        }
    }
    
    // Initialize state
    State current;
    current.x = start_x;
    current.y = start_y;
    current.dir = 0;  // Start moving up
    current.grid = grid;
    current.score = 0;
    
    set<State> visited;
    visited.insert(current);
    
    while (true) {
        // Try all possible moves according to right-hand rule
        bool moved = false;
        
        for (int i = 0; i < 4; i++) {
            int turn = turn_priority[i];
            int new_dir = getNewDirection(current.dir, turn);
            int new_x = current.x + dx[new_dir];
            int new_y = current.y + dy[new_dir];
            
            if (isValid(new_x, new_y, current.grid)) {
                // Check if we hit a ghost
                if (current.grid[new_x][new_y] == 3) {
                    current.score -= 500;
                    cout << current.score << endl;
                    return 0;
                }
                
                // Move to new position
                current.x = new_x;
                current.y = new_y;
                current.dir = new_dir;
                
                // Check if we ate a bean
                if (current.grid[new_x][new_y] == 2) {
                    current.score += 2;
                    current.grid[new_x][new_y] = 1;  // Remove bean
                }
                
                moved = true;
                break;
            }
        }
        
        if (!moved) {
            // No valid moves, game ends
            cout << current.score << endl;
            return 0;
        }
        
        // Check for loop
        if (visited.count(current)) {
            cout << "Silly Pacman" << endl;
            return 0;
        }
        
        visited.insert(current);
    }
    
    return 0;
}