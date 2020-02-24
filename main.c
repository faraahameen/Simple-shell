#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define MAXLINE 50

char command[MAXLINE];
char parsedCommand[10][MAXLINE];
int background = 0;

//Command Input from User
void inputCommand()
{
    printf(">>>");
    gets(command);
    printf("\n");
}

//Parse Command
int parseCommand()
{
    int row = 0, col=0;

    //Clear array
    for(int p=0; p<=10; p++)
    {
        for(int c=0; c<=MAXLINE; c++)
        {
            parsedCommand[p][c] ='\0';
        }
    }

    for(int i =0; i <= strlen(command); i++)
    {

        if(command[i] ==' ')  //End of Argument
        {

            parsedCommand[row][col] = '\0';
            row ++;
            col = 0;

        }
        else if(command[i] == '\0')   //End of Command
        {

            parsedCommand[row+1][col]='\0';

        }
        else
        {
            parsedCommand[row][col] = command[i];
            col++;
        }
    }

    if(isBackgroundProcess())
        background = 1;

    return row;
}


int isExit()
{
    if(strcmp(parsedCommand[0],"exit") == 0)
    {
        return 1;
    }

    return 0;
}

//Checks if '&' is present
int isBackgroundProcess()
{
    for(int i=0; i <= strlen(parsedCommand); i++)
        if (strcmp(parsedCommand[i],"&")==0)
        {
            return 1;
        }
    return 0;
}

//writes when child process has been terminated
void writeLog(int sc)
{

    FILE *f;
    f=fopen("log.txt","a");
    fprintf(f,"Child process was terminated.\n");
    fclose(f);
}


int main()
{
    int noOfArguments;
    char *argumentsArr[10];

    while(1)
    {
        inputCommand();
        noOfArguments = parseCommand();
        if(isExit())
        {
            exit(0);
        }

        pid_t childID = fork();
        signal(SIGCHLD,writeLog);
        if (childID == -1)
        {

            printf("Error! - Fork failed!");

        }
        else if (childID == 0)
        {
            for (int i = 0; i<=noOfArguments; i++)
            {
                argumentsArr[i] = parsedCommand[i];
            }
            argumentsArr[noOfArguments+1]=NULL;
            if(isBackgroundProcess())
                execvp(parsedCommand[0], NULL);
            else
                execvp(argumentsArr[0], argumentsArr);
            perror("execvp");

        }
        else
        {
            if (background == 0)
            {
                wait(childID);
            }
            else
            {
                sleep(5);
            }

        }

        signal(SIGCHLD,writeLog);
    }
    return 0;
}
