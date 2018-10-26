#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void gestor_sigint(int sig);
void gestor_sigquit(int sig);
int i;
int pids[5];

int main(int llavor){

	char buf[100];
	int sdpare[5][2];
	int sdfill[5][2];
	srand(llavor);
	int returned[5];
	int ran;
	int num;
	int result;
	
	if(signal(SIGINT,gestor_sigint)==SIG_ERR){
		perror("Signal SIGINT ERROR \n");
		exit(-1);
	}
	
	if(signal(SIGQUIT,gestor_sigquit)==SIG_ERR){
		perror("Signal SIGQUIT ERROR \n");
		exit(-1);
	}
	
	for(i=0; i<5; i++){
	//Creem pipe pare-fill i fem tractament d'errors
		if (pipe(sdpare[i])==-1){
			perror("Error en la creació del pipe direcció pare-fill \n");
			exit(-1);
		}
		//Creem pipe pare-fill i fem tractament d'errors
		if(pipe(sdfill[i])==-1){
			perror("Error en la creació del pipe direcció fill-pare \n");
			exit(-1);
		}
	}
	for(i=0; i<5; i++){
		//Creem al primer fill		
		pids[i]=fork();
		//Fem tractament de pipes dels fills i realitzem el recubriment als fills.
		if(pids[i]==0){
			close(0);
			dup(sdpare[i][0]);
			close(1);
			dup(sdfill[i][1]);
			for(i=0;i<5;i++){
				close(sdpare[i][0]);
				close(sdpare[i][1]);
				//Pipe retornar resultat al pare
				close(sdfill[i][1]);
				close(sdfill[i][0]);
			}
			execlp("./fill","fill",NULL);
		}else if(pids[i]<0){
			perror("Error en la creació del fill \n");
			exit(-1);
		}
	}
	//Tanquem els descriptors que no farà servir el pare
	for(i=0;i<5;i++){
		close(sdpare[i][0]);
		//Pipe retornar resultat al pare
		close(sdfill[i][1]);
	}
	
	while(1){
		pause();
		//Enviem la llabor al procés fill
		for(i=0;i<5;i++){
			ran=rand();
			write(sdpare[i][1], (void*)&ran, sizeof(ran));
		}
		
		//Esperem i llegim el valor random que ens torna el fill
		result=0;
		for(i=0;i<5;i++){
			read(sdfill[i][0], (void*)&num, sizeof(num));
			result=result*10+num;
			
		}
		//Printem el valor que ens torna el fill
		sprintf(buf, "Taula> Número Premiat: %d\n", result);
		write(1, buf, strlen(buf));				
		
	}
	

}


void gestor_sigint(int sig){
	
	if(signal(SIGINT,gestor_sigint)==SIG_ERR){
		perror("Signal SIGINT ERROR \n");
		exit(-1);
	}
	for(i=0; i<5;i++){
		if(kill(pids[i], SIGTERM)==-1){
			perror("Error al enviar la senyal SIGTERM \n");
			exit(-1);
		}
		wait(NULL);		
	}
	char s[100];
	sprintf(s,"S'han tancat tots els fills \n");
	write(1, s, strlen(s));
	exit(0);
}

void gestor_sigquit(int sig){
	char s[100];
	sprintf(s,"(S'ha pitjat Cntrl+4)\n");
	write(1, s, strlen(s));
	if(signal(SIGQUIT,gestor_sigquit)==SIG_ERR){
		perror("Signal SIGQUIT  \n");
		exit(-1);
	}

}
