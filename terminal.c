/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. Rahil Vasa*/

#include "tokens.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<errno.h>
#define NUM 1024

int flag_com = 0;//Global flag that keeps track of which operands have been run in 1 line. 

int main(int argc, char* argv[])
{
    int mychar = 0;
    char *filename = "";
    char *buf = (char*)malloc(NUM*sizeof(char));//allocated to store the entire command line, which can be parsed later. 
    char **commandLine;//Double pointer to call gettokens and parse command line. 
    char **store = NULL;
    while(mychar != EOF){//Let's code run until you type "control + d"
        if(flag_com == 1){
            flag_com = 0;
        }
        int i = 0;
        if(argv[1] == NULL || argc < 2){
            printf("mysh: ");
        }else if(argc == 2){
            printf("%s: ", argv[1]);
        }else{
            fprintf(stderr, "Error: Usage: mysh [prompt]\n");
            exit(-1);
        }
        while((mychar = getchar()) != '\n'){//mychar variable takes the next string input entered. 
            if(mychar == EOF){
                break;//This breaks the inner while loop if you hit "control + d"
            }
            buf[i++] = mychar;
        }
        buf[i] = '\0';
        commandLine = get_tokens(buf);//Breaking up the command line
        int j = 0;//Gets the value of each of the tokens in the loop. 
        while(commandLine[j] != NULL){//Condition to test the cases to the end of the loop. 
            if(strcmp(commandLine[j], ">") == 0){
                flag_com = 1;//flag initialized to 1 to track running of method. 
                char **commandLine2;
                char bufStore[NUM];
                pid_t pid2 = fork();//Creates a parent as well as a child process. 
                int status;//tracks the status of the wait. 
                if(pid2 == 0){
                    filename = commandLine[j+1];//sets the filename to the token after the ">".
                    int k = 0;
                    while(buf[k] != '>'){
                        bufStore[k] = buf[k];
                        k = k + 1;
                    }
                    bufStore[k] = '\0';
                    int l = 0;
                    while(bufStore[l] != '\0'){
                        if(bufStore[l] != 32){
                            break;
                        }else{
                            fprintf(stderr, "Error: Invalid null command.\n");
                            exit(-1);
                        }
                        l = l + 1;
                    }
                    if(filename == NULL){
                        fprintf(stderr, "Error: Missing filename for input redirection.\n");
                        exit(-1);
                    } 
                    int fdnew = open(filename, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);//creates the file fdnew with these specifications.
                    if(fdnew < 0){
                        if(errno == EEXIST){
                            fprintf(stderr, "Error: open(\"%s\"): %s\n", filename, strerror(errno));
                            exit(-1);
                        }
                            perror("open");
                            exit(-1);
                    }
                    commandLine2 = get_tokens(bufStore);
                    if(dup2(fdnew, 1) < 0){
                        perror("dup2");
                        exit(-1);
                    }
                    close(fdnew);
                    execvp(commandLine[0], commandLine2);
                    perror("exec");
                    exit(-1);
                }else if(pid2 < 0){
                    perror("fork");
                    exit(-1);
                }else{
                    wait(&status);
                    if(status < 0){
                        perror("wait");
                    }
                }
                free_tokens(commandLine2);
            }else if(strcmp(commandLine[j], ">>") == 0){
                flag_com = 1;//flag is hit so that the processes below are not run. 
                char bufStore2[NUM];//Variable to store everything before ">>" in the string. 
                char **commandLine2;//Double pointer to tokenize everything in before the ">>". 
                pid_t pid5 = fork();
                int status;
                if(pid5 == 0){
                    filename = commandLine[j+1];//sets the filename to the token after the ">". 
                    int fdnew = open(filename, O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);//creates the file fdnew with these specifications.
                    int k = 0;
                    int p = 0;
                    while(commandLine[p] != NULL){//loop to get the last index of the command line entered. The one before NULL. 
                        p = p + 1;
                    }
                    if(fdnew < 0){
                        fprintf(stderr, "Error: open(\"%s\"): No such file or directory\n", commandLine[p-1]);
                        exit(-1);
                    }
                    while(buf[k] != '>' && buf[k+1] != '>'){
                        bufStore2[k] = buf[k];
                        k = k + 1;
                    }
                    
                    
                    bufStore2[k] = '\0';
                    commandLine2 = get_tokens(bufStore2);
                    if(dup2(fdnew, 1) < 0){
                        perror("dup2");
                        exit(-1);
                    }
                    close(fdnew);
                    execvp(commandLine[0], commandLine2);
                    perror("exec");
                    exit(-1);
                }else if(pid5 < 0){
                    perror("fork");
                    exit(-1);
                }else{
                    wait(&status);
                    
                    if(status < 0){
                        perror("wait");
                    }
                }
                free_tokens(commandLine2);

            }if(strcmp(commandLine[j], "<") == 0){
                flag_com = 1;//flag initialized to 1 to track running of method. 
                char **commandLine2;
                char bufStore[NUM];
                int childPid = 0;
                pid_t pid2 = fork();//Creates a parent as well as a child process. 
                int status;//tracks the status of the wait. 
                if(pid2 == 0){
                    childPid = getpid();
                    filename = commandLine[j+1];//sets the filename to the token after the ">". 
                    int fdnew = open(filename, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);//creates the file fdnew with these specifications.
                    int k = 0;
                    while(buf[k] != '<'){
                        bufStore[k] = buf[k];
                        k = k + 1;
                    }
                    int p = 0;
                    while(commandLine[p] != NULL){//loop to get the last index of the command line entered. The one before NULL. 
                        p = p + 1;
                    }
                    bufStore[k] = '\0';
                    commandLine2 = get_tokens(bufStore);
                    if(dup2(fdnew, 0) < 0){
                        fprintf(stderr, "Error: open(\"%s\"): No such file or directory\n", commandLine[p-1]);
                        exit(-1);
                    }
                    close(fdnew);
                    execvp(commandLine[0], commandLine2);
                    perror("exec");
                    exit(-1);
                }else if(pid2 < 0){
                    perror("fork");
                    exit(-1);
                }else{
                    wait(NULL);
                    if(status < 0){
                        perror("wait");
                    }
                    
                }
                
            }
            
            j = j + 1;
        }
        if(strlen(buf) != 0){//Condition to see that "buf" is not just the new line indicator. 
            if(strcmp(commandLine[j-1], "&") == 0){//How do you test the working of this, and "<"?
                char **commandLine3;//Q: I am having a lag in the command line with this "&" operator. 
                char bufStore3[NUM];
                flag_com = 1;
                pid_t pid4 = fork();
                if(pid4 == 0){
                    int b = 0;
                    while(buf[b] != '&'){
                        bufStore3[b] = buf[b];
                        b++;
                    }
                    bufStore3[b] = '\0';
                    commandLine3 = get_tokens(bufStore3);
                    execvp(commandLine[0], commandLine3);
                }else if(pid4 < 0){
                    perror("fork");
                    exit(-1);
                }else{
                    continue;
                }
                free_tokens(commandLine3);

            }
        }
         
        pid_t pid = fork();//fork into the child and parent process. 
        if(pid == 0){
            int q = 0;
            int track = 0;
            while(commandLine[q] != NULL){
                if(strcmp(commandLine[q], "|") == 0){
                    flag_com = 1;
                    int fd[2]; 
                    pipe(fd);//creates a read and a write end of the pipe.
                    int c[NUM];
                    char buffStore[NUM];//Stores the lines before "|". 
                    char afterStore[NUM];//Stores the command line after "|".
                    int status;
                    char **commandLine5;
                    char **commandLine6;
                    flag_com = 1;
                    pid_t pid5 = fork();
                    if(pid5 == 0){
                        close(fd[0]);//Closing the read end of the pipe created. 
                        dup2(fd[1], 1);//Making the write end of the pipe point to stdout. 
                        close(fd[1]);
                        int k = 0;
                        while(buf[k] != '|'){
                            buffStore[k] = buf[k];//bufStore now has everything before the "|". 
                            k = k + 1;
                        }
                        buffStore[k] = '\0';
                        commandLine5 = get_tokens(buffStore);
                        execvp(commandLine[0], commandLine5);
                        perror("execvp");
                    }else if(pid5 < 0){
                        perror("fork");
                        exit(-1);
                    }else{
                        wait(&status);
                        close(fd[1]);
                        dup2(fd[0], 0);
                        close(fd[0]);
                        int x = 0;
                        while(buf[x] != '|'){
                            x = x + 1;
                        }
                        int o = 0;
                        for(int t = x + 2; t < strlen(buf); t++){
                            afterStore[o] = buf[t];
                            o++;
                        }
                        afterStore[o] = '\0';
                        commandLine6 = get_tokens(afterStore);
                        execvp(commandLine6[0], commandLine6);
                        perror("execvp");
                        free_tokens(commandLine5);
                        free_tokens(commandLine6);
        
                    }
                }
                q++;   
            }
            if(flag_com != 0){//Condition to see if any of the previous operands are existing in the command line. If yes, do not run this child. 
                exit(-1);
            }
            int execStat = execvp(commandLine[0], commandLine);
            if(execStat == -1){
                fprintf(stderr, "%s: No such file or directory\n", commandLine[0]);
                exit(-1);
            }
        }else if(pid < 0){
            perror("fork");
            exit(-1);
        }else{
            wait(NULL);
        }
    }
    free(buf);
    free_tokens(commandLine);
    return 0;
}

