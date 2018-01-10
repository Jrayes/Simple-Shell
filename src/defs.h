#define FAILED 1
#define EXIT 0
#define MAX_INPUT_SIZE 1000
#define MAX_TOKENS 500
struct current_builtins {

    int number;

    char **builtins;

};
void simple_shell();

char **tokenize(char *input);

int execute_commands(char **args);

