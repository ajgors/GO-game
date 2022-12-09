#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <math.h>
#include "conio2.h"
#include "main.h"
#include "vector.h"

#define BOARD_X 70
#define BOARD_Y 6
#define LEGEND_X 10
#define LEGEND_Y 5
#define STATS_X 10
#define STATS_Y 19
#define MANUAL_X 10
#define MANUAL_Y 9
#define COORDINATES_X 10
#define COORDINATES_Y 20
#define MENU_X 40
#define MENU_Y 7
#define MESSAGE_X 50
#define MESSAGE_Y 2
#define EMPTY 0
#define BLACK_PLAYER 1
#define WHITE_PLAYER 2
#define MARKER 4
#define MARKED_BLACK_PLAYER 5
#define MARKED_WHITE_PLAYER 6
#define LIBERTY 8
#define ZNAK_ENTER 0x0d
#define ZNAK_ESC 0x1b
#define DOWN_ARROW 0x50
#define UP_ARROW 0x48
#define LEFT_ARROW 0x4b
#define RIGHT_ARROW 0x4d
#define ZNAK_BACKSPACE 0x08
#define NEXT_Y wherey() + 1
#define ENABLED 1
#define DISABLED 0
#define CURRENT_STONE_COLOR LIGHTCYAN
#define STONES_COLOR LIGHTGRAY
#define BOARD_COLOR DARKGRAY
#define UPPER_LEFT_CORNER_CHAR 218
#define UPPER_RIGHT_CORNER_CHAR 191
#define BOTTOM_LEFT_CORNER_CHAR 192
#define BOTTOM_RIGHT_CORNER_CHAR 217
#define CROSS_CHAR 197
#define LINE_CHAR 179
#define VERTICAL_WITH_HORIZONTAL_RIGHT_CHAR 195
#define HORIZONAL_WITH_VERTICAL_BOTTOM 194
#define VERTICAL_WITH_HORIZONTAL_LEFT_CHAR 180
#define HORIZONTAL_WITH_VERTICAL_UP_CHAR 193
#define HORIZONATL_LINE_CHAR 196
#define RIGHT_BORDER game_info->board_size - 1
#define BOTTOM_BORDER game_info->board_size - 1
#define TOP_BORDER 0
#define LEFT_BORDER 0
#define SECOND_PLAYER (game_info->current_player == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER)
#define INSERT_MODE_CHAR 'i'
#define NEW_GAME_CHAR 'n'
#define REFRESH_CHAR 'r'
#define SAVE_CHAR 's'
#define LOAD_SAVE_CHAR 'l'
#define HANDICAP_MODE_CHAR 'h'
#define QUIT_GAME_CHAR 'q'

typedef struct {
	coordinates point;
	int counter;
} ko_fight;


struct game_t {
	vector_t liberties{};
	vector_t stones_group{};
	int board_size = 0;
	char** board = 0;
	int current_player = BLACK_PLAYER;
	int pressed_button = 0;
	int insert_mode = DISABLED;
	int handicap_mode = DISABLED;
	int handicap_mode_used = false;
	int board_shift = 0;

	struct {
		int black_player = 0;
		int white_player = 0;
	}score;

	struct {
		int x_relative_to_screen = BOARD_X;
		int y_relative_to_screen = BOARD_Y;
		int x = 0;
		int y = 0;
	} coords;

	struct {
		ko_fight ko_fights[5] = { 0 };
		int ko_fight_mode = DISABLED;
	}ko;

	char char_under_stone[8] = { (char)UPPER_LEFT_CORNER_CHAR };
};


int main() {

	struct game_t game_info;

	init_vector(&(game_info.stones_group));
	init_vector(&(game_info.liberties));
	show_select_board_size_menu(&game_info);

#ifndef __cplusplus
	Conio2_Init();
#endif

	settitle("Igor Stadnicki");		//title of screen
	_setcursortype(_NOCURSOR);      //hide cursor
	clrscr();						
	show_manual();					
	show_stats(&game_info);			
	row_indexing(&game_info);		
	column_indexing(&game_info, 0);	
	show_legend();					
	show_coordinates(&game_info);	
	print_board(&game_info, 0);	

	do {
		show_coordinates(&game_info); //updates coordinates 

		draw_stone(&game_info);		  

		get_pressed_button(game_info);

		check_pressed_buttton(&game_info);

	} while (game_info.pressed_button != QUIT_GAME_CHAR);

	clrscr();
	return 0;
}


