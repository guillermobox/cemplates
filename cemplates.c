#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char string[] = "The capital of {state} is {capital}.\n";

char * format(const char * key)
{
	char * answer = NULL;
	if (strcmp(key, "state") == 0)
		answer = strdup("Illinois");
	else if (strcmp(key, "capital") == 0)
		answer = strdup("Springfield");
	return answer;
};

int main(int argc, char * argv[])
{
	const char * p;
	char keybuffer[64];

	for (p = string; *p; p++) {
		if (*p == '{') {
			char *q;
			q = keybuffer;
			memset(keybuffer, 0, sizeof(keybuffer));
			p++;
			while (*p && *p != '}') {
				*q = *p;
				q++;
				p++;
			}
			char * value = format(keybuffer);
			if (value != NULL) {
				fwrite(value, 1, strlen(value), stdout);
				free(value);
			}
		} else {
			fwrite(p, 1, 1, stdout);
		}
	};
};
