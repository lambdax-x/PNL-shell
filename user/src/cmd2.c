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
#include <sys/wait.h>
#include <sys/sysinfo.h>

#define TRUE 1
#define TAILLE 256


char 	*CMD_BRUT;
char 	*CMD;
char 	*TEMP;
char 	*TEMPARG;
int 	ARGC;
char 	**ARGS;
char 	shell_prompt[200];
int 	unknown;
char 	DATA[4*TAILLE];
int	tube[2];
int	async;

void modinfo(int fd, char *module_name, int async){
	struct cmd_params 	params;
	struct cmd_status 	status;
	int 			result;
	int 			r;
	char 			name[50];

	params.asynchronous = async;
	params.status = &status;
	/* Mise en place + allocation */
	params.args.modinfo.name = malloc(sizeof(char)*strlen(module_name));
	params.args.modinfo.sizeName = strlen(module_name);
	params.args.modinfo.result = malloc(sizeof(char)*512);
	params.args.modinfo.sizeRes = 512;
	
	strcpy(params.args.modinfo.name,module_name);
	
	printf("[MODINFO] Nom du module : %s\n",params.args.modinfo.name);
	result = ioctl(fd, IOC_modinfo, &params);
	if ( result == -1 ) {
		perror("ioctl modinfo");
		//exit(errno);
	}
	printf("[MODINFO] status code: %d\n", status.code);
	if ( status.code < 0 ){
		printf("error\n");
		return;
	}
	printf("[MODINFO]\n");
	printf("%s",params.args.modinfo.result);

}

void list(int fd, int async)
{
	struct cmd_params 	params;
	struct cmd_status 	status;
	cmdid_t 		uid[10];
	int r;

	params.asynchronous = async;
	params.status = &status;
	params.args.list.uid = uid;
	params.args.list.size = 10;
	r = ioctl(fd, IOC_list, &params);
	if (r == -1) {
		perror("ioctl list");
		exit(errno);
	}
	printf("status code: %d\n", status.code);
	if (status.code < 0) {
		printf("error\n");
		exit(status.code);
	}
	printf("%u commands:\n", status.res.list.size);
	for (unsigned int i = 0 ; i < status.res.list.size ; ++i)
		printf("uid=%u\n", uid[i]);
}

void kill_fct(int fd, char **args, int async){

	struct cmd_params 	params;
	struct cmd_status 	status;
	int 			result;
	int 			pid;
	int 			signal;
	int 			r;

	params.asynchronous = async;
	params.status = &status;
	params.args.kill.pid = atoi(args[1]);
	params.args.kill.sig = atoi(args[0]);
	result = ioctl(fd, IOC_kill, &params);
	if ( result == -1 ) {
		perror("ioctl modinfo");
		exit(errno);
	}
	printf("[KILL] status code: %d\n", status.code);
	if ( status.code < 0 ){
		printf("[KILL] error\n");
		exit(status.code);
	}
	printf("[KILL] command : %d\n",status.res.kill.status);
}

void meminfo_fct(int fd, int async){

	struct cmd_params 	params;
	struct cmd_status 	status;
	struct sysinfo		info;
	int 			result;

	params.asynchronous = async;
	params.status = &status;
	params.args.meminfo.info = &info;
	result = ioctl(fd, IOC_meminfo, &params);
	if ( result == -1 ) {
		perror("ioctl meminfo");
		exit(errno);
	}
	
	if( status.code == 0){
		printf("[MEMINFO] MemTotal:	  %8lu kB\n", info.totalram);	
		printf("[MEMINFO] MemFree:	  %8lu kB\n", info.freeram);	
		printf("[MEMINFO] Buffers:	  %8lu kB\n", info.bufferram);	
		//printf("[MEMINFO] HighTotal:	  %8lu kB\n", info.totalhigh);	
		//printf("[MEMINFO] FreeHigh:	  %8lu kB\n", info.freehigh);
		printf("[MEMINFO] SwapTotal:	  %8lu kB\n", info.totalswap);
		printf("[MEMINFO] SwapFree:	  %8lu kB\n", info.freeswap);
		printf("[MEMINFO] SharedMem:	  %8lu kB\n", info.sharedram);
		printf("[MEMINFO] Memory unit:	  %8lu kB\n", info.mem_unit);
	}
}

int numberPipes(char** array){
    //Returns number of | in the array
    int i=0;
    int found;
    found=0;
    while (array[i]!= NULL){
        if (strcmp( array[i],"|")==0)
        {
            found++;
        }
        i++;
    }
    return found;

}

void wait_fct(int fd, int argc, char **args, int async){

	struct cmd_params 	params;
	struct cmd_status 	status;
	int 			result;

	params.asynchronous = async;
	params.status = &status;
	params.args.wait.nbPid = argc;
	params.args.wait.PIDS = malloc(sizeof(int)*argc);
	for(int i = 0; i < argc ; i++){
	        params.args.wait.PIDS[i] = atoi(args[i]);
	}
	
	result = ioctl(fd, IOC_wait, &params);
	if ( result == -1 ) {
		perror("ioctl wait");
		exit(errno);
	}
	printf("[WAIT] status code: %d\n", status.code);
	if ( status.code < 0 ){
		printf("[WAIT] error\n");
		exit(status.code);
	}
	printf("[WAIT] PID : %d, VALRET : %d\n"
	        ,status.res.wait.PID
	        , status.res.wait.valRet);
}

