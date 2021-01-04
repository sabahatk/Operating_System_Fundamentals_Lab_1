#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	
	pid_t pid;
	char command[1000], c[50],readpipe[1];
	char* token;
	char* args[3];
	char * FIFO_PATH = "/home/user/Downloads/EECS_3221/q1/FIFO_PATH";
	int i,execute,fd,checkfifo,rd;
	int counter =0;
	execute=0;	
	
	
	/*creating my named pipe */
	
	checkfifo = mkfifo(FIFO_PATH, 0777);
	if(checkfifo <0){
		printf("Error in mkfifo. Error code : %d\n",checkfifo);
	}
	
	
	printf("Enter a command in the following shell\n");
	printf("SHELL--214191506>>");
	
	/* Grab the complete command and store in one string */
	scanf("%[^\n]%*c", command);
	printf("Command typed: %s \n",command);
	/*Break that string up into the command and arguments */
	token = strtok(command, " ");
	/* Add each token(word) into an array */
	while(token != NULL){
		args[counter] = token;
		counter++;
		token = strtok(NULL, " ");
	}
	args[counter] = NULL;
	counter++;
	printf("Count is = %d\n",counter);
	printf("c value: %s after count\n",c);
	/* c is the string consisting of the command to run */
	
	strcat(c,"/bin/");
	strcat(c,args[0]);
	
	
	printf("c value: %s after while\n",c);
	printf("%s ran...\n",c);
	
	printf("args[0]: %s\n",args[0]);
	printf("args[1]: %s\n",args[1]);
	args[5]="Hello There";
	printf("args[5]: %s\n",args[5]);
	
	fd=open(FIFO_PATH, O_RDONLY);
	/*Open the named pipe to READ in parent */
	if(fd <0){
		fprintf(stderr,"Error");
		printf("Error on open for reading. Error code : %d",fd);
	}
	
	/* fork a child process */
	pid = fork();
	close(fd);
	if(pid < 0 ){ /* error occured creting the process */
		fprintf(stderr,"Fork Failed");	
	}
	else if (pid == 0){/* Child process */
		printf("Child Process Created Successfully!\n");
		/*Open the named pipe to WRITE in child */
		fd=open(FIFO_PATH, O_WRONLY);
		if(fd <0){
			printf("Error on open for writing. Error code : %d",fd);
		}
		
		
		/*Close the stdout so we can execute the command  */
		dup2(fd,1);
		
		/*Close after writing */
		close(fd);
		/* Running the command */
		execute = execvp(args[0],args);
		
		
		
	}
	else{
		/* parent will wait for child to complete */
		/* wait(NULL);*/
		
		printf("Execute return value = %d\n",execute);
		
			
		while(rd >0){
			rd =read(fd,readpipe,1);
			if(rd <0){
				printf("Error in reading from pipe");
				return -1;
				exit(1);
			}
			else if(fd == 0){
				printf("\n");
				break;
			}
			else{
				printf("Pipe value = %s",readpipe);
			}
		}
		/*Close after reading */
		close(fd);
		
		execlp("/bin/rm","rm","-f","myfifo",NULL);
		
		
		printf("PID : %d",pid);
		printf("Child Complete \n");
		
		
	}
	
	
	
return 0;	
}
