#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char string[] = "The capital of {{state}} is {{capital}}.\n";

char * format(const char * key)
{
	char * answer = NULL;
	if (strcmp(key, "state") == 0)
		answer = strdup("Illinois");
	else if (strcmp(key, "capital") == 0)
		answer = strdup("Springfield");
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

	for (p = string; *p; p++) {
		if (start_marker(p)) {
			key = get_marker(&p);
			value = format(key);
			if (value != NULL) {
				fwrite(value, 1, strlen(value), stdout);
				free(value);
			}
			free(key);
		} else {
			fwrite(p, 1, 1, stdout);
		}
	};
};
