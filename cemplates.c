#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cemplates.h"

#define HASHSIZE 1024


struct hash_entry {
	char * key;
	void * ptr;
	int used;
};

static struct hash_entry hashtable[HASHSIZE];

unsigned int hash(const char *s)
{
	unsigned int hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

void hash_install(const char * key, const void * ptr)
{
	unsigned int hashvalue;

	hashvalue = hash(key);

	while (hashtable[hashvalue].used) {
		if (!strcmp(hashtable[hashvalue].key, key))
			break;
		hashvalue = (hashvalue + 1) % HASHSIZE;
	}
	hashtable[hashvalue].key = strdup(key);
	hashtable[hashvalue].ptr = (void *)ptr;
	hashtable[hashvalue].used = 1;
}

void * hash_lookup(const char *key)
{
	unsigned int hashvalue;

	hashvalue = hash(key);

	while (hashtable[hashvalue].used) {
		if (!strcmp(hashtable[hashvalue].key, key))
			return hashtable[hashvalue].ptr;
		hashvalue = (hashvalue + 1) % HASHSIZE;
	}
	return NULL;
}


void cemplates_init(struct cemplates_engine * engine)
{
	memset(hashtable, 0, sizeof(hashtable));
}

void cemplates_add_template(struct cemplates_engine * engine, const char * key, const char * content)
{
	hash_install(key, (void *)content);
}

void cemplates_add_handler(struct cemplates_engine * engine, const char * path, cemplates_fn_parser parser)
{
	hash_install(path, (void *)parser);
}

void * myfirst;

void cemplates_add_iterator(struct cemplates_engine * engine, const char * path, cemplates_fn_iterator iterator, const void * first)
{
	myfirst = (void *)first;
	hash_install(path, (void *)iterator);
}

void cemplates_free(struct cemplates_engine * engine)
{
}

static int start_marker(const char *p)
{
	if (*p == '{' && *++p == '{')
		return 1;
	return 0;
}

static int end_marker(const char *p)
{
	if (*p == '}' && *++p == '}')
		return 1;
	return 0;
}

static char * get_marker(const char ** in)
{
	char buffer[128], *p;
	const char * q;

	p = buffer;
	q = *in;

	q += 2;

	while (!end_marker(q)){
		*p = *q;
		p++;
		q++;
	}

	q += 1;

	*in = q;
	*p = '\0';
	return strdup(buffer);
}

const char * cemplates_process(struct cemplates_engine engine, const char * path)
{
	char buffer[1024];

	const char *p;
	char *key, *value;
	const char * section_start;
	const void * section;
	const char * string;
	const char * localpath;
	cemplates_fn_iterator iterator;
	cemplates_fn_parser parser;

	string = (const char *)hash_lookup(path);

	buffer[0] = '\0';
	section = NULL;
	for (p = string; *p; p++) {
		if (start_marker(p)) {
			key = get_marker(&p);
			if (key[0] == '#') {
				iterator = hash_lookup(key);
				section = iterator(section, myfirst);
				localpath = strdup(key + 1);
				//section = format_section(key, section);
				section_start = p;
			} else if (key[0] == '/') {
				key[0] = '#';
				iterator = hash_lookup(key);
				section = iterator(section, myfirst);
				if (section != NULL)
					p = section_start;
			} else {
				parser = hash_lookup(localpath);
				value = (char *) parser(key, section);
				//value = format_key(key, section);
				if (value != NULL) {
					strcat(buffer, value);
					free(value);
				}
			}
			free(key);
		} else {
			strncat(buffer, p, 1);
		}
	};
	return strdup(buffer);
}

