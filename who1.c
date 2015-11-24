/* who1.c - a first version of the who program
 *		open, read UTMP file, and show results
 */

#include <stdio.h>
#include <utmpx.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define SHOWHOST	/* include remote machine on output */

void show_info(struct utmpx *);

int main() {
	struct utmpx current_record;	/* read info into here		*/
	int	utmpfd;					/* read from this descriptor*/
	int reclen = sizeof(current_record);
	if ((utmpfd = open(UTMPX_FILE, O_RDONLY)) == -1) {
		perror(UTMPX_FILE);		/* UTMP_FILE is in utmp.h	*/
		exit(1);
	}
	while (read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;
}

/*
 * show_info()
 *		displays contents of the utmp struct in human readable form
 *		*note* these sizes should not be hardwired
 */

void show_info(struct utmpx *utbufp) {
	printf("%-8.8s", utbufp->ut_user);		/* the logname */
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);		/* the tty */
	printf(" ");
	printf("%10ld", utbufp->ut_tv.tv_sec);		/* login time */
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", utbufp->ut_host);		/* the host */
#endif
	printf("\n");
}
