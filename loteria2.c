#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h> // ----------- ?? 



// Capçalera gestors de senyals
void gestor_sigquit(int sig);
void gestor_sigint(int sig);
int pid[5];
int i;

int main(int argc, char *argv[]){
	char s[100];
	int result;
	int results[5];//DEBUGGING
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
	int fds[10][2]; //Canonada on el pare escriurà (pare-fill)

	//creació de canonades 
	for(i =0;i<10;i++){
		if(pipe(fds[i]) == -1){
			perror("Error creación pipe escritura padre");
			exit(-1);
		}
	}
	//Creació de fills amb gestió canonades
	
	for(i = 0;i<5;i++){

		//creació de fills
		pid[i] = fork();
		if(pid[i] == -1){
			perror("Fork failed");
			exit(1);
		}else if(pid[i] == 0){
			
			close(1);
			dup(fds[2*i][1]);
			close(0);
			dup(fds[2*i+1][0]);
			int k;
			for(k=0;k<10;k++){
				close(fds[i][0]);
				close(fds[i][1]);
			}
			execlp("./fill","fill",NULL);
			perror("Error recobriment");
			exit(-1);
		}
	}
	int y;
	for(y=0;y<5;y++){
		close(fds[2*i][1]);
		close(fds[2*i+1][0]);
	}
	
	/*Bucle principal del programa, espera a la senyal SIQUIT o SIGINT, 
	  depenent de la que rebi seguirà la seva execució o executarà el handler*/
	int random;
	int num;
	while(1){
		pause();
		for(i=0;i<5;i++){
			random = rand();
			write(fds[2*i+1][1],(const void*)&random,sizeof(int));
		}
		for(i=0;i<5;i++){
			read(fds[2*i][0],(void *)&results[i],sizeof(int));	
		}
		for(i=0;i<5;i++){
			sprintf(s,"%d",results[i]);
			write(1,s,strlen(s));
		}
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
	char buff[100];
	for(i=0;i<5;i++){
		if(kill(pid[i],SIGTERM) == -1){
			perror("Error enviament SIGQUIT");
			exit(-1);
		}
		wait(NULL);
	}
	sprintf(buff,"Tots els meus fills han acabat\n");
	write(1,buff,strlen(buff));
	exit(0);
	//Reprogramar rutina de tractament
	if(signal(SIGINT,gestor_sigint) == SIG_ERR){
		perror("Signal SIGINT");
		exit(-1);
	} 
}

