#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <utility>
#include <array>
#include <chrono>
#include <time.h>
#include <unordered_map>
#include<bits/stdc++.h>
using namespace std;

class ReversiGame{
    private:
        int size = 8;
        array<array<int, 8>, 8> board = {{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,1,-1,0,0,0},{0,0,0,-1,1,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}}};
        int directions[8][2] = {{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};

    public:
        void reset(){
            board = {{{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,1,-1,0,0,0},{0,0,0,-1,1,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}}};
        }

        void play(){
            vector<pair<int,int>> player_1_moves;
            gen_moves(player_1_moves, 1);
            vector<pair<int,int>> player_2_moves;
            gen_moves(player_2_moves, -1);
            int turn = 1;
            int x;
            int y;
            while(player_1_moves.size() > 0 || player_2_moves.size() > 0){
                print_board();
                cout << "Player " << turn << "'s move: ";
                cin >> x >> y;
                perform_move({x,y},turn);
                turn = -turn;
            }

            int score1 = 0;
            int score2 = 0;
            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    if (board[i][j] == 1){
                        score1 ++;
                    }else if(board[i][j] == -1){
                        score2 ++;
                    }
                }
            }
            if (score1 > score2){
                cout << "Player 1 wins with " << score1 << " points";
            }else if (score1 < score2){
                cout << "Player 2 wins with " << score2 << " points";
            }else{
                cout << "Draw";
            }
            
        }

        bool find(vector<pair<int,int>> &q, const pair<int,int> &value){
            vector<pair<int,int>>::iterator itr;
            itr = std::find(q.begin(), q.end(), value);
            if (itr != q.end()){
                return true;
            }
            return false;
        }

        bool find(vector<int> &q, const int &value){
            vector<int>::iterator itr;
            itr = std::find(q.begin(), q.end(), value);
            if (itr != q.end()){
                return true;
            }
            return false;
        }

        void print_co_ords(vector<pair<int,int>> q){
            for (auto it = begin(q); it != end(q); ++it){
                cout << "(" <<(*it).first << ", " << (*it).second << "), ";
            }
            cout << "\n";
        }

        void print_board(){
            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    cout << "+--";
                }
                cout << "+ \n";
                for (int j = 0; j < size; j++){
                    cout << "|";
                    if (board[i][j] != -1){
                        cout << " ";
                    }
                    cout << board[i][j];
                }
                cout << "| \n";
            }
            for (int j = 0; j < size; j++){
                    cout << "+--";
                }
                cout << "+ \n";
        }

        void set_board(array<array<int, 8>, 8> temp){
            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    board[i][j] = temp[i][j];
                }
            }
        }

        bool compare_boards(array<array<int,8>,8> &comparison){
            for (int i = 0; i < 8; i++){
                for (int j =0; j < 8; j++){
                    if (board[i][j] != comparison[i][j]){
                        return false;
                    }
                }
            }
            return true;
        }

        void gen_moves(vector<pair<int,int>> &moves, const int &turn){
            vector<pair<int, int>> frontier;
            unordered_map<int, vector<int>> frontier_directions;
            //vector<pair<int,int>> moves;

            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    if (board[i][j] == turn){
                        for (int z = 0; z < 8; z++){
                            if (0 <= i + directions[z][0] && i + directions[z][0] < size && 0 <= j + directions[z][1] && j + directions[z][1] < size){
                                if (board[i+directions[z][0]][j+directions[z][1]] == -turn){
                                    pair<int, int> potential = {i + directions[z][0], j + directions[z][1]};
                                    vector<int> dir = {z};
                                    if (!find(frontier, potential)){
                                        frontier.push_back(potential);
                                        frontier_directions[potential.first * 8 + potential.second] = dir;
                                    }else{
                                        frontier_directions[potential.first * 8 + potential.second].push_back(z);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            while (!frontier.empty()){
                pair<int,int> curr = frontier.back();
                int curr_index = curr.first * 8 + curr.second;
                frontier.pop_back();

                vector<int> curr_directions = frontier_directions[curr_index];
                frontier_directions[curr_index] = {};

                for (auto it = begin(curr_directions); it != end(curr_directions); ++it){
                    int dir_index = *it;
                    int neighbour_x = curr.first + directions[dir_index][0];
                    int neighbour_y = curr.second + directions[dir_index][1];

                    if (0 <= neighbour_x && neighbour_x < size && 0 <= neighbour_y && neighbour_y< size){
                        pair<int,int> neighbour_pos = {neighbour_x, neighbour_y};
                        if (board[neighbour_x][neighbour_y] == 0){
                            if (!find(moves, neighbour_pos)){
                                moves.push_back(neighbour_pos);
                            }
                        }else if(board[neighbour_x][neighbour_y] == -turn){
                            int neighbour_index = neighbour_x * 8 + neighbour_y;
                            if (find(frontier,neighbour_pos)){
                                int opp_direction = (dir_index + 4) % 8;
                                auto it = std::find(frontier_directions[neighbour_index].begin(), frontier_directions[neighbour_index].end(), opp_direction);
                                if (it != frontier_directions[neighbour_index].end()){
                                    frontier_directions[neighbour_index].erase(it);
                                    if(frontier_directions[neighbour_index].empty()){
                                       auto it = std::find(frontier.begin(), frontier.end(), neighbour_pos);
                                       if (it != frontier.end()){
                                            frontier.erase(it);
                                        }
                                    }
                                }else{
                                    frontier_directions[neighbour_index].push_back(dir_index);
                                }
                            }else{
                                frontier.push_back(neighbour_pos);
                                frontier_directions[neighbour_index] = {dir_index};
                            }
                        }
                    }
                }

            }
        }

        void perform_move(const pair<int,int> &move, const int &turn){
            deque<int> valid_directions;
            if (0 > move.first || move.first >= size || move.second < 0 || move.second >= size){
                return;
            }
            if (board[move.first][move.second] != 0){
                return;
            }

            for (int curr_direction = 0; curr_direction < 8; curr_direction++){
                bool possible = true;
                bool valid = false;
                
                int new_x = move.first;
                int new_y = move.second;
                while (possible == true){
                    new_x += directions[curr_direction][0];
                    new_y += directions[curr_direction][1];
                    possible = false;

                    if (0 <= new_x && new_x < size && 0 <= new_y && new_y < size){
                        if (board[new_x][new_y]  == -turn){
                            valid = true;
                            possible = true;
                        }else if (board[new_x][new_y] == 0){
                            valid = false;
                        }
                    }
                }

                if (valid){
                    valid_directions.push_back(curr_direction);
                    board[move.first][move.second] = turn;
                }

                while(!valid_directions.empty()){
                    int curr_direction = valid_directions.front();
                    valid_directions.pop_front();
                    
                    bool replacing = true;
                    int new_x = move.first;
                    int new_y = move.second;

                    while (replacing){
                        new_x += directions[curr_direction][0];
                        new_y += directions[curr_direction][1];

                        if (0 <= new_x && new_x < size && 0 <= new_y && new_y < size){
                            if (board[new_x][new_y] == -turn){
                                board[new_x][new_y] = turn;
                            }else{
                                replacing = false;
                            }
                        }else{
                            replacing = false;
                        }
                    }
                }
            }
        }

};

void perform_generate_unit_tests(){
    vector<array<array<int, 8>, 8>> test_boards = {
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, -1, 0, 0, 0}, {0, 0, 0, -1, 1, 0, 0, 0}, {1, -1, 0, 0, 0, 0, 0, 0}, {0, -1, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, -1, 0, 0, 0},{0, 0, 0, -1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, -1, 0, 0, 0},{-1, -1, -1, -1, 1, 0, 0, 0}, {1, -1, 1, 0, 0, 0, 0, 0}, {0, -1, 1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, -1, 0, 0, 0},{-1, -1, -1, -1, 1, 0, 0, 0}, {1, -1, 1, 0, 0, 0, 0, 0}, {0, -1, 1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, -1, 0, 0, 0}, {0, 0, 0, -1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{1, 0, -1, 0, 1, 0, 0, 0}, {0, 0, -1, 0, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 1, -1, 0, 0, 0}, {0, 0, 0, -1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{1, 0, -1, 0, 1, 0, 0, 0}, {0, 0, -1, 0, 0, 0, -1, 0}, {0, 0, 0, -1, 0, 1, 0, 0}, {0, 0, -1, 1, -1, 0, 0, 0}, {0, 0, 0, -1, 1, -1, 0, 0}, {0, 0, 0, 0, -1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}}
    };
    vector<vector<pair<int,int>>> answers = {
        {{2,4},{4,2},{3,5},{5,2},{5,3},{7,2}},
        {{2, 4}, {4, 2}, {3, 5}, {5, 3}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 4}, {3, 5}, {5, 3}, {5, 4}, {6, 0}, {7, 1}, {7, 2}, {2, 5}},
        {{2, 0}, {2, 2}, {2, 3}, {2, 4}, {4, 5}, {5, 3}, {5, 4}, {6, 0}, {6, 3}, {7, 2}, {7, 3}},
        {{3, 2}, {2, 3}, {5, 4}, {4, 5}},
        {{0, 7}, {2, 4}, {4, 2}, {3, 5}, {5, 3}, {5, 2}},
        {},
        {{0, 0}, {0, 3}, {0, 6}, {3, 0}, {3, 7}, {6, 0}, {7, 3}, {7, 7}}
    };
    vector<int> turns = {1, 1, 1, -1, -1, 1, -1, 1};
    int num_tests = test_boards.size();
    vector<pair<int,int>> ans;
    ReversiGame test_game;
    for (int i = 1; i <= num_tests; i++){
        test_game.set_board(test_boards[i-1]);
        test_game.gen_moves(ans, turns[i-1]);
        bool passed = true;
        if (ans.size() != answers[i-1].size()){
            passed = false;
        }
        for (auto it = begin(answers[i-1]); it != end(answers[i-1]); ++it){
            if (!test_game.find(ans, *it)){
                passed = false;
            }  
        }
        if (passed){
            cout << "Generate move unit test number " << i << " passed! \n";
        }else{
            cout << "Generate move unit test number " << i << " failed \n";
            test_game.print_co_ords(ans);
        }
        ans.clear();
    }
}

void perform_perform_move_unit_tests(){
    vector<array<array<int,8>,8>> test_boards= {
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, -1, 0, 0, 0}, {0, 0, 0, -1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, -1, 0, 0, 0}, {0, 0, 0, -1, 1, 0, 0, 0}, {1, -1, 0, 0, 0, 0, 0, 0}, {0, -1, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0}}}
    };
    vector<array<array<int,8>,8>> ans_boards ={
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 1, 1, 0, 0}, {0, 0, 0, -1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{1, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, -1, 0, -1, 0, 0}, {0, 0, 1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 1, 0, 0, 0, 0}, {0, -1, 0, 1, 0, -1, 0, 0}, {0, 0, -1, 1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 1, 0}, {0, -1, 0, -1, 0, 1, 0, 0}, {0, 0, -1, -1, 1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {1, 1, 1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, 1, 1, 1, 1}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, 1, -1, -1, 0, 0, 0}, {0, 1, 0, -1, 0, -1, 0, 0}, {1, 0, 0, -1, 0, 0, -1, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, 1, -1, 0, 0, 0}, {0, -1, 0, 1, 0, -1, 0, 0}, {0, 0, 0, 1, 0, 0, -1, 0}, {0, 0, 0, 1, 0, 0, 0, 0}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, -1, 0, -1, 0, -1, 0, 0}, {0, 0, -1, -1, -1, 0, 0, 0}, {0, -1, -1, 1, -1, -1, -1, 0}, {0, 0, -1, -1, 1, 0, 0, 0}, {0, -1, 0, -1, 0, 1, 0, 0}, {0, 0, 0, -1, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0, 1}}},
        {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, -1, 0, 0, 0}, {0, 0, 0, -1, 1, 0, 0, 0}, {1, 1, 1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0}}}
    };
    vector<pair<int,int>> moves = {{3,5},{0,0},{0,3},{0,6},{3,0},{3,7},{6,0},{7,3},{7,7},{5,2}};
    vector<int> turns = {1,1,1,1,1,1,1,1,1,1};

    int num_tests = test_boards.size();
    ReversiGame test_game;
    for (int i = 1; i <= num_tests; i++){
        test_game.set_board(test_boards[i-1]);
        test_game.perform_move(moves[i-1],turns[i-1]);
        
        if (test_game.compare_boards(ans_boards[i-1])){
            cout << "Perfom move unit test number " << i << " passed! \n";
        }else{
            cout << "Perform move unit test number " << i << " failed \n";
            test_game.print_board();
        }
    }

}

