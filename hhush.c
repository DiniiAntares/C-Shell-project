/*
* Simon Franz 2204765
* C-Project: Build a Shell
*/

//Allowed includes
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>

/*
 * Vorwärtsdeklarationen, unnötig da funktionen vor Main sind
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
void date(char firstPipeOutput[], int pipecount){
    time_t currentTime = time(NULL);    //create a var out of time_t
    if(pipecount==2){
        firstPipeOutput = asctime(localtime(&currentTime));
    }
    else{
        printf("%s", asctime(localtime(&currentTime))); //Arange and print Time
    }
}

/**
 * Interacts with the history
 */
void historyFunc(char *parameters, char *historyContent, int histLength, char firstPipeOutput[], int pipecount){ 
    
    if ( parameters[0] == '\0' && historyContent != NULL){
            for (int i=0;i<histLength+1;i++){ //print double-linked list
                if (pipecount ==2){
                    firstPipeOutput[i*256] = historyContent[i*256];
                    firstPipeOutput = realloc(firstPipeOutput, i*256*sizeof(char)) ;
                }else {
                    printf("%s",&(historyContent[i*256]));
                }
            }
        }
    else if ( !strcmp(parameters, "-c")){
        for(int i=0 ;i<histLength*257;i++){
            historyContent[i] = '\0'; //TODO why does it only delte sometimes?
        }
    }
}

/*
 * Saves the last 1000 entrys of history in .hhush.histfile 
 */
void historySave(char *historyCurrent, int collumCount){ //BAUSTELLE!!!
    FILE *historyFile=0;
    if ((historyFile = fopen(".hhush.histfile","w+")) != NULL){
        for (int i=0; i < collumCount; i++ ){
            fputs(&(historyCurrent[i*256]), historyFile);
        }
        fclose(historyFile);
    }
}

/**
 * Changes dir.
 */
void cd(char *directory){
    if (chdir(directory)==-1){
        printf("No such directory\n");
    }
    //printf("%s",directory);
}

/**
 * Prints the current directions content.
 */
void ls(char *content, char firstPipeOutput[], int pipecount){
    if(content==NULL){
        DIR *directory;
        struct dirent *dirStruct;           //Creates a Struct
        directory = opendir(".");  //Open Directory
        if (directory != NULL){
            if (pipecount== 2){
                
                int i = 0;
                while((dirStruct = readdir(directory)) != NULL){ //loop the output
                sprintf(&firstPipeOutput[256*i] ,"%s\n", dirStruct->d_name); //Read out Directory trough the Struct
                i++;
                firstPipeOutput=realloc(firstPipeOutput, (1+i) * 256 * sizeof(char));
            }
            }else{
                while((dirStruct = readdir(directory)) != NULL){ //loop the output
                printf("%s\n", dirStruct->d_name); //Read out Directory trough the Struct
            }
            }
        }
        closedir(directory); //Close Directory{
    }
    else {
        printf("invalid arguments\n");
    }
}

/*
 * Read out content from file and search a pattern
 */
char *grep(char *parameters){
    char pattern[256];
    char filename[256];
    char *grepOutput = malloc(256 * sizeof(char)); //Malloc something to prevent NULLPOINTER
    char *grepFileContent = malloc(256 * sizeof(char)); //Malloc something to prevent NULLPOINTER
    FILE *grepFilePointer = 0;
    int enouth=0;
    
    if(parameters != 0){    //Read out the searched pattern and file name;
        int k=0;
        enouth = 1;
        for (int i = 0; !isspace(parameters[i]);i++){ //Read out the searched pattern...
            pattern[i] = parameters[i];
            pattern[i+1]='\0';
        }
        for (int i = strlen(pattern)+1; !isspace(parameters[i+1]);i++ ){ //  ...and file name.
            filename[k] = parameters[i];
            filename[k+1]='\0';
            k++;
        }
        printf("%s", filename);
        grepFilePointer = fopen(filename,"r");
/*FEHLER IN DER ZEILE, FILE WIRD NICHT GEÖFFNET*/ if(grepFilePointer != NULL){ //try to open the file to read out rhe content
            for (int i = 0 ;(fgets(&(grepFileContent[i*256]),256 , grepFilePointer)); ){ //get content of file
                if(strstr(grepFileContent, pattern)!= NULL) { //Put every appearence of pattern to output
                    for(int p=0; grepFileContent[i*256+p] != '\n' ;p++){
                        if (grepFileContent[i*256+p+1] == '\n'){
                            enouth = 1;
                        }
                    }
                    if (enouth != 1){ //Check size of grepOutput
                        grepOutput = realloc(grepOutput, (i * 256 + k) * sizeof(char)); //extend if to small
                    }
                }
                // printf("%s\n",grepOutput);
            }
            printf("%s\n",grepOutput);
            fclose(grepFilePointer);
            free(grepOutput);
        }else if ((grepFilePointer = fopen(filename,"r")) == NULL){
            printf("no such file!\n");
        }
        return "Exit Success!";
    }
    else {
        printf("invalid parameters\n");
        return "Exit but you go a problem dude!";
    }
    
}

/*
 *Filters the command
 */
void commandReader(char whatToRead[], char command[256], int *wsCounter){ //Input is given by main
    *wsCounter=0;
    if (strlen(whatToRead)<257) {
        int k = 0;
        for (int i=0; i<257 ;i++){ //Condition to look out for WS(Whitespaces)
            if (!isspace(whatToRead[i])){
                if ( isspace(whatToRead[i+1])){
                    for (int j=k;j<i+1;j++){ //j=k because of the possible WhiteSpaces at the beginning of the String
                        command[j-k] = whatToRead[j];
                        command[j-k+1] = '\0';
                    }
                    *wsCounter = k;
                    break;//end loop
                }
            }
            else k++; //Counts the WS before the command
        }
    }
    else{ 
        printf("Your input is wrong.");//Print default message for unusable input
        //char unusable[21] = "Your input is wrong.";
        //return strtok(unusable, "\0");
    }
    //return "Impossible!";
}
/*
 * Filters the content besides the command.
 */
