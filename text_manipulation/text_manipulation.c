/*
Name: Darius Kianersi
UID: 118799810
Username: radius
*/

#include <stdio.h>
#include <string.h>
#include "text_manipulation.h"

int remove_spaces(const char *source, char *result, int *num_spaces_removed) {  
    int i, j;
    int last_char;
    int count = 0;

    /* First, check for fail cases */
    if (source == NULL || strlen(source) == 0) {
        return FAILURE;
    }

    /* Now, find the first nonspace character. Also keep
    track of the number of spaces in the beginning */
    for (i = 0; source[i]; i++) {
        if (source[i] == ' ') {
            count++;
        } else {
            break;
        }
    }
    /* Next, determine the last nonspace character. */
    for (j = i; source[j]; j++) {
        if (source[j] != ' ') {
            last_char = j;
        }
    }
    /* Copy all of the nonspace characters from the source to the result. */
    strncpy(result, source + i, last_char - i + 1);
    result[last_char - i + 1] = '\0';
    if (num_spaces_removed != NULL) {
        *num_spaces_removed = count + (j - last_char - 1);
    }
    return SUCCESS;
}

int center(const char *source, int width, char *result) {       
    int i, j = 0;
    int src_length, space_length;

    /* Check for fail cases. */
    if (source == NULL || (src_length = strlen(source)) == 0 || width < src_length) {
        return FAILURE;
    }
    space_length = (width - src_length) / 2;
    width = space_length * 2 + src_length;

    /* Iterate through the expected width of the string.
    On the edges of the width, add the necessary spaces. */
    for (i = 0; i < width; i++) {
        if (i < space_length || width - i <= space_length) {
            result[i] = ' ';
        } else {
            result[i] = source[j++];
        }
    }
    result[i] = '\0';

    return SUCCESS;
}
