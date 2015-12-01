/* sigdemo1.c - show how a signal handler works.
 *			  - run this and press Ctrl-C a few times
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void f(int signum) {
	printf("OUCH!\n");
}

int main() {
	void f(int);	/* declare the handler	*/
	int i;
	signal(SIGINT, f);	/* install the handler	*/
	for (i = 0; i < 5; i++) {
		printf("hello\n");
		sleep(1);
	}
	return 0;
}
