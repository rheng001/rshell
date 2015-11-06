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

#define MAX 256

void userPrompt()
{
    char hostname[MAX];
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

void exec_cmd(string &line, const char * conn)
{
    int i = 0; //counter
    char *cmd[MAX]; //cmd line
    char_separator<char> delim(" "); //delim 
    tokenizer<char_separator<char> > tok(line,delim); //tokenizer 
    
    for(tokenizer<char_separator<char> >::iterator it = tok.begin(); it!=tok.end(); ++it, ++i) 
    {
        cmd[i] = new char [(*it).size()];
        strcpy(cmd[i], (*it).c_str());   
    }

    cmd[i] = 0;
    if(-1 == execvp(cmd[0], cmd))
    {
         perror("execvp error");
         exit(0);
    } 
}


void make_cmd(string &line, const char * conn)
{
    char_separator<char> delim(conn); //delim 
    tokenizer<char_separator<char> > tok(line,delim); //tokenizer 
    
    for(tokenizer<char_separator<char> >::iterator it = tok.begin(); it!=tok.end(); ++it)
    {
        pid_t pid;

        pid = fork();

        if (pid == -1)
        {
            perror("fork()");
            exit(0);
        }

        else if (pid == 0) //child process
        {
            string cm = *it;
            exec_cmd(cm, conn);  
        }
        else if (pid > 0)//parent process 
        {
            int status = 0;
            if(wait(&status) == -1)
            {
                perror("wait()");
                exit (1);
            }
        }
    }
}

int main()
{ 
    string line; //input line

    while (1)
    {
        userPrompt(); //prompt
        getline(cin, line); //read input line   

        if(line.find("#") != string::npos) //handling comments
        {
            continue;
        }

        else if(line.find("&&") != string::npos) //checks for ANDS
        {
            string conAND = "&&";
            make_cmd(line, conAND.c_str());
        }

        else if(line.find("||") != string::npos) //checks for ORS
        {
            string conOR = "||";
            make_cmd(line, conOR.c_str());
        }
 
        else //do SEMICOLONS
        {
            string conSEM = ";";
            make_cmd(line, conSEM.c_str());
        }
      
        if(line == "exit") //Exits shell
        {
            cout <<"Exiting shell" << endl;
            exit(0);
        }
    }   
    return 0;
}
