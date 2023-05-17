/* Implement your shell here */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sysexits.h>
#include <ctype.h>

#define MAX_LINE 1024

int main() {
    char buf[MAX_LINE + 1];

    printf("shell_jr: ");
    fflush(stdout);

    while (fgets(buf, MAX_LINE + 1, stdin)) {
        char *args[3];

        char **next = args;
        char *temp = strtok(buf, " \n");
        while (temp != NULL)
        {
            *next++ = temp;
            temp = strtok(NULL, " \n");
        }
        *next = NULL;

        if (!strcmp(args[0], "exit") || !strcmp(args[0], "hastalavista")) {
            printf("See you\n");
            fflush(stdout);
            exit(0);
        } else if (!strcmp(args[0], "cd")) {
            int status;
            sscanf(buf, " cd %s", args[1]);
            status = chdir(args[1]);
            if (status == -1) {
                printf("Cannot change to directory %s\n", args[1]);
                fflush(stdout);
            }
        } else {
            int fd = fork();
            if (fd < 0) {
                perror("fork error");
            } else if (fd == 0) {
                execvp(args[0], args);
                printf("Failed to execute %s\n", args[0]);
                fflush(stdout);
                exit(EX_OSERR);
            } else {
                wait(NULL);
            }
        }
        printf("shell_jr: ");
        fflush(stdout);
    }
}
