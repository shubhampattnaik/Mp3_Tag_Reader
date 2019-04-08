#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    e_success,
    e_failure
} status;

void print_help(void);
void print_usage(void);
status view_mp3_tags(char *);
status version_check(FILE *);
void get_size(FILE *, int *);
void fetch_info(FILE *, char *);
status edit_mp3_info(char *, char *, char *);
status search_tag(FILE*, int, char *, int *);

#endif
