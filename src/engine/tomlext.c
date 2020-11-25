#include "tomlext.h"
#include "engine.h"

toml_datum_t get_conf_prop(
	const CHR *accessor_obj,
	toml_table_t *table,
	toml_datum_t(*reader)(const toml_table_t *arr, const CHR *key))
{
	#ifndef MAX_ACCESSOR_SIZE
	#define MAX_ACCESSOR_SIZE 1024
	#endif

	CHR accbuf[MAX_ACCESSOR_SIZE], *accessor;
	strncpy(accbuf, accessor_obj, (MAX_ACCESSOR_SIZE - 1) * sizeof(CHR));
	accessor = (CHR *)accbuf;
	toml_datum_t final = {.ok = 0};
	CHR *term = accessor;

	while (*accessor != '\0') {
		if ((*accessor) == '.') {
			*accessor = '\0';
			if (0 == (table = toml_table_in(table, term))) { *term = '\0'; break; }

			term = accessor + 1;
		}

		accessor++;
	}

	if (*term != '\0') final = reader(table, term);
	else final.ok = 0;

	if (!final.ok) errwindow(
		"TOML Accessor Failure (%s)\n\n"
		"If you're getting this error, this is a serious bug.\n"
		"Please contact the game developer immediately.\n\n"
		"Tip: Ensure all values in the game config file are populated. There are no defaults.", accessor_obj);

	return final;
}