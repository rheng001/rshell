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
#include <cstdlib>
#include <sys/stat.h>

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
    tokenizer<char_separator<char> > mytok(line,delim); //tokenizer    
    
    for(tokenizer<char_separator<char> >::iterator it = mytok.begin(); it!=mytok.end(); it++, i++) 
    {
        cmd[i] = new char [(*it).size()]; 
        strcpy(cmd[i], (*it).c_str());
    }
   
    cmd[i] = 0;

    if(-1 == execvp(cmd[0], cmd))
    {
         perror("execvp error");
         exit(1);
    } 
}


void make_cmd(string &line, const char * conn)
{
    char_separator<char> delim(conn); //delim 
    tokenizer<char_separator<char> > mytok(line,delim); //tokenizer 
    
    for(tokenizer<char_separator<char> >::iterator it = mytok.begin(); it!=mytok.end(); it++)
    {
        pid_t c_pid, pid;
        int status;

        c_pid = fork();

        if (c_pid < 0)
        {
            perror("fork()");
            exit(1);
        }

        else if (c_pid == 0) //child process
        {
            string str = *it;
            exec_cmd(str, conn); 
            exit(12); 
        }
        else if (c_pid > 0)//parent process 
        {
            if((pid = wait(&status)) < 0)
            {
                perror("wait()");
                exit (1);
            }
        }
    }
}

int do_check(const char * path)
{
    struct stat statbuf;

    if(stat(path, &statbuf) != 0)
    {   
        cout << "Not a Directory" << endl;
        return 0;
    }

    return S_ISDIR(statbuf.st_mode);
};

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

        if((line == "exit") || (line == "quit")) //check exit as 1st input
        {
            cout <<"Exiting Shell" << endl;
            exit(0);
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

        else if(line.find("test") != string::npos) //test command
        {
            string testStr = "test";
            do_check(testStr.c_str());
        }
 
        else //do SEMICOLONS
        {
            string conSEM = ";";
            make_cmd(line, conSEM.c_str());
        }
    }   
    return 0;
}
