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
 * Changes dir.
 */
void cd(char directory[]){
    /*Code Here*/
    chdir(directory);
    //printf("%s",directory);
}

/**
 * Prints the current directions content.
 */
void ls(/*char content[]*/){
//     char *directoryContent;
//     directoryContent = 
//     printf("%s",directoryContent);
}

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
                        usable[j-k+1] = '\0';
                    }
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
    char content[sizeof(fullInput)-sizeOfCommand];
    int k=0;
    for (int i = sizeOfCommand+2; i < sizeof(fullInput); i++){
        content[k] = fullInput[i];
        content[k+1] = '\0';
        k++;
    }
    return strtok(content, "\0");
}

/*
 * Main method to run the shell.
 */
int main(){
    int commandSize;
    while(1){
        
        char *get_current_dir_name();
        printf("%s $ " ,get_current_dir_name());   //Print current directory
        char input[258];
        fgets(input, 258, stdin);
        
        /*{Insert code}add new line to history*/
        char *command = commandReader(input); //read out the command.
        commandReader(input);
        commandSize = sizeof(command);
        
        if (!strcmp(command,"exit")){
            historySave();
            break;
        }
        else if (!strcmp(command,"date")){ 
            date();
        }
        else if (!strcmp(command,"cd")){ 
            char *content = contentReader(input , commandSize); //read out the rest
            contentReader(input, commandSize);
            cd(content);
        }
        else if (!strcmp(command,"echo")){ //Echos the written String
                char *content = contentReader(input , commandSize); //read out the rest
                contentReader(input, commandSize);
                printf("%s", content);
        }
        else if (!strcmp(command,"history")){
                char *content = contentReader(input , commandSize); //read out the rest
                contentReader(input, commandSize);
                history(content);
        }
        else if (!strcmp(command,"ls")){
//             char *content = contentReader(input , commandSize); //read out the rest
//             contentReader(input, commandSize);
                ls();
        }
    }
return 0;
}

//Bspl: fgets([var], [length], stdin);