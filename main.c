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

const struct mydata capitals[] = {
	{.state="Illinois", .city="Springfield"},
	{.state="California", .city="Sacramento"},
	{.state="Colorado", .city="Denver"},
	{.state="Hawaii", .city="Honolulu"},
};

CEMPLATES_ARRAY_ITERATOR(iterate_capital, mydata, capitals);

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
	cemplates_add_iterator(&printer, "#capitals", iterate_capital, capitals);

	output = cemplates_process(printer, "root");
	puts(output);

	cemplates_free(&printer);
	free((char *) output);

	return (EXIT_SUCCESS);
};
