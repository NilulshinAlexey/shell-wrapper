#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


int parse_cmd(char cmd[1024], char *args[1024]){

	int  seq_size = 1, i = 0;
	char *delim = "\n ";

	args[0] = strtok(cmd, delim);
	for (i = 1; i < 1024 && (args[i] = strtok(NULL, delim)) != NULL; i++);

	for (int j = 0; j < i; j++){
		if (args[j][0] == '|')
			seq_size++;
	}

	return seq_size;
}

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

		int in  = dup(STDIN_FILENO);
		int out = dup(STDOUT_FILENO);

		int fd[2];

		if (pipe(fd) < 0){
			printf("Pipe Error\n");
			return -1;
		}

		int sequence = parse_cmd(cmd, args);

		for (int i = i; i < sequence; i++){ 

			pid_t pid = fork();

			if (pid < 0){
				printf("Fork error, pid = %d\n", pid);
				return -1;
			}      

			if (pid){
				int status;
				waitpid(pid, &status, 0);

				dup2 (in, STDIN_FILENO);
				dup2 (out, STDOUT_FILENO);

				printf("Ret code: %d\n", WEXITSTATUS(status));
				continue;
			}

			dup2 (fd[0], STDIN_FILENO);
			dup2 (fd[1], STDIN_FILENO);

			if (execvp(args[0], args) == -1){
				printf("Exec error\n");
				return -1;
			}
		}
	}

	return 0;
}
