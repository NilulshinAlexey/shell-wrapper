#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(){

	while(1){

		char cmd[1024];
		char *args[1024] = {0};

		int size = 0;

		if ((size = read(0, cmd, 1024)) == -1){
			printf("Read error\n");
			return -1;
		}

		cmd[size] = '\0';

		pid_t pid = fork();

		if (pid < 0){
			printf("Fork error, pid = %d\n", pid);
			return -1;
		}      

		if (pid){
			int status;
			waitpid(pid, &status, 0);
			printf("Ret code: %d\n", WEXITSTATUS(status));
			continue;
		}

		else{

			char delim[] = "\n ";

			args[0] = strtok(cmd, delim);
			int i = 0;
			
			for (i = 1; i < 1024 && (args[i] = strtok(NULL, delim)) != NULL; i++);

			args[i] = NULL;

			if (execvp(args[0], args) == -1){
				printf("Exec error\n");
				return -1;
			}
		}
	}
}
