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
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

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

void exec_cmd(int argc, char ** argv)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
        perror("Error (pid < 0)");

    else if (pid == 0)
    {
        execvp(argv[0], argv);
        perror("execvp error");
    }

    else 
        waitpid(pid, NULL, 0);
}

int main()
{
    while (1)
    {
        userPrompt();
        
        exec_cmd(argc, argv);

        for (int i =0; i < argc, i++)
            argv[i] = NULL;
    }

    return 0;
}
