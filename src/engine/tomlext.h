#ifndef HZTOMEXT_HEADER_FILE
#define HZTOMEXT_HEADER_FILE
#include "engine.h"

/* Ditto, but trigger a fatal engine error */
H_EXTERN toml_accessor_t get_conf_prop_err(
	const CHR *accessor_obj,
	toml_table_t *table,
	toml_type_t type);
#endif
