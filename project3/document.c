/*
    Name: Darius Kianersi
    UID: 118799810
    Username: radius
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "document.h"

/*
    init_document initializes the document pointer
    as an empty document with 0 paragraphs and sets
    its name to the given name argument. It will fail
    if the document pointer is null, the name pointer
    is null, or the length of the name is greater than
    the maximum string size.
*/
int init_document(Document *doc, const char *name) {
    if (!doc || !name || strlen(name) > MAX_STR_SIZE) {
        return FAILURE;
    } else {
        doc->number_of_paragraphs = 0;
        strcpy(doc->name, name);
        return SUCCESS;
    }
}

/*
    reset_document sets the number of paragraphs
    for the document to 0. It will return FAILURE
    if the document pointer is null.
*/
int reset_document(Document *doc) {
    if (!doc) {
        return FAILURE;
    } else {
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
}

/*
    print_document prints the document's name and
    number of paragraphs, and then each paragraph
    line by line, with paragraphs separated by blank
    lines. It will return FAILURE if the doc pointer
    is null.
*/
int print_document(Document *doc) {
    if (!doc) {
        return FAILURE;
    } else {
        int i, j;

        printf("Document name: \"%s\"\n", doc->name);
        printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
        /* Iterate through paragraphs */
        for (i = 0; i < doc->number_of_paragraphs; i++) {
            /* Iterate through lines */
            for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
                printf("%s\n", doc->paragraphs[i].lines[j]);
            }
            if (j != 0 && i != doc->number_of_paragraphs - 1) {
                /* Add an empty line between paragraphs */
                printf("\n"); 
            }
        }
        return SUCCESS;
    }
}

/*
    add_paragraph_after adds a new paragraph after the given
    paragraph number. It initializes the new paragraph to
    have 0 lines. It iterates from the end of the paragraphs
    array to the beginning to shift each paragraph down by one.
    Then, in the empty spot at the location paragraph_number,
    it initializes the new paragraph.
*/
int add_paragraph_after(Document *doc, int paragraph_number) {
    if (
        !doc ||
        doc->number_of_paragraphs == MAX_PARAGRAPHS || 
        paragraph_number > doc->number_of_paragraphs
    ) {
        return FAILURE;
    } else {
        int i;
        Paragraph new_paragraph;

        new_paragraph.number_of_lines = 0;
        /* Shift each paragraph down by one */
        for (i = doc->number_of_paragraphs; i > paragraph_number; i--) {
            doc->paragraphs[i] = doc->paragraphs[i-1];
        }
        /* Fill the empty space with the new paragraph */
        doc->paragraphs[paragraph_number] = new_paragraph;
        doc->number_of_paragraphs++;
        return SUCCESS;
    }
}

/*
    add_line_after adds a new line after the given line number 
    within the given paragraph. It iterates from the end of the
    lines array to the beginning to shift each line down by one.
    Then, in the empty spot at the location line_number, it
    initializes the new line.
*/
int add_line_after(
    Document *doc, 
    int paragraph_number, 
    int line_number, 
    const char *new_line
) {
    if (!doc || !new_line || paragraph_number > doc->number_of_paragraphs
        || doc->paragraphs[paragraph_number - 1].number_of_lines
        == MAX_PARAGRAPH_LINES
        || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines
    ) {
        return FAILURE;
    } else {
        int i;
        Paragraph *para = &doc->paragraphs[paragraph_number - 1];

        /* Shift all of the lines down by one */
        for (i = para->number_of_lines; i > line_number; i--) {
            strcpy(para->lines[i], para->lines[i-1]);
        }
        /* Fill the empty space with the new line */
        strcpy(para->lines[line_number], new_line);
        para->number_of_lines++;
        return SUCCESS;
    }
}

/*
    get_number_lines_paragraph returns the number of lines in a given paragraph.
    Rather than returning from the function, however, it stores the number of
    lines in the number_of_lines out parameter. It returns FAILURE if the doc
    or number_of_lines pointers are null, or if the paragraph number is greater
    than the number of available paragraphs. Otherwise, it returns success.
*/
int get_number_lines_paragraph(
    Document *doc, 
    int paragraph_number, 
    int *number_of_lines
) {
    if (
        !doc || !number_of_lines || 
        paragraph_number > doc->number_of_paragraphs
    ) {
        return FAILURE;
    } else {
        *number_of_lines = 
            doc->paragraphs[paragraph_number - 1].number_of_lines;
        return SUCCESS;
    }
}

