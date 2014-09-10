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
char *date(int pipecount){
    time_t currentTime = time(NULL);    //create a var out of time_t
    if(pipecount==2){
        char *firstPipeOutput = asctime(localtime(&currentTime));
        return firstPipeOutput;
    }
    else{
        printf("%s", asctime(localtime(&currentTime))); //Arange and print Time
        return "exit";
    }
}

/**
 * Interacts with the history
 */
char *historyFunc(char *parameters, char *historyContent, int *histLength, int pipecount){ 
    char *firstPipeOutput=malloc(sizeof(char));
    int collumCountCheck=0;
    collumCountCheck=atoi(parameters);
    int parametersvalid = 1;
    int tempCounter=0;

    char suchaverytempChar[strlen(historyContent)];
    
    for (int i = 0;i<strlen(parameters);i++){ //Trimm pattern to check if allowed
        if (!isspace(parameters[i+1])== 0 ){
            
            for(int wsTemp=0;wsTemp < strlen(parameters-i);wsTemp++ ){
                if (isspace(parameters[i+2+wsTemp]) == 0) parametersvalid = 0; 
                
            }
        }
            
        else if (isspace(parameters[i+1])== 0){
            for (int k=0; k<strlen(parameters)-i;k++){
            if (((isspace(parameters[i+1+k]) == 0 && parameters[i+1+k] != '\0' && strstr(parameters, "-c") == NULL && collumCountCheck<= 0)  || (collumCountCheck <= 0 && strstr(parameters, "-c") == NULL && parameters[i]!='\0'))){
                parametersvalid=0;
                }
            }
        }
    }
    if(parametersvalid == 0 ){
        printf("invalid arguments\n");
        
        if(firstPipeOutput!=NULL){
            free(firstPipeOutput);
            firstPipeOutput=NULL;
        }
        return "error";
    }
    if (firstPipeOutput==NULL){
        if(firstPipeOutput!=NULL){
            free(firstPipeOutput);
            firstPipeOutput=NULL;
        }
        return "Error";
    }
    firstPipeOutput[0] = '\0';
    
    
    if ((strcmp(parameters, "-c")) != 0){
        if ((parameters[0] == '\0' && historyContent != NULL) || collumCountCheck > (*histLength)){
            tempCounter =(*histLength);
        }
        else if (collumCountCheck > 0 && parameters[0] != '0' ){
            tempCounter = collumCountCheck;
        }
        for (int i=0;i<*histLength;i++){ //print double-linked list
            if (pipecount ==2){
            //    if (tempCounter>=((*histLength)-i)){
                    if (firstPipeOutput[0]=='\0'){
                        firstPipeOutput=realloc(firstPipeOutput, strlen(historyContent));
                        if (firstPipeOutput==NULL) {
                            if(firstPipeOutput!=NULL){
                                free(firstPipeOutput);
                                firstPipeOutput=NULL;
                            }
                            return "error";
                        }
                    }
                    else{
                        firstPipeOutput = realloc(firstPipeOutput, (*histLength)*256*sizeof(char)) ;
                        if(firstPipeOutput==NULL){
                            if(firstPipeOutput!=NULL){
                                free(firstPipeOutput);
                                firstPipeOutput=NULL;
                            }
                            return "fail";
                        }
                        firstPipeOutput[i*256] = historyContent[i*256];
                    }
                //}
                
            }else{

                for (int q=0, k=0; q < strlen(historyContent);q++, k++){
                    suchaverytempChar[k] = historyContent[q];
                    if (suchaverytempChar[k]=='\n'){
                        suchaverytempChar[k+1]='\0';
                        if (tempCounter>=((*histLength)-i)){
                            printf("%s",suchaverytempChar);
                        }
                        k=-1;
                        i++;
                    }
                }
                if(firstPipeOutput!=NULL){
                    free(firstPipeOutput);
                    firstPipeOutput=NULL;
                }
                return "exit";

            }
        }
        char *returnVar;
        returnVar=firstPipeOutput;
        if(firstPipeOutput!=NULL){
            free(firstPipeOutput);
            firstPipeOutput=NULL;
        }
        return returnVar;
        
        
    }
    else if ((strcmp(parameters, "-c")) == 0){
        for(int i=0 ;i<(*histLength) *257;i++){
            historyContent[i] = '\0'; //TODO why does it only delte sometimes?
        }
        *histLength=0;
        strcpy(firstPipeOutput, historyContent);
        
        char *returnVar;
        returnVar=firstPipeOutput;
        if(firstPipeOutput!=NULL){
            free(firstPipeOutput);
            firstPipeOutput=NULL;
        }
        return returnVar;
    }else if ((!strcmp(parameters, "-c")) == 0 && parameters[0] == '0'  && (collumCountCheck) == '0'){
        printf("invalid arguments");
    }
    char *returnVar;
    returnVar=firstPipeOutput;
    if(firstPipeOutput!=NULL){
        free(firstPipeOutput);
        firstPipeOutput=NULL;
    }
    
    return returnVar;
}

