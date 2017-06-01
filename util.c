#include <sys/types.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

char encode_table[] =
{
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
};

FILE *fileValidator(FILE *fp, char *file)
{
	fp = fopen(file, "r");

	if(!fp)
	{
		fprintf(stderr, "\r\nerror: %s %s\n", file, strerror(errno));
		exit(-1);
	}
	if(errno)
	{
		fprintf(stderr, "\r\nerror: %s %s\n", file, strerror(errno));
		exit(-1);
	}
	return fp;
}
