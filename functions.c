#include "main.h"

/* Print usage message */
void print_usage(void)
{
    printf("Error: ./mp3_tag_reader: Invalid Arguments\n");
    printf("Usage: ./mp3_tag_reader -h for help\n");
    return;
}

/* Print help message */
void print_help(void)
{
    char *filename = "help.txt";
    int ch; // Store data from file here

    FILE *fptr; //File ptr (Also called as file handle)

    // Open the file
    fptr = fopen(filename, "r");

    // Check for error
    if (fptr == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", filename);
	return;
    }

    // Read and print the file contents
    ch = fgetc(fptr);
    while (ch != EOF)
    {
	// Print the char to stdout
	putchar(ch);
	//Read the next char
	ch = fgetc(fptr);
    }

    // Close the file
    fclose(fptr);
}

/* Function to view tags */
status view_mp3_tags(char *filename)
{
    int size, count = 0;
    char ch, tag[5];
    FILE *fptr;

    fptr = fopen(filename, "r");
    if (!fptr)
    {
	perror("fopen");
	return e_failure;
    }
    if (version_check(fptr) == e_failure)
    {
	fclose(fptr);
	return e_failure;
    }
    else
    {
	get_size(fptr, &size);
	//printf("size		:	%d\n", size);

	/*============================================
	 * Check tags and string size
	 =============================================*/
	while ((ch = fgetc(fptr)) != EOF)
	{
	    fread(tag, 1, 4, fptr);
	    tag[4] = '\0';

	    //puts(tag);

	    if (strcmp("TIT2", tag) == 0)
	    {
		printf("Title		:	");
		fetch_info(fptr, tag);
	    }
	    else if (strcmp("TALB", tag) == 0)
	    {
		printf("Album		:	");
		fetch_info(fptr, tag);
	    }
	    else if (strcmp("TYER", tag) == 0)
	    {
		printf("Year		:	");
		fetch_info(fptr, tag);
	    }
	    else if (strcmp("TCON", tag) == 0)
	    {
		printf("Genre		:	");
		fetch_info(fptr, tag);
	    }
	    else if (strcmp("TRCK", tag) == 0)
	    {
		printf("Track		:	");
		fetch_info(fptr, tag);
	    }
	    else if (strcmp("TPE1", tag) == 0)
	    {
		printf("Artist		:	");
		fetch_info(fptr, tag);
	    }
	    else if (strcmp("TCOM", tag) == 0)
	    {
		printf("Composer		:	");
		fetch_info(fptr, tag);
	    }
	    else if (strcmp("COMM", tag) == 0)
	    {
		printf("Comment		:	");
		fetch_info(fptr, tag);
	    }

	    count++;
	    //printf("%d\n", count);
	    fseek(fptr, -1L, SEEK_CUR);
	}
	printf("==========================================================\n");
	printf("==========================================================\n");

	fclose(fptr);
    }
    return e_success;
}

status version_check(FILE *fptr)
{
    int r_data, w_data;
    char buf[4];
    int ver, sub_ver, ch;

    if ((r_data = fread(buf, 1, 3, fptr)) != 3)
    {
	perror("fread");
	return e_failure;
    }
    buf[3] = '\0';
    if (strcmp("ID3", buf) == 0)
    {
	while ((ch = fgetc(fptr)) != EOF)
	{
	    ver = ch;
	    break;
	}

	while ((ch = fgetc(fptr)) != EOF)
	{
	    sub_ver = ch;
	    break;
	}
	system("clear");
	printf("\n==========================================================\n");
	printf("Mp3 Tag Reader and Editor");
	printf("\n==========================================================\n");
	printf("version ID	:	%s v2.%d.%d\n\n", buf, ver, sub_ver);

	return e_success;
    }
}

void get_size(FILE *fptr, int *size)
{
    int ch, count = 0;
    *size = 0;
    int arr[5];

    while ((ch = fgetc(fptr)) != EOF)
    {
	arr[count++] = ch;
	if (count == 4)
	    break;
    }

    int b_size = *size;
    
    *size =  ((*size & 0x00ff00ff ) << 8)  | ((*size & 0xff00ff00) >> 8);
    *size = ((*size & 0xffff) << 16 ) | (( *size & 0xffff0000) >> 16);
    
    //printf("size = %d\n", *size);
    *size = 264601;

    return;
}

void fetch_info(FILE *fptr, char *tag)
{
    int tag_size = 0, c, count = 0, i = 0, flag = 0;
    char buffer[50];

    while ((c = fgetc(fptr)) != EOF)
    {
	tag_size += c;
	count++;
	if (count == 4)
	    break;
    }
    //printf("Tag = %s, %d\n", tag, tag_size);
    if(strcmp("COMM", tag) == 0)
    {
	fseek(fptr, 7L, SEEK_CUR);
    }

    while (i != (tag_size - 1))
    {
	c = fgetc(fptr);
	if (!flag)
	{
	    if ((c >= 0 && c < 32))
		continue;
	    flag = 1;
	}
	if (flag)
	{
	   //if (c != '\0')
		//printf("%c", c);
		buffer[i++] = c;
	}
	if(strcmp("COMM", tag) == 0)
	{
	    if(i == (tag_size - 5))
		break;
	}
    }
    buffer[i] = '\0';
    printf("%s\n", buffer);

    printf("\n");
}

status edit_mp3_info(char *filename, char *tag, char *data)
{
    int size, tag_size, ch, i = 0, flag = 0, rem_size;
    FILE *fptr;
    int data_len = strlen(data);

    fptr = fopen(filename, "r+");
    if (!fptr)
    {
	perror("fopen");
	return e_failure;
    }
    if (version_check(fptr) == e_failure)
    {
	fclose(fptr);
	return e_failure;
    }
    else
    {
	get_size(fptr, &size);
	//printf("size		:	%d\n", size);
    }

    if(search_tag(fptr, size, tag, &tag_size) == e_success)
    {
	if(data_len < tag_size)
	{
	    rem_size = (tag_size - data_len) - 1;

	    while (i != (data_len))
	    {
		ch = fgetc(fptr);
		if (!flag)
		{
		    if ((ch >= 0 && ch < 32) || (ch == '\0'))
			continue;
		    flag = 1;
		}
		if (flag)
		{
		    if (ch != '\0')
		    {
			fseek(fptr, -1L, SEEK_CUR);
			fputc(data[i++], fptr);
		    }
		}
	    }
	    while(rem_size)
	    {
		fputc('\0', fptr);
		rem_size--;
	    }
	    //fputc('\0', fptr);
	}
    }
    else
    {
	fprintf(stderr, "Tag not available\n");
	return e_failure;
    }

    fclose(fptr);

    return e_success;
}

status search_tag(FILE *fptr, int file_size, char *tag, int *tag_size)
{
    int i, j, count = 0, c;
    char buf_tag[5], ch;

    *tag_size = 0;

    for(i = 0; i < file_size; i++)
    {
	if((ch = fgetc(fptr)) == tag[0])
	{
	    buf_tag[0] = ch;
	    for(j = 1; j < 4; j++)
	    {
		if((ch = fgetc(fptr)) != tag[j])
		    break;
		else
		    buf_tag[j] = ch;
	    }
	    buf_tag[j] = '\0';
	    if(strcmp(tag, buf_tag) == 0)
	    {
		//puts("Match");
		while ((c = fgetc(fptr)) != EOF)
		{
		    *tag_size += c;
		    count++;
		    if (count == 4)
			break;
		}
		//printf("Tag = %s, %d\n", tag, tag_size);

		return e_success;
	    }
	}
    }
    return e_failure;
}