//gets which button was pressed
void get_pressed_button(game_t& game_info) {
	game_info.pressed_button = getch();
}


//check which button was pressed
void check_pressed_buttton(game_t* game_info) {
	//when arrows are pressed 2 results are shown the first one is 0
	if (game_info->pressed_button == 0 && game_info->insert_mode == DISABLED) {
		clear_message();
		move_stone_on_board(game_info);
	}
	else if (game_info->pressed_button == INSERT_MODE_CHAR && game_info->insert_mode == DISABLED) {
		check_if_position_is_empty(game_info);
	}
	else if (game_info->pressed_button == ZNAK_ENTER && game_info->insert_mode == ENABLED) { 
		confirm_placing_stone(game_info);
	}
	else if (game_info->pressed_button == ZNAK_ESC) {
		clear_message();
		cancel_placing_stone(game_info);
	}
	else if (game_info->pressed_button == NEW_GAME_CHAR) { 
		start_new_game(game_info);
	}
	else if (game_info->pressed_button == REFRESH_CHAR) {
		refresh_view(game_info);
	}
	else if (game_info->pressed_button == SAVE_CHAR) {
		save_game(game_info);
	}
	else if (game_info->pressed_button == LOAD_SAVE_CHAR) {
		show_enter_save_menu(game_info);
	}
	else if (game_info->pressed_button == HANDICAP_MODE_CHAR) {
		introduce_handicap(game_info);
	}

}


//saves liberties, and stones group coordinates
void count_liberties(game_t* game_info, int y, int x, int player_color) {
	if (x < 0 || x >= game_info->board_size || y < 0 || y >= game_info->board_size) return;
	int current_stone = game_info->board[y][x];

	if (current_stone == player_color) {
		push_back(&(game_info->stones_group), { x,y });
		game_info->board[y][x] += MARKER;
		count_liberties(game_info, y - 1, x, player_color);
		count_liberties(game_info, y, x - 1, player_color);
		count_liberties(game_info, y + 1, x, player_color);
		count_liberties(game_info, y, x + 1, player_color);
	}
	else if (current_stone == EMPTY) {
		game_info->board[y][x] = LIBERTY;
		push_back(&(game_info->liberties), { x,y });
	}
}


//removes captured stones from board
int remove_captured(game_t* game_info) {
	if (game_info->liberties.count == 0) {
		for (int i = 0; i < game_info->stones_group.count; i++) {
			game_info->board[game_info->stones_group.ptr[i].y][game_info->stones_group.ptr[i].x] = EMPTY;
		}
		return game_info->stones_group.count;
	}
	return 0;
}


//scanns board for stones to be captured, when found removes them from board
int capture(game_t* game_info, int player_to_capture) {
	restore_board_state(game_info);
	int points = 0;
	for (int y = 0; y < game_info->board_size; y++) {
		for (int x = 0; x < game_info->board_size; x++) {
			int piece = game_info->board[y][x];
			if (piece == player_to_capture) {
				count_liberties(game_info, y, x, player_to_capture);
				points += remove_captured(game_info);
				restore_board_state(game_info);
			}
		}
	}
	return points;
}


//unmarks board and clears liberties, group stones arrays
void restore_board_state(game_t* game_info) {
	unmark_board(game_info);
	clear_liberties_and_stones_group(game_info);
}


//unmarks stones board
void unmark_board(game_t* game_info) {
	for (int y = 0; y < game_info->board_size; y++) {
		for (int x = 0; x < game_info->board_size; x++) {
			if (game_info->board[y][x] == MARKED_BLACK_PLAYER) {
				game_info->board[y][x] = BLACK_PLAYER;
			}
			else if (game_info->board[y][x] == MARKED_WHITE_PLAYER) {
				game_info->board[y][x] = WHITE_PLAYER;
			}
			else if (game_info->board[y][x] == LIBERTY) {
				game_info->board[y][x] = EMPTY;
			}
		}
	}
}


//empties liberties and stones group vector
void clear_liberties_and_stones_group(game_t* game_info) {
	free(game_info->liberties.ptr);
	free(game_info->stones_group.ptr);
	init_vector(&(game_info->liberties));
	init_vector(&(game_info->stones_group));
}


