#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//const char string[] = "Known capitals:\n\n{{#capitals}}  * Capital of {{state}} is {{city}}.\n{{/capitals}}\nEnd of list.\n";
const char string[] = "Hosts available: {{#hosts}}{{hostname}}:{{port}}, {{/hosts}}\n";

/*
 * the expected input object should by like:
 *  { "capitals": [
 *  	{"state": ..., "city": ...},
 *  	{"state": ..., "city": ...}
 *      ]
 *  }
 */

struct mydata {
	const char state[64];
	const char city[64];
};

const struct mydata dataarray[] = {
	{.state="Illinois", .city="Springfield"},
	{.state="California", .city="Sacramento"},
	{.state="Colorado", .city="Denver"},
	{.state="Hawaii", .city="Honolulu"},
};

struct host {
	char hostname[64];
	int port;
	struct host * next;
};

struct host myhost;

const void * format_section(const char * name, const void * previous)
{
	if (strcmp(name + 1, "capitals") == 0) {
		struct mydata * pointer;
		pointer = (struct mydata *) previous;
		if (pointer == NULL)
			return &dataarray[0];
		if ((pointer - dataarray) + 1 >= sizeof(dataarray) / sizeof(struct mydata))
			return NULL;
		return pointer + 1;
	} else if (strcmp(name + 1, "hosts") == 0) {
		struct host * pointer;
		if (previous == NULL)
			return &myhost;
		pointer = (struct host *) previous;
		return pointer->next;
	} else
		return NULL;
};

char * format_key(const char * key, const void * section)
{
	char * answer = NULL;
	struct mydata * pointer;

	pointer = (struct mydata *) section;

	if (strcmp(key, "state") == 0)
		answer = strdup(pointer->state);
	else if (strcmp(key, "city") == 0)
		answer = strdup(pointer->city);
	else if (strcmp(key, "hostname") == 0) {
		struct host * h;
		h = (struct host *)section;
		answer = strdup(h->hostname);
	} else if (strcmp(key, "port") == 0) {
		struct host * h;
		char buffer[128];
		h = (struct host *)section;
		sprintf(buffer, "%d", h->port);
		answer = strdup(buffer);
	}
	return answer;
};

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


/* return the marker that appears starting in the actual position pointed by *p
 *
 */
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

int main(int argc, char * argv[])
{
	const char *p;
	char *key, *value;
	const char * section_start;
	const void * section;

	struct host other;

	strcpy(myhost.hostname, "ctdesk235");
	myhost.port = 1301;
	myhost.next = &other;
	strcpy(other.hostname, "ctcomp2");
	other.port = 22;
	other.next = NULL;

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
