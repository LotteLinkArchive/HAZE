#include "tomlext.h"
#include "engine.h"
#include <hztoml/toml.h>

toml_accessor_t get_conf_prop_err(
	const CHR *accessor_obj,
	toml_table_t *table,
	toml_type_t type)
{
	toml_accessor_t final = toml_accessor_gen(accessor_obj, table, type);

	if (!final.ok) errwindow(
		"TOML Accessor Failure (%s)\n\n"
		"If you're getting this error, this is a serious bug.\n"
		"Please contact the game developer immediately.\n\n"
		"Tip: Ensure all values in the game config file are populated. There are no defaults.", accessor_obj);

	return final;
}
