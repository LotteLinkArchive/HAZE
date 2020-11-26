#include "tomlext.h"
#include "engine.h"
#include <hztoml/toml.h>

toml_accessor_t get_conf_prop(
	const CHR *accessor_obj,
	toml_table_t *table,
	toml_type_t type)
{
	#ifndef MAX_ACCESSOR_SIZE
	#define MAX_ACCESSOR_SIZE 1024
	#endif

	CHR accbuf[MAX_ACCESSOR_SIZE], *accessor;
	strncpy(accbuf, accessor_obj, (MAX_ACCESSOR_SIZE - 1));
	
	accessor = (CHR *)accbuf;
	toml_accessor_t final = {.ok = 0, .data_type = type};
	CHR *term = accessor;

	while (*accessor != '\0') {
		if ((*accessor) == '.') {
			*accessor = '\0';
			if (0 == (table = toml_table_in(table, term))) goto tzcpunsafe;

			term = accessor + 1;
		}

		accessor++;
	}

	if (*term == '\0') goto tzcpunsafe;
	
	#define RDFPOINT(type, rfunc) type(*rfunc)(const toml_table_t *arr, const CHR *key) = NULL
	RDFPOINT(toml_table_t*, table_rdr); RDFPOINT(toml_array_t*, array_rdr); RDFPOINT(toml_datum_t, datum_rdr);

	switch (type) {
	case TOML_STRING:    datum_rdr = toml_string_in;    final.reader_type = TOML_DATUM_READER;  break;
	case TOML_BOOL:      datum_rdr = toml_bool_in;      final.reader_type = TOML_DATUM_READER;  break;
	case TOML_INT:       datum_rdr = toml_int_in;       final.reader_type = TOML_DATUM_READER;  break;
	case TOML_DOUBLE:    datum_rdr = toml_double_in;    final.reader_type = TOML_DATUM_READER;  break;
	case TOML_TIMESTAMP: datum_rdr = toml_timestamp_in; final.reader_type = TOML_DATUM_READER;  break;
	case TOML_TABLE:     table_rdr = toml_table_in;     final.reader_type = TOML_TABLE_READER;  break;
	case TOML_ARRAY:     array_rdr = toml_array_in;     final.reader_type = TOML_ARRAY_READER;  break; }
	switch (final.reader_type) {
	case TOML_DATUM_READER: final.u.d = datum_rdr(table, term); final.ok = final.u.d.ok; break;
	case TOML_ARRAY_READER: final.u.a = array_rdr(table, term); final.ok = final.u.a;    break;
	case TOML_TABLE_READER: final.u.t = table_rdr(table, term); final.ok = final.u.t;    break; }

tzcpcret:
	return final;
tzcpunsafe:
	final.ok = 0;
	goto tzcpcret;
}

toml_accessor_t get_conf_prop_err(
	const CHR *accessor_obj,
	toml_table_t *table,
	toml_type_t type)
{
	toml_accessor_t final = get_conf_prop(accessor_obj, table, type);

	if (!final.ok) errwindow(
		"TOML Accessor Failure (%s)\n\n"
		"If you're getting this error, this is a serious bug.\n"
		"Please contact the game developer immediately.\n\n"
		"Tip: Ensure all values in the game config file are populated. There are no defaults.", accessor_obj);

	return final;
}
