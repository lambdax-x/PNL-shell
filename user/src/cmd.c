#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <cmd/def.h>
#define TRUE 1



char 	*CMD_BRUT;
char 	*CMD;
char 	*TEMP;
char 	*TEMPARG;
int 	ARGC;
char 	**ARGS;
char 	shell_prompt[200];


int main(int argc, char *argv[]) {
	
	TEMP = malloc(200*sizeof(char));
	
	// Configure readline to auto-complete paths when the tab key is hit.
	rl_bind_key('\t', rl_complete);

	while (TRUE) {		
		// Create prompt string from user name and current working directory.
		snprintf(shell_prompt, sizeof(shell_prompt), "%s$ ", getenv("USER"));

		// Display prompt and read input (NB: input must be freed after use)...
		CMD_BRUT = readline(shell_prompt);
		
		// Check for EOF.
		if (!CMD_BRUT)
			continue;
		
		// Add input to history.
		add_history(CMD_BRUT);
		strcpy(TEMP, CMD_BRUT);

		if(strlen(TEMP) != 0){	// Interprétation de la commande.
			
			printf("CMD_BRUT : %s\n",CMD_BRUT);
			printf("TEMP : %s\n", TEMP);
						
			//Traitement de la chaine input
			CMD = strtok(TEMP, "\n");
					
			if(!strcmp(CMD, "list")){
				printf("inside list\n");
			}
			if(!strcmp(CMD, "fg")){
				
				
				printf("inside fg\n");
			}
			if(!strcmp(CMD, "kill")){
				
				ARGS = malloc(sizeof(char*)*2);
				int i = 0;
				while ((TEMPARG = strtok(NULL, " ")) != NULL) {
					ARGS[i] = malloc(sizeof(TEMPARG));
					strcpy(ARGS[i], TEMPARG);
					i++;
				}
				
				//Mise en place et exécution 
				for(i = 0; i< ARGC;i++){
					printf("%d : %s\n", i, ARGS[i]);
				}
				
				
				
				printf("inside kill\n");
			}
			if(!strcmp(CMD, "wait")){
				
				// Récupération du nombre de paramètres pour cette commande.
				printf("CMD_BRUT %s\n",TEMP);
				while ((TEMPARG = strtok(NULL, " ")) != NULL) {
					//printf("%d : %s\n", ARGC, TEMPARG);
					ARGC++;
				}
				
				ARGS = malloc(sizeof(char*)*ARGC);
				//Mise en place des parémètres dans un tableau de chaine de caractères
				int i = 0;
				strcpy(TEMP, CMD_BRUT);
				TEMPARG = strtok(TEMP, " ");	//On retire le premier élement.
				while ((TEMPARG = strtok(NULL, " ")) != NULL) {
					ARGS[i] = malloc(sizeof(TEMPARG));
					strcpy(ARGS[i], TEMPARG);
					i++;
				}
				
				//Mise en place et exécution 
				for(i = 0; i< ARGC;i++){
					printf("%d : %s\n", i, ARGS[i]);
				}
					
				
				printf("inside wait\n");
			}
			if(!strcmp(CMD, "meminfo")){
				printf("inside meminfo\n");
			}
			if(!strcmp(CMD, "modinfo")){
				printf("inside modinfo\n");
			}
			
			if(!strcmp(CMD, "hello")){
				
				printf("inside hello\n");
				
				int fd, r;
				struct cmd_hello_args args;
				struct cmd_status status;
				//Module
				fd = open("/dev/playground", O_RDWR);
				if (fd == -1) {
					perror("open");
					exit(errno);
				}
				
				args.asynchronous = 0;
				args.status = &status;	
				args.val = 23;	
				r = ioctl(fd, IOC_hello, &args);	//Si ioctl fonctionne
				if (r == -1) {
					perror("ioctl hello");
					exit(errno);
				}

				printf("status code: %d\n", status.code);	// résultat de la cmd (handler)
				printf("hello ret: %d\n", status.res.hello.val);	//Valeur que l'on souhaite retourner

				close(fd);
				
			}
					
		}
		
		// Free input.
		free(CMD_BRUT);
		//Libération du ARGS
		free(ARGS);
	}
	
}