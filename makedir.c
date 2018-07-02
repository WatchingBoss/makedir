/*
 * Reimplementation of UNIX mkdir utiliry
 */

#include "include/inc.h"

int main(int argc, char *argv[])
{
	char c;
	bool verbose = false;

	if(argc == 2 && (!strcmp(argv[1], "-help") || !strcmp(argv[1], "--help")) )
		print_help();
	if(argc == 2 && (!strcmp(argv[1], "-version") || !strcmp(argv[1], "--version")) )
		print_version();

	while( (c = getopt(argc, argv, "vp:")) > 0)
	{
		switch(c)
		{
			case 'v':
				verbose = true;
				break;
			case 'p':
				printf("Use option: %c\n", c);
				break;
			case '?':
				my_error("Wrong argument, error");
				break;
			default:
				break;
		}
	}

	if(argc > 2
	   && strcmp(argv[argc - 2], "-v")
	   && strcmp(argv[argc - 2], "-p"))
		create_dir(argv[argc - 2], argv[argc - 1], verbose);
	else
		create_dir("pwd", argv[argc - 1], verbose);

	exit(EXIT_SUCCESS);
}

int create_dir(char *source_dir, char *name_dir, bool verbose)
{
	char directory[256], total_path[256];

	if(!strcmp(source_dir, "pwd"))
	{
		if(!getcwd(directory, sizeof(directory)))
			my_error("getcwd error");
	}
	else
		strncpy(directory, source_dir, sizeof(directory));
		
	strncat(directory, "/", sizeof(directory) - strlen(directory));
	strncat(directory, name_dir, sizeof(directory) - strlen(directory));
	memset(total_path, 0, sizeof(total_path));
	strncat(total_path, directory, sizeof(total_path));

	if( (mkdir(total_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) < 0)
		my_error("mkdir error");

	if(verbose && strcmp(source_dir, "pwd"))
		printf("makedir: created directory \"%s\" in \"%s\"\n", name_dir, source_dir);
	else if(verbose)
		printf("makedir: created directory \"%s\"\n", name_dir);

	exit(EXIT_SUCCESS);
}

void my_error(char *e)
{
	perror(e);
	exit(EXIT_FAILURE);
}
