#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include "util.h"

int main(int argc, char *argv[])
{

	int i         = 0;
	int m         = 0;
	int n         = 0;
	int flags     = 0;
	int z         = 0;
	char temp     = 0;
	char temp_d   = 0;
	int flag      = 0;
	int count     = 0;
	int pcount    = 0;
	int total_len = 0;
	int enc_len   = 0;
	FILE *fp      = NULL;
	char buff[1];
	char print_data[BUFF_SIZE];

	char ebuff[3];
	char dbuff[4];
        unsigned int first_seq  = 0;
        unsigned int sec_seq    = 0;
        unsigned int third_seq  = 0;
        unsigned int fourth_seq = 0;
	unsigned int first      = 0;
        unsigned int sec        = 0;
        unsigned int third      = 0;

        memset(ebuff, 0, 3);
	memset(dbuff, 0, 4);
	memset(buff, 0, 1);
	memset(print_data, 0, BUFF_SIZE);

	if(argc < MIN_ARGS || argc > MAX_ARGS)
	{
		fprintf(stderr, "\r\nerror: malformed command :invalid no.of arguments\r\nusage: hw1 hexdump [file]\r\n       hw1 enc-base64 [file]\r\n       hw1 dec-base64 [file]\r\n");
                exit(-1);

	}
	if(strcmp(argv[1], "hexdump") && strcmp(argv[1], "enc-base64") && strcmp(argv[1], "dec-base64"))
        {
                fprintf(stderr, "\r\nerror: malformed command :invalid second argument\r\nusage: hw1 hexdump [file]\r\n       hw1 enc-base64 [file]\r\n       hw1 dec-base64 [file]\r\n");
                exit(-1);
        }

	if(argc == MIN_ARGS)
	{
		fp = stdin;
		
	}
	else
	{
		fp = fileValidator(fp, argv[2]);

	}
	if(!strcmp(argv[1], "hexdump"))
	{
		while(!feof(fp)) 
		{
			count++;
			fread(buff, 1, 1, fp);
		
			if(total_len % 16 == 0)
			{
				printf("%06x: ", total_len);
			}
			if(feof(fp))
			{
				break;
			}
			printf("%02x ", buff[0] & 0xff);

			if((buff[0] >= 0x20) && (buff[0] <= 0x7e))
                	{
				print_data[pcount] = buff[0];
				pcount++;
                	}
			else if( (buff[0] & 0xff) >= 0x80)
                	{
                        	print_data[pcount] = '~';
                        	pcount++;
                	}

                	else if(((buff[0] & 0xff) <= 0x1f) || ((buff[0] & 0xff) == 0x7f))
                	{
				print_data[pcount] = '.';
				pcount++;
                	}
		
			if(count % 8 == 0)
			{
				printf(" ");
			}
			total_len++;
		
			if(total_len % 16 == 0)
                	{
				for(i = 0; i < 16; i++)
				{
					printf("%c", print_data[i]);
				}
				printf("\n");
				pcount = 0;
				flag = 1;
				memset(print_data, 0, BUFF_SIZE);
                	}
			else
			{
				flag = 0;
			}
			memset(buff, 0, 1);
		}
		if(!flag)
		{
			while(count % 16 != 0)
			{
				printf("-- ");
				if(count % 8 == 0)
                		{
                        		printf(" ");
                		}
				count++;
			}
			printf("--  ");
			for(i = 0; i < pcount; i++)
        		{
        			printf("%c", print_data[i]);
        		}
			for(; i < 16; i++)
			{
				printf(" ");
			}
		}
		printf("\n");
   		fclose(fp);
	}
	if(!strcmp(argv[1], "enc-base64"))
        {
		memset(ebuff, 0, 3);
		n = 0;
		while(!feof(fp))
		{
			fread(&temp, 1, 1, fp);
			if(feof(fp))
			{
				break;
			}
			ebuff[n++] = temp;
			if(n < 3)
			{
				continue;
			}
			else
			{
				n = 0;
			}
			first_seq  = (ebuff[0] & 0xff) >> 2;
                        sec_seq    = (((ebuff[0] & 0xff) & 0x03) << 4) | ((ebuff[1] & 0xff) >> 4);
			third_seq  = (((ebuff[1] & 0xff) & 0x0f) << 2) | (((ebuff[2] & 0xff) & 0xc0) >>6);
                        fourth_seq = (((ebuff[2] & 0xff) & 0x3f));
                        printf("%c%c%c%c", encode_table[first_seq], encode_table[sec_seq],
                                       encode_table[third_seq], encode_table[fourth_seq]);
			enc_len += 4;
			if(enc_len == 64)
			{
				enc_len = 0;
				printf("\n");
			}
        	}
		if(n == 1)
		{
			ebuff[1]   = 0x00;
                        ebuff[2]   = 0x00;
			first_seq  = (ebuff[0] & 0xff) >> 2;
                        sec_seq    = (((ebuff[0] & 0xff) & 0x03) << 4) | ((ebuff[1] & 0xff) >> 4);
			printf("%c%c==\n", encode_table[first_seq], encode_table[sec_seq]);
			enc_len += 4;
		}
		else if(n == 2)
		{
			ebuff[2]   = 0x00;
			first_seq  = (ebuff[0] & 0xff) >> 2;
                        sec_seq    = (((ebuff[0] & 0xff) & 0x03) << 4) | ((ebuff[1] & 0xff) >> 4);
			third_seq  = (((ebuff[1] & 0xff) & 0x0f) << 2) | (((ebuff[2] & 0xff ) & 0xc0)>>6);
                        printf("%c%c%c=\n", encode_table[first_seq], encode_table[sec_seq], encode_table[third_seq]);
			enc_len += 4;
			
		}
		else
		{
			if(enc_len != 64)
                        {
                                printf("\n");
                        }
		}
		fclose(fp);
	}
	if(!strcmp(argv[1], "dec-base64"))
        {
                memset(ebuff, 0,  4);
		m = 0;
		while(!feof(fp))
                {
			flags = 0x00;
			fread(&temp_d, 1, 1, fp);
                        if(feof(fp))
                        {
                                break;
                        }
			if((temp_d & 0xff) == '\n')
			{
				continue;
			}
                        dbuff[m++] = temp_d;
                        if(m < 4)
                        {
                                continue;
                        }
                        else
                        {
                                m = 0;
                        }

			for(z = 0; z < 64; z++)
		        {
                		if((dbuff[0]& 0xff) == encode_table[z])
                		{
                       			 first_seq = z;
					 flags |= 0x01;
                		}
                		if((dbuff[1]& 0xff) == encode_table[z])
                		{
                	     		 sec_seq   = z;
					 flags |= 0x02;
                		}
                		if((dbuff[2]& 0xff) == encode_table[z])
                		{
                        		third_seq  = z;
					flags |= 0x04;
                		}
                		if((dbuff[3]& 0xff) == encode_table[z])
                		{
                        		fourth_seq = z;
					flags |= 0x08;
                		}
        		}
			if((flags & 0x01) != 1)
			{
				if((dbuff[0]& 0xff) != '=')
				{
					fprintf(stderr,"error: not a valid encoded file - invalid characters\r\n");
					exit(-1);
				}
			}
			if(((flags & 0x02) >> 1) != 1)
                        {
                                if((dbuff[1]& 0xff) != '=')
                                {
                                        fprintf(stderr,"error: not a valid encoded file - invalid characters\r\n");
                                        exit(-1);
                                }
                        }
			if(((flags & 0x04) >> 2) != 1)
                        {
                                if((dbuff[2]& 0xff) != '=')
                                {
                                        fprintf(stderr,"error: not a valid encoded file - invalid characters\r\n");
                                        exit(-1);
                                }
                        }
			if(((flags & 0x08) >> 3) != 1)
                        {
                                if((dbuff[3]& 0xff) != '=')
                                {
                                        fprintf(stderr,"error: not a valid encoded file - invalid characters\r\n");
                                        exit(-1);
                                }
                        }


			if(((dbuff[2]& 0xff) == '=') && ((dbuff[3]& 0xff) == '='))
			{
				first  = (first_seq << 2) | (sec_seq >> 4);
                                printf("%c", first);
			}
			else if((dbuff[3]& 0xff) == '=')
			{
				first  = (first_seq << 2) | (sec_seq >> 4);
                                sec    = (((sec_seq) & 0x0f) << 4) | (third_seq >> 2);
				printf("%c%c", first, sec);

			}
			else
			{
				first  = (first_seq << 2) | (sec_seq >> 4);
        			sec    = (((sec_seq) & 0x0f) << 4) | (third_seq >> 2);
       				third  = (((third_seq) & 0x03) << 6) | (fourth_seq);
				printf("%c%c%c", first, sec, third);	
			}
		}
		if(m != 0)
		{
			fprintf(stderr, "error: not a valid encoded file - invalid file size\r\n");
			exit(-1);
		}
		fclose(fp);
	}
   	return(0);
	
}
