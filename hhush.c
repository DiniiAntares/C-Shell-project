/*
* Simon Franz 2204765
* C-Project: Build a Shell
*/

#include <studio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


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
String cd(String /*insert lineString*/);
String echo(String /*insert lineString*/);
String[] history();
int ls();

/*
 * Prints the current date+time.
 */
int date(){
    
}

/**
 * Closes the program.
 */
int exit(){
    
    return 0;
}

/**
 * Changes dir.
 */
String cd(String /*insert lineString*/){
    
}

/**
 * Shows the history.
 */
String[] history(){
    String i[];
    return i;
}

/**
 * Echos the written String.
 */
String echo(String /*insert lineString*/){
    
}

/*
 * Main
 */
int main(){
  
  for (_Bool active=true; active==true;){ //loop to run the projec
      
      
      if (){}
      else if (){exit();}//do exit
      else if (){dater();}//do date
      else if (){cd();}//do cd
      else if (){echo();}//do echo
      else if (){history();}//do history
      else if (){ls();}//do ls
      else if (){grep();}//do grep
      else{ println("Command not found."); }//Print "Command not found."
  }
}

//fgets(buf, 255, stdin);