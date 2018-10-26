#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>



/* Signal manager headers */
void gestor_sigquit(int sig);
void gestor_sigint(int sig);
void gestor_sigpipe(int sig);
/* Declaration of global variables */
int pid[5];	/* Child PID storage */
int fdR[5][2]; /* Father reading pipes, writing child pipes */
int fdW[5][2]; /* Father writing pipes, reading child pipes */	

/* Main function */
int main(int argc, char *argv[]){

	char s[100];	/* Writing buffer */
	int result;
	int i;	/* Iterator variable */

	/* NULL seed control */
	if(argv[1] == NULL){
		perror("No s'ha passat cap paràmetre.");
		exit(-1);
	}
	int seed = atoi(argv[1]);
	srand(seed);

	/* Initialization of signal managers */
	if(signal(SIGQUIT,gestor_sigquit) == SIG_ERR){
		perror("Signal SIGQUIT");
		exit(-1);
	}
	if(signal(SIGINT,gestor_sigint)==SIG_ERR){
		perror("Signal SIGINT");
		exit(-1);
	}	
	if(signal(SIGPIPE,gestor_sigpipe) == SIG_ERR){
		perror("Signal SIGPIPE");
		exit(-1);
	}	

	/*Pipe creation*/
	for(i = 0;i<5;i++){
		if(pipe(fdR[i]) == -1){
			perror("Error creación pipe ");
			exit(-1);		
		}
		if(pipe(fdW[i]) == -1){
			perror("Error creación pipe");
			exit(-1);		
		}
	}
	/*Child creation, child pipe management and exec*/
	for(i = 0; i<5;i++){
		pid[i] = fork();
		if(pid[i] == -1){
			perror("Fork failed");
			exit(1);
		}else if(pid[i] == 0){
			close(0);	    /* Assign child stdin and stdout */
			dup(fdW[i][0]);	    /* to the pertinent pipes */
			close(1);
			dup(fdR[i][1]);
			int k;
			for(k=0;k<5;k++){
				close(fdW[k][0]);
				close(fdW[k][1]);  /* Close child unused pipes */
				close(fdR[k][0]);
				close(fdR[k][1]);
			}
			execlp("./fill","fill",NULL);
			perror("Error recobriment");
			exit(-1);
		}
	}
	/* Close father unused pipes */
	for(i=0;i<5;i++){
		close(fdW[i][0]);
		close(fdR[i][1]);
	}
	
	/* Main loop, waits to SIGQUIT or SIGINT signal, SIGQUIT will keep the loop 
	   going, SIGINT will end the program */
	int random;
	int num=0;
	while(1){
		pause();
		result = 0;	/* Puts result and num back to 0  */
		num = 0;	/* to avoid result overlaping     */
		for(i=0;i<5;i++){
			random = rand();
			write(fdW[i][1],(const void*)&random,sizeof(random));	
		}
		for(i=0;i<5;i++){
			read(fdR[i][0],(void*)&num,sizeof(num));
			result = result*10 + num;
			
		}
		sprintf(s,"Taula> Número premiat: %d\n",result);
		write(1,s,strlen(s));
	}
		
	//Being here is an error
	perror("Unreachable position: before while");
	exit(-1);


}
void gestor_sigquit(int sig){
	char buff[100];
	sprintf(buff,"(S'ha pitjat CTRL+4)\n");
	write(1,buff,strlen(buff));
	//Reprogramar rutina de tractament
	if(signal(SIGQUIT,gestor_sigquit) == SIG_ERR){
		perror("Signal SIGQUIT");
		exit(-1);
	}
	
}
void gestor_sigint(int sig){
	char buff[100];
	sprintf(buff,"(S'ha pitjat CTRL+C)\n");
	write(1,buff,strlen(buff));
	int i; 	/*Iterator variable*/
	

	for(i=0;i<5;i++){
		if(kill(pid[i],SIGTERM) == -1){
			perror("Error enviament SIGTERM");
			exit(-1);
		}
		wait(NULL);
	}
	/*Close father pipes*/
	for(i=0;i<5;i++){
		close(fdW[i][0]);
		close(fdW[i][1]);
		close(fdR[i][0]);
		close(fdR[i][1]);	

	}
	sprintf(buff,"Tots els meus fills han acabat\n");
	write(1,buff,strlen(buff));
	exit(0);
	/*Reschedule management routine*/
	if(signal(SIGINT,gestor_sigint) == SIG_ERR){
		perror("Signal SIGINT");
		exit(-1);
	} 
}
void gestor_sigpipe(int sig){
	perror("S'ha escrit en una tuberia tancada.");
	exit(-1);
	/*Reschedule managment routine*/
	if(signal(SIGPIPE,gestor_sigpipe) == SIG_ERR){
		perror("Signal SIGPIPE");
		exit(-1);
	}

}