//check if player next to has liberties, when not returns his coordinates, when has liberties returns {-1,-1}
coordinates check_liberties_player_next_to(game_t* game_info) {
	restore_board_state(game_info);
	int x = game_info->coords.x;
	int y = game_info->coords.y;
	int right = x + 1;
	if (right < game_info->board_size) {
		if (game_info->board[y][right] == SECOND_PLAYER) {
			count_liberties(game_info, y, right, SECOND_PLAYER);
			if (game_info->liberties.count == 0) return { right, y };
			restore_board_state(game_info);
		}
	}
	int left = x - 1;
	if (x >= 0) {
		if (game_info->board[y][left] == SECOND_PLAYER) {
			count_liberties(game_info, y, left, SECOND_PLAYER);
			if (game_info->liberties.count == 0) return { left, y };
			restore_board_state(game_info);

		}
	}
	int up = x + 1;
	if (up < game_info->board_size) {
		if (game_info->board[up][x] == SECOND_PLAYER) {
			count_liberties(game_info, up, x, SECOND_PLAYER);
			if (game_info->liberties.count == 0) return { x, up };
			restore_board_state(game_info);
		}
	}
	int down = y - 1;
	if (down > 0) {
		if (game_info->board[down][x] == SECOND_PLAYER) {
			count_liberties(game_info, down, x, SECOND_PLAYER);
			if (game_info->liberties.count == 0) return { x, down };
			restore_board_state(game_info);
		}
	}
	return { -1, -1 };
}


//check if ko fight array contains x,y coordinates
int kofights_contains(ko_fight a[], int size, int x, int y) {
	for (int i = 0; i < size; i++) {
		if (a[i].point.x == x && a[i].point.y == y) return i;
	}
	return -1;
}


//sets coords in ko fight to ko fight array
void set_ko_fight_cords(game_t* game_info) {
	if (game_info->ko.ko_fight_mode == ENABLED) {
		coordinates ko_fight_coords = check_liberties_player_next_to(game_info);
		int length = sizeof(game_info->ko.ko_fights) / sizeof(ko_fight);
		for (int i = 0; i < length; i++) {
			if (game_info->ko.ko_fights[i].counter == 0) {
				game_info->ko.ko_fights[i].point = ko_fight_coords;
				game_info->ko.ko_fights[i].counter = 1;
			}
		}
		game_info->ko.ko_fight_mode = DISABLED;
	}
}


//updates counters of ko fights 
void update_ko_fight_counters(game_t* game_info) {
	int length = sizeof(game_info->ko.ko_fights) / sizeof(ko_fight);
	for (int i = 0; i < length; i++) {
		if (game_info->ko.ko_fights[i].counter >= 1) {
			game_info->ko.ko_fights[i].counter++;
		}
		if (game_info->ko.ko_fights[i].counter = 3) {
			game_info->ko.ko_fights[i] = { 0 };
		}
	}
}


//Funkcja zwraca false je¿eli gracz anuluje wpisywanie (wciœnie ESC)
int get_input_from_user(char string[]) {
	char entered_char = getche();
	int index = 0;
	while (entered_char != ZNAK_ENTER) {
		if (entered_char == ZNAK_ESC) return false;
		string[index] = entered_char;
		index++;
		entered_char = getche();
	}
	return true;
}


//save current game state to file 
void save_game_to_file(game_t* game_info, char file_name[]) {
	FILE* file = fopen(file_name, "w");
	if (file == NULL) {
		char error_message[] = "Error while saving";
		writte_message(error_message);
	}
	else {
		fwrite(&game_info->board_size, sizeof(int), 1, file);
		for (int i = 0; i < game_info->board_size; i++) {
			fwrite(game_info->board[i], sizeof(char), game_info->board_size, file);
		}
		fwrite(&game_info->score, sizeof(game_info->score), 1, file);
		fwrite(&game_info->current_player, sizeof(game_info->current_player), 1, file);
		fwrite(&game_info->ko, sizeof(game_info->ko), 1, file);

		char save_message[500] = "Game saved succesfully at: ";
		strcat_s(save_message, file_name);
		writte_message(save_message);
		fclose(file);
	}
};


