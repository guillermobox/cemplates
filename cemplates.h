#ifndef _CEMPLATES_H_
#define _CEMPLATES_H_

struct cemplates_engine {
	int placeholder;
};

typedef const char *(*cemplates_fn_parser)(const char *, const void *);
typedef const void *(*cemplates_fn_iterator)(const void *, const void *);

void cemplates_add_template(struct cemplates_engine * engine, const char * key, const char * content);
void cemplates_add_handler(struct cemplates_engine * engine, const char * path, cemplates_fn_parser parser);
void cemplates_add_iterator(struct cemplates_engine * engine, const char * path, cemplates_fn_iterator iterator, const void * first);
const char * cemplates_process(struct cemplates_engine engine, const char * path);
void cemplates_free(struct cemplates_engine * engine);

#define CEMPLATES_ARRAY_ITERATOR(name, structname, variablename) \
const void * name(const void * previous, const void * first) \
{\
	struct structname * pointer, * pfirst; \
 \
	if (previous == NULL) \
		return first; \
 \
	pointer = (struct structname *) previous; \
	pfirst = (struct structname *) first; \
 \
	if ((pointer - pfirst) + 1 >= sizeof(variablename) / sizeof(struct structname)) \
		return NULL; \
 \
	return (void *)(pointer + 1); \
}

#endif
