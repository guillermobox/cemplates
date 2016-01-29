#ifndef _CEMPLATES_H_
#define _CEMPLATES_H_

struct cemplates_engine {

};

typedef const char (*cemplates_fn_parser)(const char *, const void *);
typedef const char (*cemplates_fn_iterator)(const char *, const void *);

int cemplates_add_template(const char *);
int cemplates_add_parser(const char *, cemplates_fn_parser);
int cemplates_add_iterator(const char *, cemplates_fn_iterator);

#endif
