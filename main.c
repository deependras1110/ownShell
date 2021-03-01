// The MIT License (MIT)
// 
// Copyright (c) 2016, 2017, 2021 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 7f704d5f-9811-4b91-a918-57c1bb646b70
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10     // Mav shell only supports five arguments

int main()
{

  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
  pid_t list[15];
  char* history[100];
  int i;
  int count=0;
  int historyIndex=0;
//   for(int i=0;i<15;i++){
//       history[i]=(char*)malloc(255);
//   }

  while( 1 )
  {
    
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    //memset(&history[historyIndex],0,255);
    history[historyIndex]=strdup(cmd_str);
    historyIndex++;
    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr; 

    //this thing giving segfault 

    // memset(&history[historyIndex],0,255);                                                       
    char *working_str  = strdup( cmd_str );
    // strncpy(history[historyIndex],working_str,255);
    // historyIndex++;
                  

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input strings with whitespace used as the delimiter
    while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // Now print the tokenized input as a debug check
    // \TODO Remove this code and replace with your shell functionality
    if(token[0]!=NULL){
        

        if(strcmp(token[0],"exit")==0){
            exit(0);
            list[count]=0;
            count++;
        }

        else if(strcmp(token[0],"cd")==0){
        // printf("place for cd!\n");
            chdir(token[1]);
            list[count]=0;
            count++;
            
        }
        else if(strcmp(token[0],"showpids")==0){
            
            printf("place for list\n");
            if(count>15){
                count=15;
            }
            for(i=0;i<count;i++){
                printf("%d\n",list[i]);
            }
        }
        else if(strcmp(token[0],"history")==0){
            list[count]=0;
            count++;
            printf("place for history\n");
            for(i=0;i<historyIndex;i++){
                printf("%d:%s",i+1,history[i]);
            }
        }
        else if(strcmp(token[0],"!n")==0){
            list[count]=0;
            count++;
            printf("place for !n\n");
        }
        
        else{
            pid_t pid = fork( );
            list[count]=pid;
            count++;
            if( pid == 0 )
             {
                // Notice you can add as many NULLs on the end as you want
                int ret = execvp( token[0], &token[0] );  
                if( ret == -1 )
                {
                    printf("%s:command not found\n",token[0]);
                    break;
                }
                
            }
            else 
            {
                int status;
                wait( & status );
            }
        }
        free( working_root );
    }

    

    // int token_index  = 0;
    // for( token_index = 0; token_index < token_count; token_index ++ ) 
    // {
    //   printf("token[%d] = %s\n", token_index, token[token_index] );  
    // }

    

  }
  return 0;
}