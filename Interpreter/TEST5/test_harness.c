#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main()
{
   char command[1000];

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test1.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test2.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test3.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test4.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test5.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test6.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test7.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test8.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test9.txt");
    system(command);

    strcpy(command, "gcc interpreter_main.c interpreter_sdl.c -o do -Wall -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm $(pkg-config --libs sdl2 --cflags)");
    system(command);
    strcpy(command, "./do test10.txt");
    system(command);

   return 0;
}
