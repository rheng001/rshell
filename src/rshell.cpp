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

#define DELIMS "#?%|& \t;"
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

    if (pid < 0)
        perror("Error (pid < 0)");

    else if (pid == 0)
    {
        execvp(*cmd, cmd);
        perror("execvp error");
    }

    else 
        waitpid(pid, NULL, 0);
}

int parse_cmd(char *line, char **cmd)
{
    int pos = 0;
    char *cmdTok = NULL;
    cmdTok = strtok(line, DELIMS);
    
    while(cmdTok != NULL)
    {
        cmd[pos] = strdup(cmdTok);
        cmdTok = strtok(NULL, DELIMS);
        pos++;
    }
    return pos;

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

        int args;
        args = parse_cmd(readCmd, cmd);

        if (strcmp(readCmd, "exit") == 0)
        {
            cout <<"Exiting Shell" << endl;
            exit(0);
        }
        exec_cmd(cmd);
        //readCmd = strtok(line, DELIMS);
    }   
    return 0;
}
