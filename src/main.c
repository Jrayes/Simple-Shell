#include <stdio.h>
#include "defs.h"

int main(int argc, char **argv) {

    //read in the config file to set the shell's environment.
    //Config's will load current built-in's and pass them to simple_shell()

    //if we wish to enable job control, we will type nt, for new terminal.

    simple_shell();



}