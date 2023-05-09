#include "shorty.h"

struct game default_game = {.down = {-1,-1,-1},
							.cards_to_turn = 3,
							.card_col = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							.previous_card = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							.foundations = {-4,-3,-2,-1},
							.bottom_cards = {-1,-1,-1,-1,-1,-1,-1},
							.odds = 1
};

const char * ordered_deck = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\0";

void Initial_Shuffle(char *hand) {
	int i, to;
	char temp;
	FILE* randomData = fopen("/dev/urandom", "r");
	unsigned int myRandomInteger;
	to = fread(&myRandomInteger, sizeof myRandomInteger,1,randomData);
	srand(time(NULL)*myRandomInteger);
	for (i = 0; i < 51;i++) {
		to = fread(&myRandomInteger, sizeof myRandomInteger,1,randomData);
		to = (myRandomInteger % (51+1-i))+i;
		temp = hand[i];
		hand[i] = hand[to];
		hand[to] = temp;
	}
	fclose(randomData);
}


void Shuffle(char *hand) {
	int i, to;
	char temp;
	for (i = 0; i < 51;i++) {
		to = (rand() % (51+1-i))+i;
		temp = hand[i];
		hand[i] = hand[to];
		hand[to] = temp;
	}
}


struct game Hand_to_Game(char *hand) {
	struct game g = default_game;
    int count = 0;
    for (short int i = 0;i <= 6;i++) {
       for (short int j = 0;j <= 6;j++) {
    	  int card = get_card_int(hand[count]);
       	  if (i == 6) {
             g.bottom_cards[j] = card;
          }
          if (i > 0) {
             g.previous_card[card] = get_card_int(hand[count-7]);
          }
          if ((j >= 2) && (j <= 4) && (i <= 4)) {
             g.card_col[card] = ((i + 1) * -1);
          } else {
             g.card_col[card] = j;
          }
          count++;
       }
    }
    g.down[0] = get_card_int(hand[count]);
    g.card_col[get_card_int(hand[count])] = 9;
    count++;
    g.down[1] = get_card_int(hand[count]);
    g.card_col[get_card_int(hand[count])] = 8;
    count++;
    g.down[2] = get_card_int(hand[count]);
    g.card_col[get_card_int(hand[count])] = 7;
	return g;
}

char Card_Suit_Display(short int card) {
	switch (card % 4) {
		case 0:
			return 'S';
		case 1:
			return 'H';
		case 2:
			return 'C';
		case 3:
			return 'D';
	}
	return ' ';
}

char Card_Value_Display(short int card) {
	if (card == -1) {
		return ' ';
	} else if ((card / 4) <= 8)  {
		return ('0'+((card / 4)+1));
	} else {
		switch (card / 4) {
			case 9:
				return '0';
			case 10:
				return 'J';
			case 11:
				return 'Q';
			case 12:
				return 'K';
		}
	}
	return ' ';
}

void Print_Card(short int card) {
	putchar(Card_Value_Display(card));
	putchar(Card_Suit_Display(card));
}


