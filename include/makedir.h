#ifndef MAKEDIR_H
#define MAKEDIR_H

typedef enum{false, true} bool;

extern void call_creation(int argc, char *argv[]);

extern void create_dir(const char *);
extern void create_parent_dir(const char *);

extern int check_if_slash(const char *string);
extern int check_if_existing_path(const char *string);

extern void user_error(const char *, ...);
extern void system_error(const char *);

extern void *xrealloc(void *, size_t);
extern void *xmalloc(size_t);

#endif
