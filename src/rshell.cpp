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

#define DELIMS "#?%"
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

void parse_cmd(char *line, char **cmd)
{
    

}

int main()
{
    char *cmd; //cmd line arg
    char line[MAX]; //input line

    while (1)
    {
        userPrompt();
        fgets(line, MAX, stdin);
        parse_cmd(line, cmd);
        
        if(strcmp(cmd, "exit") == 0)
        {
            cout << "Exiting Shell" << endl;
        }
       
    }   
    return 0;
}
