#include "shorty.h"

#define MAX_RANDOM 1000000
#define MAX_WP 30

int main(int argc, char *argv[]) {
	struct game g;
	int i;
	int random = 0;
	int wp = -1;
	bool test_straight_win = false;
	bool test_straight_last_win = false;
	bool print_hand = false;
	char *hand;
	char* pEnd;

	for (i = 0; i < argc;i++) {
		if (strcmp(argv[i],"r") == 0) {
			i++;
			random = (int)strtol(argv[i],&pEnd,10);
			if (random > MAX_RANDOM) {
				random = MAX_RANDOM;
			}
		} else if (strcmp(argv[i],"sw") == 0) {
			test_straight_win = true;
		} else if (strcmp(argv[i],"slw") == 0) {
			test_straight_last_win = true;
		} else if (strcmp(argv[i],"p") == 0) {
			print_hand = true;
		} else if (strcmp(argv[i],"wp") == 0) {
			i++;
			wp = (int)strtol(argv[i],&pEnd,10);
			if (wp > MAX_WP) {
				wp = MAX_WP;
			}
		} else if (i + 1 == argc) {
			if (strlen(argv[i]) == 52) {
				hand = argv[i];
				g = Hand_to_Game(hand);
			} else {
				printf("Valid hand not supplied.\n");
			}
		}
	}

	if (wp >= 0) {
		char return_string[50];
		i = sprintf(return_string,"%%2.%df\n",wp);

		bool winable = Get_Percent(&g);
		if (winable) {
			printf(return_string,g.odds);
		} else {
			printf("0\n");
		}
	} else if (test_straight_win) {
		printf("%d\n",Straight_Win(g));
	} else if (test_straight_last_win) {
		printf("%d\n",Straight_Last_Win(g));
	} else if (print_hand) {
		Print_Game(&g);
	} else if (random > 0) {
		printf("%u\n",Run_Random (g,random));

	}
}
