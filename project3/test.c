#include <stdio.h>

int main() {
    char first_arg[30], second_arg[30], third_arg[30];
    int num = scanf("%s", first_arg);
    printf("%d %s", num, first_arg);
    return 0;
}
