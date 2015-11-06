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
#include <cstring>

#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

#define DELIMS " ?%;\n\t"
#define MAX 1024

#define FLAG_o 1 //ors flag
#define FLAG_a 2 // and flag
#define FLAG_s 3 //semi flag

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

void exec_cmd(char** cmd, int flag)
{
   // int status;
    int pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        exit(0);
    }

    else if (pid == 0) //child process
    {
        if( execvp(cmd[0], cmd) == -1)
        {
            perror("execvp error");
            exit(0);
        }
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)//parent process 
    {
        int status = 0;
        //wait(NULL);
         
        if(wait(&status) == -1)
        {
            perror("Wait");
            exit (1);
        }
    
        bool successful = WIFEXITED(status);
        int exit = WEXITSTATUS(status);

        if(flag == 2)
        {
            cout <<"You put an &&" << endl;
            return;
        }
    }

}

void parse_cmd(string &line, int flag)
{

    char *cmd[MAX]; //cmd line
    char_separator<char> sep(DELIMS); //delim 
    tokenizer<char_separator<char> > tok(line,sep); //tokenizer 
   
    int i = 0;
    
    for(tokenizer<char_separator<char> >::iterator it = tok.begin(); it!=tok.end(); ++it, ++i) 
    {
        cmd[i] = new char [(*it).size()];
        strcpy(cmd[i], (*it).c_str());   
    }

    cmd[i] = 0;
    exec_cmd(cmd, flag);
}

int main()
{ 
    string line; //input line
    int flags = 0;

    while (1)
    {
        userPrompt(); //prompt
        getline(cin, line); //read input line   

        if(line.find("#") != string::npos) //handling comments
        {
            continue;
        }

        if(line.find("&&") != string::npos)
        {
            flags |= FLAG_a; 
        }

        else if(line.find("||") != string::npos)
        {
            flags |= FLAG_o;
        }
        
        else if(line.find(";") != string::npos)
        {
            flags |= FLAG_s;
        }
                 
        if(line == "exit") //Exits shell
        {
            cout <<"Exiting shell" << endl;
            exit(0);
        }

        parse_cmd(line, flags); //parse start
    }   
    return 0;
}
