#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "uthash.h"
#if !defined(uint)
	typedef unsigned int uint;
#endif
#if !defined(ulong)
	typedef unsigned long ulong;
#endif
//#include <mysql/my_global.h>
//#include <mysql/mysql.h>

struct game {
	const char *game_string;
	char down[3];
	char cards_to_turn;
	char card_col[52];
	char previous_card[52];
	char foundations[4];
	char bottom_cards[7];
	char cards_flipped;
	double odds;
	UT_hash_handle hh;
};

extern struct game default_game;

//extern MYSQL *conn;

extern const char * ordered_deck;

#define get_card_int(letter) (isupper(letter) ? (letter - 65) : (letter - 71))

#define card_movable(card,game) (((game->card_col[card] >= 0) && (game->card_col[card] <= 6)) ? true : (((game->card_col[card] >= 7) && ((game->card_col[card] - 7) >= game->cards_to_turn)) ? true : false))

#define won(game) ((((game).foundations[0] == 48) && ((game).foundations[1] == 49) && ((game).foundations[2] == 50) && ((game).foundations[3] == 51)) ? true : false)

#define copy_game(og,ng) 	\
do { 						\
	memcpy((ng).down, (og).down, sizeof((og).down)); \
	(ng).cards_to_turn = (og).cards_to_turn;\
	memcpy((ng).card_col, (og).card_col, sizeof((og).card_col)); \
	memcpy((ng).previous_card, (og).previous_card, sizeof((og).previous_card)); \
	memcpy((ng).foundations, (og).foundations, sizeof((og).foundations)); \
	memcpy((ng).bottom_cards, (og).bottom_cards, sizeof((og).bottom_cards)); \
	(ng).cards_flipped = (og).cards_flipped;\
	(ng).odds = (og).odds;\
} while(0)

void New_Hand(char *hand);

void Initial_Shuffle(char *hand);

void Shuffle(char *hand);

struct game Hand_to_Game(char *hand);

void Print_Card(short int card);

char Card_Suit_Display(short int card);

char Card_Value_Display(short int card);

void Print_Game(struct game *g);

void Stack_Cards(struct game *g);

int Get_Moves(struct game *g, unsigned short int[]);

void Make_Move(struct game *g, unsigned short int);

unsigned int Run_Random(struct game g,unsigned int);

bool Straight_Win (struct game g);

bool Straight_Last_Win (struct game g);

void Set_Game_String(struct game *g);

bool Get_Percent(struct game *g);

float Get_Memory();