int test_speed(){
    auto start = clock();
    //auto t2 = std::chrono::high_resolution_clock::now();
    int count = 0;
    vector<pair<int,int>> moves;
    ReversiGame game;
    while ((clock() - start)/ CLOCKS_PER_SEC < 1){
        game.reset();
        int turn = 1;
        game.gen_moves(moves, turn);
        while (moves.size() > 0){
            game.perform_move(moves[0], turn);
            turn = -turn;
            moves.clear();
            game.gen_moves(moves, turn);
            count++;
        }
    }
    return count;
}

void test_time(int n){
    auto start = clock();
    int count = 0;
    vector<pair<int,int>> moves;
    while (count < n){
        ReversiGame game;
        int turn = 1;
        game.gen_moves(moves, turn);
        while (moves.size() > 0){
            game.perform_move(moves[0], turn);
            turn = -turn;
            moves.clear();
            game.gen_moves(moves, turn);
            count++;
        }
    }
    float time = (clock() - start) / CLOCKS_PER_SEC;
    cout << time << "\n";
}

int main(){
    ReversiGame game;

    perform_generate_unit_tests();
    perform_perform_move_unit_tests();
    int total = 0;
    int n = 10;
    for (int i = 0; i < n; i++){
        total += test_speed();
    }
    cout << total / n << "\n";
    

    // int x; 
    // ReversiGame game2;
    game.play();
    return 0;
}