//loads saved game file and loads game state from save
int load_saved_game(game_t* game_info, char file_name[]) {
	FILE* file = fopen(file_name, "r");
	if (file == NULL) {
		return false;
	}
	else {
		int board_size = 0;
		fread(&board_size, sizeof(int), 1, file);
		initialize_board_with_size(game_info, board_size);
		for (int i = 0; i < game_info->board_size; i++) {
			fread(game_info->board[i], sizeof(char), board_size, file);
		}
		fread(&game_info->score, sizeof(game_info->score), 1, file);
		fread(&game_info->current_player, sizeof(game_info->current_player), 1, file);
		fread(&game_info->ko, sizeof(game_info->ko), 1, file);
		fclose(file);
		return true;
	}
}


//shows custom menu where custom board size can be entered
void show_custom_board_menu(game_t* game_info) {
	clrscr();
	char message[] = "Enter board size: ";
	writte_message(message);
	int board_size = get_board_size_from_user();

	if (board_size > 0) {
		initialize_board_with_size(game_info, board_size);
	}
	else {
		clear_message();
		show_select_board_size_menu(game_info);
	}
}


//get input from user and save it to int
int get_board_size_from_user() {
	char c[20];
	get_input_from_user(c);
	int size = atoi(c);
	return size;
}


//enables, and disables hadicap mode
void introduce_handicap(game_t* game_info) {
	if (is_board_empty(game_info)) {
		game_info->handicap_mode = ENABLED;
		char message[] = "Handicap mode - press h to disable";
		writte_message(message);
	}
	else if (game_info->handicap_mode == ENABLED) {
		game_info->handicap_mode_used = true;
		game_info->handicap_mode = DISABLED;
		char message[] = "Handicap mode disabled";
		game_info->current_player = WHITE_PLAYER;
		writte_message(message);
	}
	else {
		char message[] = "Game started can't use handicap mode";
		writte_message(message);
	}
}


//checks if board is empty
int is_board_empty(game_t* game_info) {
	for (int y = 0; y < game_info->board_size; y++) {
		for (int x = 0; x < game_info->board_size; x++) {
			if (game_info->board[y][x] != EMPTY) return false;
		}
	}
	return true;
}


//refreshes view 
void refresh_view(game_t* game_info) {
	clrscr();
	print_board(game_info, 0);
	show_manual();
	show_stats(game_info);
	show_legend();
	row_indexing(game_info);
	column_indexing(game_info, 0);
	struct text_info info;
	gettextinfo(&info);
	game_info->board_shift = 0;
	game_info->coords.x_relative_to_screen = BOARD_X;
	game_info->coords.y_relative_to_screen = BOARD_Y;
	game_info->coords.x = 0;
	game_info->coords.y = 0;
	if (game_info->board[0][0] == WHITE_PLAYER) game_info->char_under_stone[0] = 'o';
	else if (game_info->board[0][0] == BLACK_PLAYER) game_info->char_under_stone[0] = '*';
	else game_info->char_under_stone[0] = { (char)UPPER_LEFT_CORNER_CHAR };
	_setcursortype(_NOCURSOR);
}


//shows menu for selecting board size and loading game save
void show_select_board_size_menu(game_t* game_info) {
	gotoxy(MENU_X, MENU_Y);
	cputs("select board size to select press 1-5");
	gotoxy(MENU_X, NEXT_Y);
	cputs("1) 9x9");
	gotoxy(MENU_X, NEXT_Y);
	cputs("2) 13x13");
	gotoxy(MENU_X, NEXT_Y);
	cputs("3) 19x19");
	gotoxy(MENU_X, NEXT_Y);
	cputs("4) enter custom size: ");
	gotoxy(MENU_X, NEXT_Y);
	cputs("5) load save file: ");
	gotoxy(MENU_X, NEXT_Y);
	char pressed_number = 0;
	while (pressed_number != '1' && pressed_number != '2' && pressed_number != '3' && pressed_number != '4' && pressed_number != '5' && pressed_number != ZNAK_ESC) {
		pressed_number = getch();
	}
	if (pressed_number == '1') {
		int board_size = 9;
		initialize_board_with_size(game_info, board_size);
	}
	else if (pressed_number == '2') {
		int board_size = 13;
		initialize_board_with_size(game_info, board_size);
	}
	else if (pressed_number == '3') {
		int board_size = 19;
		initialize_board_with_size(game_info, board_size);
	}
	else if (pressed_number == '4') {
		show_custom_board_menu(game_info);
	}
	else if (pressed_number == '5') {
		clrscr();
		show_enter_save_menu(game_info);
	}
}


