#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

extern int errno;

//asynchronous signal safe
volatile sig_atomic_t sigusr1 = false;

void sigusr1Handler (int signal_number){
	if (signal_number == SIGUSR1)
		sigusr1 = true;
}


int main(){

	pid_t pid;

	pid = fork();

	if(pid > 0){ //parent

		signal(SIGUSR1, sigusr1Handler);
		
		while (!sigusr1)
			pause();
		
		printf("SIGUSR1 was raised!\n");
		exit(EXIT_SUCCESS);
		
	}else if (pid == 0){ //child
	
	 	kill(getppid(), SIGUSR1);
	 	exit(EXIT_SUCCESS);

	}else{ //error
	
		fprintf(stderr, "Fork failed: %s", strerror(errno));
		//perror("Fork failed");	
		exit(EXIT_FAILURE);
	}

	return 0;
}
