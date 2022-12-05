//int check_liberties_player_next_to(game_t *game_info, int boardIndex);
//void restore_char_after_moving(game_t *game_info, int boardIndex);

#include "stos.h"


struct game_t;
void clear_board(game_t* game_info);
void print_row_indexes(game_t* game_info, int left_to_board);
void print_colum_indexes(int y, char last_column_index, int x);
void clear_message();
void writte_message(char s[]);
void initialize_board_with_new_size(game_t* game_info, int new_size);
//void initialize_board_with_new_size(game_t* game_info);
void print_board(game_t* game_info, int x_offset);
void print_horizontal_line(int x);
void print_char_from_board(game_t* game_info, int y, int x, int x_offset);
void show_manual();
void show_coordinates(game_t* game_info);
void show_stats(game_t* game_info);
void show_legend();
void initial_game_state(game_t* game_info);
void column_indexing(game_t* game_info, int x);
void row_indexing(game_t* game_info);
int get_input_from_user(char string[]);
int get_board_size_from_user();
void open_saved_game(game_t* game_info);
void show_custom_board_menu(game_t* game_info);
void draw_stone(game_t* game_info);
void save_game(game_t* game_info);
void start_new_game(game_t* game_info);
void cancel_placing_stone(game_t* game_info);
void confirm_placing_stone(game_t* game_info);
void add_points_to_player(game_t* game_info, int points);
void swap_current_player(game_t* game_info);
void check_pressed_buttton(game_t* game_info);
void check_if_position_is_empty(game_t* game_info);
void try_to_place_stone(game_t* game_info);
void move_stone_on_board(game_t* game_info);
void save_char_under(game_t* game_info);
void check_for_left_scroll(game_t* game_info);
void check_for_right_scroll(game_t* game_info);
void introduce_handicap(game_t* game_info);
int is_board_empty(game_t* game_info);
void clear_liberties_and_stones_group(game_t* game_info);
void unmark_board(game_t* game_info);
void show_select_board_size_menu(game_t* game_info);
void refresh_view(game_t* game_info, int x, int y);
void restore_board_state(game_t* game_info);
void print_board_column(game_t* game_info, int col_x);
void swap_current_player(game_t* game_info);
void get_pressed_button(game_t& game_info);
void print_char_from_board(game_t* game_info, int y, int x, int x_offset);
void go_line_below(int x);
void save_char_under(game_t* game_info);
void finish_game(game_t* game_info);
void check_dead_stones(game_t* game_info, int player);