//check if current board position is empty
void check_if_position_is_empty(game_t* game_info) {
	if (game_info->board[game_info->coords.y][game_info->coords.x] == EMPTY) {
		try_to_place_stone(game_info);
	}
	else {
		char message[] = "Position taken";
		writte_message(message);
	}
}


void try_to_place_stone(game_t* game_info) {
	game_info->board[game_info->coords.y][game_info->coords.x] = game_info->current_player;
	count_liberties(game_info, game_info->coords.y, game_info->coords.x, game_info->current_player);

	//if current position has any liberties we can place stone
	if (game_info->liberties.count > 0) {
		game_info->insert_mode = true;
		char message[] = "Insert mode";
		writte_message(message);
	}
	else {
		//we can place stone at position that doesnt have liberties
		//if and only if it captures at least one oposite player stone
		coordinates ko_fight_cords = check_liberties_player_next_to(game_info);
		int ko_fights_length = sizeof(game_info->ko.ko_fights) / sizeof(game_info->ko.ko_fights[0]);
		int is_position_in_ko_fight = kofights_contains(game_info->ko.ko_fights, ko_fights_length , game_info->coords.x, game_info->coords.y);
		int is_player_next_to_captured = false;
		if (ko_fight_cords.x != -1 && ko_fight_cords.y != -1) is_player_next_to_captured = true;
	
		//check if player captured oposite stone and current position is not in ko fight
		if (is_player_next_to_captured && is_position_in_ko_fight == -1) {
			game_info->insert_mode = ENABLED;
			game_info->ko.ko_fight_mode = ENABLED;
			char message[] = "Inset mode KO";
			writte_message(message);
		}
		else {
			//if can capture but position is in ko fight
			game_info->board[game_info->coords.y][game_info->coords.x] = EMPTY;
			if (is_position_in_ko_fight != -1) {
				char ko_message[] = "Position ko fight";
				writte_message(ko_message);

			}
			else {
				char suicide_message[] = "Suicide position";
				writte_message(suicide_message);
			}
		}
	}
	restore_board_state(game_info);
}


//prints char that was at current position before current player stone here  
void restore_char_after_moving(game_t* game_info, char c[]) {
	gotoxy(game_info->coords.x_relative_to_screen, game_info->coords.y_relative_to_screen);
	if (c[0] != '*' && c[0] != 'o')  textcolor(BOARD_COLOR);
	putch(c[0]);
	c = { 0 };
	textcolor(STONES_COLOR);
}


//saves char under current player stone 
void save_char_under(game_t* game_info) {
	int x = game_info->coords.x_relative_to_screen;
	int y = game_info->coords.y_relative_to_screen;
	gettext(x, y, x, y, game_info->char_under_stone);
}


//check which arrow was pressed and moves current stone int that direction
void move_stone_on_board(game_t* game_info) {
	game_info->pressed_button = getch();

	if (game_info->pressed_button == UP_ARROW) {
		if ((game_info->coords.y) - 1 >= 0) {
			restore_char_after_moving(game_info, game_info->char_under_stone);
			game_info->coords.y_relative_to_screen -= 1;
			game_info->coords.y -= 1;
			save_char_under(game_info);
		}
	}
	else if (game_info->pressed_button == DOWN_ARROW) {
		if ((game_info->coords.y) + 1 < game_info->board_size) {
			restore_char_after_moving(game_info, game_info->char_under_stone);
			game_info->coords.y_relative_to_screen += 1;
			game_info->coords.y += 1;
			save_char_under(game_info);
		}
	}
	else if (game_info->pressed_button == LEFT_ARROW) {
		if ((game_info->coords.x) - 1 >= 0) {
			restore_char_after_moving(game_info, game_info->char_under_stone);
			game_info->coords.x -= 1;
			check_for_left_scroll(game_info);
			save_char_under(game_info);
		}
	}
	else if (game_info->pressed_button == RIGHT_ARROW) {
		if ((game_info->coords.x) + 1 < game_info->board_size) {
			restore_char_after_moving(game_info, game_info->char_under_stone);
			game_info->coords.x += 1;
			check_for_right_scroll(game_info);
			save_char_under(game_info);
		}
	}
}


