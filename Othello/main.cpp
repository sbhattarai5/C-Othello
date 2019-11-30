#include <iostream>

const int SPACE = ' ';
const int WHITE = 'O';
const int BLACK = '@';
const int R = 4;
const int C = 4;

void init_board(char board[][C])
{
    for (int i = 0; i < R; ++i)
    {
        for (int j = 0; j < C; ++j)
        {
            board[i][j] = SPACE;
        }
    }
    int mid_row = R / 2;
    int mid_col = C / 2;
    board[mid_row - 1][mid_col - 1] = WHITE;
    board[mid_row][mid_col - 1] = BLACK;
    board[mid_row][mid_col] = WHITE;
    board[mid_row - 1][mid_col] = BLACK;
    return;
}

void print_board(char board[][C])
{
    std::cout << std::endl;
    // col name
    std::cout << "  ";
    char col_name = 'A';
    for (int i = 0; i < C; ++i)
    {
        std::cout << col_name;
        col_name += 1;
    }
    std::cout << std::endl;
    
    // +- printing
    std::cout << " +";
    for (int i = 0; i < C; ++i)
    {
        std::cout << '-';
    }
    std::cout << "+\n";

    // print board

    int row_name = 1;
    for (int i = 0; i < R; ++i)
    {
        std::cout << row_name << '|';
        for (int j = 0; j < C; ++j)
        {
            std::cout << board[i][j];
        }
        std::cout << '|' << row_name << std::endl;
        row_name += 1;
    }
    
    // +- printing
    std::cout << " +";
    for (int i = 0; i < C; ++i)
    {
        std::cout << '-';
    }
    std::cout << "+\n";

    // col name
    std::cout << "  ";
    col_name = 'A';
    for (int i = 0; i < C; ++i)
    {
        std::cout << col_name;
        col_name += 1;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    return;
}

bool capturable(char board[][C], int i, int j, char turn, int dx, int dy)
{
    char op_piece = (turn == BLACK ? WHITE : BLACK);
    bool op_piece_found = false;
    if (board[i][j] != ' ') return false;
    i += dx;
    j += dy;
    while (i >= 0 && i < R && j >= 0 && j < C)
    {
        if (board[i][j] == ' ') return false;
        if (board[i][j] == op_piece) op_piece_found = true;
        if (board[i][j] == turn && op_piece_found) return true;
        if (board[i][j] == turn) return false;
        i += dx;
        j += dy;
    }
    return false;    
}

void capture(char board[][C], int r, int c, int turn, int dx, int dy)
{
    while (r >= 0 && r < R && c >= 0 && c < C)
    {
        if (board[r][c] == turn) return;
        board[r][c] = turn;
        r += dx;
        c += dy;
    }
    return;
}

bool checkij(char board[][C], int i, int j, char turn)
{
    return (capturable(board, i, j, turn, 0, 1) ||
            capturable(board, i, j, turn, -1, 1) ||
            capturable(board, i, j, turn, -1, 0) ||
            capturable(board, i, j, turn, -1, -1) ||
            capturable(board, i, j, turn, 0, -1) ||
            capturable(board, i, j, turn, 1, -1) ||
            capturable(board, i, j, turn, 1, 0) ||
            capturable(board, i, j, turn, 1, 1));
}

void check_available_moves(char board[][C], char & turn, int available_rows[], int available_cols[], int & available_len, bool & last_turn_passed, bool & game_over)
{
    available_len = 0;
    for (int i = 0; i < R; ++i)
    {
        for (int j = 0; j < C; ++j)
        {
            bool available = checkij(board, i, j, turn);
            if (available)
            {
                available_rows[available_len] = i;
                available_cols[available_len] = j;
                available_len++;
            }
        }
    }
    if (available_len == 0)
    {
        turn = (turn == BLACK ? WHITE : BLACK);
        std::cout << "Turn passed\n";
        if (last_turn_passed) game_over = true;
        else last_turn_passed = true;
    }
    return;
}

void take_input(char input[], char turn)
{
    if (turn == BLACK) std::cout << "BLACK'S turn: ";
    else std::cout << "WHITE's turn: ";
    std::cin >> input;
    return;
}

void make_move(char input[], char board[][C], char & turn, bool & last_turn_passed, bool & illegal_move_made)
{
    char c = input[0];
    char r = input[1];
    bool capture_made = false;
    int row = r - '0' - 1;
    if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
    int col = c - 'A';
    if (row < 0 || row >= R || col < 0 || col > C || !checkij(board, row, col, turn))
    {
        std::cout << "Illegal move!\n";
        illegal_move_made = true;
        return;
    }

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if ((i != 0 || j != 0) && capturable(board, row, col, turn, i, j))
            {
                capture(board, row, col, turn, i, j);
                capture_made = true;
            }
        }
    }
    
    if (!capture_made)
    {
        std::cout << "Illegal move!\n";
        illegal_move_made = true;
    }
    else
    {
        turn = (turn == BLACK ? WHITE : BLACK);
        last_turn_passed = false;
        illegal_move_made = false; 
    }
    return;
}

void check_game_over(char board[][C], bool & game_over)
{
    for (int i = 0; i < R; ++i)
    {
        for (int j = 0; j < C; ++j)
        {
            if (board[i][j] == SPACE) return;
        }
    }
    game_over = true;
    return;
}

void check_winner(char board[][C], char & winner)
{
    int black_count = 0;
    int white_count = 0;
    for (int i = 0; i < R; ++i)
    {
        for (int j = 0; j < C; ++j)
        {
            if (board[i][j] == BLACK) black_count++;
            if (board[i][j] == WHITE) white_count++;
        }
    }
    std::cout << "Black pieces: " << black_count << '\n';
    std::cout << "White pieces: " << white_count << '\n';
    if (black_count > white_count) winner = BLACK;
    if (white_count > black_count) winner = WHITE;
    return;
}

int main()
{
    //initialization
    char board[R][C];
    char input[3];
    char turn = BLACK;
    char winner = SPACE;
    bool game_over = false;
    bool last_turn_passed = false;
    bool illegal_move_made = false;
    int available_rows[1024];
    int available_cols[1024];
    int available_len = 0;
    
    init_board(board);
    print_board(board);

    while (!game_over)
    {
        if (!illegal_move_made) check_available_moves(board, turn, available_rows, available_cols, available_len, last_turn_passed, game_over);
        if (available_len != 0)
        {
            take_input(input, turn);
            make_move(input, board, turn, last_turn_passed, illegal_move_made);
            check_game_over(board, game_over);
        }
        print_board(board);
    }

    check_winner(board, winner);
    if (winner == SPACE) std::cout << "DRAW!!\n";
    if (winner == BLACK) std::cout << "BLACK WINS!\n";
    if (winner == WHITE) std::cout << "WHITE WINS!\n";
    return 0;
}