/*
 * Saves the last 1000 entrys of history in .hhush.histfile 
 */
void historySave(char *historyCurrent, int collumCount){ //BAUSTELLE!!!
    FILE *historyFile=0;
    if ((historyFile = fopen(".hhush.histfile","w+")) != NULL){
        //for (int i=0; i < collumCount; i++ ){
            fputs((historyCurrent/*[i*256]*/), historyFile);
        //}
        fclose(historyFile);
    }
}

/*
 *Add input line to history
 */

void  addHistoryLine(int historyCollumCount, char input[], char *history[]){
    char temp[strlen(input)+2];
    (*history)=realloc(*history, (1+historyCollumCount) * 256 * sizeof(char)); //add new line to history
    historyCollumCount--;
    sprintf(temp, "%i %s",historyCollumCount, input);
    strcat(*history, temp);
}

/**
 * Changes dir.
 */
void cd(char *directory){
    if (chdir(directory)==-1){
        printf("no such file or directory\n");
    }
    //printf("%s",directory);
}

/**
 * Prints the current directions content.
 */
char *ls(char *content, int pipecount){
    char *firstPipeOutput=malloc(sizeof(char));
    char verytemp[256];
    firstPipeOutput[0]='\0';
    if(content[0]== '\0'){
        DIR *directory;
        struct dirent *dirStruct;           //Creates a Struct
        //Open current Directory
        if ((directory = opendir(".")) != NULL){
            if (pipecount== 2){
                
                
            while((dirStruct = readdir(directory)) != NULL){ //loop the output
                if(firstPipeOutput==NULL){
                        firstPipeOutput=malloc(strlen(dirStruct->d_name)+1);
                        if(firstPipeOutput==NULL) break;
                        
                        if (verytemp[0]!='.'){
                            strcpy(firstPipeOutput,dirStruct->d_name);
                            strcat(firstPipeOutput,"\n");
                        }
                }else{
                        firstPipeOutput=realloc(firstPipeOutput,strlen(firstPipeOutput)+1+1+strlen(dirStruct->d_name)+1); //länge der strings + ein trennzeichen z.b. '\n' oder ' '
                        if (firstPipeOutput==NULL){
                            if(firstPipeOutput!=NULL){
                                free(firstPipeOutput);
                                firstPipeOutput=NULL;
                            }
                            if(directory!=NULL){
                                closedir(directory); //Close Directory{
                                
                                directory=NULL;
                            }
                            return "error";
                            
                        }
                        strcpy(verytemp, dirStruct->d_name);
                        if (verytemp[0]!='.'){
                            strcat(firstPipeOutput,dirStruct->d_name);
                            strcat(firstPipeOutput,"\n");
                        }
                }
            }
            
            
            
            char *returnVar;
                returnVar=firstPipeOutput;
                if(firstPipeOutput!=NULL){
                    free(firstPipeOutput);
                    firstPipeOutput=NULL;
                }
                if(directory!=NULL){
                    closedir(directory); //Close Directory{
                    
                    directory=NULL;
                }
                return returnVar;
            }else{
                while((dirStruct = readdir(directory)) != NULL){ //loop the output
                    strcpy(verytemp, dirStruct->d_name);
                    if (verytemp[0]!='.'){
                        printf("%s\n", dirStruct->d_name); //Read out Directory trough the Struct
                    }

                }
                if(firstPipeOutput!=NULL){
                    free(firstPipeOutput);
                    firstPipeOutput=NULL;
                }
                if(directory!=NULL){
                    closedir(directory); //Close Directory{
                    
                    directory=NULL;
                }
                    directory=NULL;
                }
                return "EXIT";
            }
        
            if(directory!=NULL){
                closedir(directory); //Close Directory{
                
                directory=NULL;
            }
    }
    else {
        printf("invalid arguments\n");
        if(firstPipeOutput!=NULL){
            free(firstPipeOutput);
            firstPipeOutput=NULL;
        }

        return "error!";
    }
    if(firstPipeOutput!=NULL){
        free(firstPipeOutput);
        firstPipeOutput=NULL;
    }
    return "exit";
}