/*
    append_line adds a line to the end of the given paragraph. It returns
    FAILURE if the doc or new_line pointers are null, or if the paragraph
    number is greater than the number of available paragraphs. Otherwise,
    it returns SUCCESS.
*/
int append_line(Document *doc, int paragraph_number, const char *new_line) {
    if (!doc || !new_line || paragraph_number > doc->number_of_paragraphs
        || doc->paragraphs[paragraph_number - 1].number_of_lines 
        == MAX_PARAGRAPH_LINES
    ) {
        return FAILURE;
    } else {
        Paragraph *para = &doc->paragraphs[paragraph_number - 1];

        strcpy(para->lines[para->number_of_lines++], new_line);
        return SUCCESS;
    }
}

/*
    remove_line removes the given line from the given paragraph. It
    returns FAILURE if the doc pointer is null, if the paragraph number is
    greater than the number of available paragraphs, or if the line number
    is greater than the number of available lines. Otherwise, it returns 
    SUCCESS.
*/
int remove_line(Document *doc, int paragraph_number, int line_number) {
    if (!doc || paragraph_number > doc->number_of_paragraphs 
        || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines
    ) {
        return FAILURE;
    } else {
        Paragraph *para = &doc->paragraphs[paragraph_number - 1];
        int i, num_lines = --(para->number_of_lines);

        for (i = line_number - 1; i < num_lines; i++) {
            /* Iterates through the lines and shifts them all up by one
            to remove the line at position line_number - 1 */
            strcpy(para->lines[i], para->lines[i + 1]);
        }
        return SUCCESS;
    }
}

/* is_spaces determines whether a line is 
completely composed of whitespace. */
int is_spaces(const char *line) {
    const char *ptr;

    for (ptr = line; *ptr != '\0'; ptr++) {
        if (!isspace(*ptr)) {
            return 0;
        }
    }
    return 1;
}

/*
    load_document adds the first data_lines of data to the doc pointer.
    If one of the data lines is an empty string, then it creates a new
    paragraph. It returns FAILURE if the doc or data pointers are null,
    or if data_lines is 0. Otherwise, it returns SUCCESS.
*/
int load_document(
    Document *doc, 
    char data[][MAX_STR_SIZE + 1], 
    int data_lines
) {
    if (!doc || !data || data_lines == 0) {
        return FAILURE;
    } else {
        int i, j = 0;
        Paragraph curr_paragraph;

        curr_paragraph.number_of_lines = 0;
        for (i = 0; i < data_lines; i++) {
            /* Iterates through the data lines and checks if the
            data is the empty string */
            if (!is_spaces(data[i])) {
                /* If the line is not empty, it copies the line to the end
                of the current paragraph and increments the number of lines */
                strcpy(curr_paragraph.lines[j++], data[i]);
                curr_paragraph.number_of_lines = j;
            } else {
                /* If the line is empty, it copies the current paragraph to the
                list of paragraphs and resets the current line to 0. */
                doc->paragraphs[doc->number_of_paragraphs++] = curr_paragraph;
                j = 0;
            }
        }
        doc->paragraphs[doc->number_of_paragraphs++] = curr_paragraph;
        return SUCCESS;
    }
}

/*
    replace_text replaces all instances of the target text with the replacement
    text in the document. If the doc, target, or replacement pointers are null,
    then the function returns FAILURE. Otherwise, it returns SUCCESS.
*/
int replace_text(Document *doc, const char *target, const char *replacement) {
    if (!doc || !target || !replacement) {
        return FAILURE;
    } else {
        int i, target_length = strlen(target);
        int repl_length = strlen(replacement);

        for (i = 0; i < doc->number_of_paragraphs; i++) {
            /* Iterates through the paragraphs. */
            int j;
            Paragraph *curr_para = &doc->paragraphs[i];
            for (j = 0; j < curr_para->number_of_lines; j++) {
                /* Iterates through the lines. */
                int l = 0, k = 0, line_length = strlen(curr_para->lines[j]);
                char *curr_char = curr_para->lines[j];
                char output_line[MAX_STR_SIZE + 1];

                /* Iterates curr_char through each character in the line. */
                for (
                    curr_char = curr_para->lines[j]; 
                    curr_char && l <= line_length - target_length; 
                    l++, curr_char++
                ) {
                    /* The for loop should terminate before the n-th
                    last character in the line, where n is the length
                    of the target. This is because we will later check 
                    if the curr_char and target are equal using strncmp, 
                    and we don't want a segmentation fault if there 
                    aren't enough characters in curr_char to check. */
                    if (!strncmp(target, curr_char, target_length)) {
                        /* If this is true, the target is at this character. */
                        strcpy(output_line + k, replacement);
                        /* Copy the replacement string to the output line. */
                        /* Increment our position in the output line (k) 
                        and the position in the source line (curr_char) */
                        k += repl_length;
                        curr_char += target_length - 1;
                    } else {
                        /* If the target is not at the current char, 
                        we simply copy the current char to the output. */
                        output_line[k++] = curr_char[0];
                    }
                }
                /* Since we terminated the for loop before the n-th 
                last character in the line to prevent a segfault, 
                we now need to copy those end characters to the output. */
                strcpy(output_line + k, curr_char);
                /* Copy the output line to the actual lines 
                in the paragraph. */
                strcpy(curr_para->lines[j], output_line);
            }
        }
        return SUCCESS;
    }
}

