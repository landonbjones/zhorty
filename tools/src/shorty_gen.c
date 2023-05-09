#define _POSIX_SOURCE
#include "shorty.h"
#include <signal.h>

bool run = true;

void handler(int signum) {
	run = false;
}

int main (int argc, char *argv[]) {
	FILE* fp;
	char* file_name;
    struct sigaction psa;
    psa.sa_handler = &handler;
    sigaction(SIGQUIT, &psa, NULL);

	char hand[53] = "rMjCIsEXfVubxnJcqwYehpHFdTlAzyLmDvgSUWNZKQPaBOGRtkio";
	char query[250];
	int n;

	if (argc == 2) {
		file_name = argv[1];
		fp = fopen(argv[1],"w");
	} else {
		printf("Error: File Name Must Be Specified\n");
		exit(1);
	}

	struct game g;
	Initial_Shuffle(hand);
	while (run) {
		Shuffle(hand);
		g = Hand_to_Game(hand);
		if (!Straight_Win(g)) {
			if (!Straight_Last_Win(g)) {
				if (Get_Percent(&g)) {
					// if (.001 > g.odds) {
						if (g.odds < .001) {
							n = sprintf (query, "INSERT into hard_game (hand,win_percent) values ('%s',%2.15f);\n", hand,g.odds);
							fputs(query,fp);
						}
						// } else {
						// 	n = sprintf (query, "INSERT into game (hand,win_percent) values ('%s',%2.3f);\n", hand,g.odds);
					// }
				}
			}
		}
	}
	fclose(fp);
	char new_name[150];
	n = sprintf (new_name,"%s.sql",file_name);
	n = rename(file_name,new_name);
	exit(0);
}