/*
 * Read out content from file and search a pattern
 */
char *grep(char *parameters, int pipecount, char firstPipeOutput[], char fullInput[]){
    char pattern[256];
    char filename[256];
    int enouth=0;
    FILE *grepFilePointer = 0;
    char *grepOutput = malloc(256 * sizeof(char)); //Malloc something
    char *grepFileContent = malloc(256 * sizeof(char)); //Malloc something
    char *secondPipeOutput=malloc(256*sizeof(char));
    
    
    if(parameters[0] != '\0'){    //Read out the searched pattern and file name;
        int k=0;
        enouth = 1;
        for (int i = 0; (isspace(parameters[i]))==0 ;i++){ //Read out the searched pattern...
            pattern[i] = parameters[i];
            pattern[i+1]='\0';
        }
        for (int i = ((strlen(pattern))+1); (i<(strlen(parameters)+1)) /*|| ((isspace(parameters[i])!=0))*/ ;i++ ){ //  ...and file name.
            filename[k] = parameters[i]; 
            filename[k+1]='\0';
            k++;
            if ((isspace(filename[k-1]))!=0 && (isspace(filename[k-2]))==0 && filename[k-1] != '\0') {
                printf("invalid arguments\n");
                if(grepOutput!=NULL){ 
                    free(grepOutput);
                    grepOutput=NULL;
                }
                if (grepFileContent!=NULL){
                    free(grepFileContent);
                    grepFileContent=NULL;
                }
                if(secondPipeOutput!=NULL){
                    free(secondPipeOutput);
                    secondPipeOutput=NULL;
                }
                return "BÄM!";
            }
        }
        
        if ((parameters[((strlen(pattern))+(strlen(filename))+1)])!='\0' && (isspace(parameters[((strlen(pattern))+(strlen(filename))+1)])==0) ){
            printf("invalid arguments\n");
            
            if(grepOutput!=NULL){ 
                free(grepOutput);
                grepOutput=NULL;
            }
            if (grepFileContent!=NULL){
                free(grepFileContent);
                grepFileContent=NULL;
            }
            if(secondPipeOutput!=NULL){
                free(secondPipeOutput);
                secondPipeOutput=NULL;
            }
            
            return "MIST!";
        }
        
         if((strstr(fullInput, "|")) == NULL){ //TODO Prüfe ob grepFileContent überhaupt & und [i*256] braucht!
            printf("%s\n", filename);
            grepFilePointer = fopen(filename,"r");
            if(grepFilePointer != NULL){ //try to open the file to read out rhe content

                
                
                
                
                for (int i = 0 ;(fgets(&(grepFileContent[i*256]),256 , grepFilePointer)); ){ //get content of file
                    if((strstr(grepFileContent, pattern))!= NULL) { //Put every appearence of pattern to output
                        for(int p=0; grepFileContent[i*256+p] != '\n' ;p++){
                            if (grepFileContent[i*256+p+1] == '\n'){
                                enouth = 1;
                            }
                        }
                        if (enouth != 1){ //Check size of grepOutput
                            grepOutput = realloc(grepOutput, (i * 256 + k) * sizeof(char)); //extend if to small
                        }
                    }
                }
                
                
                
                
                printf("%s\n",grepOutput);
                fclose(grepFilePointer);
                if (grepOutput!= NULL){
                    free(grepOutput);
                    grepOutput=NULL;
                }
            }else{
                printf("no such file or directory\n");
            }
        
        }
        else if (strstr(fullInput, "|") != NULL){
            char tempString[256];
            if (secondPipeOutput==NULL && pipecount == 1){
                if( (secondPipeOutput = realloc(secondPipeOutput, (sizeof(char)))) != NULL);
                secondPipeOutput[0]='\0';
            }
            for (int i = 0, t=0; i < strlen(firstPipeOutput);t++, i=i+1+strlen(tempString)){  //Fehler hier!
                tempString[0]='\0';
                for (int c = 0; firstPipeOutput[i+c] != '\n'  ;c++ ){
                    tempString[c]=firstPipeOutput[i+c];
                    tempString[c+1]='\0';
                }
                if (strstr(tempString, pattern) != 0){
                    if((secondPipeOutput = realloc(secondPipeOutput, (1+t)*256*sizeof(char)))!= NULL){
                    strcat(secondPipeOutput, tempString);
                    strcat(secondPipeOutput, "\n");
                    }
                }
            }
            if (pipecount==1){
            printf("%s", secondPipeOutput);
            }
            if (pipecount==2){
                if ((firstPipeOutput=realloc(firstPipeOutput, strlen(secondPipeOutput)*sizeof(char))) != NULL){
                    strcpy(firstPipeOutput, secondPipeOutput);
                    
                    if(grepOutput!=NULL){ 
                        free(grepOutput);
                        grepOutput=NULL;
                    }
                    if (grepFileContent!=NULL){
                        free(grepFileContent);
                        grepFileContent=NULL;
                    }
                    if(secondPipeOutput!=NULL){
                    free(secondPipeOutput);
                    secondPipeOutput=NULL;
                    }
                    char *returnVar;
                    returnVar=firstPipeOutput;
                    if(firstPipeOutput!=NULL){
                        free(firstPipeOutput);
                        firstPipeOutput=NULL;
                    }
                    return returnVar;
                }
            }
        }
        
        if(grepOutput!=NULL){ 
            free(grepOutput);
            grepOutput=NULL;
        }
        if (grepFileContent!=NULL){
            free(grepFileContent);
            grepFileContent=NULL;
        }
        if(secondPipeOutput!=NULL){
            free(secondPipeOutput);
            secondPipeOutput=NULL;
        }
        return "Exit Success!";
    }
    else {
        printf("invalid arguments\n");

        if(grepOutput!=NULL){ 
            free(grepOutput);
            grepOutput=NULL;
        }
        if (grepFileContent!=NULL){
            free(grepFileContent);
            grepFileContent=NULL;
        }
        if(secondPipeOutput!=NULL){
            free(secondPipeOutput);
            secondPipeOutput=NULL;
        }
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
        for (int i=0; i<257 ;i++){
            if (!isspace(whatToRead[i])){//Condition to look out for WS(Whitespaces)
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
    }
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
    }else{
        content[0] = '\0';
    }
}

/*
 * Pipes the line
 */
void pipes(char fullInput[], char firstInput[256], char secondInput[256]){
    int counter=0;
    
    //firstInput=NULL;
    //secondInput=NULL;
    
    for (counter=0; counter<(strlen(fullInput));counter++){
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
    
    
    char *get_current_dir_name();
    int commandSize = 0;
    
    char *startDir = get_current_dir_name(); // To prevent false dir for history File
    char command[256];
    char content[256];
    FILE *historyFile = NULL;
    char *history = NULL;
    int historyCollumCount = 0;
    history=malloc(256*sizeof(char));
    int wsCounter=0;
    static int exitint=1;
    
    char firstInput[256]; //Used...
    char secondInput[256];//...for...
    int pipecount=0;//...Piplines
    
    
    char *firstPipeOutputMain=malloc(sizeof(char));
    //firstPipeOutputMain="1";
    
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

        char *directoryVar = get_current_dir_name();
        printf("%s $ " ,directoryVar);   //Print current directory
        free(directoryVar);
        
        fgets(input, 256, stdin); //Why does it always missread the first input?

        if (input[0]!='\n'){
            historyCollumCount++;
            addHistoryLine(historyCollumCount, input, &history);
            
            pipes(input, firstInput, secondInput);     //split input to prepare pipes //First the function cheks...
            if (strstr(input, "|") != NULL)/*secondInput[0] != '\0')*/{                      //if there is a pipeline (pipecount= 1 or 2)...
                pipecount = 2;                            //then it splits the fullInput into two and...
            }                                           //use both in loop.
            else if (strstr(input, "|") == NULL){
                pipecount=1;
            }
            for (int i=0; pipecount != 0 ; pipecount--, i++){
                if (i==0){
                    commandReader(firstInput, command, &wsCounter); //read out the command.
                    commandSize = strlen(command) + wsCounter;
                }else if (i==1){
                    commandReader(secondInput, command, &wsCounter); //read out the command.
                    commandSize = strlen(command) + wsCounter;
                }
            
//Chek if input is a legit command

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
                    if (firstPipeOutputMain!=NULL) {
                        free(firstPipeOutputMain);
                        firstPipeOutputMain=NULL;
                    }

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
                    //firstPipeOutputMain=realloc() Maybe needed!
                    if (pipecount==2){
                        firstPipeOutputMain=date(pipecount);
                    }else date(pipecount);
                }
            }
            else if (!strncmp(command,"cd",2)){ 
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                }
                if (content[0]!='\0' && strstr(content, " ")==0 ){
                    cd(content);
                }else{
                    printf("invalid arguments\n");
                }
            }
            else if (!strncmp(command,"echo",4)){ //Echos the written String
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                }
                if (content[0]!='\0'){
                    if (strstr(input, "|")) strcpy(firstPipeOutputMain, content);
                    else printf("%s\n", content);
                }else(printf("\n"));
            }
            else if (!strncmp(command,"history",7)){
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                } 
                if (pipecount==2){
                        firstPipeOutputMain=historyFunc(content, history, &historyCollumCount, pipecount);
                }else historyFunc(content, history, &historyCollumCount, pipecount);
                
                
                
            }
            else if (!strncmp(command,"ls",2)){
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                }
                if (content[0]!='\0'){
                    printf("invalid arguments\n");
                }else{
                
                    if (pipecount==2){
                        firstPipeOutputMain=ls(content, pipecount);
                    }else ls(content, pipecount);
                }
            }
            else if (!strncmp(command,"grep",4)){
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                } 
                if(content[0]!='\0'){
                    if (i==0){
                        grep(content, pipecount, firstPipeOutputMain, input);
                    }else if (i==1){
                        firstPipeOutputMain=grep(content, pipecount, firstPipeOutputMain, input);
                    }
                }
            }
            else{
                printf("command not found\n");
            }
            if( firstPipeOutputMain!=NULL && pipecount == 1){
                free(firstPipeOutputMain);
                firstPipeOutputMain=NULL;
            }
            }
            if (exitint==0){
                break;
            }
            
        }
    }
    if (history!=NULL){
        free(history);
        history = NULL;
    }
    if (firstPipeOutputMain!=NULL) free(firstPipeOutputMain);
    //free(firstPipeOutputMain);
    free (startDir);
    if (input!=NULL) free(input);
    if (fullOutput!=NULL) free(fullOutput);
    return 0;
}

//Bspl: fgets([var], [length], stdin);
