#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h> // ----------- ?? 



// Capçalera gestors de senyals
void gestor_sigquit(int sig);
void gestor_sigint(int sig);
char s[100];
int pid[5];
int i;

int main(int argc, char *argv[]){
	//Control de llavor nula
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
	//Creació descriptors de canonades
	int pipefdW[5][2]; //Canonada on el pare escriurà (pare-fill)
	int pipefdR[5][2]; //Canonada on el pare llegirà (fill-pare)

	
	//Creació de fills amb gestió canonades
	
	for(i = 0;i<5;i++){
		//creació de canonades 
		if(pipe(pipefdW[i]) == -1){
			perror("Error creación pipe escritura padre");
			exit(-1);
		}
		if(pipe(pipefdR[i]) == -1){
			perror("Error creación pipe lectura padre");
			exit(-1);
		}
		
		//creació de fills
		pid[i] = fork();
		int k;
		if(pid[i] == -1){
			perror("Fork failed");
			exit(1);
		}else if(pid[i] == 0){
			
			close(0);
			dup(pipefdW[i][0]);
			close(1);
			dup(pipefdR[i][1]);
			close(pipefdW[i][0]);
			close(pipefdR[i][1]);
			for(k = 0;k<=i;k++){
				close(pipefdR[k][0]);
				close(pipefdW[k][1]);
			}
			execlp("./fill","fill",NULL);
			perror("Error recobriment");
			exit(-1);
		}else{
			close(pipefdW[i][0]);
			dup(pipefdR[i][0]);
			close(pipefdR[i][0]);
			close(pipefdR[i][1]);
		}
	
	}
	
	//Bucle principal del programa, espera a la senyal SIQUIT o SIGINT, depenent de la que rebi seguirà la seva execució o executarà el handler
	while(1){
		pause();
		
		//-------------> DEBUGGING
		printf("CTRL + 4\n");
		
		sprintf(s,"Ha arribat!\n");
		
		write(pipefdW[0][1],s,strlen(s));
		perror("hello");
	}
	


}
void gestor_sigquit(int sig){
	
	//Reprogramar rutina de tractament
	if(signal(SIGQUIT,gestor_sigquit) == SIG_ERR){
		perror("Signal SIGQUIT");
		exit(-1);
	}
	
}
void gestor_sigint(int sig){
	for(i=0;i<5;i++){
		if(kill(pid[i],SIGTERM) == -1){
			perror("Error enviament SIGQUIT");
			exit(-1);
		}
		wait(NULL);
	}
	sprintf(s,"Tots els meus fills han acabat\n");
	write(1,s,strlen(s));
	exit(0);
	//Reprogramar rutina de tractament
	if(signal(SIGINT,gestor_sigint) == SIG_ERR){
		perror("Signal SIGINT");
		exit(-1);
	} 
}


























