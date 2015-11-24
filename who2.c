/* who2.c	- read /etc/utmp and list info therein
 *			- suppresses empty records
 *			- formats time nicely
 */

#include <stdio.h>
#include <unistd.h>
#include <utmpx.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

/* #define SHOWHOST */

void showtime(long);
void show_info(struct utmpx *);

int main() {
	struct utmpx utbuf;		/* read info into here */
	int utmpfd;				/* read from this descriptor */
	if ((utmpfd = open(UTMPX_FILE, O_RDONLY)) == -1) {
		perror(UTMPX_FILE);
		exit(1);
	}
	while (read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
		show_info(&utbuf);
	close(utmpfd);
	return 0;
}

/*
 *	show_info()
 *		displays the contents of the utmpx struct
 *		in human readable form
 *		* displays nothing if record has no user name
 */

void show_info(struct utmpx *utbufp) {
	if (utbufp->ut_type != USER_PROCESS)
		return;
	printf("%-8.8s", utbufp->ut_user);	/* the logname */
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);	/* the tty */
	printf(" ");
	showtime(utbufp->ut_tv.tv_sec);		/* display time */
#ifdef SHOWHOST
	if (utbufp->ut_host[0] != '\0')
		printf(" (%s)", utbufp->ut_host);	/* the host */
#endif
	printf("\n");						/* newline */
}

void showtime(long timeval) {
/*
 *	display time in a format fit for human consumption
 *	uses ctime to build a string then picks parts out of it
 *	Note: %12.12s prints a string 12 chars wide and LIMITS
 *	it to 12chars.
 */
	char * cp;				/* to hold address of time		*/
	cp = ctime(&timeval);	/* convert time to string		*/
							/* string looks like			*/
							/* Mon Feb  4 00:46:40 EST 1991	*/
							/* 0123456789012345.			*/
	printf("%12.12s", cp+4);
}
