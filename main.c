#include "main.h"

int main(int argc, char *argv[])
{
    char tag[5];

    if(argc < 2)
    {
	print_usage();
	return 1;
    }

    switch(argv[1][1])
    {
	case 'h':
	    print_help();
	    break;

	case 'v':
	    if(argc < 3)
	    {
		print_usage();
		return 2;
	    }
	    else
	    {
		//view tag function
		if(view_mp3_tags(argv[2]) == e_failure)
		{
		    fprintf(stderr, "Invalid File\n");
		    return 3;
		}
	    }
		break;

/*	case 'i':
		if(extract_album_art(argv[2]) == e_success)
		{
		    system("xdg-open icon.png");
		}
		break;*/

	case 'e':
	    if(argc < 5)
	    {
		print_usage();
		return 3;
	    }
	    else
	    {
		switch(argv[3][1])
		{
		    case 't'://title
			strcpy(tag, "TIT2");
			if((edit_mp3_info(argv[2], tag, argv[4])) == e_success)
			{
			    view_mp3_tags(argv[2]);
			}
			break;

		    case 'T'://track
			strcpy(tag, "TRCK");
			if((edit_mp3_info(argv[2], tag, argv[4])) == e_success)
			{
			    view_mp3_tags(argv[2]);
			}
			break;

		    case 'a'://artist
			strcpy(tag, "TPE1");
			if((edit_mp3_info(argv[2], tag, argv[4])) == e_success)
			{
			    view_mp3_tags(argv[2]);
			}
			break;

		    case 'A'://album
			strcpy(tag, "TALB");
			if((edit_mp3_info(argv[2], tag, argv[4])) == e_success)
			{
			    view_mp3_tags(argv[2]);
			}
			break;

		    case 'y'://year
			strcpy(tag, "TYER");
			if((edit_mp3_info(argv[2], tag, argv[4])) == e_success)
			{
			    view_mp3_tags(argv[2]);
			}
			break;

		    case 'c'://comment
			strcpy(tag, "COMM");
			if((edit_mp3_info(argv[2], tag, argv[4])) == e_success)
			{
			    view_mp3_tags(argv[2]);
			}
			break;

		    case 'g'://genre
			strcpy(tag, "TCON");
			if((edit_mp3_info(argv[2], tag, argv[4])) == e_success)
			{
			    view_mp3_tags(argv[2]);
			}
			break;

		    default:
			printf("Invalid Modifier\n");
		}
	    }
	    break;

	default:
	    printf("Invalid Option\n");
    }
    return 0;
}
