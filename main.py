import random
import time


class ReversiGame:

    def __init__(self):
        self.size = 8
        self.board = [[0 for _ in range(self.size)] for _ in range(self.size)]
        self.board[3][3] = 1
        self.board[4][4] = 1
        self.board[3][4] = -1
        self.board[4][3] = -1
        self.directions = [[1, 0], [1, 1], [0, 1], [-1, 1], [-1, 0], [-1, -1], [0, -1], [1, -1]]

    def set_board(self, board):
        self.board = board

    def set_random(self):
        for i in range(self.size):
            for j in range(self.size):
                self.board[i][j] = random.choice([1, 0, -1])

    def create_unit_test(self, turn):
        self.set_random()
        self.print_board()
        moves = self.gen_moves(turn)
        print(moves)
        print((self.board, moves, turn))

    def print_board(self):
        for i in range(self.size):
            for j in range(self.size):
                print("+---", end='')
            print("+")
            for j in range(self.size):
                print("|", self.board[i][j], end='')
                if self.board[i][j] != -1:
                    print(" ", end='')
            print("|")
        for j in range(self.size):
            print("+---", end='')
        print("+")

    def gen_moves(self, turn):
        frontier = []
        frontier_directions = {}
        moves = []
        for i in range(self.size):
            for j in range(self.size):
                if self.board[i][j] == turn:
                    for z in range(8):
                        if 0 <= i+self.directions[z][0] < self.size and 0 <= j+self.directions[z][1] < self.size:
                            if self.board[i+self.directions[z][0]][j+self.directions[z][1]] == -turn:
                                potential = (i+self.directions[z][0], j+self.directions[z][1])
                                if potential not in frontier:
                                    frontier.append(potential)
                                    frontier_directions[potential] = [z]
                                else:
                                    frontier_directions[potential].append(z)

        while frontier:
            curr = frontier.pop(0)
            curr_directions = frontier_directions.pop(curr)

            for dir_index in curr_directions:
                neighbour_x = curr[0] + self.directions[dir_index][0]
                neighbour_y = curr[1] + self.directions[dir_index][1]
                if 0 <= neighbour_x < self.size and 0 <= neighbour_y < self.size:
                    if self.board[neighbour_x][neighbour_y] == 0:
                        if (neighbour_x, neighbour_y) not in moves:
                            moves.append((neighbour_x, neighbour_y))
                    elif self.board[neighbour_x][neighbour_y] == -turn:
                        if (neighbour_x, neighbour_y) in frontier:
                            opp_direction = (dir_index + 4) % 8
                            if opp_direction in frontier_directions[(neighbour_x, neighbour_y)]:
                                frontier_directions[(neighbour_x, neighbour_y)].remove(opp_direction)
                                if not frontier_directions[(neighbour_x, neighbour_y)]:
                                    frontier.remove((neighbour_x, neighbour_y))
                                    frontier_directions.pop((neighbour_x, neighbour_y))
                            else:
                                frontier_directions[(neighbour_x, neighbour_y)].append(dir_index)
                        else:
                            frontier.append((neighbour_x, neighbour_y))
                            frontier_directions[(neighbour_x, neighbour_y)] = [dir_index]
        return moves

    def perform_move(self, move, turn):
        valid_directions = []
        if 0 > move[0] or move[0] >= self.size or move[1] < 0 or move[1] >= self.size:
            return
        if self.board[move[0]][move[1]] != 0:
            return

        for curr_direction in range(8):
            possible = True
            valid = False
            new_x = move[0]
            new_y = move[1]
            while possible:
                new_x += self.directions[curr_direction][0]
                new_y += self.directions[curr_direction][1]

                if 0 <= new_x < self.size and 0 <= new_y < self.size:
                    if self.board[new_x][new_y] == -turn:
                        valid = True
                    elif self.board[new_x][new_y] == turn:
                        possible = False
                    else:
                        possible = valid = False
                else:
                    possible = valid = False

            if valid:
                valid_directions.append(curr_direction)

        if len(valid_directions) > 0:
            self.board[move[0]][move[1]] = turn

        for curr_direction in valid_directions:
            replacing = True
            new_x = move[0]
            new_y = move[1]
            while replacing:
                new_x += self.directions[curr_direction][0]
                new_y += self.directions[curr_direction][1]

                if 0 <= new_x < self.size and 0 <= new_y < self.size:
                    if self.board[new_x][new_y] == -turn:
                        self.board[new_x][new_y] = turn
                    else:
                        replacing = False
                else:
                    replacing = False

    def play(self):
        turn = 1
        while len(self.gen_moves(turn)) > 0:
            self.print_board()
            print("Player ", turn, "'s turn, please enter a move: ", end='')
            move1 = input()
            move2 = input()
            move = (int(move1), int(move2))
            self.perform_move(move, turn)
            turn = -turn


