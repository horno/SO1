#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

	int llavor;
	read(0, &llavor, sizeof(llavor));
	srand(llavor);
	int rnd=rand()%10;
	write(1, &rnd, sizeof(rnd));

	return (0);
}
