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


int main()
{
    userPrompt();

    return 0;
}
