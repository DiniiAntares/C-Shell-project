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
#include <dirent.h>

//Allowed includes
// #include <assert.h>
// #include <errno.h>
// #include <float.h>
// #include <limits.h>
// #include <locale.h>
// #include <math.h>
// #include <setjmp.h>
// #include <signal.h>
// #include <stdarg.h>
// #include <stddef.h>

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
    time_t currentTime = time(NULL);    //create a var out of time_t
    printf("%s", asctime(localtime(&currentTime))); //Arange and print Time
}

/**
 * Interacts with the history
 */
void historyFunc(char *parameters, char *historyContent, int histLength){ //Doppelchained List (history[][])
    
    if (historyContent != NULL){
        for (int i=0;i<histLength;i++){
        printf("%s",&(historyContent[i*258]));
        }
    }
    
}

/*
 * Saves the last 1000 entrys of history in .hhush.histfile 
 */
void historySave(char *historyCurrent, int collumCount){ //BAUSTELLE!!!
    FILE *historyFile;
    historyFile = fopen(".hhush.histfile","rw+");
    if (historyFile != NULL){

        while(fputs(historyCurrent, historyFile));
        for (int i=0;i<collumCount;i++){
            fputs(&(historyCurrent[i*258]), historyFile);
        }
        
        fclose(historyFile);
    }
    
}

/**
 * Changes dir.
 */
void cd(char *directory){
    if (chdir(directory)==-1){
        printf("You shall not Pass!\n");
    }
    //printf("%s",directory);
}

/**
 * Prints the current directions content.
 */
void ls(/*char content[]*/){
    DIR *directory;
    struct dirent *dirStruct;           //Creates a Struct
    directory = opendir(".");  //Open Directory
    if (directory != NULL){
        while((dirStruct = readdir(directory)) != NULL){ //loop the output
            printf("%s\n", dirStruct->d_name); //Read out Directory trough the Struct
        }
    }
    closedir(directory); //Close Directory
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
        char unusable[21] = "Your input is wrong.";
        return strtok(unusable, "\0");
    }
    return "Impossible!";
}
/*
 * Filters the content besides the command.
 */
char *contentReader(char fullInput[], int sizeOfCommand){
    char content[strlen(fullInput)-sizeOfCommand];
    int k=0;
    for (int i = sizeOfCommand+1; i < strlen(fullInput)-1; i++){
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
    int commandSize=0;
    
    FILE *historyFile;
    historyFile = fopen(".hhush.histfile","rw+");
    char *history;
    int historyCollumCount=0;
    history=malloc(258*sizeof(char));
    if (historyFile != NULL){
        for(historyCollumCount=0; fgets(&(history[historyCollumCount * 258]), 258, historyFile) ; ){//Read out .hhush.histfile and put it in the history
            historyCollumCount++;
            history=realloc(history,(1+historyCollumCount) * 258 * sizeof(char)); //malloc enouth to put the fileContent in the historyList
        }
    fclose(historyFile);
    }

    while(1){
        
        char *get_current_dir_name();
        printf("%s $ " ,get_current_dir_name());   //Print current directory
        char input[258];
        fgets(input, 256, stdin);

        if (input[0]!='\n'){
            history=realloc(history, (2+historyCollumCount) * 258 * sizeof(char)); //add new line to history
            strcpy(&(history[historyCollumCount * 258]), input);
            historyCollumCount++;


            char *command = commandReader(input); //read out the command.
            commandReader(input);
            commandSize = strlen(command);
            
            if (!strcmp(command,"exit")){
                historySave(history, historyCollumCount);
                break;
            }
            else if (!strcmp(command,"date")){ 
                char *content = contentReader(input , commandSize); //read out the rest
                if (strlen(content)>0){
                    printf("invalid arguments\n");
                }
                else{
                date();
                }
            }
            else if (!strcmp(command,"cd")){ 
                char *content = contentReader(input , commandSize); //read out the rest
                cd(content);
            }
            else if (!strcmp(command,"echo")){ //Echos the written String
                    char *content = contentReader(input , commandSize); //read out the rest
                    printf("%s\n", content);
            }
            else if (!strcmp(command,"history")){
                    char *content = contentReader(input , commandSize); //read out the rest
                    historyFunc(content, history, historyCollumCount);
            }
            else if (!strcmp(command,"ls")){
    //             char *content = contentReader(input , commandSize); //read out the rest
                    ls();
            }
        }
    }
    if (history!=NULL){
        free(history);
    }
return 0;
}

//Bspl: fgets([var], [length], stdin);