//scoll board to left when current move will go outside of board 
void check_for_left_scroll(game_t* game_info) {
	text_info info;
	gettextinfo(&info);
	if (game_info->board_shift > 0 && game_info->coords.x_relative_to_screen == BOARD_X) {
		game_info->board_shift -= 1;
		movetext(BOARD_X, BOARD_Y, info.screenwidth - 3, info.screenheight + game_info->board_size, BOARD_X + 2, BOARD_Y);
		gotoxy(BOARD_X, BOARD_Y);
		print_board_column(game_info, game_info->board_shift);
		column_indexing(game_info, game_info->board_shift);
	}
	else {
		game_info->coords.x_relative_to_screen -= 2;
	}
}


//scrolls board to right when current move will go outside of board
void check_for_right_scroll(game_t* game_info) {
	text_info info;
	gettextinfo(&info);
	if (game_info->coords.x_relative_to_screen + 2 >= info.screenwidth) {
		movetext(BOARD_X + 2, BOARD_Y, info.screenwidth - 2, info.screenheight + game_info->board_size, BOARD_X, BOARD_Y);
		game_info->board_shift += 1;
		gotoxy(game_info->coords.x_relative_to_screen, BOARD_Y);
		print_board_column(game_info, game_info->coords.x);
		column_indexing(game_info, game_info->board_shift);
	}
	else {
		game_info->coords.x_relative_to_screen += 2;
	}
}


//print one board column 
void print_board_column(game_t* game_info, int col_x) {
	for (int y = 0; y < game_info->board_size; y++) {
		print_char_from_board(game_info, y, col_x, 0);
		if (col_x == game_info->board_size - 1) clreol();
		gotoxy(game_info->coords.x_relative_to_screen, NEXT_Y);
	}
}


//places stone at current position, saves and updates ko fight, checks for capture, switches player to oposite 
void confirm_placing_stone(game_t* game_info) {
	clear_message();
	game_info->insert_mode = DISABLED;

	if (game_info->handicap_mode == DISABLED) {
		update_ko_fight_counters(game_info);

		set_ko_fight_cords(game_info);

		int points = capture(game_info, SECOND_PLAYER); // gracz przeciwny
		add_points_to_player(game_info, points);
		swap_current_player(game_info);

		if (points != 0) {
			print_board(game_info, game_info->board_shift);
			show_stats(game_info);
		}
		restore_board_state(game_info);
	}
	save_char_under(game_info);
}


void add_points_to_player(game_t* game_info, int points) {
	game_info->current_player == BLACK_PLAYER ? game_info->score.black_player += points : game_info->score.white_player += points;
}


//swaps current player to oposite one
void swap_current_player(game_t* game_info) {
	game_info->current_player == BLACK_PLAYER ? game_info->current_player = WHITE_PLAYER : game_info->current_player = BLACK_PLAYER;
}


void cancel_placing_stone(game_t* game_info) {
	if (game_info->insert_mode == ENABLED) {
		restore_board_state(game_info);
		game_info->board[game_info->coords.y][game_info->coords.x] = EMPTY;
		game_info->insert_mode = DISABLED;
	}
}


void start_new_game(game_t* game_info) {
	clrscr();
	show_select_board_size_menu(game_info);
	initial_game_state(game_info);
	refresh_view(game_info);
}


int get_save_file_name(char file_name[]) {
	int if_successed = get_input_from_user(file_name);
	strcat(file_name, ".bin");
	return if_successed;
}


void save_game(game_t* game_info) {
	if (game_info->insert_mode == DISABLED) {
		char message[] = "Enter save name: ";
		writte_message(message);
		char file_name[256] = { 0 };
		int if_successed = get_save_file_name(file_name);
		if (if_successed) {
			save_game_to_file(game_info, file_name);
		}
		else {
			clear_message();
		}
	}
	else {
		char message[] = "Can not save when inserting stone";
		writte_message(message);
	}
}


//draws stone at current position
void draw_stone(game_t* game_info) {
	gotoxy(game_info->coords.x_relative_to_screen, game_info->coords.y_relative_to_screen);
	textcolor(CURRENT_STONE_COLOR);
	game_info->current_player == BLACK_PLAYER ? putch('*') : putch('o');
	textcolor(STONES_COLOR);
}


