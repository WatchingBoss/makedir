/*
 * Reimplementation of UNIX mkdir utiliry
 */

#include "include/inc.h"

int main(int argc, char *argv[])
{
	char c;
	bool verbose = false, parents = false;

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
				if(argc > 2
				   && strcmp(argv[argc - 2], "-v")
				   && strcmp(argv[argc - 2], "-p"))
					parent_dirs(argv[argc - 2], argv[argc - 1], verbose, parents);
				else
					parent_dirs("pwd", argv[argc - 1], verbose, parents);
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
		create_dir(argv[argc - 2], argv[argc - 1], verbose, parents);
	else
		create_dir("pwd", argv[argc - 1], verbose, parents);

	exit(EXIT_SUCCESS);
}

void parent_dirs(char *source_dir, char *names, bool verbose, bool parents)
{
	int words_count = 0;
	char dir_names[10][50], directory[DIR_NAME];

	parents = true;
	
	for(int l = 0, i = names[l], x = 0;
		(char)i != '\0';
		i = names[++l])
	{
		if(i == '/')
		{
			dir_names[words_count][x] = '\0';
			x = 0;
			++words_count;
			continue;
		}

		dir_names[words_count][x++] = i;
	}
	dir_names[words_count][strlen(dir_names[words_count])] = '\0';

	if(!strcmp(source_dir, "pwd"))
	{
		if(!getcwd(directory, sizeof(directory)))
			my_error("getcwd error");
	}
	else
		strncpy(directory, source_dir, sizeof(directory));

	for(int i = 0; i <= words_count; ++i)
	{
		create_dir(directory, dir_names[i], verbose, parents);
		strncat(directory, "/", sizeof(directory) - strlen(directory));
		strncat(directory, dir_names[i], sizeof(directory) - strlen(directory));
	}

	exit(EXIT_SUCCESS);
}

int create_dir(char *source_dir, char *name_dir, bool verbose, bool parents)
{
	char directory[DIR_NAME], total_path[DIR_NAME];

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

	if(!parents)
		exit(EXIT_SUCCESS);
}

/*
  Permissions for mkdir()
 
  User: S_IRUSR (read), S_IWUSR (write), S_IXUSR (execute)
  Group: S_IRGRP (read), S_IWGRP (write), S_IXGRP (execute)
  Others: S_IROTH (read), S_IWOTH (write), S_IXOTH (execute)

  Additionally, some shortcuts are provided (basically a bitwise-OR combination of the above

  Read + Write + Execute: S_IRWXU (User), S_IRWXG (Group), S_IRWXO (Others)
  DEFFILEMODE: Equivalent of 0666 = rw-rw-rw-
  ACCESSPERMS: Equivalent of 0777 = rwxrwxrwx 
*/
void my_error(char *e)
{
	perror(e);
	exit(EXIT_FAILURE);
}
