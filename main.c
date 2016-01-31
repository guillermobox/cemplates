/*
 * Driver to test the cemplates library
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cemplates.h"

const char string[] = "Known capitals:\n\n{{#capitals}}  * Capital of {{state}} is {{city}}.\n{{/capitals}}\nEnd of list.\n";

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

const void * iterate_capital(const void * previous, const void * first)
{
	struct mydata * pointer, * pfirst;

	if (previous == NULL)
		return first;

	pointer = (struct mydata *) previous;
	pfirst = (struct mydata *) first;

	if ((pointer - pfirst) + 1 >= sizeof(dataarray) / sizeof(struct mydata))
		return NULL;

	return (void *)(pointer + 1);
};

const char * format_capital(const char * key, const void * object)
{
	struct mydata * pointer;
	char * answer = NULL;

	pointer = (struct mydata *) object;

	if (strcmp(key, "state") == 0)
		answer = strdup(pointer->state);
	else if (strcmp(key, "city") == 0)
		answer = strdup(pointer->city);

	return answer;
};

int main(int argc, char *argv[])
{
	struct cemplates_engine printer;
	const char * output;

	cemplates_add_template(&printer, "root", string);
	cemplates_add_handler(&printer, "capitals", format_capital);
	cemplates_add_iterator(&printer, "#capitals", iterate_capital, dataarray);

	output = cemplates_process(printer, "root");
	puts(output);

	cemplates_free(&printer);
	free((char *) output);

	return (EXIT_SUCCESS);
};
