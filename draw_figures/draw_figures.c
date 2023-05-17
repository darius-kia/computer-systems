#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int draw_rectangle(char *symbol, int width, int length) {
    int i, j;
    if (width <= 0 || length <= 0) {
        return 0;
    }
    for (i = 0; i < width; i++) {
        for (j = 0; j < length; j++) {
            printf("%s", symbol);
        }
        printf("\n");
    }
    return 1;
}

int draw_triangle(char *symbol, int size) {
    int char_length;
    const int line_length = size * 2 - 1;
    if (size <= 0) {
        return 0;
    }
    for (char_length = 1; char_length <= line_length; char_length += 2) {
        int i, j, k;
        int spaces = (line_length - char_length) / 2;
        for (i = 0; i < spaces; i++) {
            printf(" ");
        }
        for (j = 0; j < char_length; j++) {
            printf("%s", symbol);
        }
        for (k = 0; k < spaces; k++) {
            printf(" ");
        }
        printf("\n");
    }
    return 1;
}

int valid_character(char *symbol) {
    return !(strcmp(symbol, "%") && strcmp(symbol, "*") && strcmp(symbol, "#"));
}

int main() {
    int type;
    while (1) {
        int scan_res;
        printf("Enter 1(rectangle), 2(triangle), 3(other), 0(quit): ");
        fflush(stdin);
        scan_res = scanf("%d", &type);
        if (scan_res == 1) {
            if (type == 0) {
                printf("Bye Bye.\n");
                break;
            } else if (type == 1) {
                char symbol[2];
                int width, length;
                printf("Enter character, width and length: ");
                if (scanf(" %s %d %d", symbol, &width, &length) == 3 && valid_character(symbol)) {
                    int draw_res;
                    draw_res = draw_rectangle(symbol, width, length);
                    if (draw_res == 0) {
                        printf("Invalid data provided.\n");
                    }
                } else {
                    printf("Invalid data provided.\n");
                }
            } else if (type == 2) {
                int size;
                char symbol[2];
                printf("Enter character and size: ");
                if (scanf(" %s %d", symbol, &size) == 2 && valid_character(symbol)) {
                    int draw_res;
                    draw_res = draw_triangle(symbol, size);
                    if (draw_res == 0) {
                        printf("Invalid data provided.\n");
                    }
                } else {
                    printf("Invalid data provided.\n");
                }
            } else if (type == 3) {
            } else {
                printf("Invalid choice.\n");
            }
        } else {
            printf("Invalid choice.\n");
        }
    }
}
