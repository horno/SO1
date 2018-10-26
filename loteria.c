#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void gestor_sigint(int sig);
void gestor_sigquit(int sig);

int main(int llavor){

	int pids[5];
	int sdpare[5][2];
	int sdfill[5][2];
	srand(llavor);
	int returned[5];
	int ran;
	
	if(signal(SIGINT,gestor_sigint)==SIG_ERR){
		perror("Signal SIGINT");
		exit(-1);
	}
	
	if(signal(SIGQUIT,gestor_sigquit)==SIG_ERR){
		perror("Signal SIGQUIT");
		exit(-1);
	}
	
	for(int i=0; i<5; i++){

		//Creem pipe pare-fill i fem tractament d'errors
		if (pipe(sdpare[i])==-1){
			perror("Error en la creació del pipe direcció pare-fill");
			exit(-1);
		}
		//Creem pipe pare-fill i fem tractament d'errors
		if(pipe(sdfill[i])==-1){
			perror("Error en la creació del pipe direcció fill-pare");
			exit(-1);
		}
		//Creem al primer fill		
		pids[i]=fork();
		//Fem tractament de pipes dels fills i realitzem el recubriment als fills.
		if(pids[i]==0){
			//Pipe llegir del pare
			close(0);
			dup(sdpare[i][0]);
			close(sdpare[i][0]);
			close(sdpare[i][1]);
			//Pipe retornar resultat al pare
			close(1);
			dup(sdfill[i][1]);
			close(sdfill[i][1]);
			close(sdfill[i][0]);
			execlp("./fill","fill",NULL);
		}else if(pids[i]>0){
			//Tanquem els descriptors que no farà servir el pare
			close(sdpare[i][0]);
			close(sdfill[i][1]);
			close(0);
		}
	}

	
	while(1){
		pause();
		//Enviem la llabor al procés fill
		for(int i=0;i<5;i++){
			ran=rand();
			write(sdpare[i][1], &ran, sizeof(ran));
		}
		
		//Esperem i llegim el valor random que ens torna el fill
		for(int i=0;i<5;i++){
			read(sdfill[i][0], &returned[i], sizeof(ran));
		}
		
		//Printem el valor que ens torna el fill
		for(int i=0; i<5;i++){
			write(1, &returned[i], sizeof(returned[i]));
		}					
		
	}
	

}


void gestor_sigint(int sig){

	printf("Gestor SIGINT activat");
	if(signal(SIGINT,gestor_sigint)==SIG_ERR){
		perror("Signal SIGINT");
		exit(-1);
	}
	exit(0);
}

void gestor_sigquit(int sig){

	printf("Gestor SIGQUIT activat");
	if(signal(SIGQUIT,gestor_sigquit)==SIG_ERR){
		perror("Signal SIGQUIT");
		exit(-1);
	}

}
