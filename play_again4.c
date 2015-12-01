/* play_again4.c
 *		purpose: ask if user wants another transaction
 *		 method: set tty into chr-by-chr, no-echo mode
 *				 set tty into no-delay mode
 *				 read char, return result
 *				 resets terminal modes on SIGINT, ignores SIGQUIT
 *		returns: 0=>yes, 1=>no, 2=>timeout
 *		 better: reset terminal mode on Interrupt
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

#define ASK "Do you want another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')

void ctrl_c_handler(int);
void set_cr_noecho_mode();
void set_nodelay_mode();
void tty_mode(int);
int get_ok_char();
int get_response(char *, int);

int main() {
	int response;
	void ctrl_c_handler(int);

	tty_mode(0);
	set_cr_noecho_mode();
	set_nodelay_mode();
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	response = get_response(ASK, TRIES);
	tty_mode(1);
	return response;
}

int get_response(char *question, int maxtries) {
	/* 
	 * purpose: ask a question and wait for a y/n answer or timeout
	 *  method: use getchar and complain about non-y/n input
	 * returns: 0=>yes, 1=>no
	 */
	int input;
	printf("%s (y/n)?", question);
	fflush(stdout);
	while(1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if (input == 'y') return 0;
		if (input == 'n') return 1;
		if (maxtries-- == 0) return 2;
		BEEP;
	}
}

int get_ok_char() {
	int c;
	while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

void set_cr_noecho_mode() {
	/* 
	 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
	 *  method: use bits in termios
	 */
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;
	ttystate.c_lflag	&= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void set_nodelay_mode() {
	/*
	 * purpose: put file descriptor 0 into no-delay mode
	 *  method: use fcntl to set bits
	 *   noths: tcsetattr() will do something similar, but it is complicated
	 */
	int termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}

void tty_mode(int how) {
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;
	if (how == 0) {
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	} else {
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}

void ctrl_c_handler(int signum) {
	/*
	 * purpose: called if SIGINT is detected.
	 *  action: reset tty and scram
	 */
	tty_mode(1);
	exit(1);
}
