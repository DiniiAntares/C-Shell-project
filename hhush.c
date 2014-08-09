/*
* Simon Franz 2204765
* C-Project: Build a Shell
*/

#include <studio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//Allowed includes
// #include < stdlib>
// #include < assert.h>
// #include < ctype.h>
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
int exit();
int date();
char cd[](char /*insert lineString*/);
char echo[](char /*insert lineString*/);
char history[]();
int ls();
int time();
/*
 * Prints the current date+time.
 */
int date(){
    char day[];
    int day;
    char month[];
    int year;
}
/**
 * help method for date() and print's current time.
 */
char time[](){
    
    time_t currentTime = time();
    printf("%s", asctime(localtime(&result)));
    return currentTime;
}
/**
 * Closes the program.
 */
int exit(){
    main.active = false; //Stops the loop.
    return EXIT_SUCCES;
}

/**
 * Changes dir.
 */
char cd[](char []/*insert lineString*/){
    switch (){
}

/**
 * Shows the history.
 */
char history[](int lines){
    char list[];
    return i;
}

/**
 * Echos the written String.
 */
char echo[](char whatToEcho[]){
    printf("%s", whatToEcho);
    
}

/**
 * Prints the current directions content.
 */
char ls[](){
    printf();
}


/*
 *Filters the command
 */
char commandReader[](char input[]){ //Input is given by main
    
    if (strlen(input)<259) {
        int k = 0;
        for (int i=0; i<259 ;i++){ //Condition to look out for WS(Whitespaces)
            if (input[i] != " " || inpur[i] != "\t"){
                if ( input[i+1] = " " || input[i+1] = "\t"){
                    
                    char usable[i+1];       //Create a new String/CharArray to contain the non whitespace parts
                    for (int j=k;j<i+1;j++){ //j=k because of the possible WhiteSpaces at the beginning of the String
                        usable[j-k] = input[j];
                    }
                    return usable[];
                }
            }
            else k++; //Counts the WS before the command
        }
    }
    else{ 
        printf("Your input is wrong.");//Print default message for unusable input
        return EXIT_SUCCES;
        
    }
}

/*
 * Filters the content besides the command.
 */
char contentReader[](char fullInput[], int sizeOfCommand){
    char content;
    for (int i = sizeOfCommand-1 ; i < sizeof(fullInput[])+1; i++){
        content[j] = fullInput[i];
    }
    return content[];
}

/*
 * Main method to run the shell.
 */
int main(){
    for (_Bool active=true; active==true;){ //loop to run the projec
        println(/*directory*/  + " $ ",%s);   //Print current directory
        char input[258];
        fgets(input, 258, stdin);
        
        {Insert code}//add new line to history
        
        char input[258];
        fgets(input, 258, stdin);
        char command[] = commandReader(input[]); //read out the command.
        char content[] = contentReader(input[], sizeof(command)); //read out the rest
        
        do{
            if (command[] == exit){
                exit();
            }
            else if (command[] == date){ 
                date();
            }
            else if (command[] == cd){ 
                cd(content[]);
            }
            else if (command[] == echo){
                echo(content[]);
            }
            else if (command[] == history){
                history(content[]);
            }
            else if (command[] == ls){
                ls();
            }
            
        }

    }
return EXIT_SUCCES;
}

//Bspl: fgets([var], [length], stdin);