/*
    highlight_text highlights the target text with the given 
    start and end strings. It will return FAILURE if the doc 
    or target pointers are null. Otherwise, it will return SUCCESS.
 */
int highlight_text(Document *doc, const char *target) {
    if (!doc || !target) {
        return FAILURE;
    } else {
        /* The function works simply by using the replace_text function
        defined above with the target str and the target str surrouned by
        the highlight characters. */
        char replacement[MAX_STR_SIZE + 1] = HIGHLIGHT_START_STR;

        strcat(replacement, target);
        strcat(replacement, HIGHLIGHT_END_STR);
        replace_text(doc, target, replacement);
        return SUCCESS;
    }
}

/*
    remove_text removes all instances of the string target in the document.
    It will return FAILURE if the doc or target pointers are null. Otherwise,
    it will return SUCCESS.
*/
int remove_text(Document *doc, const char *target) {
    if (!doc || !target) {
        return FAILURE;
    } else {
        /* Similarly to highlight_text, the function uses the replace_text
        method with target and an empty string as the replacement text. */
        replace_text(doc, target, "");
        return SUCCESS;
    }
}

/* 
    load_file uses similar logic as load_document to populate the paragraphs
    and lines of a document using input from a file stream. If the doc or
    filename pointers are null, or if the file can't be opened, then the
    function returns FAILURE. Otherwise, it returns success.
*/
int load_file(Document *doc, const char *filename) {
    FILE *input;
    if (!doc || !filename || (input = fopen(filename, "r")) == NULL) {
        return FAILURE;
    } else {
        char buf[MAX_STR_SIZE + 1];
        int paragraph = 1;

        /* By default, load_file creates a paragraph at the
        beginning of the document. */
        if (add_paragraph_after(doc, 0) == FAILURE) {
            /* If the paragraph can't be created, 
            e.g. if the document already has the 
            maximum paragraphs, then the function 
            returns FAILURE. */
            fclose(input);
            return FAILURE;
        }
        /* Iterate through the file stream. */
        while (fgets(buf, MAX_STR_SIZE + 1, input)) {
            if (!is_spaces(buf)) {
                /* If the line is not all spaces, then 
                we should append it to the current paragraph. */
                buf[strlen(buf) - 1] = '\0'; /* Remove newline char. */

                append_line(doc, paragraph, buf);
            } else if (doc->number_of_paragraphs == MAX_PARAGRAPHS) {
                return FAILURE;
            } else {
                /* If the line is completely composed of spaces,
                then we should create a new paragraph. */
                add_paragraph_after(doc, paragraph++);
            }
        }
        fclose(input);
        return SUCCESS;
    }
}

/* 
    save_document saves document to a file called filename. 
    The function returns FAILURE if the doc or filename
    pointers are null, or if the file can't be opened.
    Otherwise, it saves the document and returns SUCCESS.
*/
int save_document(Document *doc, const char *filename) {
    FILE *output;

    if (!doc || !filename || (output = fopen(filename, "w")) == NULL) {
        return FAILURE;
    } else {
        int i, j;

        /* Iterate through the paragraphs. */
        for (i = 0; i < doc->number_of_paragraphs; i++) {
            /* Iterate through the lines. */
            for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
                fprintf(output, "%s\n", doc->paragraphs[i].lines[j]);
            }
            if (j != 0 && i != doc->number_of_paragraphs - 1) {
                /* If we are not at the last paragraph, then add
                and empty line between paragraphs. */
                fputs("\n", output);
            }
        }
        fclose(output);
        return SUCCESS;
    }
}
