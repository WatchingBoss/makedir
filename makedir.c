/*
 * Reimplementation of UNIX mkdir utiliry
 */

#include "include/inc.h"

bool verbose = false, parents = false;

int main(int argc, char *argv[])
{
	char c;

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
				parents = true;
				break;
			case '?':
				system_error("Wrong argument, error");
				break;
			default:
				break;
		}
	}

	call_creation(argc, argv);

	exit(EXIT_SUCCESS);
}

void call_creation(int argc, char *argv[])
{
	if(check_if_existing_path(argv[argc - 1]))
		user_error("You do not want create %s", argv[argc - 1]);

	for(int i = argc - 1; i != 0; --i)
	{
		if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "-v"))
			break;

		if(check_if_existing_path(argv[i]))
			continue;
		else
		{
			if(check_if_slash(argv[i]))
			{
				if(parents == true)
				{
					if(check_if_existing_path(argv[i - 1]))
						create_parent_dir(argv[i - 1], argv[i]);
					else
						create_parent_dir("pwd", argv[i]);
				}
				else
					user_error("You did not specify '-p' option to create %s", argv[i]);
			}
			else
			{
				if(check_if_existing_path(argv[i - 1]))
					create_dir(argv[i - 1], argv[i]);
				else
					create_dir("pwd", argv[i]);
			}
		}
	}

	exit(EXIT_SUCCESS);
}

void create_parent_dir(const char *source_dir, const char *names)
{
	int words_count = 0;
	char dir_names[10][50], directory[DIR_NAME];

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
			system_error("getcwd error");
	}
	else
		strncpy(directory, source_dir, sizeof(directory));

	for(int i = 0; i <= words_count; ++i)
	{
		create_dir(directory, dir_names[i]);
		strncat(directory, "/", sizeof(directory) - strlen(directory));
		strncat(directory, dir_names[i], sizeof(directory) - strlen(directory));
	}
}

void create_dir(const char *source_dir, const char *name_dir)
{
	char directory[DIR_NAME], total_path[DIR_NAME];

	if(!strcmp(source_dir, "pwd"))
	{
		if(!getcwd(directory, sizeof(directory)))
			system_error("getcwd error");
	}
	else
		strncpy(directory, source_dir, sizeof(directory));

	strncat(directory, "/", sizeof(directory) - strlen(directory));
	strncat(directory, name_dir, sizeof(directory) - strlen(directory));
	memset(total_path, 0, sizeof(total_path));
	strncat(total_path, directory, sizeof(total_path));

	if( (mkdir(total_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) < 0)
		system_error("mkdir error");

	if(verbose && strcmp(source_dir, "pwd"))
		printf("makedir: created directory \"%s\" in \"%s\"\n", name_dir, source_dir);
	else if(verbose)
		printf("makedir: created directory \"%s\"\n", name_dir);
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

int check_if_slash(const char *string)
{
	for(int l = 0, i = string[l]; i != '\0'; i = string[++l])
		if(i == '/')
			return(1);
	return(0);
}

int check_if_existing_path(const char *string)
{
	DIR *dirName = opendir(string);
	if(dirName)
	{
		closedir(dirName);
		return(1);
	}
	return(0);
}

void user_error(const char *e, ...)
{
	va_list args;
	va_start(args, e);
	vprintf(e, args);
	va_end(args);
	printf("\n");
	exit(EXIT_FAILURE);
}

void system_error(const char *e)
{
	perror(e);
	exit(EXIT_FAILURE);
}
