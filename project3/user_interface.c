#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sysexits.h>

#include "document.h"

#define VALID_STR_LENGTH 1024

/* 
    do_nothing determines whether a command
    should actually execute anything.
    Specifically, if it is entirely composed
    of spaces, or if it begins with a # character.
*/
int do_nothing(const char *line) {
    const char *ptr;
    for (ptr = line; *ptr != '\0'; ptr++) {
        if (*ptr == '#') {
            return 1;
        } else if (!isspace(*ptr)) {
            return 0;
        }
    }
    return 1;
}

/*
    copy_line copies all the characters following
    a * character from source into dest.
*/
void copy_line(const char *source, char *dest) {
    const char *ptr;
    for (ptr = source; *ptr != '*'; ptr++);
    strcpy(dest, ptr+1);
    dest[strlen(dest) - 1] = '\0'; /* remove newline character */
}

/* 
    count_quotes returns the integer number of quotes
    in a string source. 
*/
int count_quotes(const char *source) {
    int i, len = strlen(source), count = 0;
    for (i = 0; i < len; i++) {
        count += (source[i] == '"');
    }
    return count;
}

/*
    data_after determines if there is extraneous
    data after a given number of args in a command.
*/
int data_after(const char *source, int args) {
    int i;
    const char *ptr = source;
    for (i = 0; i < args; i++) {
        while (isspace(*ptr)) {
            ptr++;
        }
        /* reached first nonspace char */
        while (!isspace(*ptr)) {
            ptr++;
        }
        /* reached end of arg */
    }
    while (*ptr != '\0') {
        /* check if there is any leftover data */
        if (!isspace(*ptr)) {
            return 1;
        }
        ptr++;
    }
    return 0;
}

/*
    store_quotes takes a string that has num_words
    words surrounded by quotes, and stores the text
    within the quotes into dest_one, and optionally
    dest_two if num_words > 1.
*/
void store_quotes(
    const char *source, 
    int num_words, 
    char *dest_one, 
    char *dest_two
) {
    int j, loop_start = 0;
    char *dest = dest_one;
    for (j = 0; j < num_words; j++) {
        int start, end, len = strlen(source);
        /* iterate until we reach the start quote */
        for (start = loop_start; start < len && source[start] != '"'; start++);
        /* iterate until we reach the end quote */
        for (end = start + 1; end < len && source[end] != '"'; end++);
        /* copy the string in between to dest */
        strncpy(dest, source + start + 1, end - start - 1);
        dest[end - start - 1] = '\0';
        loop_start = end + 1;
        dest = dest_two;
    }
}

