#ifndef HZTOMEXT_HEADER_FILE
#define HZTOMEXT_HEADER_FILE
#include "engine.h"

/* Get a property from a nested toml table using a single accessor */
H_EXTERN toml_datum_t get_conf_prop(
	const CHR *accessor_obj,
	toml_table_t *table,
	toml_datum_t(*reader)(const toml_table_t *arr, const CHR *key));
#endif