#include <sys/types.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define BUFF_SIZE 18
#define BYTE_LEN 8
#define MIN_ARGS 2
#define MAX_ARGS 3

extern FILE *fileValidator(FILE *fp, char *file);
extern char encode_table[];