int main(int argc, char *argv[]) {
	int fd;

	fd = open("/dev/playground", O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(errno);
	}


	
	// Configure readline to auto-complete paths when the tab key is hit.
	rl_bind_key('\t', rl_complete);

	while (TRUE) {
		unknown = 1;
		async = 0;
		TEMP = malloc(200*sizeof(char));
		
		// Create prompt string from user name and current working directory.
		snprintf(shell_prompt, sizeof(shell_prompt), "%s@CustomSHELL$ ", getenv("USER"));

		// Display prompt and read input (NB: input must be freed after use)...
		CMD_BRUT = readline(shell_prompt);

		// Check for EOF.
		if (CMD_BRUT == NULL){
			free(CMD_BRUT);
			continue;
		}
			

		// Add input to history.
		add_history(CMD_BRUT);
		//memcpy(TEMP,CMD_BRUT,strlen(CMD_BRUT));
		strcpy(TEMP, CMD_BRUT);

                printf("TEMP : %s, taille %d\n",TEMP,strlen(TEMP));

		if(strlen(TEMP) != 0){	// Interprétation de la commande.


			CMD = strtok(TEMP, "\n");
			
			//if(strchr(CMD,'&')){ async = 1;}
			
			CMD = strtok(TEMP, " ");        //CMD 
			printf("CMD : %s, taille %d\n",CMD,strlen(CMD));
			
			
			if(!strcmp(CMD, "list")){
				unknown = 0;
				list(fd,async);
			}
			if(!strcmp(CMD, "fg")){
				unknown = 0;

				printf("inside fg\n");
			}
			if(!strcmp(CMD, "kill")){
				unknown = 0;
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
				
				kill_fct(fd,ARGS,async); // SYNC pour le moment.
			}
			if(!strcmp(CMD, "wait")){
				unknown = 0;
				ARGC = 1;
				printf("PASSAGE 0 \n");
				/* On détermine le nombre d'arguments */
				while ((TEMPARG = strtok(NULL, " "))
				        != NULL) {
					ARGC++;
				}
				ARGS = (char**) malloc(sizeof(char*)*(ARGC+3));
				printf("PASSAGE 1 \n");
				//Mise en place des paramètres
				strcpy(TEMP, CMD_BRUT);
				TEMPARG = strtok(TEMP, " ");	//On retire le premier élement.	
				int i = 0;
				while ((TEMPARG = strtok(NULL, " ")) != NULL) {
					ARGS[i] = strdup(TEMPARG);
					i++;
				}
				ARGS[i] = NULL;
				
				/* APL CMD */
				printf("PASSAGE\n");
				wait_fct(fd,i,ARGS,async);
				
				for(int j=0; j<i;j++){
				        printf("%d : %s\n",j,ARGS[j]);
					free(ARGS[j]);	
				}
				free(ARGS);
			}
			if(!strcmp(CMD, "meminfo")){
				unknown = 0;
				printf("inside meminfo\n");
				meminfo_fct(fd,async);
			}
			if(!strcmp(CMD, "modinfo")){
				unknown = 0;
				//printf("inside modinfo\n");

				ARGS = malloc(sizeof(char*));
				int i = 0;
				while ((TEMPARG = strtok(NULL, " ")) != NULL) {
					ARGS[i] = strdup(TEMPARG);
					//malloc(sizeof(TEMPARG));
					//strcpy(ARGS[i], TEMPARG);
					i++;
				}

				//Mise en place et exécution 
				for(i = 0; i< ARGC;i++){
					printf("%d : %s\n", i, ARGS[i]);
				}


				modinfo(fd, ARGS[0], async);
			}

			if(!strcmp(CMD, "hello")){
				
			}
			if(!strcmp(CMD, "exit")){
				unknown = 0;
				exit(EXIT_SUCCESS);
			}
			//printf("unknown : %d\n",unknown);
			if(unknown  == 1 && async == 0 ){
				//Exécution d'une commande classique et attente du résultat	
				//strcpy(TEMP, CMD_BRUT);
				int number = 0;//numberPipes(TEMP);
				
				if(number == 0){
					ARGC = 1;
				
					/* On détermine le nombre d'arguments */
					while ((TEMPARG = strtok(NULL, " "))
					        != NULL) {
						ARGC++;
					}
				
					ARGS = (char**) malloc(sizeof(char*)*(ARGC+3));
				
					//Mise en place des paramètres
					strcpy(TEMP, CMD_BRUT);
					TEMPARG = strtok(TEMP, " ");	//On retire le premier élement.	
				
				
					ARGS[0] = strdup(CMD);
					int i = 1;
					while ((TEMPARG = strtok(NULL, " ")) != NULL) {
						ARGS[i] = strdup(TEMPARG);
						i++;
					}
					ARGS[i] = NULL;
								
					/* pipe pour le fils */
					pipe(tube);
					if(fork() == 0){
						dup2(tube[1], STDOUT_FILENO);
						dup2(tube[1], STDERR_FILENO); 
						close(tube[0]);
						close(tube[1]);
						execvp(CMD, ARGS);
						perror("error");
						exit(EXIT_FAILURE);
					}
					close(tube[1]);
					wait(NULL); //Attente du fils
				
					/* Récupération du résultat */
					while(read(tube[0], DATA, 4*TAILLE)>0){				
						printf("%s",DATA);
						memset(DATA, 0, 4*TAILLE);
					}
					close(tube[0]);
				
					/* Libération des arguments */
					for(int j=0; j<i;j++){
						free(ARGS[j]);	
					}
					free(ARGS);
				}else{
					
					// A FAIRE 
					
					
					
					
				}
				
				
			}else if(unknown  == 1 && async == 1  ){	// Asynchronous
			
				printf("ASYNCHRONOUS");
			
				//strcpy(TEMP, CMD_BRUT);
				//numberPipes(TEMP);
			
			
			
			}
			
			
					
		}
		// Free input.
		free(CMD_BRUT);
		//printf("CMD_BRUT\n");
		free(TEMP);
		//printf("TEMP\n");
		
		
	}
	
}
