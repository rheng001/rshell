#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <stdio.h> 
#include <string>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pwd.h>

#define DELIMS " #?%|&;\n\t"
#define MAX 1024

using namespace std;

void userPrompt()
{
    char hostname[256];
    string login;

    login = getlogin();

    if(getlogin() == NULL){
        perror("getlogin()");
    }
    
    if((gethostname(hostname, sizeof(hostname)-1))==-1){
        perror("gethostname()");
    }

    cout << login << "@" << hostname << "$ ";
}

void exec_cmd(char** cmd)
{
    pid_t pid;
    pid = fork();

    int status;

    if (pid < 0)
    {
        perror("fork failed");
        exit(0);
    }

    else if (pid == 0) //child process
    {
        if( execvp(cmd[0], cmd) == -1)
        {
            perror("execvp error");
        }
    }
    else //parent process 
        waitpid(pid, NULL, 0);
}

void parse_cmd(char *line, char **cmd)
{
    int pos = 0;
    char *cmdTok;
    cmdTok = strtok(line, DELIMS);

    while(cmdTok != NULL)
    {
        cout << cmdTok << endl; 
        cmd[pos] = strdup(cmdTok); 
        cmdTok = strtok(NULL, DELIMS);
        pos++;
    }

}

int main()
{ 
    char line[MAX]; //input line
    char* cmd[MAX]; //cmd line arg
    char *readCmd = NULL;
    
    while (1)
    {
        userPrompt(); 
        
        cin.getline(line, MAX); //read input line 

        readCmd = strtok(line, DELIMS);
        
        parse_cmd(readCmd, cmd);

        if (strcmp(readCmd, "exit") == 0)
        {
            cout <<"Exiting Shell" << endl;
            exit(0);
        }
        exec_cmd(cmd); 
    }   
    return 0;
}
