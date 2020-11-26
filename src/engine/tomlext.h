#ifndef HZTOMEXT_HEADER_FILE
#define HZTOMEXT_HEADER_FILE
#include "engine.h"

enum toml_public_types {
	TOML_STRING,
	TOML_BOOL,
	TOML_INT,
	TOML_DOUBLE,
	TOML_TIMESTAMP,
	TOML_TABLE,
	TOML_ARRAY
};
typedef U8 toml_type_t;

enum toml_reader_types {
	TOML_DATUM_READER,
	TOML_ARRAY_READER,
	TOML_TABLE_READER
};

typedef struct {
	U1 ok;
	toml_type_t reader_type;
	toml_type_t data_type;
	union {
		toml_datum_t d;
		toml_array_t *a;
		toml_table_t *t;	
	} u;
} toml_accessor_t;

/* Get a property from a nested toml table using a single accessor */
H_EXTERN toml_accessor_t get_conf_prop(
	const CHR *accessor_obj,
	toml_table_t *table,
	toml_type_t type);

/* Ditto, but trigger a fatal engine error */
H_EXTERN toml_accessor_t get_conf_prop_err(
	const CHR *accessor_obj,
	toml_table_t *table,
	toml_type_t type);
#endif
