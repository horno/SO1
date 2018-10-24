#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h> // ----------- ?? 



// Capçalera gestors de senyals
void gestor_sigquit(int sig);
void gestor_sigint(int sig);

int main(int argc, char *argv[]){
	//Creació buffer, vector de pids i control de llavor nula
	char s[100];
	int i;
	if(argv[1] == NULL){
		perror("No s'ha passat cap paràmetre.");
		exit(-1);
	}
	int seed = atoi(argv[1]);
	srand(seed);
	//Inicialitzar senyals amb pertinent control d'errors
	if(signal(SIGQUIT,gestor_sigquit) == SIG_ERR){
		perror("Signal SIGQUIT");
		exit(-1);
	}
	if(signal(SIGINT,gestor_sigint)==SIG_ERR){
		perror("Signal SIGINT");
		exit(-1);
	}	
	//Creació de canonades
	int pipefdW[5][2]; //Canonada on el pare escriurà (pare-fill)
	int pipefdR[5][2]; //Canonada on el pare llegirà (fill-pare)

	
	//Creació de fills amb gestió canonades
	int pid[5];
	for(i = 0;i<5;i++){
		pid[i] = fork();
		
		if(pid[i] == -1){
			perror("Fork failed");
			exit(1);
		}else if(pid[i] == 0){
			
			close(0);
			dup(pipefdW[i][0]);
			close(1);
			dup(pipefdR[i][1]);
			close(pipefdW[i][0]);
			close(pipefdW[i][1]);
			close(pipefdR[i][0]);
			close(pipefdR[i][1]);
			//-----------> Aquí falta el recobriment
		}else{
			close(pipefdW[i][0]);
			close(pipefdR[i][1]);
			dup(pipefdW[i][1]);
			dup(pipefdR[i][0]);
			close(pipefdW[i][1]);
			close(pipefdR[i][0]);
		}
	
	}
	//Bucle principal del programa, espera a la senyal SIQUIT o SIGINT, depenent de la que rebi seguirà la seva execució o executarà el handler
	/*while(1){
		pause();
		printf("I made it here\n");
	}*/
	


}
void gestor_sigquit(int sig){
	// ---------------------> Més endavant es programarà l'acabament del programa
	//Reprogramar rutina de tractament
	if(signal(SIGQUIT,gestor_sigquit) == SIG_ERR){
		perror("Signal SIGQUIT");
		exit(-1);
	}
	
}
void gestor_sigint(int sig){
	//Reprogramar rutina de tractament
	if(signal(SIGINT,gestor_sigint) == SIG_ERR){
		perror("Signal SIGINT");
		exit(-1);
	} 
}


