void Print_Game(struct game *g) {
	for (int i = 0;i <= 3;i++) {
		if (g->foundations[i] >= 0) {
			Print_Card(g->foundations[i]);
		} else {
			putchar('*');
			putchar('*');
		}
		putchar(' ');
	}
	for (int i = 0;i <= 2;i++) {
		if (g->cards_to_turn <= i) {
			Print_Card(g->down[i]);
		} else {
			putchar('*');
			putchar('*');
		}
		putchar(' ');
	}
	putchar('\n');
	putchar('\n');
	short int row_counts[] = {0,0,0,0,0,0,0};
	short int max_rows = 0;
	for (short int i = 0;i <= 6;i++) {
		short int card = g->bottom_cards[i];
		while (card >= 0) {
			row_counts[i]++;
			card = g->previous_card[card];
		}
		if (row_counts[i] > max_rows) {
			max_rows = row_counts[i];
		}
	}

	char board[max_rows][21];
	for (short int i = 0;i <= 6;i++) {
		short int card = -1;
		for (short int j = max_rows -1;j >= 0;j--) {
			if ((row_counts[i] - 1) == j) {
				card = g->bottom_cards[i];
			} else if ((row_counts[i] - 1) > j) {
				card = g->previous_card[card];
			}
			if (card >= 0) {
				if (g->card_col[card] <= -3) {
					board[j][i * 3] = '*';
					board[j][(i * 3) + 1] = '*';
				} else {
					board[j][i * 3] = Card_Value_Display(card);
					board[j][(i * 3) + 1] = Card_Suit_Display(card);
				}
			} else {
				board[j][i * 3] = ' ';
				board[j][(i * 3) + 1] = ' ';
			}
			board[j][(i * 3) + 2] = ' ';
		}
	}

	for (int i = 0;i < max_rows;i++) {
		for (int j = 0;j < 21;j++) {
			putchar(board[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
}

void Stack_Cards(struct game *g) {
	//Flip Cards First
	if (g->cards_flipped < 9) {
		for (short int i = 2;i <= 4;i++) {
			if ((g->bottom_cards[i] > -1) && (g->card_col[g->bottom_cards[i]] < 0)) {
				if (g->card_col[g->bottom_cards[i]] == -3) {
					g->card_col[g->previous_card[g->bottom_cards[i]]] = i;
					g->card_col[g->previous_card[g->previous_card[g->bottom_cards[i]]]] = i;
				}
				g->card_col[g->bottom_cards[i]] = i;
				g->cards_flipped++;
			}
		}
	}

	//Stack Cards
	bool card_stacked = false;
	short int to_stack;
	for (short int i = 0;i <= 3;i++) {
		if (g->foundations[i] != 48 + i) {
			to_stack = g->foundations[i] + 4;
			bool card_status = card_movable(to_stack,g);
			if (card_status && (g->card_col[to_stack] >= 7)) {
				g->foundations[i] = to_stack;
				g->down[g->card_col[to_stack] - 7] = -1;
				card_stacked = true;
				g->card_col[to_stack] = -1;
			} else if (card_status && (to_stack  == g->bottom_cards[g->card_col[to_stack]])) {
				g->foundations[i] = to_stack;
				g->bottom_cards[g->card_col[to_stack]] = g->previous_card[to_stack];
				g->previous_card[to_stack] = -1;
				g->card_col[to_stack] = -1;
				card_stacked = true;
			}
		}
	}
	if (card_stacked) {
		Stack_Cards(g);
	}
}

int Get_Moves(struct game *g, unsigned short int moves[]) {
	short int count = 0;
	short int card_to_check;
	short int king_col = -1;
	for (short int i = 0; i <= 6; i++) {
		if (g->bottom_cards[i] == -1) {
			if (king_col == -1) {
				king_col = i;
			}
		} else if (g->bottom_cards[i] >= 4) {
			card_to_check = g->bottom_cards[i] - 4;
			if ((g->card_col[card_to_check] != i) && card_movable(card_to_check,g)) {
				moves[count] = (i * 100) + card_to_check;
				count++;
			}
		}
	}
	short int king_to_check = 48;
	while ((king_col >= 0) && (king_to_check <= 51)) {
		if (card_movable(king_to_check,g) && ((g->card_col[king_to_check] >= 7) || (g->previous_card[king_to_check] != -1))) {
			moves[count] = (king_col * 100) + king_to_check;
			count++;
		}
		king_to_check++;
	}
	if ((count == 0) && (g->cards_to_turn > 0)) {
		g->cards_to_turn--;
		Stack_Cards(g);
		count = Get_Moves(g,moves);
	}
	return count;
}

void Make_Move(struct game *g, unsigned short int move) {
	short int card_to_move = move % 100;
	short int to_col = move / 100;
	short int fromCol = g->card_col[card_to_move];
	short int new_bottom = g->previous_card[card_to_move];
	if (fromCol >= 7) {
		g->previous_card[card_to_move] = g->bottom_cards[to_col];
		g->card_col[card_to_move] = to_col;
		g->bottom_cards[to_col] = card_to_move;
		g->down[fromCol-7] = -1;
	} else {
		g->previous_card[card_to_move] = g->bottom_cards[to_col];
		short int cardMoving = g->bottom_cards[fromCol];
		bool keep_moving = true;
		while (keep_moving) {
			if (cardMoving == card_to_move) {
				keep_moving = false;
			}
			g->card_col[cardMoving] = to_col;
			cardMoving = g->previous_card[cardMoving];
			if (cardMoving == -1) {
				keep_moving = false;
			}
		}
		g->bottom_cards[to_col] = g->bottom_cards[fromCol];
		g->bottom_cards[fromCol] = new_bottom;
	}
}

unsigned int Run_Random (struct game g, unsigned int number_of_games) {
	unsigned int wins = 0;
	unsigned int ran = 0;
	unsigned short int moves[10];
	int num_of_moves;
	struct game game_copy;
	game_copy = g;
	srand(time(NULL));
	while (ran < number_of_games) {
		g = game_copy;
		Stack_Cards(&g);
		while ((num_of_moves = Get_Moves(&g,moves)) > 0) {
			Make_Move(&g,moves[rand() % num_of_moves]);
			Stack_Cards(&g);
		}
		if (won(g)) {
			wins++;
		}
		ran++;
	}
	return wins;
}

bool Straight_Win (struct game g) {
	unsigned short int moves[10];
	int num_of_moves;
	Stack_Cards(&g);
	while ((num_of_moves = Get_Moves(&g,moves)) > 0) {
		Make_Move(&g,moves[0]);
		Stack_Cards(&g);
	}
	if (won(g)) {
		return true;
	}
	return false;
}

bool Straight_Last_Win (struct game g) {
	unsigned short int moves[10];
	int num_of_moves;
	Stack_Cards(&g);
	while ((num_of_moves = Get_Moves(&g,moves)) > 0) {
		Make_Move(&g,moves[num_of_moves-1]);
		Stack_Cards(&g);
	}
	if (won(g)) {
		return true;
	}
	return false;
}

void New_Hand(char* hand) {
	strcpy(hand,ordered_deck);
	srand(time(NULL));
	for (int i = 0; i <= 2;i++) {
		for (int j = 0; j <= 51;j++) {
			int swap = rand() % 51;
			char temp = hand[swap];
			hand[swap] = hand[j];
			hand[j] = temp;
		}
	}
}

void Set_Game_String(struct game *g) {
	char* temp = malloc(4+4+3+7+52+1);
    int cards_to_flip[] = {0,0,0};
    int spot = 12;
	for (int i = 0;i <= 6;i++) {
		if (g->bottom_cards[i] >= 0) {
			int card = g->bottom_cards[i];
			if ((i >= 2) && (i <= 4) && (g->card_col[card] < cards_to_flip[i-2])) {
				cards_to_flip[i-2] = g->card_col[card];
			}
			temp[spot] = ordered_deck[card];
			spot++;
			while (g->previous_card[card] >= 0) {
				card = g->previous_card[card];
				if ((i >= 2) && (i <= 4) && (g->card_col[card] < cards_to_flip[i-2])) {
					cards_to_flip[i-2] = g->card_col[card];
				}
				temp[spot] = ordered_deck[card];
				spot++;
			}
		}
		if (i != 6) {
			temp[spot] = '|';
			spot++;
		} else {
			//temp[spot] = ('0'+(((g->current_level%10))+1));
			//spot++;
			temp[spot] = '\0';
		}
	}
	for (int i = 0;i <=2;i++) {
		switch (cards_to_flip[i]) {
			case -5:
				temp[i] = '3';
				break;
			case -4:
				temp[i] = '2';
				break;
			case -3:
				temp[i] = '1';
				break;
			default:
				temp[i] = '0';
				break;
		}
	}
	temp[3] = (((int)'0')+g->cards_to_turn);
	for (int i = 0;i <= 3;i++) {
		if (g->foundations[i] < 0) {
			temp[4 + i] = ' ';
		} else {
			temp[4 + i] = ordered_deck[g->foundations[i]];
		}
	}
	for (int i = 0;i <= 2;i++) {
		if (g->down[i] < 0) {
			temp[8 + i] = ' ';
		} else {
			temp[8 + i] = ordered_deck[g->down[i]];
		}
	}
	temp[11] = '|';
	g->game_string = temp;
}

bool Get_Percent(struct game *game_to_use) {
	struct game *games = NULL;
	struct game *g, *tmp, *g2, *g3;
	double win_percent = 0;
	unsigned int size = 1;
	unsigned short int moves[10];
	// int total_moves = 0;
	// int total_merges = 0;
	// int total_games = 0;
	// int total_wins = 0;
	// int total_levels = 0;
	short int i, num_of_moves;
	bool winable = false;
	tmp = malloc(sizeof(struct game));
	copy_game(*game_to_use,*tmp);
	Stack_Cards(tmp);
	Set_Game_String(tmp);
	int n;
	HASH_ADD_KEYPTR( hh, games, tmp->game_string, strlen(tmp->game_string), tmp );
	while (size > 0) {
		// total_levels++;
		HASH_ITER(hh, games, g, tmp) {
			num_of_moves = Get_Moves(g,moves);
			// total_moves += num_of_moves;
			if (num_of_moves == 0) {
				if (won(*g)) {
					win_percent += g->odds;
					winable = true;
					// total_wins++;
				}
				// printf("%d %2.20f\n",total_levels, g->odds);
				// total_games++;
			} else {
				for (i = 0; i < num_of_moves;i++) {
					g2 = malloc(sizeof(struct game));
					copy_game(*g,*g2);
					Make_Move(g2,moves[i]);
					Stack_Cards(g2);
					Set_Game_String(g2);
					HASH_FIND_STR(games,g2->game_string,g3);
					if (g3) {
						g3->odds += (g2->odds/num_of_moves);
						free((void *)g2->game_string);
						free(g2);
						// total_merges++;
					} else {
						g2->odds = (g2->odds/num_of_moves);
						HASH_ADD_KEYPTR( hh, games, g2->game_string, strlen(g2->game_string), g2 );
					}
				}
			}
			HASH_DEL(games, g);
			free((void *)g->game_string);
			free(g);
			size--;
			if (size == 0)
				goto start2;
		}
		start2:;
		size = HASH_COUNT(games);
	}
	free(games);
	game_to_use->odds = (win_percent * 100);
	// printf("Total Moves: %i\n",total_moves);
	// printf("Total Merges: %i\n",total_merges);
	// printf("Total Levels: %i\n",total_levels);
	// printf("Total Wins: %i\n",total_wins);
	// printf("Total Games: %i\n",total_games);
	// if (total_wins == 1) {
	// 	exit(0);
	// }
	return winable;
}

float Get_Memory() {
	char memory_usage[80];
	char line[80];
	char* memory_string;
	FILE* status = fopen( "/proc/self/statm", "r" );
	if (fgets(line, 80, status) != NULL) {
		int i;
		for (i = 0;i < 80;i++) {
			if (line[i] == ' ') {
				break;
			}
		}
		memcpy(memory_usage,line,i);
		fclose(status);
		return ((float)4096 * strtol(memory_usage,&memory_string,10) / (float)1048576);
	} else {
		fclose(status);
		return 0;
	}
}
