/*
 * Reimplementation of UNIX mkdir utiliry
 */

#include "include/inc.h"

typedef unsigned char uchar_t;

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
				user_error("Wrong argument: %c", c);
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
	for(int i = argc - 1; i != 0; --i)
	{
		if(argv[i][0] ==  '-')
			break;
		else
		{
			if(check_if_slash(argv[i]))
			{
				if(check_if_existing_path(argv[i]))
					create_dir(argv[i]);
				else if(parents == true)
						create_parent_dir(argv[i]);
				else
					user_error("You did not specify '-p' option to create %s", argv[i]);
			}
			else
				create_dir(argv[i]);
		}
	}

	exit(EXIT_SUCCESS);
}

void create_parent_dir(const char *path_names)
{
	int count_of_names = 0;
	char name[256] = {0};
	char **names = NULL, **temp_names = NULL;
	char *path = NULL, *temp_path = NULL;
	size_t length = strlen(path_names) + 1;

	for(uchar_t l = 0, i = path_names[l], x = 0;
		l < length; i = path_names[++l])
	{
		if(l == length)
			i = 0;
		else
			name[x] = i;

		if(i == '/' || !i)
		{
			name[x + 1] = '\0';

			temp_names = xrealloc(names, sizeof *names * (count_of_names + 1));
			if(temp_names)
				names = temp_names;
			else
				system_error("xrealloc temp_names error");

			names[count_of_names] = xmalloc(sizeof *names * (strlen(name) + 1));

			strcpy(names[count_of_names], name);

			x = 0;
			++count_of_names;
			memset(name, 0, sizeof name);
		}
		else
			++x;
	}
	
	for(int i = 0; i < count_of_names; ++i)
	{
		temp_path = xrealloc(path, sizeof *path * (strlen(names[i]) + 1));
		if(temp_path)
			path = temp_path;
		else
			system_error("xrealloc temp_path error");
		path = strcat(path, names[i]);
		create_dir(path);
	}

	for(int i = 0; i < count_of_names; ++i)
		free(names[i]);
	free(path);
}

void create_dir(const char *path_name)
{
	if( (mkdir(path_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) < 0)
		system_error("mkdir error");

	if(verbose)
		printf("makedir: created directory \"%s\"\n", path_name);
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

int check_if_existing_path(const char *str)
{
	char *path = NULL;
	size_t slash = 0;

	for(int i = 0; i <= strlen(str); ++i)
		if(str[i] == '/')
			++slash;

	if(slash)
	{
		for(uchar_t l = 0, i = str[l], x = 0; x != slash; i = str[++l])
		{
			path = xrealloc(path, sizeof *path * (l + 1));
			path[l] = i;

			if(str[l] == '/')
				++x;
		}
		path = xrealloc(path, sizeof *path * (strlen(str) + 1));
		path[strlen(str)] = '\0';
	}
	else
	{
		path = xmalloc(strlen(str) + 1);
		strcpy(path, str);
	}
	
	DIR *dirName = opendir(path);
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

void * xrealloc(void *ptr, size_t bytes_number)
{
	ptr = realloc(ptr, bytes_number);
	if(!ptr)
		system_error("realloc error");
	return ptr;
}

void * xmalloc(size_t bytes_number)
{
	void *ptr = malloc(bytes_number);
	if(!ptr)
		system_error("malloc error");
	return ptr;
}
