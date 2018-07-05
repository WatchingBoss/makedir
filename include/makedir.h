#ifndef MAKEDIR_H
#define MAKEDIR_H

#define DIR_NAME 256

typedef enum{false, true} bool;

extern void my_error(char *);
extern void create_dir(char *, char *, bool, bool);
extern void parent_dirs(char *, char *, bool, bool);

#endif
