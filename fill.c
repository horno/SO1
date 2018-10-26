#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/* Signal manager headers */
void gestor_sigterm(int sig);
void gestor_sigquit(int sig);
/* Main function */
int main(){
	
	int seed;
	int random;
	/* Initialization of signal managers */
	if(signal(SIGTERM,gestor_sigterm) == SIG_ERR){
		perror("Signal SIGTERM child");
		exit(-1);
	}
	if(signal(SIGINT,SIG_IGN) == SIG_ERR){	/* SIGINT spreads, so we have to ignore it as we */
		perror("Signal SIGINT child");	/* are using it in the father process */
		exit(-1);
	}
	if(signal(SIGQUIT,SIG_IGN) == SIG_ERR){	/* Ignores SIGQUIT too in case */
		perror("Signal SIGQUIT child");
		exit(-1);
	}
	/* Main loop */
	while(1){
		read(0,(void *)&seed,sizeof(seed));		/* Reads the seed from the father, */
		srand(seed);					/* generates a random number and */
		random = rand()%10;				/* sends it back to the father */
		write(1,(void *)&random,sizeof(random));
	}
}
/* Handler functions */
void gestor_sigterm(int sig){
	
	exit(0);

	/* Reschedules management routine */
	if(signal(SIGTERM,gestor_sigterm) == SIG_ERR){
		perror("Signal SIGTERM child");
		exit(-1);
	}	
}



