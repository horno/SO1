#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


void gestor_sigterm(int sig);
//void gestor_sigquit(int sig);

int main(){
	
	char s[100];
	int seed;
	int num;
	int random;
	if(signal(SIGTERM,gestor_sigterm) == SIG_ERR){
		perror("Signal SIGTERM child");
		exit(-1);
	}
	if(signal(SIGINT,SIG_IGN) == SIG_ERR){
		perror("Signal SIGINT child");
		exit(-1);
	}
	if(signal(SIGQUIT,SIG_IGN) == SIG_ERR){
		perror("Signal SIGQUIT child");
		exit(-1);
	}
	
	while(1){
		read(0,(void *)&seed,sizeof(seed));
		srand(seed);
		random = rand()%10;
		write(1,(void *)&random,sizeof(random));
	}
	


}
void gestor_sigterm(int sig){
	
	exit(0);

	//Reprogramar rutina de tractament
	if(signal(SIGTERM,gestor_sigterm) == SIG_ERR){
		perror("Signal SIGTERM child");
		exit(-1);
	}	
	

}
/*void gestor_sigquit(int sig){
	//Reprogramar rutina de tractament
	if(signal(SIGQUIT,gestor_sigquit) == SIG_ERR){
		perror("Signal SIGQUIT child");
		exit(-1);
	}


}*/

/*read(0,(void *)&seed,sizeof(int));
		srand(seed);
		random = rand()%10;
		sprintf(s,"%d\n",random);
		write(2,s,strlen(s));
		write(1,(void *)&random,sizeof(int));*/