//shows menu for loading game save file
void show_enter_save_menu(game_t* game_info) {
	char message[] = "Enter save file name: ";
	writte_message(message);
	char file_name[256] = { 0 };
	int if_successed = get_save_file_name(file_name);
	int save = load_saved_game(game_info, file_name);

	if (if_successed) {
		if (save) {
			refresh_view(game_info);
			char load_message[] = "Game loaded succesfully";
			writte_message(load_message);
		}
		else {
			char error_message[] = "Error while loading save";
			gotoxy(50, NEXT_Y);
			cputs("Error while loading save");
			show_enter_save_menu(game_info);
		}
	}
	//if player cancels enterng file
	else {
		if (game_info->board_size != 0) {
			char message[] = "Cancelled";
			writte_message(message);
		}
		//check if player is in select board menu show it again 
		else {
			clrscr();
			show_select_board_size_menu(game_info);
		}
	}

}


//frees memory allocated to board array
void free_board_memory(game_t* game_info) {
	for (int i = 0; i < game_info->board_size; i++) {
		free(game_info->board[i]);
	}
	free(game_info->board);
}


//alocates memory for board array with specific size
void initialize_board_with_size(game_t* game_info, int size) {
	free_board_memory(game_info);
	game_info->board = (char**)malloc(size * sizeof(char*));
	if (game_info->board) {
		for (int i = 0; i < size; i++) {
			if (game_info->board) {
				*(game_info->board + i) = (char*)malloc(size * sizeof(int));
			}
		}
		game_info->board_size = size;
		clear_board(game_info);
	}
}


void clear_message() {
	gotoxy(MESSAGE_X, MESSAGE_Y);
	clreol();
}

void writte_message(char s[]) {
	clear_message();
	cputs(s);
}


void go_line_below(int x) {
	gotoxy(x, wherey() + 1);
}


// empties whole board
void clear_board(game_t* game_info) {
	for (int y = 0; y < game_info->board_size; y++)
		for (int x = 0; x < game_info->board_size; x++)
			game_info->board[y][x] = EMPTY;
}


//print row indexing left and right to the board
void row_indexing(game_t* game_info) {
	int number_of_digits_of_board_size = (int)(log10(game_info->board_size) + 1); 
	int left_to_board_x = BOARD_X - number_of_digits_of_board_size - 1;
	int right_to_board_x = BOARD_X + game_info->board_size * 2;

	print_row_indexes(game_info, left_to_board_x);
	print_row_indexes(game_info, right_to_board_x);
}

//prints whole board row at specific x
void print_row_indexes(game_t* game_info, int x) {
	gotoxy(x, BOARD_Y);
	char number[10];
	for (int i = game_info->board_size; i > 0; i--) {
		_itoa(i, number, 10);
		cputs(number);
		gotoxy(x, NEXT_Y);
	}
}


//prints board columns idexes above and under board 
void column_indexing(game_t* game_info, int index_offset) {
	char last_column_index = 'A' + game_info->board_size;
	int above_board_y = BOARD_Y - 1;
	int below_board_y = BOARD_Y + game_info->board_size;
	print_colum_indexes(above_board_y, last_column_index, index_offset);
	print_colum_indexes(below_board_y, last_column_index, index_offset);

}


//prints column idexes at specific y, including board scroll
void print_colum_indexes(int y, char last_column_index, int index_offset) {
	gotoxy(BOARD_X, y);
	for (char col_index = 'A' + index_offset; col_index < last_column_index; col_index++) {
		putch(col_index);
		gotoxy(wherex() + 1, y);
	}
	clreol();
}


//resets game to initial state
void initial_game_state(game_t* game_info) {
	clear_board(game_info);
	game_info->current_player = BLACK_PLAYER;
	game_info->coords.x_relative_to_screen = BOARD_X, game_info->coords.y_relative_to_screen = BOARD_Y;
	game_info->coords.x = 0;
	game_info->coords.y = 0;
	game_info->score.black_player = 0;
	game_info->score.white_player = 0;
	game_info->handicap_mode_used = 0;
	game_info->board_shift = 0;
	memset(game_info->ko.ko_fights, 0, sizeof game_info->ko.ko_fights);
}


