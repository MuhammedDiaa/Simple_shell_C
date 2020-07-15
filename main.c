#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //execvp
#include <sys/types.h> //fork
#include <sys/wait.h>

#define MAX_WORD 15
#define MAX_CHAR 120

void read_line(char line[]){
    int i = 0 ;
    fgets(line,MAX_CHAR,stdin);
    //remove enter
     while(line[i]!='\n')
        i++;
    line[i] ='\0';
}

//Check if the process is to be executed in background or not
int check_background(char line[]){
    char* ret;

    ret = memchr(line, '&', strlen(line));
    if (ret == NULL) return 0;
    else return 1;
}

int process_line(char*str[],char line[]){
    int i =0 ;
    str[i] = strtok(line, " ");

    if(str[i] == NULL) return 1 ;

    while(str[i] != NULL){
        i++;
        str[i] = strtok(NULL," ");
    }
    return 1 ;
}


int read_parse_line(char*str[],char line[]){
    int IsBackground;

    read_line(line);
    IsBackground = check_background(line);
    process_line(str,line);
    return IsBackground;
}

void execute(char* command, char *str[MAX_WORD], int IsBackground){
    pid_t pid, pid_done; //process ID
    int status;

    pid = fork();
//For the CHILD PROCESS pid = 0,
//For PARENT PRROCESS pid > 0,
//pid < 0 if CHILD PROCESS failed to be created.

    if(pid < 0){                                    //No child process created
        printf ("FAILED TO CREATE CHILD PROCESS");
        exit(666);                                  //exit program with error code 666
    } else if (pid == 0){                           //For the child process, execute this
        execvp(command, str);
        exit(0);
    } else {                                        //For the parent process, execute this
        if (IsBackground) pid_done = wait(&status);
    }
}

int main(){
    char *str[MAX_WORD];
    char line[MAX_CHAR];
    int IsBackground;

    printf("$ ");
    while (1){
    IsBackground = read_parse_line(str,line);
    execute(str[0], str, IsBackground);
    printf("$ ");
    }

    return 0;
}
