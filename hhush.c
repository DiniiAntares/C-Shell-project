/*
* Simon Franz 2204765
* C-Project: Build a Shell
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

//Allowed includes
// #include < stdlib>
// #include < assert.h>
// #include < errno.h>
// #include < float.h>
// #include < limits.h>
// #include < locale.h>
// #include < math.h>
// #include < setjmp.h>
// #include < signal.h>
// #include < stdarg.h>
// #include < stddef.h>
// #include < stdio.h>
// #include < stdlib.h>
// #include < string.h>

/*
 * Vorwärtsdeklarationen 
 */
// int exit();
//void date();
// char cd[](char directory[]);
// char echo[](char whatToEcho[]);
// char history[]();
// int ls();
//void historySave();

/*
 * Prints the current date+time.
 */
void date(){
    time_t currentTime = time(NULL);
    printf("%s", asctime(localtime(&currentTime)));
}

/**
 * Changes dir.
 */
void cd(char directory[]){
    /*Code Here*/
}

/**
 * Interacts with the history
 */
void history(char parameters[]){ //Doppelchained List (history[][])
/*    char list[];
    if (){ // if parameters = -c then clear
        
    }else if (){ //if parameter is a int then print x lines of the history
        printf();
    }*/
}

/*
 * Saves the last 1000 entrys of history in .hhush.histfile 
 */
void historySave(){
    
}


/**
 * Echos the written String.
 */
void echo(char whatToEcho[]){
    printf("%s", whatToEcho);
    
}

/**
 * Prints the current directions content.
 */
void ls(char content[]){
    char directoryContent[sizeof(content)];
    printf("%s",directoryContent);
}

/**
 * Closes the program.
 */
/*void exitShell(int *temp){
    historySave();
    *temp = 0; //Stops the loop.
    
}*/

/*
 *Filters the command
 */
char *commandReader(char whatToRead[]){ //Input is given by main
    
    if (strlen(whatToRead)<259) {
        int k = 0;
        for (int i=0; i<259 ;i++){ //Condition to look out for WS(Whitespaces)
            if (!isspace(whatToRead[i])){
                if ( isspace(whatToRead[i+1])){
                    char usable[i+1];       //Create a new String/CharArray to contain the non whitespace parts
                    for (int j=k;j<i+1;j++){ //j=k because of the possible WhiteSpaces at the beginning of the String
                        usable[j-k] = whatToRead[j];
                    }
                    
                    printf("%s \n \n",usable);
                    return strtok(usable, "\0");
                }
            }
            else k++; //Counts the WS before the command
        }
    }
    else{ 
        printf("Your input is wrong.");//Print default message for unusable input
         return 0;
        
    }
    return 0;
}

/*
 * Filters the content besides the command.
 */
char *contentReader(char fullInput[], int sizeOfCommand){
    char content[sizeof(fullInput)- sizeOfCommand];
    for (int i = sizeOfCommand-1 ; i < sizeof(fullInput)+1; i++){
        content[i] = fullInput[i];
    }
    return strtok(content, "\n");
}

/*
 * Main method to run the shell.
 */
int main(){
//     int timecount = 0;
//     for (int isActive = 1; isActive == 1 ; timecount++){ //loop to run the project
    while(1){
        printf(/*directory  +*/ " $ "/*,%s*/);   //Print current directory
        char input[258];
        fgets(input, 258, stdin);
        
        /*{Insert code}add new line to history*/
        
        char *command = commandReader(input); //read out the command.
        char *content = contentReader(input , sizeof(command)); //read out the rest
        if (!strcmp(command,"exit")){
            //exitShell(&isActive);
            historySave();
            break;
        }
        else if (!strcmp(command,"date")){ 
            date();
        }
        else if (!strcmp(command,"cd")){ 
            cd(content);
        }
        else if (!strcmp(command,"echo")){
            echo(input);
        }
        else if (!strcmp(command,"history")){
            history(content);
        }
        else if (!strcmp(command,"ls")){
            ls(content);
        }
//         printf("%s : %s",input, content);
    }
return 0;
}

//Bspl: fgets([var], [length], stdin);