//print board
void print_board(game_t* game_info, int x_shift) {
	gotoxy(BOARD_X, BOARD_Y);
	text_info info;
	gettextinfo(&info);
	for (int y = 0; y < game_info->board_size; y++) {
		for (int x = x_shift; x < game_info->board_size; x++) {
		gettextinfo(&info);
		//cut board when it doesnt fit in screen
		if (info.curx + 1 > info.screenwidth) break;
			print_char_from_board(game_info, y, x, x_shift);
			if (x < game_info->board_size - 1) print_horizontal_line(x);
		}
		gotoxy(BOARD_X, NEXT_Y);
	}
}


//prints one horizontal line char
void print_horizontal_line(int x) {
	textcolor(DARKGRAY);
	putch(HORIZONATL_LINE_CHAR);
	textcolor(LIGHTGRAY);
}


//prints one char from board at x,y including x board scroll
void print_char_from_board(game_t* game_info, int y, int x, int x_shift) {
	if (game_info->board[y][x] == BLACK_PLAYER) cputs("*");
	else if (game_info->board[y][x] == WHITE_PLAYER) cputs("o");
	else if (game_info->board[y][x] == EMPTY) {
		textcolor(DARKGRAY);
		if (y == TOP_BORDER) {
			if (x == LEFT_BORDER || x == x_shift) putch(UPPER_LEFT_CORNER_CHAR);
			else if (x == RIGHT_BORDER) putch(UPPER_RIGHT_CORNER_CHAR);
			else putch(HORIZONAL_WITH_VERTICAL_BOTTOM);
		}
		else if (y == BOTTOM_BORDER) {
			if (x == LEFT_BORDER || x == x_shift) putch(BOTTOM_LEFT_CORNER_CHAR);
			else if (x == RIGHT_BORDER) putch(BOTTOM_RIGHT_CORNER_CHAR);
			else putch(HORIZONTAL_WITH_VERTICAL_UP_CHAR);
		}
		else {
			if (x == LEFT_BORDER || x == x_shift) putch(VERTICAL_WITH_HORIZONTAL_RIGHT_CHAR);
			else if (x == RIGHT_BORDER) putch(VERTICAL_WITH_HORIZONTAL_LEFT_CHAR);
			else putch(CROSS_CHAR);
		}
		textcolor(LIGHTGRAY);
	}
}


//prints legend
void show_legend() {
	gotoxy(LEGEND_X, LEGEND_Y);
	cputs("Igor Stadnicki 193435");
	go_line_below(LEGEND_X);
	cputs("implemented things list:");
	go_line_below(LEGEND_X);
	cputs("a) b) c) d) e) f) g) h) i) j) k)");

}


//prints players score
void show_stats(game_t* game_info) {
	gotoxy(STATS_X, STATS_Y);
	cputs("Black score: ");
	char int_as_string[20];
	_itoa(game_info->score.black_player, int_as_string, 10);
	cputs(int_as_string);
	cputs("     White score: ");
	_itoa(game_info->score.white_player, int_as_string, 10);
	cputs(int_as_string);

}

//prints current player coordinates
void show_coordinates(game_t* game_info) {
	gotoxy(COORDINATES_X, COORDINATES_Y);
	cputs("                            ");
	gotoxy(COORDINATES_X, COORDINATES_Y);
	char first_coord = game_info->coords.x + 'A';
	int second_coord = game_info->board_size - game_info->coords.y;
	cputs("Pozycja: ");
	putch(first_coord);
	char int_as_string[20];
	_itoa(second_coord, int_as_string, 10);
	cputs(int_as_string);
}


//prints manual
void show_manual() {
	gotoxy(MANUAL_X, MANUAL_Y);
	cputs("arrows: moving the cursor over the board");
	go_line_below(MANUAL_X);
	cputs("q: quit the program");
	go_line_below(MANUAL_X);
	cputs("n: start a new game");
	go_line_below(MANUAL_X);
	cputs("i: place a stone on the board");
	go_line_below(MANUAL_X);
	cputs("enter: confirm choice and end player's turn");
	go_line_below(MANUAL_X);
	cputs("esc: cancel current action");
	go_line_below(MANUAL_X);
	cputs("s: save the game state");
	go_line_below(MANUAL_X);
	cputs("l: load the game state");
	go_line_below(MANUAL_X);
	cputs("h: introduce handicap");
}