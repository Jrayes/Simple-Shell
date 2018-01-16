#define FAILED -2
#define EXIT -1
#define EXIT_CODE 1
#define MAX_INPUT_SIZE 100
#define MAX_TOKENS 50
#define MAX_ALLOWABLE_JOBS 10
struct current_builtins {

    int number;

    char **builtins;

};

struct process {

    int pgid;

    char open_file_descriptor;

    int infile;

    int outfile;

};

struct termios {

};

void simple_shell();

char **tokenize(char *input, char **args);

int execute_commands(char **args);

void init_shell();