def generate_moves_unit_tests():
    test_game = ReversiGame()
    tests = [
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 1, -1, 0, 0, 0],
          [0, 0, 0, -1, 1, 0, 0, 0], [1, -1, 0, 0, 0, 0, 0, 0], [0, -1, 0, 0, 0, 0, 0, 0], [1, 0, 0, 0, 0, 0, 0, 0]],
         [(2, 4), (4, 2), (3, 5), (5, 2), (5, 3), (7, 2)], 1),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 1, -1, 0, 0, 0],
          [0, 0, 0, -1, 1, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         [(2, 4), (4, 2), (3, 5), (5, 3)], 1),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, 0, 0, 0, 0, 0], [1, 1, 1, 1, -1, 0, 0, 0],
          [-1, -1, -1, -1, 1, 0, 0, 0], [1, -1, 1, 0, 0, 0, 0, 0], [0, -1, 1, 0, 0, 0, 0, 0], [1, 0, 0, 0, 0, 0, 0, 0]],
         [(1, 0), (1, 1), (1, 2), (2, 4), (3, 5), (5, 3), (5, 4), (6, 0), (7, 1), (7, 2), (2, 5)], 1),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, 0, 0, 0, 0, 0], [1, 1, 1, 1, -1, 0, 0, 0],
          [-1, -1, -1, -1, 1, 0, 0, 0], [1, -1, 1, 0, 0, 0, 0, 0], [0, -1, 1, 0, 0, 0, 0, 0], [1, 0, 0, 0, 0, 0, 0, 0]],
         [(2, 0), (2, 2), (2, 3), (2, 4), (4, 5), (5, 3), (5, 4), (6, 0), (6, 3), (7, 2), (7, 3)], -1),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 1, -1, 0, 0, 0],
          [0, 0, 0, -1, 1, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         [(3, 2), (2, 3), (5, 4), (4, 5)], -1),
        ([[1 for _ in range(8)] for _ in range(8)], [], 1),
        ([[1 for _ in range(8)] for _ in range(8)], [], -1),
        ([[-1 for _ in range(8)] for _ in range(8)], [], 1),
        ([[-1 for _ in range(8)] for _ in range(8)], [], -1),
        ([[0 for _ in range(8)] for _ in range(8)], [], 1),
        ([[1, -1, 1, -1, 1, -1, 1, -1] for _ in range(8)], [], 1),
        ([[1, -1, -1, -1, -1, -1, 1, -1] for _ in range(8)], [], 1),
        ([[1, 0, -1, 0, 1, 0, 0, 0], [0, 0, -1, 0, 0, 0, -1, 0], [0, 0, 0, 0, 0, 1, 0, 0], [0, 0, 0, 1, -1, 0, 0, 0],
          [0, 0, 0, -1, 1, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         [(0, 7), (2, 4), (4, 2), (3, 5), (5, 3), (5, 2)], 1),
        ([[1, 0, -1, 0, 1, 0, 0, 0], [0, 0, -1, 0, 0, 0, -1, 0], [0, 0, 0, -1, 0, 1, 0, 0], [0, 0, -1, 1, -1, 0, 0, 0],
          [0, 0, 0, -1, 1, -1, 0, 0], [0, 0, 0, 0, -1, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         [], -1),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         [(0, 0), (0, 3), (0, 6), (3, 0), (3, 7), (6, 0), (7, 3), (7, 7)], 1)

    ]
    test_number = 1
    for test in tests:
        test_game.set_board(test[0])

        if test_game.gen_moves(test[2]).sort() == test[1].sort():
            print("Generating moves unit test ", test_number, " passed")
        else:
            print("Generating moves unit test ", test_number, " failed")
            print(test_game.gen_moves(test[2]))
        test_number += 1


def perform_moves_unit_tests():
    test_game = ReversiGame()
    tests = [
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 1, -1, 0, 0, 0],
          [0, 0, 0, -1, 1, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (3, 5), 1,
         [[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 1, 1, 1, 0, 0],
          [0, 0, 0, -1, 1, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (0, 0), 1,
         [[1, 0, 0, 0, 0, 0, 0, 0], [0, 1, 0, -1, 0, -1, 0, 0], [0, 0, 1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (0, 3), 1,
         [[0, 0, 0, 1, 0, 0, 0, 0], [0, -1, 0, 1, 0, -1, 0, 0], [0, 0, -1, 1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (0, 6), 1,
         [[0, 0, 0, 0, 0, 0, 1, 0], [0, -1, 0, -1, 0, 1, 0, 0], [0, 0, -1, -1, 1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (3, 0), 1,
         [[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [1, 1, 1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (3, 7), 1,
         [[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, 1, 1, 1, 1],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (6, 0), 1,
         [[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, 1, -1, -1, 0, 0, 0], [0, 1, 0, -1, 0, -1, 0, 0], [1, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (7, 3), 1,
         [[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, 1, -1, 0, 0, 0], [0, -1, 0, 1, 0, -1, 0, 0], [0, 0, 0, 1, 0, 0, -1, 0], [0, 0, 0, 1, 0, 0, 0, 0]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, 0, -1, 0, 0, -1, 0], [0, 0, 0, 0, 0, 0, 0, 0]],
         (7, 7), 1,
         [[0, 0, 0, 0, 0, 0, 0, 0], [0, -1, 0, -1, 0, -1, 0, 0], [0, 0, -1, -1, -1, 0, 0, 0], [0, -1, -1, 1, -1, -1, -1, 0],
          [0, 0, -1, -1, 1, 0, 0, 0], [0, -1, 0, -1, 0, 1, 0, 0], [0, 0, 0, -1, 0, 0, 1, 0], [0, 0, 0, 0, 0, 0, 0, 1]]),
        ([[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 1, -1, 0, 0, 0],
          [0, 0, 0, -1, 1, 0, 0, 0], [1, -1, 0, 0, 0, 0, 0, 0], [0, -1, 0, 0, 0, 0, 0, 0], [1, 0, 0, 0, 0, 0, 0, 0]],
         (5, 2), 1,
         [[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 1, -1, 0, 0, 0],
          [0, 0, 0, -1, 1, 0, 0, 0], [1, 1, 1, 0, 0, 0, 0, 0], [0, 1, 0, 0, 0, 0, 0, 0], [1, 0, 0, 0, 0, 0, 0, 0]])
    ]

    test_number = 1
    for test in tests:
        test_game.set_board(test[0])
        test_game.perform_move(test[1], test[2])

        if test_game.board == test[3]:
            print("Performing moves unit test ", test_number, " passed")
        else:
            print("Performing moves unit test ", test_number, " failed")
            test_game.print_board()
        test_number += 1


def test_speed():
    start = time.time()
    count = 0
    while time.time() - start < 1:
        board = ReversiGame()
        moves = board.gen_moves(1)
        turn = 1
        while moves:
            board.perform_move(next(iter(moves)), turn)
            turn = -turn
            moves = board.gen_moves(turn)
            count += 1
    return count


generate_moves_unit_tests()
perform_moves_unit_tests()
n = 10
t = 0
for i in range(n):
    t += test_speed()
    print(100/n * (i+1), "% ")
print(t / n)
