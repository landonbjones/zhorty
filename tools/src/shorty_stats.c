#define _POSIX_SOURCE
#include "shorty.h"
#include <signal.h>
#include <time.h>

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
    bool straight_win, straight_last_win, winable;

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
		clock_t t; 
	    t = clock();
		winable = Get_Percent(&g); 
		t = clock() - t; 
		double solve_time = ((double)t)/CLOCKS_PER_SEC; // in seconds 
	
		if (winable) {
			straight_win = Straight_Win(g);
			straight_last_win = Straight_Last_Win(g);
		} else {
			straight_win = false;
			straight_last_win = false;
		}
		n = sprintf (query, "INSERT into game_stats (hand,win_percent,winable,straight_win,straight_last_win,solve_time) "
		 					    "values ('%s',%2.15f,%s,%s,%s,%5.6f);\n",\
								hand,g.odds,winable ? "true" : "false",straight_win ? "true" : "false",\
								straight_last_win  ? "true" : "false",solve_time);
		fputs(query,fp);
	}
	fclose(fp);
	char new_name[150];
	n = sprintf (new_name,"%s.sql",file_name);
	n = rename(file_name,new_name);
	exit(0);
}
