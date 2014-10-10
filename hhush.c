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
char *date(int pipecount, char **firstPipeOutput){
    time_t currentTime = time(NULL);    //create a var out of time_t
    if(pipecount==2){
        char *timeTemp = asctime(localtime(&currentTime));
        *firstPipeOutput=realloc(*firstPipeOutput, (strlen(timeTemp)+1)*sizeof(char));
        strcpy(*firstPipeOutput, timeTemp);
        return "Success!";
    }
    else{
        printf("%s", asctime(localtime(&currentTime))); //Arange and print Time
        return "exit";
    }
}

/**
 * Interacts with the history
 */
char *historyFunc(char *parameters, char *historyContent, int *histLength, int pipecount, char **firstPipeOutput){ 
    int collumCountCheck=0;
    collumCountCheck=atoi(parameters); //String to Integer
    int parametersvalid = 1;
    int tempCounter=0;

    char suchaverytempChar[strlen(historyContent)];
    
    for (int i = 0;i<strlen(parameters);i++){ //Trimm pattern to check if allowed
        if (!isspace(parameters[i+1])== 0 ){
            for(int wsTemp=0;wsTemp < strlen(parameters)-i-1;wsTemp++ ){
                if (!isspace(parameters[i+2+wsTemp])) parametersvalid = 0; 
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
        return "error";
    }
    if ((strcmp(parameters, "-c")) != 0){
        if ((parameters[0] == '\0' && historyContent != NULL) || collumCountCheck > (*histLength)){
            tempCounter =(*histLength);
        }
        else if (collumCountCheck > 0 && parameters[0] != '0' ){
            tempCounter = collumCountCheck;
        }
        for (int i=0;i<*histLength;i++){ //print double-linked list
            if (pipecount ==2){
                
                *firstPipeOutput=realloc(*firstPipeOutput, (1+strlen(historyContent))*sizeof(char));
                strcpy(*firstPipeOutput, historyContent);
                break;
//                 for (int q=0, k=0; q < strlen(historyContent);q++, k++){
//                     suchaverytempChar[k] = historyContent[q];
//                     if (suchaverytempChar[k]=='\n'){
//                         suchaverytempChar[k+1]='\0';
//                         if (tempCounter>=((*histLength)-i)){
//                             printf("%s",suchaverytempChar);
//                         }
//                         k=-1;
//                         i++;
//                     }
//                 }
                
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

                return "exit";

            }
        }
        return "SUCCESS";
        
        
    }
    else if ((strcmp(parameters, "-c")) == 0){
        for(int i=0 ;i<(*histLength) *257;i++){
            historyContent[i] = '\0';
        }
        *histLength=0;
        strcpy(*firstPipeOutput, historyContent);
        return "SUCCESS";
    }else if ((!strcmp(parameters, "-c")) == 0 && parameters[0] == '0'  && (collumCountCheck) == '0'){
        printf("invalid arguments");
    }
    return "SUCCESS";
}

/*
 * Saves the last 1000 entrys of history in .hhush.histfile 
 */
void historySave(char *historyCurrent, int collumCount){ //TODO Fehler finden, wird nicht richtig gespeichert.
    FILE *historyFile=0;
    if ((historyFile = fopen(".hhush.histfile","w+")) != NULL){ 
        fputs(historyCurrent, historyFile);
        fclose(historyFile);
    }
}

/*
 *Add input line to history
 */

void  addHistoryLine(int historyCollumCount, char input[], char **history){
    char temp[strlen(input)+2];
    temp[0]='\0';
    *history=realloc(*history, (2+historyCollumCount) * 256 * sizeof(char)); //add new line to history
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
char *ls(char *content, int pipecount, char **firstPipeOutput){
    char verytemp[256];
    *firstPipeOutput[0]='\0';
    if(content[0]== '\0'){
        DIR *directory = NULL;
        struct dirent *dirStruct;           //Creates a Struct
        //Open current Directory
        if ((directory = opendir(".")) != NULL){
            if (pipecount== 2){
                
                while((dirStruct = readdir(directory)) != NULL){ //loop the output
                    if(*firstPipeOutput==NULL){
                            *firstPipeOutput=malloc(strlen(dirStruct->d_name)+1); //Check if something is malloced (unlikely)
                            if(*firstPipeOutput==NULL) break;
                            if (verytemp[0]!='.'){  //Much more 
                                strcpy(*firstPipeOutput,dirStruct->d_name);
                                strcat(*firstPipeOutput,"\n");
                            }
                    }else{
                            *firstPipeOutput=realloc(*firstPipeOutput,strlen(*firstPipeOutput)+1+1+strlen(dirStruct->d_name)+1); //länge der strings + ein trennzeichen z.b. '\n' oder ' '
                            if (*firstPipeOutput==NULL){ //Check if realloc is valid
                                if(directory!=NULL){
                                    closedir(directory); //Close Directory{
                                    directory=NULL;
                                }
                                return "Error";
                            }
                    }
                    strcpy(verytemp, dirStruct->d_name);
                    if (verytemp[0]!='.'){
                        strcat(*firstPipeOutput,dirStruct->d_name);
                        strcat(*firstPipeOutput,"\n");
                    }
                }
            
                if(directory!=NULL){
                    closedir(directory); //Close Directory{
                    directory=NULL;
                }
                return "SUCCESS!";
    
            }else{
                while((dirStruct = readdir(directory)) != NULL){ //loop the output
                    strcpy(verytemp, dirStruct->d_name);
                    if (verytemp[0]!='.'){
                        printf("%s\n", dirStruct->d_name); //Read out Directory trough the Struct
                    }
                }
                if(directory!=NULL){
                    closedir(directory); //Close Directory{
                
                    directory=NULL;
                }
            }
            return "success";
        }else{
            printf("invalid arguments\n");
            return "error!";
        }
        return "Success";
    }
    return "error";
}

/*
 * Read out content from file and search a pattern
 */

char *grep(char parameters[256], int pipecount, char **firstPipeOutput, char fullInput[]){
    
    char pattern[256];
    char tepmoraryVarToCutParameters[256];
    tepmoraryVarToCutParameters[0]='\0';
    strcpy(tepmoraryVarToCutParameters,parameters) ;
    pattern[0]='\0';
    char filename[256];
    filename[0]='\0';
    unsigned int k=0;
    unsigned int i=0;
    unsigned int j=0;
    FILE *grepFilePointer = NULL;    
    
    
    if(tepmoraryVarToCutParameters[0] != '\0'){    //Read out the searched pattern and file name;
        for (i=0; !isspace(tepmoraryVarToCutParameters[i]) && tepmoraryVarToCutParameters[i]!= '\0' ;i++){ //Read out the searched pattern...
            pattern[i] = tepmoraryVarToCutParameters[i];
            pattern[i+1]='\0';
        }
        for (i = ((strlen(pattern))+1); (i<(strlen(tepmoraryVarToCutParameters)+1)) ;i++ ){ //  ...and file name.
            filename[k] = tepmoraryVarToCutParameters[i]; 
            filename[k+1]='\0';
            k++;
        }
        for ( i=0, k=0; i<strlen(pattern);i++){ //kick out the WhiteSpaces
            if((isspace(pattern[i]))==0){
                pattern[k]=pattern[i];
                k++;
            }
        } 
        for (i=0, k=0; i<strlen(filename)+1;i++){ //Purrrrr-fect pattern and filename!
            if((isspace(filename[i]))==0 ){
                filename[k]=filename[i];
                k++;
                if ( filename[i]!='\0' && isspace(filename[i+1])){ //Check if input is valid and set \0 if usable
                    for (int j=i; (j< strlen(filename)+1 ) ;j++){ 
                        if(filename[j]=='\0') filename[k]='\0';
                        if( !(isspace(filename[j+1])) && filename[j+1]!='\0' ){
                            printf("invalid arguments\n");
                            return "Error: invalid arguments";
                        }
                    }
                }
            }
        }
    }

    if (filename[0]=='\0'){
        if( (strstr(fullInput, "|")) && (pipecount == 1) ){ //Take this case if firstPipeOutput is filled.
            
            char temporaryVarToUseFirstOutput[strlen(*firstPipeOutput) + 4 ];
            temporaryVarToUseFirstOutput[0]='\0';
            for (i=0, j=0; i < strlen(*firstPipeOutput)+1;i++){//strcpy with cut token...
                
                temporaryVarToUseFirstOutput[j]=(*firstPipeOutput)[i];
                
                if((temporaryVarToUseFirstOutput[j] =='\n') ||(temporaryVarToUseFirstOutput[j] == '\0' )){
                    temporaryVarToUseFirstOutput[j+1]='\0';
                    if ( strstr(temporaryVarToUseFirstOutput,pattern) ){ //...and here the actual grep.
                        printf("%s",temporaryVarToUseFirstOutput);
                    }
                    j=-1;
                }
                j++;
            }
            return "Success for greped content"; 
        }
        printf("invalid arguments\n");
        return "Maybe something bad happend...";
        
        
    }else if (filename[0]!='\0'){
        if((grepFilePointer = fopen(filename,"r")) != NULL){
            
            char line[4097];
            line[0]='\0';
            char *fileContentTemp=malloc(sizeof(char));
            fileContentTemp[0]='\0';
            
            while (fgets(line, 4096,grepFilePointer)){
                if(strstr(line,pattern)){
                    fileContentTemp=realloc(fileContentTemp, (strlen(fileContentTemp) + strlen(line) +1 )*sizeof(char));
                    strcat(fileContentTemp, line);
                }
            }
            
            
            if ( strstr(fullInput, "|" ) ){  //Print or copy the greped Stuff to see content or to prepare pipe.
                
                *firstPipeOutput=realloc(*firstPipeOutput, strlen(fileContentTemp)*sizeof(char)+2);
                strcpy(*firstPipeOutput, fileContentTemp);
                if (fileContentTemp!= NULL){
                    free(fileContentTemp);
                    fileContentTemp=NULL;
                }
                fclose(grepFilePointer);
                grepFilePointer=NULL;
                return "Successfull Grep if Pipe is active";
                
            }else if(!strstr(fullInput,"|")){
                printf("%s", fileContentTemp);
                if (fileContentTemp!= NULL){
                    free(fileContentTemp);
                    fileContentTemp=NULL;
                }
                fclose(grepFilePointer);
                grepFilePointer=NULL;
                return "Successfull Grep if no Pipe is active";
            }
            
            if (fileContentTemp!= NULL){
                free(fileContentTemp);
                fileContentTemp=NULL;
            }
            fclose(grepFilePointer);
            grepFilePointer=NULL;
            
        }else{
            printf("no such file or directory\n");
            return "Error, file not opened";
        }
        
    }
     
     
     //return "Error with Parameters";  TODO Find solutions to check if grep is usable
     
//     if (pipecount==1){
// 
//                                      //TODO Nach hinten verschieben, glaube bessere lösung gefundne zu haben...
//     }else if( pipecount==2){
// 
//     }
    printf("invalid arguments\n");
    return "Probably not what you excpectet";

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
        int j=0;
        for (;j<strlen(fullInput) && (isspace(fullInput[sizeOfCommand+j])!=0);j++);
        for(; (isspace(fullInput[sizeOfCommand+j])) != 0 ; j++);
        for (int i = sizeOfCommand+j; i < strlen(fullInput)-1 && fullInput[i+1] != '|' ; i++){
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
    command[0]='\0';
    char content[256];
    content[0]='\0';
    FILE *historyFile = NULL;
    int historyCollumCount = 0;
    char *history=malloc(257*sizeof(char));
    history[0]='\0';
    int wsCounter=0;
    static int exitint=1;
    char firstInput[256]; //Used...
    char secondInput[256];//...for...
    int pipecount=0;//...Piplines
    char *firstPipeOutput=malloc(2*sizeof(char));
    char *fullOutput=malloc(256*sizeof(char));
    fullOutput[0]='\0' ;
    char *input=malloc(258*sizeof(char));
    input[0]='\0' ;
    
    
    if ((historyFile = fopen(".hhush.histfile","r")) != NULL){
        int characterRunVar=0;
        while ((history[characterRunVar]=fgetc(historyFile)) != EOF){//for(historyCollumCount=0; fgets(history, 256, historyFile) ; ){//Read out .hhush.histfile and put it in the history
            if (history[characterRunVar] == '\n') historyCollumCount++;
            characterRunVar++;
            history=realloc(history,(3+characterRunVar)*sizeof(char)); //malloc enouth to put the fileContent in the historyList
//             history[characterRunVar+1] = '\0';
        }
        history[characterRunVar] = '\0';
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
        fgets(input, 256, stdin);
        int l=0;
        int m=0;
        for (l=0;l < strlen(input)+1;l++){ //Delete all useless WhiteSpaces
            if ( isspace(input[l]) && input[l]!= '\n'){
                input[l]=' ';
            }
            input[m] = input[l];
            if ( isspace(input[l]) && !isspace(input[l+1]) ){
                m++;
                continue;
            }
            if (isspace(input[l]) && isspace(input[l+1]) ){
                continue;
            }
            m++;
        }
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
//Chek if input contains a legit and usable command
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
                    //firstPipeOutput=realloc() Maybe needed!
                    if (i==1){
                        /*firstPipeOutput=*/date(pipecount, &firstPipeOutput);
                    }else date(pipecount, &firstPipeOutput);
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
                    if (strstr(input, "|")){
                        firstPipeOutput=realloc(firstPipeOutput, strlen(content)*sizeof(char) + 5);
                        strcpy(firstPipeOutput, content);
                        strcat(firstPipeOutput, "\n");
                        
                    } 
                    else printf("%s\n", content);
                }else(printf("\n"));
            }
            else if (!strncmp(command,"history",7)){
                if(i==0){
                    contentReader(firstInput , commandSize, content); //read out the rest
                }else if(i==1){
                    contentReader(secondInput , commandSize, content); //read out the rest
                } 
                if (i==0){
                        /*firstPipeOutput=*/historyFunc(content, history, &historyCollumCount, pipecount, &firstPipeOutput);
                }else historyFunc(content, history, &historyCollumCount, pipecount, &firstPipeOutput);
                
                
                
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
                        ls(content, pipecount, &firstPipeOutput);
                    }else ls(content, pipecount, &firstPipeOutput);
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
                        grep(content, pipecount, &firstPipeOutput, input);
                    }else if (i==1){
                        grep(content, pipecount, &firstPipeOutput, input);
                    }
                }
            }
            else{
                printf("command not found\n");
                break;
            }
        }

        if (exitint==0){
            break;
        }
            
        }
    }
    
        
    
    free(firstPipeOutput);
    firstPipeOutput=NULL;
    free(history);
    free(startDir);
    free(input);
    free(fullOutput);
    return 0;
}
