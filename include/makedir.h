#ifndef MAKEDIR_H
#define MAKEDIR_H

typedef enum{false, true} bool;

extern void my_error(char *);
extern int create_dir(char *, char *, bool);

#endif
