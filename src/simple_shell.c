#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;
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
 * Read a line of input from stdin.
 *
 */

char * read_line() {

    char *buffer = malloc(sizeof(char) * MAX_INPUT_SIZE);
    int position = 0;
    char c = getchar();
    int max_input_size = MAX_INPUT_SIZE;
    while(c != '\n' && c != EOF) {

        if(position == max_input_size - 1) {

            char *temp = malloc(strlen(buffer));

            strcpy(temp, buffer);

            buffer = malloc(strlen(buffer) * 2);

            strcpy(buffer, temp);

            max_input_size = max_input_size * 2;

            buffer[position] = c;

            position++;

        }

        else {

            buffer[position] = c;

            position++;

        }

        c = getchar();
    }

    buffer[position] = 0;

    return buffer;

}

void simple_shell(){


        char *input;
        char **args;
        do {

            input = malloc(sizeof(char) * MAX_INPUT_SIZE);
            /*
            * Initialize argument array
            *
            */
            args = malloc(MAX_TOKENS * sizeof(char *));

           if(input == NULL || args == NULL){

               exit(1);
           }

            //implement a log?

            printf("> ");

            input = read_line();

            tokenize(input, args);

            //reset the code to normal before each iteration.

            execute_commands(args);

            free(input);

            free(args);

            //exit is the only built-in added by default.

        } while(1);

}


/*
 * Tokenize an input stream given to the shell.
 * @param *input
 * @returns char **args (a character array of commands)
 */

char** tokenize(char *input, char **args) {

    char *delim = malloc(sizeof(char) * 2);
    char *new_token;
    //implement a log?

    if(delim == NULL) {

        exit(EXIT_FAILURE);
    }

    delim = " ";

    char *token = strtok(input, delim);

    int num_tokens = 0;

    while(token != NULL) {

        new_token = malloc(strlen(token));

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

        args = malloc(num_tokens * sizeof(char *));

        args = new_args;

    }


    return args;

}

/*
 * Forks the process, and executes the program on the child process.
 * @param args
 *
 */
int execute_commands(char **args) {

    pid_t pid, wpid;

    pid = fork();

    int status;

    //best way will be to suppress file descriptor.

    if (pid == 0) {
        // Child process
        //in general make sure that we don't print this for any built-in's.

        if (execvp(args[0], args) == -1 && strncmp(args[0],"exit",strlen("exit")) != 0) {

            //we'll write any additional error codes to file. We'll include a timestamp, and the command executed.
            perror("Command not found\n");

        }

        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("Error forking process\n");

    } else {
        // Parent process
        do {

            //make sure the parent is running in the foreground.


            if(strncmp(args[0],"exit",strlen("exit")) == 0){
                printf("Exiting shell...\n");
                exit(EXIT_SUCCESS);
            }



            wpid = waitpid(pid, &status, WUNTRACED);

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;

}

/*
 * Ensure that shell is running interactively as the foreground job
 * This should be for use only when shell is invoked as a job by parent.
 *
 */

void init_shell() {

    /* See if we are running interactively.  */
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty (shell_terminal);

    if (shell_is_interactive)
    {
        /* Loop until we are in the foreground.  */
        while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
            kill (- shell_pgid, SIGTTIN);

        /* Ignore interactive and job-control signals.  */
        signal (SIGINT, SIG_IGN);
        signal (SIGQUIT, SIG_IGN);
        signal (SIGTSTP, SIG_IGN);
        signal (SIGTTIN, SIG_IGN);
        signal (SIGTTOU, SIG_IGN);
        signal (SIGCHLD, SIG_IGN);

        /* Put ourselves in our own process group.  */
        shell_pgid = getpid ();
        if (setpgid (shell_pgid, shell_pgid) < 0)
        {
            perror ("Couldn't put the shell in its own process group");
            exit (1);
        }

        /* Grab control of the terminal.  */
        tcsetpgrp (shell_terminal, shell_pgid);

        /* Save default terminal attributes for shell.  */
        //tcgetattr (shell_terminal, &shell_tmodes);
    }

}

/*
 * Interface for adding built-in's.
 * If the code is defined in a module with a specific name, we will load it.
 *
 */
void add_shell_builtins(char **builtins){

    struct current_builtins currentBuiltins;


}
/*
 * Checks all allocated data structures and garbage collects, any unused ones.
 *
 */
void collect(char ** unused_structures, int length) {
    for(int i=0; i < length; i++){

        free(*(unused_structures + i));
    }

    free(unused_structures);

}