/* 
    exec_cmd performs the necessary logic
    to execute commands on the document
    based on a command string.
*/
int exec_cmd(const char *cmd, Document *doc) {
    if (!do_nothing(cmd)) {
        char first_arg[VALID_STR_LENGTH + 1];

        /* Store the first argument of the
        command into first_arg. */
        sscanf(cmd, " %s", first_arg);

        /* Perform logic depending on the command. */
        if (!strcmp(first_arg, "add_paragraph_after")) {
            int paragraph_number;
            int num = sscanf(cmd, " add_paragraph_after %d", &paragraph_number);

            /* Fails if PARAGRAPH_NUMBER is missing, it's negative,
            not a number, or there is additional info after
            the second argument. */
            if (num != 1 || paragraph_number < 0 || data_after(cmd, 2)) {
                return -1;
            } else {
                if (add_paragraph_after(doc, paragraph_number) == FAILURE) {
                    printf("add_paragraph_after failed\n");
                }
            }
        } else if (!strcmp(first_arg, "add_line_after")) {
            int paragraph_number, line_number;
            char line[MAX_STR_SIZE + 1];
            int num = sscanf(
                cmd,
                " add_line_after %d %d *%s", 
                &paragraph_number, &line_number, line
            );

            /* Fails if there aren't three additional arguments,
            the paragraph number is 0 or negative, or the line
            number is negative. */
            if (num != 3 || paragraph_number <= 0 || line_number < 0) {
                return -1;
            } else {
                copy_line(cmd, line);
                if (
                    add_line_after(doc, paragraph_number, line_number, line)
                    == FAILURE
                ) {
                    printf("add_line_after failed\n");
                }
                    
            }
        } else if (!strcmp(first_arg, "print_document")) {
            if (data_after(cmd, 1)) {
                return -1;
            } else {
                print_document(doc);
            }
        } else if (!strcmp(first_arg, "quit")) {
            if (data_after(cmd, 1)) {
                return -1;
            } else {
                return 0;
            }
        } else if (!strcmp(first_arg, "exit")) {
            if (data_after(cmd, 1)) {
                return -1;
            } else {
                return 0;
            }
        } else if (!strcmp(first_arg, "append_line")) {
            int paragraph_number;
            char line[MAX_STR_SIZE + 1];
            int num = sscanf(
                cmd, 
                " append_line %d * %s", 
                &paragraph_number, line
            );

            /* Fails if there aren't two additional arguments
            or the paragraph number is 0 or negative. */
            if (num != 2 || paragraph_number <= 0) {
                return -1;
            } else {
                copy_line(cmd, line);
                if (append_line(doc, paragraph_number, line) == FAILURE) {
                    printf("append_line failed\n");
                }
            }
        } else if (!strcmp(first_arg, "remove_line")) {
            int paragraph_number, line_number;
            int num = sscanf(
                cmd, 
                " remove_line %d %d", 
                &paragraph_number, &line_number
            );

            /* Fails if there aren't two additional arguments, the
            paragraph number is 0 or negative, the line number
            is 0 or negative, or there is data after the 3rd argument. */
            if (
                num != 2 || 
                paragraph_number <= 0 || 
                line_number <= 0 || 
                data_after(cmd, 3)
            ) {
                return -1;
            } else {
                if (
                    remove_line(doc, paragraph_number, line_number) 
                    == FAILURE
                ) {
                    printf("remove_line failed\n");
                }
            }
        } else if (!strcmp(first_arg, "load_file")) {
            char filename[VALID_STR_LENGTH + 1];
            int num = sscanf(cmd, " load_file %s", filename);

            /* Fails if there isn't one additional argument,
            or there is data after the second argument. */
            if (num != 1 || data_after(cmd, 2)) {
                return -1;
            } else {
                if (load_file(doc, filename) == FAILURE) {
                    printf("load_file failed\n");
                }
            }
        } else if (!strcmp(first_arg, "replace_text")) {
            char target[MAX_STR_SIZE + 1], repl[MAX_STR_SIZE + 1];

            /* Fails if there aren't two sets of quotes
            i.e., there isn't a target and replacement. */
            if (count_quotes(cmd) != 4) {
                return -1;
            } else {
                store_quotes(cmd, 2, target, repl);
                if (replace_text(doc, target, repl) == FAILURE) {
                    printf("replace_text failed\n");
                }
            }
        } else if (!strcmp(first_arg, "highlight_text")) {
            char target[MAX_STR_SIZE + 1];

            /* Fails if there isn't one set of quotes,
            i.e. there isn't a target. */
            if (count_quotes(cmd) != 2) {
                return -1;
            } else {
                store_quotes(cmd, 1, target, NULL);
                highlight_text(doc, target);
            }
        } else if (!strcmp(first_arg, "remove_text")) {
            char target[MAX_STR_SIZE + 1];

            /* Fails if there isn't one set of quotes,
            i.e. there isn't a target. */
            if (count_quotes(cmd) != 2) {
                return -1;
            } else {
                store_quotes(cmd, 1, target, NULL);
                remove_text(doc, target);
            }
        } else if (!strcmp(first_arg, "save_document")) {
            char filename[VALID_STR_LENGTH + 1];
            int num = sscanf(cmd, " save_document %s", filename);

            /* Fails if there isn't one additional argument,
            or there is data after the second argument. */
            if (num != 1 || data_after(cmd, 2)) {
                return -1;
            } else {
                if (save_document(doc, filename) == FAILURE) {
                    printf("save_document failed\n");
                }
            }
        } else if (!strcmp(first_arg, "reset_document")) {
            /* Fails if there is data after the first argument. */
            if (data_after(cmd, 1)) {
                return -1;
            } else {
                reset_document(doc);
            }
        } else {
            return -1;
        }
        return 1;
    } else {
        return 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        /* There shouldn't be more than two arguments. */
        fprintf(
            stderr, 
            "Usage: user_interface\nUsage: user_interface <filename>\n"
        );
        exit(EX_USAGE);
    } else {
        char line[VALID_STR_LENGTH + 1];
        FILE *input;
        Document doc;
        int input_type;

        init_document(&doc, "main_document");
        
        /* If there are two arguments, then a file
        was specified, so that should be the input
        stream. Otherwise, use standard input. */
        if (argc == 2) {
            input = fopen(argv[1], "r");
            input_type = 0;
        } else {
            input = stdin;
            input_type = 1;
        }

        if (input_type == 1) {
            printf("> ");
        }
        while (fgets(line, VALID_STR_LENGTH + 1, input)) {
            int status = exec_cmd(line, &doc);

            /* status of -1 indicates an invalid
            command, status of 0 indicates quit/exit,
            status of 1 indicates successful command. */
            if (status == -1) {
                printf("Invalid Command\n");
                if (input_type == 1) {
                    printf("> ");
                }
            } else if (status == 0) {
                break;
            } else {
                if (input_type == 1) {
                    printf("> ");
                }
            }
        }
    }
    return 0;
}
