#include "pathy.h"
#include "config.h"
#include "engine.h"
#include <cwalk.h>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

SX cwk_fspath_join(const CHR *base, const CHR *path, CHR *buffer, SX buffer_size)
{
	CHR cwd[MAX_PATH_LENGTH + 1];
	if(!getcwd(cwd, MAX_PATH_LENGTH)) errwindow("Unable to get current working directory!");

	cwk_path_get_absolute(cwd, base, buffer, buffer_size);
	cwk_path_get_absolute(buffer, path, buffer, buffer_size);
	return cwk_path_get_relative(cwd, buffer, buffer, buffer_size);
}