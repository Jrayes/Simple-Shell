#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void simple_shell(){
    int status;
    char ** args = malloc(5 * sizeof(char *));
    for(int i=0; i < 5; i++){

        *(args + i) = malloc(5 * sizeof(char));


    }

    size_t buffer_size = MAX_INPUT_SIZE;

    char *input = malloc(sizeof(char) * MAX_INPUT_SIZE);

    if(input == NULL){

        return;
    }

    do {

        printf("> ");

        getline(&input, &buffer_size, stdin);

        tokenize(input);

        status = execute_commands(args);


        //exit is the only built-in added by default.
        switch (status){
            case FAILED:
                printf("Command not found");
                break;
            case EXIT:
                printf("Exiting shell..\n");
                break;

        }

    } while(status);



}

/*
 *
 * Trim leading and trailing whitespace for the token
 * @param *str
 */

char *trimwhitespace(char *str)
{
    char *str1 = malloc(strlen(str));
    strcpy(str1,str);
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str1)) str1++;

    if(*str1 == 0) // All spaces?
        return str1;
    // Trim trailing space
    end = str1 + strlen(str1) - 1;

    while(end > str1 && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str1;
}

/*
 * Tokenize an input stream given to the shell.
 * @param *input
 * @returns char **args (a character array of commands)
 */

char** tokenize(char *input) {

    char *delim = malloc(sizeof(char) * 2);
    char *token;
    //implement a log?
    if(delim == NULL){

        return NULL;
    }

    delim = " ";


    /*
     * Initialize argument array
     *
     */
    char ** args = malloc(MAX_TOKENS * sizeof(char *));

    //implement a log?
    if(args == NULL){

        return NULL;
    }


    token = strtok(input, delim);

    int num_tokens = 0;

    while(token != NULL) {


        char *new_token = malloc(strlen(token));

        new_token = trimwhitespace(token);

        *(args + num_tokens) = malloc(strlen(new_token));

        *(args + num_tokens) = new_token;

        token = strtok(NULL,delim);

        num_tokens++;
    }

    if(num_tokens < MAX_TOKENS) {
        char **new_args = malloc(num_tokens * sizeof(char *));

        for(int i = 0; i < num_tokens; i++) {

           *(new_args + num_tokens) = *(args + num_tokens);
        }

        free(args);

        return new_args;
    }

    return args;

}

/*
 * Forks the process, and executes the program on the child process.
 * @param args
 *
 */
int execute_commands(char **args){
    
    return 0;
}
/*
 * Interface for adding built-in's.
 * If the code is defined in a module with a specific name, we will load it.
 *
 */
void add_shell_builtins(char **builtins){

    struct current_builtins currentBuiltins;


}