void contentReader(char fullInput[], int sizeOfCommand, char content[256]){
    
    int k=0;
    if (fullInput != NULL && fullInput[sizeOfCommand+1] != '\0'){
            for (int i = sizeOfCommand+1; i < strlen(fullInput)-1 && fullInput[i+1] != '|' ; i++){
            content[k] = fullInput[i];
            content[k+1] = '\0';
            k++;
            }
            //return strtok(content,"\0");
    }else{
        content[0] = '\0';
        //return strtok(content,"\0");
    }
}

/*
 * Pipes the line
 */
void pipes(char fullInput[], char firstInput[256], char secondInput[256]){
    int counter=0;
    
    //firstInput=NULL;
    //secondInput=NULL;
    
    for (counter=0; counter<(strlen(fullInput))+1;counter++){
        firstInput[counter]=fullInput[counter]; //first command and content
        firstInput[counter+1]='\0';
        if( fullInput[counter+1]== '|'){
            for(int i=0;i < strlen(fullInput);i++){ //second command and content
                secondInput[i] = fullInput[i+counter+2];
                secondInput[i+1]= '\0';
            }
            break;
            }
    }
}

/*
 * Main method to run the shell.
 */
int main(){
    int commandSize = 0;
    char *get_current_dir_name();
    char *startDir = get_current_dir_name(); // To prevent false dir for history File
    char command[256];
    char content[256];
    FILE *historyFile = NULL;
    char *history = NULL;
    int historyCollumCount = 0;
    history=malloc(256*sizeof(char));
    int *wsCounter=malloc(sizeof(int));
    static int exitint=1;
    
    char firstInput[256]; //Used...
    char secondInput[256];//...for...
    int pipecount=0;//...Piplines
    
    char *firstPipeOutput=malloc(256*sizeof(char));
    char *secondPipeOutput=malloc(256*sizeof(char));
    char *fullOutput=malloc(256*sizeof(char));
    
    char *input=malloc(258*sizeof(char));
    if ((historyFile = fopen(".hhush.histfile","r")) != NULL){
        for(historyCollumCount=0; fgets(&(history[historyCollumCount * 256]), 256, historyFile) ; ){//Read out .hhush.histfile and put it in the history
            historyCollumCount++;
            history=realloc(history,(1+historyCollumCount) * 256 * sizeof(char)); //malloc enouth to put the fileContent in the historyList
        }
    fclose(historyFile);
    }
    input[0] = '\0';
    while(1){
        
        firstInput[0] = '\0';
        secondInput[0]= '\0';
        content[0]= '\0';
        command[0]= '\0';


        printf("%s $ " ,get_current_dir_name());   //Print current directory
        
        fgets(input, 256, stdin); //Why does it always missread the first input?

        if (input[0]!='\n'){
            history=realloc(history, (2+historyCollumCount) * 256 * sizeof(char)); //add new line to history
            historyCollumCount++;
            strcpy(&(history[historyCollumCount * 256]), input);
            
            pipes(input, firstInput, secondInput);      //First the function cheks...
            if (secondInput[0] != '\0'){                      //if there is a pipeline (pipecount= 1 or 2)...
                pipecount=2;                            //then it splits the fullInput into two and...
            }                                           //use both in loop.
            else{
                pipecount=1;
            }
            for (int i=0; pipecount != 0 ; pipecount--, i++){ 
                if (i==0){
                    commandReader(firstInput, command, wsCounter); //read out the command.
                    commandSize = strlen(command) + *wsCounter;
                }else if (i==1){
                    commandReader(secondInput, command, wsCounter); //read out the command.
                    commandSize = strlen(command) + *wsCounter;
                }
            

            if (!strncmp(command,"exit", 4)){
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                }
                if (content[0]!='\0'){
                    printf("invalid arguments\n");
                }
                else{
                    cd(startDir);
                    historySave(history, historyCollumCount);
                    exitint = 0;
                    break;
                }
            }
            else if (!strncmp(command,"date",4)){ 
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize + strlen(firstInput), content); //read out the rest
                }
                if (content[0]!='\0'){
                    printf("invalid arguments\n");
                }
                else{
                date(firstPipeOutput, pipecount);
                }
            }
            else if (!strncmp(command,"cd",2)){ 
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                }
                if (content[0]!='\0'){
                    cd(content);
                }else{
                    printf("there is no such path\n");
                }
            }
            else if (!strncmp(command,"echo",4)){ //Echos the written String
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                }
                if (content[0]!='\0'){
                    printf("%s\n", content);
                }
            }
            else if (!strncmp(command,"history",7)){
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                } 
                historyFunc(content, history, historyCollumCount, firstPipeOutput, pipecount);
            }
            else if (!strncmp(command,"ls",2)){
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                }
                if (content[0]!='\0'){
                    printf("invalid parameters");
                }else{
                    ls(content, firstPipeOutput, pipecount);
                }
            }
            else if (!strncmp(command,"grep",4)){
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                } 
                if(content[0]!='\0'){
                    grep(content);
                }
            }
            else{
                printf("comand not found\n");
            }
            
            }
        if (exitint==0){
            break;
        }
        }
        
    }
    if (history!=NULL){
        free(history);
    }
    free(input);
    free(firstPipeOutput);
    free(secondPipeOutput);
    free(fullOutput);
return 0;
}

//Bspl: fgets([var], [length], stdin);
