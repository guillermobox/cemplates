#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cemplates.h"

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

/*
int main(int argc, char * argv[])
{
	const char *p;
	char *key, *value;
	const char * section_start;
	const void * section;

	section = NULL;
	for (p = string; *p; p++) {
		if (start_marker(p)) {
			key = get_marker(&p);
			if (key[0] == '#') {
				section = format_section(key, section);
				section_start = p;
			} else if (key[0] == '/') {
				section = format_section(key, section);
				if (section != NULL)
					p = section_start;
			} else {
				value = format_key(key, section);
				if (value != NULL) {
					fwrite(value, 1, strlen(value), stdout);
					free(value);
				}
			}
			free(key);
		} else {
			fwrite(p, 1, 1, stdout);
		}
	};
};
*/
