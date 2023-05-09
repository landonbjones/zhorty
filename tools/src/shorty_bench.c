#include "shorty.h"

#define MAX_HARD_HANDS 33
#define MAX_HANDS 7584
#define MAX_RAND_HANDS 1000000

extern char hard_hands[MAX_HARD_HANDS][53];
extern char hands[MAX_HANDS][53];


int main (int argc, char *argv[]) {
	bool display_hands = true;
	int i, hands_to_run, hard_hands_to_run, rand_hands_to_run;
	hands_to_run = hard_hands_to_run = rand_hands_to_run = 0;
	struct game g;
	char* hands_to_run_string;

	for (i = 0; i < argc;i++) {
		if (strcmp(argv[i],"h") == 0) {
			i++;
			double hands_to_run_double;
			hands_to_run_double = strtol(argv[i],&hands_to_run_string,10);
			if (hands_to_run_double > 0) {
				hands_to_run = (int)hands_to_run_double;
			}
			if (hands_to_run > MAX_HANDS) {
				hands_to_run = MAX_HANDS;
			}
		} else if (strcmp(argv[i],"hh") == 0) {
			i++;
			hard_hands_to_run = (int)strtol(argv[i],&hands_to_run_string,10);
			if (hard_hands_to_run > MAX_HARD_HANDS) {
				hard_hands_to_run = MAX_HARD_HANDS;
			}
		} else if (strcmp(argv[i],"-e") == 0) {
			display_hands = false;
		} else if (strcmp(argv[i],"rand_test") == 0) {
			i++;
			rand_hands_to_run = (int)strtol(argv[i],&hands_to_run_string,10);
			if (rand_hands_to_run > MAX_RAND_HANDS) {
				rand_hands_to_run = MAX_RAND_HANDS;
			}
		}
	}

	if (rand_hands_to_run > 0) {
		char hand[53] = "rMjCIsEXfVubxnJcqwYehpHFdTlAzyLmDvgSUWNZKQPaBOGRtkio";
		int ran, sw, slw, nw, te, games, hard_games;
		ran = sw = slw = nw = te = games = hard_games = 0;
		struct game g;
		Initial_Shuffle(hand);
		while (rand_hands_to_run > ran) {
			Shuffle(hand);
			g = Hand_to_Game(hand);
			if (!Straight_Win(g)) {
				if (!Straight_Last_Win(g)) {
					if (Get_Percent(&g)) {
						if (34 >= g.odds) {
							if (g.odds < .001) {
								hard_games++;
							} else {
								games++;
							}
						} else {
							te++;
						}
					} else {
						nw++;
					}
				} else {
					slw++;
				}
			} else {
				sw++;
			}
			ran++;
		}
		printf("Stright   Wins:\t%6d\n",sw);
		printf("Stright L Wins:\t%6d\n",slw);
		printf("No Wins:\t%6d\n",nw);
		printf("Too Easy:\t%6d\n",te);
		printf("Games:\t\t%6d\n",games);
		printf("Hard Games:\t%6d\n",hard_games);
		printf("Total:\t\t%6d\n",ran);
	} else {
		//Run Hands
		for (i = 0; i < hands_to_run;i++) {
			g = Hand_to_Game(hands[i]);
			Set_Game_String(&g);
			Get_Percent(&g);
			if (display_hands) {
				printf("%s Win Percent: %19.15f\n",hands[i],g.odds);
			}
		}

		//Run Hard Hands
		for (i = 0; i < hard_hands_to_run;i++) {
			g = Hand_to_Game(hard_hands[i]);
			Set_Game_String(&g);
			Get_Percent(&g);
			if (display_hands) {
				printf("%s Win Percent: %19.15f\n",hard_hands[i],g.odds);
			}
		}
	}
}
