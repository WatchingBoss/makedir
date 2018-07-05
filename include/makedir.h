#ifndef MAKEDIR_H
#define MAKEDIR_H

#define DIR_NAME 256

typedef enum{false, true} bool;

extern int check_if_slash(const char *string);
extern int check_if_existing_path(const char *string);

extern void call_creation(int argc, char *argv[]);

extern void create_dir(const char *, const char *);
extern void create_parent_dir(const char *, const char *);

extern void user_error(const char *, ...);
extern void system_error(const char *);

#endif
