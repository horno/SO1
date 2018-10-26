#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void sigterm_handler(int sig);


int main(){

	if(signal(SIGTERM,sigterm_handler)==SIG_ERR){
		perror("Signal SIGTERM \n");
		exit(-1);
	}
	if(signal(SIGINT, SIG_IGN)==SIG_ERR){
		perror("Signal SIGINT\n");
		exit(-1);
	}
	if(signal(SIGQUIT, SIG_IGN)==SIG_ERR){
		perror("Signal SIGQUIT\n");
		exit(-1);
	}

	int llavor;
	int rnd;
	while(1){
		read(0, (void*)&llavor, sizeof(llavor));
		srand(llavor);
		rnd=rand()%10;
		write(1, (void *)&rnd, sizeof(rnd));
	}

}

void sigterm_handler (int sig){
	exit(0);
	if(signal(SIGTERM,sigterm_handler)==SIG_ERR){
		perror("Signal SIGTERM \n");
		exit(-1);
	}
}
